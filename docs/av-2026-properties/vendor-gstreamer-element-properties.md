# Vendor GStreamer Element Properties — Common Cross-SoC Contract

**Status:** DRAFT v0.1 — for review (tracking issue [#38](https://github.com/rdkcentral/rdk-hpk-documentation/issues/38))
**Audience:** Architecture, SoC vendor engineering

## 1. Purpose & scope

The interface between RDK middleware and the SoC port includes the **GStreamer element surface of the vendor-supplied elements** — the vendor video sink (westeros-sink-shaped) and the vendor audio sink — expressed as properties, action signals, and caps. This document defines the **common contract**: the property set every SoC port (Broadcom, Amlogic, MediaTek, Realtek, and future ports) must implement identically, derived from the 2026 AV certification programmes (concurrent playback / dual decode, graphics player decode-to-texture, audio mixing, per-session DRM, codec capability reporting).

Scope boundaries:

- **DRM key management stays in the CDM/OCDM stack.** The element-level DRM surface is limited to session *binding* and secure-path selection.
- **Metrics ride the vendor metrics interface, not element properties.** Counters and the quality-event ring need atomic multi-field snapshots and a seq-addressed event ring, which do not fit per-property reads. See [rdk-halif-aidl#754](https://github.com/rdkcentral/rdk-halif-aidl/issues/754).
- **HDCP has no element surface.** Enforcement is in the CDM/TEE at decrypt time; link status rides the device-settings APIs. The element's only HDCP-adjacent obligation is frame-presentation behaviour under output restriction ([Section 3.1](#31-placement--plane)).
- **Caps carry the bitstream** (codec, profile/level, resolution, HDR/DV, `codec_data`, CENC); **properties carry placement, decode control, resource intent, secure mode, volume/mute**.
- Structured data (capability matrices, decode budget) is carried in **`GstStructure`-valued read-only properties**; per-frame operations use **action signals**.
- **Asynchronous vendor→middleware notification (QoS eventing) is callbacks on the vendor HAL backend** — first frame, underflow, decode/PTS errors, ramp completion, frame-ready — the same backend surface that carries metrics. It is not `g_signal` on the element; see [Section 5](#5-qos-callbacks--vendor-hal-backend).

## 2. Contract conventions

| Convention | Rule |
|---|---|
| Discovery | Property presence **is** capability discovery: middleware probes with `g_object_class_find_property`. A property in the MANDATORY tables below must exist on every SoC. |
| Versioning | Read-only `api-version` (uint) on both vendor elements identifies the contract revision. |
| Atomic reads | Multi-field data (capability matrix, decode budget) is one `GstStructure` property — a single `g_object_get` returns a coherent snapshot. Per-field scalar reads of the same data are non-goals. |
| Errors | Admission and runtime failures post `GST_MESSAGE_ERROR`/`WARNING` on the bus with a standardised error domain and code set: resource-denied, secure-pool-exhausted, decode-fault, output-restricted, plus the texture-ring set ring-reservation-failed, format-unsupported, decoder-busy, hardware-fault. |
| Units | Pixels for geometry, milliseconds for durations, linear 0.0–1.0 for volume, 0 = highest for priority. |

Column legend for the tables below: **Dir** = access direction (RO / RW, with the state constraint where writes are only honoured before the element reaches READY). **Today** = current support state across ports (`Common core` = present on all ports; `none` = net-new on every port; otherwise the ports that have it, `→ all` meaning the remainder must add it). **NEW** marks properties that exist on no SoC today.

## 3. Video sink — mandatory properties

### 3.1 Placement & plane

| Property | Type · range | Dir | Semantics | Today |
|---|---|---|---|---|
| `rectangle` | string `"x,y,w,h"` px | RW | Plane geometry | Common core; Realtek variant must align |
| `zorder` | uint | RW | Plane stacking, deterministic across sessions | Common core |
| `opacity` | uint 0–255 | RW | Plane alpha | Common core |
| `show-video-window` | boolean | RW | Video mute (hide plane, keep decoding) | BCM/V4L2/RAW → all |
| `zoom-mode` | enum | RW | Scaling policy | BCM/V4L2/RAW → all |
| `flush-clears-last-frame` **NEW** | boolean | RO | Sink renders transparency where video was after a flush | none |
| `flush-retains-last-frame` **NEW** | boolean | RO | Sink continues presenting the last rendered frame after a flush | none |

Behavioural clauses:

- Geometry/zorder changes applied to multiple sinks within one frame period MUST land in a single vsync (atomic multi-plane commit — see open item O-1).
- `rectangle` repositioning from any rectangle to any rectangle MUST sustain at least 5 updates per second without visual artefact or A/V interruption.
- On frame starvation — underflow or DRM output restriction — the sink MUST hold the last rendered frame, not blank the plane; `show-video-window=false` remains the sole explicit blank control.
- At least one of the two flush declarations MUST be TRUE; the sink honours whichever it declares.

### 3.2 Dual decode & admission

| Property | Type · range | Dir | Semantics | Today |
|---|---|---|---|---|
| `video-role` **NEW** | enum `primary`/`secondary` | RW (before READY) | Selects the concurrent-decode slot. Replaces four divergent per-SoC selection mechanisms (a `pip` property, an environment variable, a `plane` enum, and a `res-usage=0` convention). This revision defines two roles; platforms whose `max-concurrent-video-decoders` exceeds 2 get an index form in a future contract revision | 4 divergent mechanisms |
| `res-priority` | uint, 0 = highest | RW | Resource-manager priority; realises primary-player preferential treatment and reclaim ordering | BCM/V4L2/MTK; RTK adds |
| `res-usage` | flags `fullResolution\|fullQuality\|fullPerformance` | RW | Resource-manager usage hint (not a decoder selector) | RTK plain-uint variant must conform |
| `max-width` / `max-height` | uint px | RW (before READY) | Declared workload ceiling; sizes the secure pool | Cobalt-only `max-video-width/height` → all |
| `max-framerate` **NEW** | uint fps | RW (before READY) | Workload ceiling for pool sizing | none |
| `max-bit-depth` **NEW** | uint bits | RW (before READY) | Workload ceiling for pool sizing | none |
| `decode-budget` **NEW** | `GstStructure` `{total, used, secure-total, secure-used, cost-of(config)}` | RO | Decode-budget feasibility surface — fungible-silicon budget, not a discrete instance count. Includes secure-heap capacity so admission can answer "can the heap hold this session" before allocation, not at failure time | none |

Behavioural clauses: a second decoder's secure allocation MUST NOT evict or shrink the first; each decoder's output MUST be routable to a distinct plane; admission denial is a bus error, not a silent stall; each sink instance MUST honour its segment rate independently (0.0 pause / 1.0 / 2.0 — per-SoC rate properties are not the contract).

### 3.3 Sync & first frame

| Property | Type · range | Dir | Semantics | Today |
|---|---|---|---|---|
| `avsync-mode` | enum `vmaster/amaster/pcrmaster/iptv/freerun` | RW | Master-clock selection; middleware sets explicitly — no per-SoC defaults | V4L2/RAW only; divergent defaults elsewhere |
| `avsync-session` | int | RW | Pairs the audio and video sinks of one stream — required with two concurrent streams | V4L2/RAW → all |
| `wait-for-video` | boolean | RW | Hold audio-mastered clock until first video frame | V4L2 → all |
| `immediate-output` | boolean | RW | First-frame/low-latency lever | BCM/V4L2 → all |
| `frame-step-on-preroll` | boolean | RW | Render on preroll | BCM/V4L2 → all |
| `low-latency-mode` | boolean | RW | Low-latency decode | V4L2/MTK → all |
| `video-pts` | int64, 90 kHz | RO | Current presentation position | Common core (`video_pts`) |

### 3.4 Secure video path

| Property | Type · range | Dir | Semantics | Today |
|---|---|---|---|---|
| `secure-video` | boolean | RW (before READY) | Secure decode required. The **property** is the contract on every SoC; a caps feature (e.g. `(memory:DMABuf)`) may remain a transport detail behind it | BCM-only property; AML caps feature; MTK/RTK none |
| `drm-session` **NEW** | uint, 0 = no decrypt context (middleware allocates session ids ≥ 1, so 0 never aliases a live session) | RW (before READY) | Binds this decoder to one decrypt context at create, before any data flows — no rebind after READY; no sharing/aliasing across decoders | none (implicit via caps/protection meta today) |

Behavioural clauses: the per-session secure (SVP) pool is sized from `max-width/height/framerate/bit-depth` — not a fixed UHD pool; the pool MUST be allocated at the declared size **at allocation time**, before the first buffer — allocation at a fixed default with later resize is non-conformant; two simultaneous secure paths MUST be supported where both streams are protected.

### 3.5 Graphics player (decode-to-texture)

| Property | Type · range | Dir | Semantics | Today |
|---|---|---|---|---|
| `graphics-texture-mode` **NEW** | boolean | RW (before READY) | Selects NV12 + linear DMA-BUF ring capture instead of plane output. Replaces divergent legacy `enable-texture` behaviours | divergent legacy |
| `texture-slot-count` | uint | RW (before READY) | Ring slots. Middleware configures ring geometry from the per-product HFP before READY; values read back as configured and are valid before the first frame | none |
| `texture-slot-size` | uint bytes | RW (before READY) | Per-slot size — consumer needs it to mmap the ring | none |
| `texture-format` | uint fourcc (NV12) | RW (before READY) | Frame format | none |
| `texture-modifier` | uint64 (LINEAR) | RW (before READY) | DRM modifier | none |
| `texture-plane-stride` **NEW** | uint[3] | RO | Per-plane stride, constant across slots — vendor-filled, valid before the first frame so middleware can populate the session ring descriptor at create | none |

Action signals (the frame contract):

| Action signal | Args → return | Semantics |
|---|---|---|
| `acquire-latest-frame` **NEW** | → `GstStructure` `{fd[], offset[], stride[], length[], width, height, fourcc, pts-ns, slot}` or NULL | Non-blocking pull of the newest decoded frame not previously delivered; NULL when nothing new has been decoded since the previous acquire — never same-frame re-delivery. Truthful per-plane byte offsets |
| `release-frame` **NEW** | `(uint slot)` → void | Return slot to ring; idempotent. The slot index is the sole frame identity for release — per-plane byte offsets are payload description only |

Plus an opt-in `frame-ready` push delivered as a HAL-backend callback ([Section 5](#5-qos-callbacks--vendor-hal-backend)). All DMA-BUFs are released on transition to NULL. The corresponding HAL-level capture surface is tracked in [rdk-halif-aidl#753](https://github.com/rdkcentral/rdk-halif-aidl/issues/753).

Behavioural clauses:

- **Fail loud.** Ring capability limits (max slot count/size, supported FOURCCs/modifiers) are declared in the per-product HFP; asked for an unsupported configuration — or `graphics-texture-mode=TRUE` on a product without the capability — the sink MUST fail the bind with a bus error (format-unsupported / ring-reservation-failed), never silently fall back to plane output.
- **Decoder autonomy.** Decode proceeds continuously regardless of consumer acquire cadence, which may be sparse or delayed; the all-slots-locked policy (stall vs drop-newest) is HFP-declared.
- **Frame-drop budget.** The capture path sustains ≤ 1 dropped frame per 15-second clip, 144p through 2160p.
- **Admission.** A texture-mode session occupies a decoder slot: it takes a `video-role` and counts against `max-concurrent-video-decoders` and `decode-budget`.

### 3.6 Codec capabilities

| Surface | Form | Semantics |
|---|---|---|
| Pad template caps | caps | Truthful codec/profile/level/resolution/rate enumeration — never overstated |
| `codec-capabilities` **NEW** | `GstStructure` RO | Full matrix, per codec: profiles with **per-profile max level**, per-render-mode max levels (plane output, texture, texture-with-alpha), max width/height/framerate, dynamic ranges, secure-path support, supported plane types (video/graphics) |
| `max-concurrent-video-decoders` | uint RO | Concurrent decode ceiling |
| `max-video-planes` **NEW** | uint RO | Concurrent video plane / z-order slot ceiling — distinct from the decoder count on SoCs where planes ≠ decoders |
| `max-drm-sessions` | uint RO | Concurrent DRM context ceiling |

The profile/level/dynamic-range matrix is **not** expressible as flat property names — hence the structured property.

## 4. Audio sink — mandatory properties

| Property | Type · range | Dir | Semantics | Today |
|---|---|---|---|---|
| `volume` | double 0.0–1.0 linear | RW | Per-stream gain, pre-mix. One name on every SoC (per-SoC aliases such as `stream-volume` retired to alias status) | name diverges |
| `mute` | boolean | RW | Per-stream mute — must actually mute on every SoC. Muting/disabling MUST NOT provoke an underflow event | diverges |
| `audio-fade` | string `"<target%>,<ms>,<L\|I\|O>"` | RW | Eased per-stream volume ramp; Linear / In-Cubic / Out-Cubic curves. Mandatory on every vendor sink — retires probe-and-fallback paths and un-eased steps | probe + 3-way fallback |
| `fade-volume` | int % | RO | Current eased volume during an active fade | partial |
| `avsync-session` | int | RW | Pairs with the stream's video sink | none |
| `stream-sync-mode` | int | RW | Sync mode | BCM-only → all |
| `limit-buffering-ms` | uint ms | RW | Buffering cap | BCM-only → all |
| `low-latency` / `sync` / `sync-off` | boolean | RW | Latency/clock levers | server-guarded per-SoC → all |
| `audio-codec-capabilities` **NEW** | `GstStructure` | RO | Static per-codec matrix — read once before any codec is active. Per codec: channels, supported, passthrough, continuous, can-mix-ui-audio, can-mix-accessibility-audio, transition-support (none/mute/volume/ease), ease support (linear/in-cubic/out-cubic), reset-on-discontinuity, ease-on-discontinuity, encoder-latency-compensation (units 1/24000 s, HE-AAC only), supported-speeds (pitch-correct rate multipliers) | none |
| `mixer-capabilities` **NEW** | `GstStructure` | RO | MIX-session feasibility: input count, output/transcode formats per output port, limiter presence, MS12 presence, dual-Dolby reach | none |
| `continuous` | boolean | RO | Any-codec summary of the per-codec `continuous` declaration: receiver stays locked to the codec across play/pause/discontinuity/flush | none |
| `passthrough` | boolean | RO | Any-codec summary of the per-codec `passthrough` declaration (a player configured with that codec bypasses platform decode) | none |
| `can-mix-ui-audio` | boolean | RO | Any-codec summary: mixer can add UI LPCM | none |
| `can-mix-accessibility-audio` | boolean | RO | Any-codec summary: mixer can add TTS | none |
| `post-mix-limiter` | boolean | RO | Post-mix limiter present | none |
| `max-concurrent-audio-decoders` | uint | RO | Concurrent decoded-input ceiling | none |

The normative capability source is the per-codec matrix; the flat booleans are its property-bag summary. Matrix consistency rule: `passthrough = true` for a codec forces `can-mix-ui-audio = false` and `can-mix-accessibility-audio = false` for that codec — the passthrough payload cannot be sample-mixed downstream; `continuous` is the corollary receiver-lock declaration. The audio sink carries no workload-declaration properties — audio buffers are not secure-pool sized.

Behavioural clauses (vendor-internal, surfaced only via the capability structures): two concurrent decoded inputs mixed; post-mix limiter so summed inputs never clip; UI LPCM + TTS injection with passthrough degrade-to-transcode; ducking ramps without zipper noise; all sources sample-rate-converted to a common mixing rate before summing — UI fixed at 48 kHz, decode at source rate; mixed output routed to all active outputs (SPDIF/HDMI/analog) simultaneously.

## 5. QoS callbacks — vendor HAL backend

QoS eventing is **callbacks on the vendor HAL backend**, the same surface that carries metrics — not `g_signal` on the element. The per-SoC `g_signal_new` declarations in today's westeros-sink backends (Broadcom the fullest, others partial) are the divergence this replaces; every SoC backend registers the full callback set:

| Callback | Path | Today | Semantics |
|---|---|---|---|
| `first-video-frame` | video | brcm/v4l2/raw signals → all | First frame rendered |
| `buffer-underflow` | both | brcm/v4l2/raw signals → all | Underflow episode. MUST NOT be raised while a frame is available for rendering, and mute/audio-disable MUST NOT provoke one |
| `decode-error` | video | brcm/v4l2 signals → all | Decode fault, with PTS |
| `pts-error` | video | brcm-only signal → all | PTS discontinuity |
| `frame-ready` **NEW** | video | none | Opt-in texture-mode push |
| `volume-ramp-complete` **NEW** | audio | none | Fade finished |

## 6. Lifecycle contract (behavioural, testable)

On transition to `GST_STATE_NULL` the vendor element MUST synchronously release: decoder instance, plane binding, AV/stream clock, SVP/secure pool grant, all exported DMA-BUFs, and the bound decrypt context — bounded so a relaunched app reaches first frame inside the 5000 ms fast-relaunch budget. The plane is left visually clear — no residual frame. Orphaned sink instances are reapable by the platform.

## 7. Per-SoC migration summary

| SoC | Must add | Must change |
|---|---|---|
| Broadcom | `video-role` (replace `pip`), `low-latency-mode`, `avsync-mode/-session`, texture-ring surface (NEXUS→DMA-BUF), audio capability structures | `secure-video` stays |
| Amlogic | `secure-video` **property** (in front of caps feature), `video-role` (replace `res-usage=0` convention), `audio-fade` on sink, `volume`/`mute` names | dual-audio + dual-DRM platform gaps remain |
| MediaTek | `video-role` (replace env var), `secure-video`, per-session SVP pool sizing (drop hardcoded UHD), texture path, signals set | dual-DRM process limit |
| Realtek | `rectangle`/`zorder` common shape (drop `plane` enum as contract), flags-typed `res-usage`, `res-priority`, texture path (DMA-BUF export), signals set | timecode-callback-only divergence |

## 8. Open items

- **O-1 Atomic multi-plane commit** — properties are per-element; the certification programmes need cross-sink single-vsync commit. Candidate: transaction-id property pair, or vendor-side frame-period coalescing rule; prior art is the AIDL track's single-call array form `IPlaneControl.setVideoSourceDestinationPlaneMapping(SourcePlaneMapping[])`. Decision needed.
- **O-2 `decode-budget` cost query** — "cost of config X" is query-shaped; a RO structure carries totals, but per-config cost may need a `GstQuery` or an action signal.
- **O-3 Element identity** — on SoCs where the decoder is a separate element from the sink, the contract binds to whichever vendor element owns decoded frames; property names are identical either way.
- **O-4 Active-decoded-codec surface** — whether the now-playing decoded audio codec is read from upstream decoder caps or exposed as a sink property is unresolved.
- **O-5 PTS-targeted acquire** — `acquire-frame-at-pts(target-pts-ns)` is reserved in the action-signal namespace for a sync frame-selection extension. Not part of this revision.
- **Phase 2 — full inventory.** Extend this document to every element property in active use across the RDK stack, so each is either defined here or explicitly port-private (tracking issue [#38](https://github.com/rdkcentral/rdk-hpk-documentation/issues/38)).
