# Legacy AV HAL Extension for dual decode

## Table of contents

- [Revision history](#revision-history)
- [0. System context and deployment model](#0-system-context-and-deployment-model)
- [1. Purpose](#1-purpose)
- [2. Roles and responsibilities](#2-roles-and-responsibilities)
- [3. Scope](#3-scope)
- [4. Architecture facts and assumptions](#4-architecture-facts-and-assumptions)
- [5. GStreamer context and `res-usage` documentation](#5-gstreamer-context-and-res-usage-documentation)
- [6. Resource identification model](#6-resource-identification-model)
- [7. Video decoder and video plane resource model](#7-video-decoder-and-video-plane-resource-model)
- [8. Audio decoder resource model](#8-audio-decoder-resource-model)
- [9. Audio mixer resource model](#9-audio-mixer-resource-model)
- [10. SVP allocator resource model](#10-svp-allocator-resource-model)
- [11. DRM resource model](#11-drm-resource-model)
- [12. AV player transitions and resource acquisition](#12-av-player-transitions-and-resource-acquisition)
- [13. Audio behaviour in dual-decode modes](#13-audio-behaviour-in-dual-decode-modes)
- [14. Conformance and acceptance gates](#14-conformance-and-acceptance-gates)

**Status:** Draft for review.

**Specification version:** 0.1

## Revision history

| Version | Status | What changed | What reviewers should check |
|---|---|---|---|
| 0.1 | Draft for review | First version of the legacy AV HAL extension specification covering concurrent playback resource semantics and AV resource-management requirements. | Property semantics, capability reporting, resource selection, transition behaviour, and ESSOS integration requirements. |

### What this specification covers

- AV sync for dual-decode use cases.
- Video decoder and video plane resource selection using the existing `res-usage` property.
- Audio decoder resource handling for symmetric decoders.
- SVP allocator resource handling for asymmetric and symmetric platforms.
- SoC vendor selection and use of video and SVP resources based on GStreamer context information.
- Audio mixer behaviour during AV player transitions involving dual decode.
- DRM resource expectations for concurrent playback.
- AV player transition and resource-acquisition rules for application-managed GStreamer players and non-GStreamer players.

### What this specification does not cover

- AV sync for use cases other than dual decode.
- Unrelated platform features outside decoder, audio, SVP, DRM, and transition handling.
- Metrics HAL requirements.
- Frame-capture HAL behaviour.

---

## 0. System context and deployment model

This specification defines the contract between the middleware playback stack, vendor GStreamer plugins, and platform resource-management components for legacy concurrent dual-decode playback.

```mermaid
graph LR
    subgraph APP[Application Process]
        App[Application Process]
        subgraph GSTP[GStreamer Pipeline A / B]
            GST[Pipeline flow]
            Ctx[GStreamer Context]
            DEC[decryptor]
            SVPP[svp payload]
            subgraph VE[Video Element]
                VDEC[video decoder]
                WSU[westeros sink]
                WSOC[Westeros sink SoC]
            end
            subgraph AE[Audio Element]
                ADEC[Audio decoder]
                ASINK[Audio Sink]
            end
        end
        App --> GST
        GST --> DEC
        GST --> SVPP
        GST --> VDEC
        GST --> WSU
        GST --> ADEC
        GST --> ASINK
        App --> Ctx
        Ctx -->|SVP Usage| DEC
        Ctx -->|SVP Usage| SVPP
        Ctx -->|Video decoder usage| VE
        App --> YAML[Capability YAML]
        DEC --> OCDM[OCDM Adaptor]
        SVPP --> SVPC[gst-svp-ext-common]
        SVPC -->|Resolved SVP usage: API 🔴 **TBC**| SVPE[gst-svp-ext-soc<br/>SVP Allocator]
        WSU -->|GstWesterosSink pointer: resUsage| WSOC
        OCDM --> SVPC
    end

    subgraph OCP[openCDMi Process]
        OPENCDM[openCDMi]
        DRM[DRM System]
    end

    subgraph LEG[Legend]
        LRESP[Highlighted blocks = SoC vendor responsibility]
    end

    OCDM -->|IPC| OPENCDM
    OPENCDM --> DRM

    subgraph RMP[ESSOS Resource Manager Process]
        RM[ESSOS Resource Manager]
    end

    WSU -->|IPC| RM
    AE -->|IPC| RM
    SVPC -->|IPC| RM

    style APP fill:none,stroke:#666,stroke-width:1px,stroke-dasharray: 5 5
    style GSTP fill:#e6e6e6,stroke:#666,stroke-width:1px
    style LEG fill:none,stroke:#666,stroke-width:1px,stroke-dasharray: 5 5
    style OCP fill:none,stroke:#666,stroke-width:1px,stroke-dasharray: 5 5
    style RMP fill:none,stroke:#666,stroke-width:1px,stroke-dasharray: 5 5
    style VDEC fill:#d9e8fb,stroke:#3c78d8,stroke-width:1px,color:#000
    style WSOC fill:#d9e8fb,stroke:#3c78d8,stroke-width:1px,color:#000
    style ADEC fill:#d9e8fb,stroke:#3c78d8,stroke-width:1px,color:#000
    style ASINK fill:#d9e8fb,stroke:#3c78d8,stroke-width:1px,color:#000
    style SVPE fill:#d9e8fb,stroke:#3c78d8,stroke-width:1px,color:#000
    style DRM fill:#d9e8fb,stroke:#3c78d8,stroke-width:1px,color:#000
    style LRESP fill:#d9e8fb,stroke:#3c78d8,stroke-width:1px,color:#000
```

Note: Depending on SoC implementation design, the audio decoder may be integrated within the audio sink component, and the video decoder may be integrated within the westeros sink component.

### Process and IPC boundaries

- **Application Process** owns playback-pipeline construction for GStreamer-based players.
- **ESSOS Resource Manager** arbitrates the assignment and release of video, audio, and SVP resources and returns the concrete instance identifiers.
- **Resource usage** is resolved by the generic westeros sink component for video, the resource-owning audio element for audio, and `gst-svp-ext-common` for SVP. The generic westeros sink component handles all video ESSOS communication; the resource-owning audio element handles audio ESSOS communication. `gst-svp-ext-common` communicates with ESSOS for SVP allocation and passes the resolved usage and granted resource identifier to `gst-svp-ext-soc` for memory-pool binding.
- **Vendor plugins** bind the returned `assignedId` values and any GStreamer-context signalling to the correct hardware resources.

### Contract scope

This specification defines the contract between:

- **Application layer / playback owner**, which creates and tears down each GStreamer AV pipeline, owns application-level transition behaviour, and supplies each pipeline with GStreamer context information based on the capability YAML file.
- **SoC vendor**, which provides decoders, sinks, SVP handling, DRM integration, and resource-aware vendor plugins. Vendor components use resource-usage information from an explicitly set GStreamer property for westeros sink, or from player-supplied GStreamer context (for asymmetric video and SVP only), to select the required resource profiles. Direct ESSOS resource-management integration is the SoC vendor's responsibility for: (a) the resource-owning audio element for audio resources, which requests and releases audio resources following the first-available decoder model and stops using and releases them when ESSOS requests revocation; (b) SVP handling via `gst-svp-ext-soc`, which binds granted SVP resources to memory pools. Video and SVP ESSOS communication is handled by the generic westeros sink component and `gst-svp-ext-common`, respectively.
- **Vendor layer team**, which integrates the SoC delivery, capability YAML, and ESSOS resource-manager configuration.

---

## 1. Purpose

This specification defines how concurrent playback extends the legacy AV HAL surface without redesigning the legacy GStreamer pipeline model.

The goal is to support multiple playback pipelines in a single application process while preserving existing single-pipeline behaviour unless explicitly extended by this document.

The contract covers:

- existing resource-usage properties and GStreamer context;
- capability declaration and resource identification;
- selection of high-specification and low-specification video and SVP resources;
- video-decoder, audio-decoder and SVP resource allocation rules; and
- behaviour during transitions between GStreamer-based applications and non-GStreamer AV players.

## 2. Roles and responsibilities

| Role | Definition | Responsibilities |
|---|---|---|
| **Application layer / playback owner** | Owns the GStreamer-based playback pipeline and player transition logic. | Builds playback pipelines, tears down exiting pipelines, sets sink properties, propagates GStreamer context, and coordinates player transitions. Implements the generic westeros sink component, `gst-svp-ext-common` changes, OCDM adaptor changes, and SVP payload and decryptor plugin changes. |
| **SoC vendor** | Provides vendor GStreamer elements and platform resource integration. | Implements the westeros sink SoC component, audio sink/audio decoder, `gst-svp-ext-soc`, DRM integration, assigned-resource binding, and AV sync for dual decode. |
| **Vendor layer team** | Integrates the SoC delivery into the platform image. | Supplies capability YAML, ESSOS resource-manager configuration, and any platform-specific resource identifiers consumed by application and vendor plugins. |

## 3. Scope

### In scope

- Single application process owning up to two playback pipelines.
- Pipeline independence for play, pause, seek, and trick mode.
- Independent AV sync per pipeline.
- Capability reporting for concurrent decode, mixing, SVP, and DRM.
- Existing resource-selection semantics using `res-usage` for asymmetric video-decoder selection.
- ESSOS-managed video-decoder, audio-decoder, and SVP resources.
- DRM-session expectations for concurrent playback.
- Resource-acquisition rules during AV player transitions.

### Out of scope

- Platform topics unrelated to decoder, audio, SVP, DRM, and transition handling.
- Metrics collection and Metrics HAL behaviour.
- Frame-capture behaviour and APIs.
- A redesign of the legacy GStreamer pipeline topology.

## 4. Architecture facts and assumptions

### 4.1 Architecture facts

- A single application process may own multiple playback pipelines.
- Maximum of two playback sessions in one application process.
- Each pipeline operates independently and keeps its own play, pause, seek, and trick-mode state.
- AV sync is maintained independently inside each pipeline. No AV synchronisation is required between pipelines.
- During dual decode, AV sync remains the responsibility of the SoC vendor implementation.
- Existing SoC-vendor behaviour for a single playback pipeline remains unchanged unless explicitly extended by this specification.

### 4.2 Assumptions

- Legacy pipeline construction remains unchanged.
- New functionality is delivered primarily through sink properties, GStreamer context, capability YAML, `gst-svp-ext-soc`/`gst-svp-ext-common` API changes, OCDM adaptor API changes, and existing middleware orchestration.
- During dual decode, the SoC vendor may assume that the corresponding audio and video sinks belong to the same process.
- Where helpful for sink association inside the same playbin, the SoC vendor may use `gst_element_parent()` to locate the corresponding audio and video sinks that require AV synchronisation.
- Unsupported capability requests must fail positively rather than producing undefined behaviour.
- Capability reporting is available before runtime configuration occurs.

### 4.3 Capability reporting

Capability YAML shall report at least the following platform capabilities when supported:

- dual video decode;
- dual audio decode;
- audio mixing support;
- dual SVP support; and
- dual DRM support.

The capability YAML and ESSOS resource-manager configuration shall refer consistently to the same logical resources, including video decoder, audio decoder, video sink, audio sink, SVP allocator, and DRM-related platform identifiers where applicable.

### 4.4 Capability YAML

[🔴 **TBC** - Link to Capability schema to be added]

Capability YAML shall describe the declared resource model and list the supported concurrent-playback features together with the platform-specific resource classes used by the middleware.

It shall distinguish whether dual video decode, dual audio decode, audio mixing, dual SVP support, and dual DRM support are present for the relevant video and audio paths.

Example structure (🔴 **TBC**):

```yaml
capabilities:
  dualVideoDecode: true
  dualAudioDecode: true
  audioMixing: true
  dualSvpSupport: true
  dualDrmSupport: true
  videoDecoderResources:
    primary: high-spec
    secondary: low-spec
  svpResources:
    primary: high-bandwidth
    secondary: standard-bandwidth
```

The capability description and the ESSOS resource-manager configuration shall use the same logical names for the primary and secondary resource pools. Where a platform exposes limits on simultaneous playback, those limits shall be stated in the capability model and reflected in the accepted runtime transitions.

### 4.5 ESSOS Resource Manager

ESSOS Resource Manager is the platform arbitration layer for the video-decoder/display, audio-decoder/audio-output, and SVP-allocator paths.

The application layer and vendor plugins shall request an assigned resource instance, apply the returned `assignedId`, release it during teardown, and destroy the ESSOS connection only when the resource-manager path is no longer needed.

The generic westeros sink component for video, the resource-owning SoC-vendor audio element for audio, and `gst-svp-ext-common` for SVP shall handle the full ESSOS interaction model for their respective resources: request, grant handling, release, and revoke handling where arbitration requires it. The vendor video and SVP components shall bind the granted resources and relinquish them when instructed by the component handling ESSOS communication.

[🔴 **TBC** - Documentation of ESSOS Resource Manager APIs to be added; see https://github.com/rdkcmf/westeros/blob/master/essos/essos-resmgr.h]

The platform-specific configuration shall document the concrete resource identifiers and any limits that apply to concurrent audio and video acquisition. Where the platform exposes asymmetric pools, the configuration shall clearly distinguish the relevant high-specification and low-specification pools.

## 5. GStreamer context and `res-usage` documentation

This specification documents the existing `res-usage` flag and GStreamer context usage for resource selection. These mechanisms apply **only when resources are asymmetric**. For symmetric resources, context is not used for selection.

### 5.1 Existing `res-usage` flag

`res-usage` is an existing westeros sink property used to distinguish resource profiles for **asymmetric video decoder platforms only**. GStreamer context carries resource-specific usage values for video and SVP when platforms expose asymmetric resources. This specification introduces no additional GStreamer properties.

| Property | Type | Purpose | Required behaviour |
|---|---|---|---|
| `res-usage` | Enum | Identifies the intended resource profile for the video path on asymmetric decoder platforms. | On platforms with asymmetric video decoders, the property value shall distinguish primary and secondary resource usage and shall participate in video-decoder selection as defined in Section 7. For symmetric platforms, `res-usage` may be ignored or treated as metadata. |

### 5.2 Property and context application timing (asymmetric platforms only)

- For **asymmetric video decoders**: The video property and resource-usage context shall be available before the owning element resolves usage and requests a resource.
- For **asymmetric SVP resources**: SVP usage context shall be resolved before secure-buffer allocation.

### 5.3 Context decision tree and element coverage

The owning component shall resolve usage according to Sections 7, 8, and 10. The selection rules differ between asymmetric and symmetric resources:

**For asymmetric resources (video decoders, asymmetric SVP):**
- Explicit property (`res-usage`) or GStreamer context values override defaults
- An absent context value retains the applicable element default (first available resource)
- Every selection path requests an ESSOS-managed resource and uses the granted `assignedId` for hardware binding

**For symmetric resources (audio decoders, symmetric SVP):**
- No context-based selection occurs
- The first available symmetric resource is selected automatically
- Context, if present, is ignored for audio (which has no concept of usage profiles); symmetric SVP similarly ignores context and always selects the first available allocator
- Every resource request uses the ESSOS-managed grant process and the returned `assignedId` for hardware binding

## 6. Resource identification model

Resource selection across middleware and vendor plugins relies on consistent identifiers and usage signalling.

The platform shall provide capability YAML, ESSOS resource-manager configuration, and a stable mapping between logical usage and the assigned `assignedId` values used by vendor plugins.

Usage identifies the requested resource profile, not a granted instance. ESSOS assigns the concrete video, audio, or SVP resource; the vendor implementation binds the returned `assignedId`. Context field names and the SVP handoff API remain 🔴 **TBC** where not already defined by the implementation.

## 7. Video decoder and video plane resource model

### 7.1 General requirements

All AV players supporting dual video decode shall use the application-managed GStreamer path for the concurrent-playback use case.

Video decoder resource model depends on whether resources are asymmetric or symmetric:

- **Asymmetric video decoders**: The platform supports selection between a high-specification video decoder path and a low-specification video decoder path. Resource selection is driven by `res-usage` property and GStreamer context.
- **Symmetric video decoders**: All video decoders are symmetric; the first available decoder is assigned automatically without usage signalling.

### 7.2 `res-usage` signalling and precedence (asymmetric platforms only)

**This section applies only to platforms with asymmetric video decoder resources.**

Westeros sink shall resolve video usage in the following order:

| Priority | Condition | Selected usage |
|---|---|---|
| 1 | `res-usage` was explicitly set on this sink instance. | Property value, including an explicitly set default value. |
| 2 | The property was not explicitly set and GStreamer context supplies video `res-usage`. | Context value. |
| 3 | Neither supplies a value. | Existing element default: the first available video decoder resource. |

For platforms with **symmetric video decoders**, the first available resource is selected automatically without regard to `res-usage` or context values.

### 7.3 Resource-class mapping (asymmetric platforms only)

**This section applies only to platforms with asymmetric video decoder resources.**

For platforms implementing asymmetric video resources:

- `res-usage = 7` indicates the high-specification decoder profile using full resolution, full quality, and full performance.
- `res-usage = 0` indicates the low-specification decoder profile.

The generic westeros sink component shall track whether the property was explicitly set. It shall store the resolved value in `GstWesterosSink.resUsage`, use it when requesting a resource from ESSOS, and pass the structure pointer to the SoC module through `gst_westeros_sink_soc_init()`. The SoC implementation shall use this value to distinguish primary and secondary decoder profiles and bind the video-decoder instance granted through the generic component.

The `res-usage` value shall be established before the pipeline transitions from `NULL` to `READY`, so that decoder selection can complete when the resource-owning element first requests the video decoder.

### 7.4 ESSOS interaction

The generic westeros sink component of the westeros sink plugin shall handle all ESSOS Resource Manager communication for video resources, including connection management, resource requests, grant callbacks, release, and revoke handling. The SoC component shall not communicate directly with ESSOS for video resources.

The generic component shall create its ESSOS Resource Manager connection once for the lifetime of the playback pipeline and shall request the video decoder during the GStreamer `NULL` to `READY` transition.

ESSOS Resource Manager shall return the appropriate video-decoder instance identifier through `assignedId` in the resource request result, either immediately from the request call or asynchronously through the registered notify callback.

The generic component shall pass the returned `assignedId` to the SoC component, which shall use it to open the concrete video-decoder instance.

If ESSOS later issues a revoke event for that decoder, the generic component shall coordinate with the SoC component to stop using the decoder and release any vendor-side resources associated with it, then call `EssRMgrReleaseResource()` before another player is granted the same decoder.

When playback is torn down and ESSOS communication is no longer required, the generic component shall destroy its ESSOS Resource Manager connection.

For legacy AV players not using the application-managed GStreamer path:

- only one video decoder is typically requested;
- usage selection shall follow the precedence and default rules in Section 7.2.

### 7.5 Video plane relationship

The high-specification video decoder is expected to pair with the high-specification video plane and PQ pipeline.

The low-specification decoder and associated plane may omit support for Dolby Vision or other PQ features on some platforms. That limitation shall be reflected in platform capability reporting.

### Example ESSOS configuration entries

For an asymmetric video configuration, the `essrmgr.conf` file may contain entries in the following form:

```text
video: hardware
video: hardware, limitedQuality, limitedPerformance
```

The first entry represents the high-specification decoder profile; the second represents a low-specification profile with quality and performance restrictions. Platform-specific resolution limits and supported PQ features shall be documented consistently with Section 7.5 and capability YAML.

For a symmetric video configuration, the `essrmgr.conf` file may contain entries in the following form:

```text
video: hardware
video: hardware
```

Both entries represent equivalent video decoder resources; the first available decoder is assigned without regard to usage signalling.

## 8. Audio decoder resource model

### 8.1 General requirements

All AV players supporting dual audio decode shall use the application-managed GStreamer path for the concurrent-playback use case.

Audio decoder resources are assumed to be symmetric (this specification does not define asymmetric audio decoder resource handling). Symmetric audio decoders do not use GStreamer context or usage signalling for selection; the first available audio decoder resource is selected automatically.

The audio sink or audio decoder that owns the decoder resource shall request the corresponding resource from ESSOS without context-based selection. The owning element shall not propagate GStreamer context for audio resource selection. ESSOS shall grant the first available audio-decoder instance.

### 8.2 ESSOS Resource Manager configuration

The ESSOS Resource Manager configuration shall advertise two available audio-decoder resources.

### 8.3 Example ESSOS configuration entries

The `essrmgr.conf` file is expected to contain two audio resource entries in the following form (🔴 **TBC**):

```text
audio: hardware
audio: hardware
```

### 8.4 ESSOS configuration and allocation

ESSOS shall allocate an instance compatible with the resolved audio usage and platform configuration. Symmetric decoder instances may satisfy the same profile; allocation shall not depend on a fixed first-request/first-decoder ordering.

### 8.5 Vendor plugin behaviour

The resource-owning audio element shall bind the granted `assignedId` to the hardware decoder. The mapping remains private to the vendor implementation.

The audio resource-owning element shall follow the same ESSOS communication lifecycle as the video case: create its ESSOS connection once for the lifetime of the playback pipeline, request the audio decoder during `NULL` to `READY`, accept either an immediate `assignedId` or an asynchronous callback grant, and release the resource with `EssRMgrReleaseResource()` when playback stops or when a revoke event is received.

When ESSOS communication is no longer required for that pipeline, the element shall destroy its ESSOS connection.

For legacy AV players not using the application-managed GStreamer path, only one audio decoder is typically requested.

## 9. Audio mixer resource model

### 9.1 Process-scoped mixer ownership

Audio-mixer usage for main and secondary audio inputs is tied to the Linux process that is currently using the mixer resource, such as the application process or a dedicated playback process.

For the purposes of this specification, process ownership applies to the media-stream inputs that feed the main and secondary mixer paths. It does not change the behaviour of other platform audio sources that are already allowed to mix through the existing audio system.

### 9.2 Behaviour during process handover

When a new Linux process starts using the audio mixer resource, the SoC vendor implementation shall ensure that any audio data still being injected by another process for primary or secondary playback inputs is ignored and is not mixed into the audible output.

This requirement applies to transitions involving both of these process categories:

- an application process using the legacy playback path; and
- a separate playback process using the GStreamer-managed playback path.

The purpose of this rule is to prevent stale or overlapping playback audio from an exiting process from leaking into the mixer while the entering process takes ownership of the playback audio path.

### 9.3 Non-playback audio sources

Application audio, System Audio, and TTS audio shall continue to be handled according to the existing platform mixing behaviour.

These audio classes are not excluded by the process-scoped ownership rule for main and secondary playback inputs. Any such input from any Linux process may continue to be mixed as supported by the platform's existing audio policy.

### 9.4 Conformance requirement

The SoC vendor shall provide mixer-input isolation such that a new owner of the playback mixer path cannot receive residual main or secondary audio injected by an older process once handover has occurred.

## 10. SVP allocator resource model

### 10.1 General requirements

All AV players supporting dual secure-video paths shall use the application-managed GStreamer path for the concurrent-playback use case.

Platforms shall declare whether their SVP allocator resources are asymmetric or symmetric.

**GStreamer context for SVP usage applies only to platforms with asymmetric SVP resources.** For symmetric SVP resources, context is not propagated and the first available allocator is selected automatically.

#### Asymmetric SVP platforms only:

The application shall propagate GStreamer context to `gst-svp-ext-common` through one of the following paths:

- Application -> decryptor -> OCDM adaptor -> `gst-svp-ext-common`.
- Application -> SVP payload -> `gst-svp-ext-common`.

The components in the selected path shall forward the context before SVP usage is resolved and before secure-buffer allocation.

`gst-svp-ext-common` shall resolve SVP usage as follows:

| Condition | Selected usage |
|---|---|
| GStreamer context supplies SVP resource usage (asymmetric platform only). | Context value. |
| No SVP usage value is supplied (asymmetric platform). | Existing element default: the first available SVP resource. |

#### Symmetric SVP platforms:

For symmetric SVP platforms, `gst-svp-ext-common` shall:
- Not use context-based selection
- Always request the first available allocator from ESSOS without usage signalling
- Ignore any GStreamer context values propagated by the application

#### All SVP platforms (symmetric and asymmetric):

`gst-svp-ext-common` shall request the corresponding resource from ESSOS. It shall pass the resolved usage and granted resource identifier to `gst-svp-ext-soc` through an API to be defined (🔴 **TBC**). The SoC module shall use the granted instance to select the SVP memory pool.

The SVP resource-owning element shall create its ESSOS connection once for the lifetime of the playback pipeline, request the SVP allocator during the initial pipeline-establishment path before secure playback depends on that allocator, and handle grant or revoke using the same ESSOS request/callback/release pattern used for video.

### 10.2 ESSOS Resource Manager configuration

The ESSOS Resource Manager configuration shall advertise two SVP allocator resources for the dual-decode use case.

### 10.3 Example ESSOS configuration entries

For a symmetric configuration, the `essrmgr.conf` file is expected to contain two SVP resource entries in the following form:

```text
svpa: hardware
svpa: hardware
```

### 10.4 Asymmetric SVP resources (asymmetric platforms only)

**This section applies only to platforms with asymmetric SVP allocator resources.**

On asymmetric platforms, the resolved SVP usage shall distinguish the required memory-pool profile. GStreamer context is used to signal the requested resolution profile to `gst-svp-ext-common`.

The recommended logical usage values are:

- `EssRMgrSVPAUse_fullResolution` for the UHD-capable pool; and
- `EssRMgrSVPAUse_none` for the FHD-capable pool.

On such platforms:

- ESSOS resource-manager configuration shall define both the UHD SVP pool resource and the FHD SVP pool resource;
- `gst-svp-ext-common` shall resolve usage from GStreamer context and forward it to `gst-svp-ext-soc` as defined in Section 10.1; and
- `gst-svp-ext-soc` shall bind the ESSOS-granted resource to the corresponding memory pool based on the resolved usage.

An asymmetric configuration may, for example, contain entries in `essrmgr.conf` in the following form:

```text
svpa: hardware
svpa: hardware, limitedResolution(1920, 1080)
```

### 10.5 SVP resource lifecycle

The SVP resource-owning element (`gst-svp-ext-common`) shall create its ESSOS connection once for the lifetime of the playback pipeline, request the SVP allocator during the initial pipeline-establishment path before secure playback depends on that allocator, and handle grant or revoke using the same ESSOS request/callback/release pattern used for video.

The element shall release the allocator with `EssRMgrReleaseResource()` when playback stops or a revoke event is received, and destroy the ESSOS connection only when resource-manager communication is no longer needed.

### 10.6 Legacy behaviour

For legacy AV players not using the application-managed GStreamer path:

- only one SVP allocator is typically requested; and
- absent usage signalling, the default is the first available SVP resource.

### 10.7 Extensions to ESSOS Resource Manager for asymmetric SVP resources

Asymmetric SVP resource platforms require extensions to the ESSOS Resource Manager to support resolution-aware allocation and usage signalling.

**Changes required in ESSOS Resource Manager implementation** (see https://github.com/rdkcmf/westeros/blob/master/essos/):

- Extend capability declarations to distinguish UHD-capable and FHD-capable SVP allocator resources
- Implement usage-based allocation matching the requested resolution profile to the available memory pool
- Support the `EssRMgrSVPAUse_fullResolution` and `EssRMgrSVPAUse_none` usage values or platform-defined equivalents
- Ensure configuration parsing recognizes the `limitedResolution(width, height)` capability modifier in `essrmgr.conf`

**GStreamer component expectations:**

- `gst-svp-ext-common` resolves usage based on GStreamer context and passes it to `gst-svp-ext-soc`
- `gst-svp-ext-soc` receives the usage value and the ESSOS-granted `assignedId` and binds the returned allocator instance to the corresponding memory pool
- Platforms without asymmetric SVP resources shall treat all allocators as symmetric and ignore usage signalling for pool selection

API details and integration patterns with `gst-svp-ext-soc` remain 🔴 **TBC**.

### 10.8 Extensions to the OCDM Adaptor, gst-svp-ext-common, and gst-svp-ext-soc

API changes shall support propagation of GStreamer context through the OCDM adaptor to `gst-svp-ext-common` and passing resolved SVP usage and the granted resource identifier from `gst-svp-ext-common` to `gst-svp-ext-soc`. The API signatures are 🔴 **TBC**; this specification does not define new function prototypes.

### 10.9 GStreamer context structure for carrying SVP pool information

The SVP context structure and field names are 🔴 **TBC**. The context shall carry the requested usage; ESSOS supplies the granted resource identifier.

## 11. DRM resource model

### 11.1 General requirements

Dual-DRM support applies to both video and audio paths. In a dual-decode implementation, the platform may therefore require up to four concurrent DRM sessions: one for the primary video path, one for the primary audio path, one for the secondary video path, and one for the secondary audio path.

DRM resources are not managed by ESSOS Resource Manager.

### 11.2 Provisioning expectations

The platform shall provision the relevant key identifiers for the playback session before the GStreamer pipeline is constructed.

This applies equally to encrypted video and encrypted audio data. Encrypted media passed during decryption shall carry sufficient key-identification information for the DRM implementation to select the correct decryption key.


## 12. AV player transitions and resource acquisition

### 12.1 General transition rules

Resource ownership shall be exclusive at any given time. For GStreamer-managed transitions, the application manager orders teardown of the exiting pipeline before startup of the entering one. For non-GStreamer players, the existing platform path remains responsible for AV ownership, with ESSOS arbitration only when a conflicting request arrives before teardown completes.

Even when application-level sequencing reduces contention between GStreamer-managed players, ESSOS arbitration support remains mandatory for non-GStreamer competitors such as HDMI In, Composite In, and AirPlay where request timing cannot be assumed to be orderly.

In all cases, the exiting player must release the relevant video and audio resources before the entering player can acquire them. When the playback path allocates SVP resources, those SVP resources shall also follow the same ESSOS-managed release and arbitration model.

If ESSOS revokes one video resource during handover, the platform shall revoke the remaining video resources owned by the exiting player in the same transition to avoid mixed video ownership. Audio-mixer ownership remains process-scoped and follows Section 9.

Within one AV pipeline, `EssRMgrCreate()` is called once during playback setup; the owning element requests the resource during `NULL` to `READY`, and `EssRMgrDestroy()` occurs only when ESSOS communication is no longer needed.

### 12.2 GStreamer-based application to GStreamer-based application

For transitions between two GStreamer-based applications, two distinct paths are supported:

**Orderly-release path (normal case):**
- the application manager orders teardown of the exiting pipeline and launch of the entering one;
- the exiting pipeline releases video, audio, and SVP resources before the entering pipeline requests them; and
- the entering pipeline waits for the release confirmation before it begins its next resource acquisition.

**Disorderly-release path (exception/fallback):**
- when the exiting application does not complete teardown before the entering application requests resources;
- ESSOS arbitration is invoked and may revoke resources from the exiting player;
- this path is supported to handle race conditions and non-compliant applications.

Both orderly-release and disorderly-release paths shall be supported. Where the transition includes secure playback, SVP resources shall follow the same ESSOS-managed grant, revoke, and release behaviour as other managed resources.

Orderly-release path, where the exiting application releases resources before the entering application requests them:

```mermaid
sequenceDiagram
    autonumber
    participant AM as Application manager
    participant APP1 as Entering GStreamer<br/>Application Process
    participant G1 as GStreamer pipeline<br/>A/B
    participant V1 as Entering Video<br/>Element A/B
    participant A1 as Entering Audio<br/>Element A/B
    participant S1 as Entering gst-svp-ext-common A/B
    participant RM as ESSOS Resource<br/>Manager
    participant APP2 as Exiting GStreamer<br/>Application Process
    participant G2 as GStreamer pipeline<br/>A/B
    participant V2 as Exiting Video<br/>Element A/B
    participant A2 as Exiting Audio<br/>Element A/B
    participant S2 as Exiting gst-svp-ext-common A/B

    AM->>APP2: stop exiting GStreamer-based application
    APP2->>G2: stop exiting pipeline A/B
    G2->>V2: stop playback and release video resources A/B
    V2->>RM: EssRMgrReleaseResource(video A/B)
    G2->>A2: release audio resources A/B
    A2->>RM: EssRMgrReleaseResource(audio A/B)
    G2->>S2: release SVP resources A/B
    S2->>RM: EssRMgrReleaseResource(svp A/B)
    APP2-->>AM: resource release complete

    AM->>APP1: activate entering GStreamer-based application
    APP1->>G1: activate entering pipeline A/B
    G1->>V1: initialise pipeline A/B
    V1->>RM: EssRMgrCreate(notifyCB)
    G1->>V1: transition NULL -> READY
    V1->>RM: EssRMgrRequestResource(video A/B, notifyCB)
    RM-->>V1: request result or notifyCB(EssRMgrEvent::Granted, assignedId)
    G1->>A1: initialise audio path A/B
    A1->>RM: EssRMgrCreate(notifyCB)
    G1->>A1: transition NULL -> READY
    A1->>RM: EssRMgrRequestResource(audio A/B, notifyCB)
    RM-->>A1: request result or notifyCB(EssRMgrEvent::Granted, assignedId)
    G1->>S1: initialise SVP path A/B
    S1->>RM: EssRMgrCreate(notifyCB)
    G1->>S1: transition NULL -> READY
    S1->>RM: EssRMgrRequestResource(svp A/B, notifyCB)
    RM-->>S1: request result or notifyCB(EssRMgrEvent::Granted, assignedId)
    APP1-->>AM: acquisition complete
```

Disorderly-release path, where the entering GStreamer-based application requests AV and SVP resources before the exiting GStreamer-based application has fully released them:

```mermaid
sequenceDiagram
    autonumber
    participant AM as Application manager
    participant APP1 as Entering GStreamer<br/>Application Process
    participant G1 as GStreamer pipeline<br/>A/B
    participant V1 as Entering Video<br/>Element A/B
    participant A1 as Entering Audio<br/>Element A/B
    participant S1 as Entering gst-svp-ext-common A/B
    participant RM as ESSOS Resource<br/>Manager
    participant APP2 as Exiting GStreamer<br/>Application Process
    participant G2 as Exiting GStreamer pipeline<br/>A/B
    participant V2 as Exiting Video<br/>Element A/B
    participant A2 as Exiting Audio<br/>Element A/B
    participant S2 as Exiting gst-svp-ext-common A/B

    Note over V2,S2: Exiting GStreamer pipeline still owns AV and SVP resources while teardown is in progress
    Note over V1,S1: Entering GStreamer pipeline requests AV and SVP resources before exit teardown has completed

    AM->>APP1: activate entering GStreamer-based application before exit teardown completes
    APP1->>G1: activate entering pipeline A/B
    G1->>V1: request video resources A/B
    V1->>RM: EssRMgrRequestResource(video A/B, notifyCB)
    RM->>V2: revoke both exiting video resources and reassign them to entering pipeline A/B
    RM-->>V2: notifyCB(EssRMgrEvent::Revoked)
    V2->>RM: EssRMgrReleaseResource(video primary)
    V2->>RM: EssRMgrReleaseResource(video secondary)
    RM-->>V1: notifyCB(EssRMgrEvent::Granted, assignedId)

    G1->>A1: request audio resources A/B
    A1->>RM: EssRMgrRequestResource(audio A/B, notifyCB)
    RM-->>A2: notifyCB(EssRMgrEvent::Revoked)
    A2->>RM: EssRMgrReleaseResource(audio A/B)
    RM-->>A1: notifyCB(EssRMgrEvent::Granted, assignedId)

    G1->>S1: request SVP resources A/B
    S1->>RM: EssRMgrRequestResource(svp A/B, notifyCB)
    RM-->>S2: notifyCB(EssRMgrEvent::Revoked)
    S2->>RM: EssRMgrReleaseResource(svp A/B)
    RM-->>S1: notifyCB(EssRMgrEvent::Granted, assignedId)

    APP2->>G2: exiting teardown completes after arbitration
```

### 12.3 GStreamer-based application to non-GStreamer-based AV player

For transitions from a GStreamer-based application to a non-GStreamer player, the resource-safe behaviour is the same as the GStreamer-to-GStreamer case: the exiting pipeline must release the AV resources before the entering player can acquire them.

The platform shall support an orderly release path, and if the non-GStreamer player arrives before teardown completes, ESSOS Resource Manager may arbitrate the handover and revoke the exiting AV resources.

Orderly-release path, where the exiting GStreamer-based player releases resources before the entering non-GStreamer player requests them:

```mermaid
sequenceDiagram
    autonumber
    participant AM as Application manager
    participant APP_GST_EXIT as Exiting GStreamer<br/>Application Process
    participant G_GST_EXIT as GStreamer pipeline<br/>A/B
    participant V_GST_EXIT as Exiting Video<br/>Element A/B
    participant A_GST_EXIT as Exiting Audio<br/>Element A/B
    participant S_GST_EXIT as Exiting gst-svp-ext-common A/B
    participant RM as ESSOS Resource<br/>Manager
    participant APP_NG as Non-GStreamer<br/>AV Player

    Note over APP_GST_EXIT: Exiting GStreamer-based application is shutting down cleanly
    Note over APP_NG: Entering non-GStreamer player is launched only after the GStreamer teardown is complete

    AM->>APP_GST_EXIT: stop exiting GStreamer-based application
    APP_GST_EXIT->>G_GST_EXIT: stop exiting pipeline A/B
    G_GST_EXIT->>V_GST_EXIT: stop playback and release video resources A/B
    V_GST_EXIT->>RM: EssRMgrReleaseResource(video A/B)
    G_GST_EXIT->>A_GST_EXIT: release audio resources A/B
    A_GST_EXIT->>RM: EssRMgrReleaseResource(audio A/B)
    G_GST_EXIT->>S_GST_EXIT: release SVP resources A/B
    S_GST_EXIT->>RM: EssRMgrReleaseResource(svp A/B)
    APP_GST_EXIT-->>AM: resource release complete

    AM->>APP_NG: launch non-GStreamer player
    APP_NG->>RM: EssRMgrRequestResource(video, notifyCB)
    RM-->>APP_NG: request result or notifyCB(EssRMgrEvent::Granted, assignedId)
    APP_NG->>RM: EssRMgrRequestResource(audio, notifyCB)
    RM-->>APP_NG: request result or notifyCB(EssRMgrEvent::Granted, assignedId)
```

Disorderly-release path, where the entering non-GStreamer player requests AV resources before the exiting GStreamer-based application has fully released them:

```mermaid
sequenceDiagram
    autonumber
    participant AM as Application manager
    participant RM as ESSOS Resource<br/>Manager
    participant APP_GST_EXIT as Exiting GStreamer<br/>Application Process
    participant G_GST_EXIT as Exiting GStreamer pipeline<br/>A/B
    participant V_GST_EXIT as Exiting Video<br/>Element A/B
    participant A_GST_EXIT as Exiting Audio<br/>Element A/B
    participant S_GST_EXIT as Exiting gst-svp-ext-common A/B
    participant APP_NG as Non-GStreamer<br/>AV Player

    Note over APP_GST_EXIT: Exiting GStreamer pipeline is still tearing down and has not yet fully released all AV resources
    Note over APP_NG: Entering non-GStreamer player requests video and audio resources sequentially

    AM->>APP_NG: launch non-GStreamer player before GStreamer teardown has completed
    APP_NG->>RM: EssRMgrRequestResource(video, notifyCB)
    RM->>V_GST_EXIT: revoke both exiting video resources and reassign them to the entering player
    V_GST_EXIT-->>RM: notifyCB(EssRMgrEvent::Revoked)
    V_GST_EXIT->>RM: EssRMgrReleaseResource(video primary)
    V_GST_EXIT->>RM: EssRMgrReleaseResource(video secondary)
    RM-->>APP_NG: request result or notifyCB(EssRMgrEvent::Granted, assignedId)

    APP_NG->>RM: EssRMgrRequestResource(audio, notifyCB)
    RM->>A_GST_EXIT: revoke or reassign the exiting audio resource to the entering player
    A_GST_EXIT-->>RM: revoke callback and release in progress
    RM-->>APP_NG: request result or notifyCB(EssRMgrEvent::Granted, assignedId)

    APP_GST_EXIT->>G_GST_EXIT: final teardown of exiting pipeline completes after arbitration
    G_GST_EXIT->>A_GST_EXIT: release remaining audio references
    A_GST_EXIT->>RM: EssRMgrReleaseResource(audio A/B)
    G_GST_EXIT->>S_GST_EXIT: release remaining SVP references
    S_GST_EXIT->>RM: EssRMgrReleaseResource(svp A/B)
```

### 12.4 Non-GStreamer-based AV player to GStreamer-based application

For transitions from a non-GStreamer player to a GStreamer-based application:

- the exiting non-GStreamer player releases AV resources through the current platform path when it is torn down;
- the entering GStreamer-based application does not rely on a delayed-release path; and
- if the non-GStreamer player is still active when the GStreamer application requests resources, ESSOS Resource Manager arbitrates the handover and revokes the exiting AV resources before granting the new owner.

The orderly-release path remains required because non-GStreamer applications such as HDMI In or Composite In may still be active while the GStreamer-based application starts.

Orderly-release path, where the exiting non-GStreamer player releases resources before the entering GStreamer-based application requests them:

```mermaid
sequenceDiagram
    autonumber
    participant AM as Application manager
    participant APP_NG as Exiting Non-GStreamer<br/>AV Player
    participant RM as ESSOS Resource<br/>Manager
    participant APP_GST_ENTER as Entering GStreamer<br/>Application Process
    participant G_GST_ENTER as GStreamer pipeline<br/>A/B
    participant V_GST_ENTER as Entering Video<br/>Element A/B
    participant A_GST_ENTER as Entering Audio<br/>Element A/B
    participant S_GST_ENTER as Entering gst-svp-ext-common A/B

    Note over APP_NG: Exiting non-GStreamer player is still the current owner of the audio and video resources
    Note over APP_GST_ENTER: Entering GStreamer-based application waits until the previous player has released ownership

    AM->>APP_NG: stop exiting non-GStreamer player
    APP_NG->>RM: EssRMgrReleaseResource(video)
    APP_NG->>RM: EssRMgrReleaseResource(audio)
    APP_NG-->>AM: resource release complete

    AM->>APP_GST_ENTER: launch incoming GStreamer-based application
    APP_GST_ENTER->>G_GST_ENTER: activate entering pipeline A/B
    G_GST_ENTER->>V_GST_ENTER: initialise video path A/B
    V_GST_ENTER->>RM: EssRMgrCreate(notifyCB)
    V_GST_ENTER->>RM: EssRMgrRequestResource(video A/B, notifyCB)
    RM-->>V_GST_ENTER: request result or notifyCB(EssRMgrEvent::Granted, assignedId)

    G_GST_ENTER->>A_GST_ENTER: initialise audio path A/B
    A_GST_ENTER->>RM: EssRMgrCreate(notifyCB)
    A_GST_ENTER->>RM: EssRMgrRequestResource(audio A/B, notifyCB)
    RM-->>A_GST_ENTER: request result or notifyCB(EssRMgrEvent::Granted, assignedId)
    G_GST_ENTER->>S_GST_ENTER: initialise SVP path A/B
    S_GST_ENTER->>RM: EssRMgrCreate(notifyCB)
    S_GST_ENTER->>RM: EssRMgrRequestResource(svp A/B, notifyCB)
    RM-->>S_GST_ENTER: request result or notifyCB(EssRMgrEvent::Granted, assignedId)
```

Disorderly-release path, where the entering GStreamer-based application requests AV resources before the exiting non-GStreamer-based player has fully released them:

```mermaid
sequenceDiagram
    autonumber
    participant AM as Application manager
    participant APP_GST_ENTER as Entering GStreamer<br/>Application Process
    participant G_GST_ENTER as GStreamer pipeline<br/>A/B
    participant V_GST_ENTER as Entering Video<br/>Element A/B
    participant A_GST_ENTER as Entering Audio<br/>Element A/B
    participant S_GST_ENTER as Entering gst-svp-ext-common A/B
    participant RM as ESSOS Resource<br/>Manager
    participant APP_NG as Exiting Non-GStreamer<br/>AV Player

    Note over APP_NG: Exiting non-GStreamer player still owns the current AV resources
    Note over APP_GST_ENTER: Entering GStreamer-based application requests AV resources while the non-GStreamer player is still unwinding

    AM->>APP_GST_ENTER: launch incoming GStreamer-based application before non-GStreamer teardown is complete
    APP_GST_ENTER->>G_GST_ENTER: activate entering pipeline A/B
    G_GST_ENTER->>V_GST_ENTER: request video resources A/B
    V_GST_ENTER->>RM: EssRMgrRequestResource(video A/B, notifyCB)
    RM->>APP_NG: revoke exiting video resource and reassign to the entering GStreamer-based application
    APP_NG-->>RM: notifyCB(EssRMgrEvent::Revoked)
    APP_NG->>RM: EssRMgrReleaseResource(video)
    RM-->>V_GST_ENTER: request result or notifyCB(EssRMgrEvent::Granted, assignedId)

    G_GST_ENTER->>A_GST_ENTER: request audio resources A/B
    A_GST_ENTER->>RM: EssRMgrRequestResource(audio A/B, notifyCB)
    RM->>APP_NG: revoke or reassign the exiting audio resource to the entering GStreamer-based application
    APP_NG-->>RM: revoke callback and release in progress
    RM-->>A_GST_ENTER: request result or notifyCB(EssRMgrEvent::Granted, assignedId)

    G_GST_ENTER->>S_GST_ENTER: request SVP resources A/B
    S_GST_ENTER->>RM: EssRMgrRequestResource(svp A/B, notifyCB)
    RM-->>S_GST_ENTER: request result or notifyCB(EssRMgrEvent::Granted, assignedId)
```

### 12.5 Non-GStreamer-based AV player to non-GStreamer-based AV player

For transitions between two non-GStreamer-based AV players, release occurs through the existing platform method.

This specification does not require application-managed GStreamer coordination in that case, but the platform shall still preserve exclusive ownership of the relevant AV resources.

The transition shall work according to the current non-GStreamer model already in place on the platform.

Orderly-release path, where the exiting non-GStreamer player releases AV resources before the entering non-GStreamer player requests them:

```mermaid
sequenceDiagram
    autonumber
    participant AM as Application manager
    participant APP_NG_EXIT as Exiting Non-GStreamer<br/>AV Player
    participant RM as ESSOS Resource<br/>Manager
    participant APP_NG_ENTER as Entering Non-GStreamer<br/>AV Player

    Note over APP_NG_EXIT: Exiting non-GStreamer player owns current AV resources
    Note over APP_NG_ENTER: Entering non-GStreamer player starts after release confirmation

    AM->>APP_NG_EXIT: stop exiting non-GStreamer player
    APP_NG_EXIT->>RM: EssRMgrReleaseResource(video)
    APP_NG_EXIT->>RM: EssRMgrReleaseResource(audio)
    APP_NG_EXIT-->>AM: resource release complete

    AM->>APP_NG_ENTER: launch entering non-GStreamer player
    APP_NG_ENTER->>RM: EssRMgrRequestResource(video, notifyCB)
    RM-->>APP_NG_ENTER: request result or notifyCB(EssRMgrEvent::Granted, assignedId)
    APP_NG_ENTER->>RM: EssRMgrRequestResource(audio, notifyCB)
    RM-->>APP_NG_ENTER: request result or notifyCB(EssRMgrEvent::Granted, assignedId)
```

Disorderly-release path, where the entering non-GStreamer player requests AV resources before the exiting non-GStreamer player has fully released them:

```mermaid
sequenceDiagram
    autonumber
    participant AM as Application manager
    participant APP_NG_ENTER as Entering Non-GStreamer<br/>AV Player
    participant RM as ESSOS Resource<br/>Manager
    participant APP_NG_EXIT as Exiting Non-GStreamer<br/>AV Player

    Note over APP_NG_EXIT: Exiting non-GStreamer player still owns AV resources
    Note over APP_NG_ENTER: Entering non-GStreamer player requests resources while exit teardown is still in progress

    AM->>APP_NG_ENTER: launch entering non-GStreamer player before exit teardown completes
    APP_NG_ENTER->>RM: EssRMgrRequestResource(video, notifyCB)
    RM->>APP_NG_EXIT: revoke exiting video resource and reassign to the entering player
    APP_NG_EXIT-->>RM: notifyCB(EssRMgrEvent::Revoked)
    APP_NG_EXIT->>RM: EssRMgrReleaseResource(video)
    RM-->>APP_NG_ENTER: request result or notifyCB(EssRMgrEvent::Granted, assignedId)

    APP_NG_ENTER->>RM: EssRMgrRequestResource(audio, notifyCB)
    RM->>APP_NG_EXIT: revoke or reassign exiting audio resource
    APP_NG_EXIT-->>RM: revoke callback and release in progress
    RM-->>APP_NG_ENTER: request result or notifyCB(EssRMgrEvent::Granted, assignedId)
```

## 13. Audio behaviour in dual-decode modes

This specification does not redefine the application-facing audio UX policy, but the platform shall support at least these behavioural classes when declared by capability YAML:

- **Dual decode + dual mix**: both audio streams decoded and mixed, with independent volume control.
- **Dual decode + single audible output**: both streams continue decoding, but one stream may be muted through the platform's existing audio policy while AV sync is preserved.
- **Single audio decode + dual video decode**: only the primary stream performs audio decode, while the platform still supports other platform audio inputs as defined by the existing mixer design.

## 14. Conformance and acceptance gates

An implementation is conformant only when all of the following are true:

| # | Requirement |
|---|---|
| 1 | Existing single-pipeline behaviour remains unchanged unless extended by this specification. |
| 2 | Up to two playback pipelines in one application process can be represented by capability reporting when supported. |
| 3 | `res-usage` is available early enough to select the appropriate video decoder before the `NULL` to `READY` resource request is issued. |
| 4 | On asymmetric video-decoder platforms, `res-usage` distinguishes the high-specification UHD path from the lower-specification FHD path. |
| 5 | ESSOS `assignedId` values are used by vendor plugins to open the selected video-decoder and audio-decoder instances. |
| 6 | The owning audio element requests the first available audio decoder resource from ESSOS (audio decoders are symmetric and do not use context-based selection). |
| 7 | Audio mixer ownership for main and secondary playback inputs is process-scoped, and an entering process is isolated from stale playback audio injected by an exiting process. |
| 8 | `gst-svp-ext-common` resolves context-supplied SVP usage (asymmetric platforms only) or the first available resource default (all platforms), and passes it to `gst-svp-ext-soc` for ESSOS allocation and pool binding. |
| 9 | Westeros sink resolves video usage in explicit-property, context (asymmetric platforms only), then element-default order and passes `GstWesterosSink.resUsage` to the SoC module. |
| 10 | DRM-session provisioning occurs before pipeline construction and supports the platform's declared concurrent playback model, including sufficient key sessions for concurrent encrypted audio and video streams. |
| 11 | Transition handling prevents mixed ownership that would produce overlapping or blended video from exiting and entering players. |
| 12 | Audio from an exiting process is ignored once the mixer is in use by the entering process, consistent with process-scoped mixer ownership. |
| 13 | ESSOS Resource Manager support is available for switching between non-GStreamer-based applications and between non-GStreamer-based and GStreamer-based applications, including the revoke-driven arbitration path required when the competing application requests video or audio resources before teardown completes. |
| 14 | Vendor-level validation demonstrates HDMI In, Composite In, and equivalent non-GStreamer contention cases against GStreamer pipelines, including orderly acquisition, revoke-driven arbitration, and clean release. |
| 15 | Hardware-backed vendor elements that consume managed resources demonstrate request, grant, release, and revoke behaviour consistent with the ESSOS model defined in this specification. |
| 16 | Capability YAML and ESSOS resource-manager configuration agree on logical resource pools, concurrent-playback limits, and asymmetric/symmetric declarations for video and SVP allocators. |
| 17 | Independent per-pipeline AV sync, play/pause/seek, and trick-mode behavior are maintained; each pipeline operates autonomously for media control. |
| 18 | Symmetric video, audio, and SVP default behavior (first-available selection) is correctly implemented; asymmetric video and SVP context-based selection is correctly implemented. |
| 19 | SVP acquisition, release, and revoke behavior during both orderly and disorderly transitions follows the ESSOS-managed pattern described in Sections 10 and 12. |
| 20 | DRM coverage for concurrent encrypted audio and video is verified, including the stated maximum of four key sessions where required by the platform. |
| 21 | The three audio behavior modes declared in Section 13 (dual decode + dual mix, dual decode + single audible output, single audio decode + dual video decode) are supported as declared in capability YAML and demonstrated in validation. |
| 22 | Video-plane and picture-quality pairing constraints are respected; capability YAML correctly reports limitations on the lower-specification video path where applicable. |

Failure of any mandatory gate means concurrent playback is unsupported on that implementation or supported only within a narrower capability set explicitly declared by the platform.

