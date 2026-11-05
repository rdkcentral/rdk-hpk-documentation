# DPI9 Legacy AV HAL Extension for dual decode

## Table of contents

- [Revision history](#revision-history)
- [0. System context and deployment model](#0-system-context-and-deployment-model)
- [1. Purpose](#1-purpose)
- [2. Roles and responsibilities](#2-roles-and-responsibilities)
- [3. Scope](#3-scope)
- [4. Architecture facts and assumptions](#4-architecture-facts-and-assumptions)
- [5. GStreamer property extensions](#5-gstreamer-property-extensions)
- [6. Resource identification model](#6-resource-identification-model)
- [7. Video decoder and video plane resource model](#7-video-decoder-and-video-plane-resource-model)
- [8. Audio decoder resource model](#8-audio-decoder-resource-model)
- [9. Audio mixer resource model](#9-audio-mixer-resource-model)
- [10. SVP allocator resource model](#10-svp-allocator-resource-model)
- [11. DRM resource model](#11-drm-resource-model)
- [12. AV player transitions and resource acquisition](#12-av-player-transitions-and-resource-acquisition)
- [13. Audio behaviour in dual-decode modes](#13-audio-behaviour-in-dual-decode-modes)
- [14. Conformance and acceptance gates](#14-conformance-and-acceptance-gates)
- [15. Clarifications and implementation notes](#15-clarifications-and-implementation-notes)

**Status:** Draft for review.

**Specification version:** 0.1

## Revision history

| Version | Status | What changed | What reviewers should check |
|---|---|---|---|
| 0.1 | Draft for review | First version of the DPI9 legacy AV HAL extension specification covering concurrent playback property extensions and AV resource-management requirements. | Property semantics, capability reporting, resource selection, transition behaviour, and ESSOS integration requirements. |

### What this specification covers

- AV sync for dual-decode use cases.
- Video decoder and video plane resource selection using the existing `res-usage` property.
- Audio decoder resource handling for symmetric decoders.
- SVP allocator resource handling for asymmetric and symmetric platforms.
- DRM resource expectations for concurrent playback.
- AV player transition and resource-acquisition rules for application-managed GStreamer players and non-GStreamer players.

### What this specification does not cover

- AV sync for use cases other than dual decode.
- Unrelated platform features outside decoder, audio, SVP, DRM, and transition handling.
- Metrics HAL requirements.
- Frame-capture HAL behaviour.

---

## 0. System context and deployment model

This specification defines the contract between the middleware playback stack, vendor GStreamer plugins, and platform resource-management components for DPI9 concurrent playback.

```mermaid
graph LR
    subgraph APP[Application Process]
        App[Application Process]
        subgraph GSTP[GStreamer Pipeline A / B]
            GST[Pipeline flow]
            Ctx[GStreamer Context]
            DEC[decryptor]
            SVPP[svp payload]
            VARB["Video Element: video decoder or westeros sink"]
            AARB["Audio Element: audio decoder or audio sink"]
        end
        App --> GST
        GST --> DEC
        GST --> SVPP
        GST --> VARB
        GST --> AARB
        App --> Ctx
        Ctx -->|SVP Usage| DEC
        Ctx -->|SVP Usage| SVPP
        App --> YAML[Capability YAML]
        DEC --> OCDM[OCDM Adaptor]
        SVPP --> SVPC[gst-svp-ext-common]
        SVPC --> SVPE[gst-svp-ext-soc<br/>SVP Allocator]
        OCDM --> SVPE
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

    VARB -->|IPC| RM
    AARB -->|IPC| RM
    SVPE -->|IPC| RM

    style APP fill:none,stroke:#666,stroke-width:1px,stroke-dasharray: 5 5
    style GSTP fill:#e6e6e6,stroke:#666,stroke-width:1px
    style LEG fill:none,stroke:#666,stroke-width:1px,stroke-dasharray: 5 5
    style OCP fill:none,stroke:#666,stroke-width:1px,stroke-dasharray: 5 5
    style RMP fill:none,stroke:#666,stroke-width:1px,stroke-dasharray: 5 5
    style VARB fill:#d9e8fb,stroke:#3c78d8,stroke-width:1px,color:#000
    style AARB fill:#d9e8fb,stroke:#3c78d8,stroke-width:1px,color:#000
    style SVPE fill:#d9e8fb,stroke:#3c78d8,stroke-width:1px,color:#000
    style DRM fill:#d9e8fb,stroke:#3c78d8,stroke-width:1px,color:#000
    style LRESP fill:#d9e8fb,stroke:#3c78d8,stroke-width:1px,color:#000
```

### Process and IPC boundaries

- **Application Process** owns playback-pipeline construction for GStreamer-based players.
- The application process directly interfaces with GStreamer and the platform vendor elements; it does not depend on a separate playback-middleware client/server layer.
- **Application Process** does not communicate directly with **ESSOS Resource Manager** in the normal orderly path; the vendor elements request and release ESSOS resources on behalf of the pipeline.
- **ESSOS Resource Manager** arbitrates resource assignment and returns resource instance identifiers.
- **Video resource arbitration** is performed by either a dedicated Video Element or by westeros sink when the platform combines sink and decoder responsibilities.
- **Audio resource arbitration** is performed by either a dedicated Audio Element or by audio sink when the platform combines sink and decoder responsibilities.
- **Vendor plugins** use the assigned ESSOS identifiers and GStreamer-context signalling to bind to platform resources.

### Contract scope

This specification defines the contract between:

- **Application layer / playback owner**, which creates and tears down the GStreamer-owned playback pipeline and owns application-level transition behaviour.
- **SoC vendor**, which provides the decoder, sinks, SVP handling, DRM integration, and resource-aware vendor plugins.
- **Vendor layer team**, which integrates the SoC delivery, capability YAML, and ESSOS resource-manager configuration.

---

## 1. Purpose

This specification defines how DPI9 concurrent playback extends the legacy AV HAL surface without redesigning the legacy GStreamer pipeline model.

The goal is to support multiple playback pipelines in a single application process while preserving existing single-pipeline behaviour unless explicitly extended by this document.

The contract covers:

- new or extended sink properties;
- capability declaration and resource identification;
- selection of high-specification and low-specification video resources;
- audio-decoder and SVP resource allocation rules; and
- behaviour during transitions between GStreamer-managed OTT applications and non-GStreamer AV players.

## 2. Roles and responsibilities

| Role | Definition | Responsibilities |
|---|---|---|
| **Application layer / playback owner** | Owns the GStreamer-based playback pipeline and player transition logic. | Builds playback pipelines, tears down exiting pipelines, sets sink properties, propagates GStreamer context, and coordinates player transitions. |
| **SoC vendor** | Provides vendor GStreamer elements and platform resource integration. | Implements westeros sink, audio sink, decoder selection, SVP usage handling, DRM integration, assigned-resource binding, and AV sync for dual decode. |
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
- For DPI9 the initial requirement is a maximum of two playback sessions in one application process.
- Each pipeline operates independently and keeps its own play, pause, seek, and trick-mode state.
- AV sync is maintained independently inside each pipeline. No AV synchronisation is required between pipelines.
- During dual decode, AV sync remains the responsibility of the SoC vendor implementation.
- Existing SoC-vendor behaviour for a single playback pipeline remains unchanged unless explicitly extended by this specification.

### 4.2 Assumptions

- Legacy pipeline construction remains unchanged.
- New functionality is delivered primarily through sink properties, GStreamer context, capability YAML, and existing middleware orchestration.
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

Capability YAML shall describe the declared resource model in a stable manner and shall list the supported concurrent-playback features together with the platform-specific resource classes used by the middleware.

The YAML shall be sufficient for the platform to distinguish:

- whether dual video decode is supported;
- whether dual audio decode is supported;
- whether audio mixing is supported;
- whether dual SVP support is present; and
- whether dual DRM support is available for the relevant video and audio paths.

Example structure:

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

## 5. GStreamer property extensions

### 5.1 Existing property usage

`res-usage` is not a new property introduced by this specification. It is an existing GStreamer property that continues to be used as defined by the current implementation and is now expressly required for differentiating asymmetric video-decoder resources during dual-decode operation.

| Property | Type | Purpose | Required behaviour |
|---|---|---|---|
| `res-usage` | Enum | Identifies the intended resource profile for the video path. | The property value shall continue to distinguish primary and secondary resource usage and shall participate in video-decoder selection as defined in Section 7. For asymmetric decoder platforms, `res-usage` distinguishes the high-specification UHD path from the lower-specification FHD path. |

### 5.2 Property application timing

- `res-usage` shall be set before the pipeline moves from `NULL` to `READY`, because the resource-owning element may request its ESSOS-managed resource during that transition.
- On asymmetric video-decoder platforms, the selected `res-usage` shall be used to choose the correct high-specification or low-specification decoder instance before resource acquisition begins.

## 6. Resource identification model

Resource selection across middleware and vendor plugins relies on consistent identifiers and usage signalling.

The platform shall provide:

- capability YAML describing available concurrent-playback resources;
- ESSOS resource-manager configuration listing the concrete instances for those resources; and
- a stable mapping between logical usage and the assigned ESSOS resource identifiers used by vendor plugins.

The following principles apply:

- middleware decides intent, such as high-specification versus low-specification video decode;
- ESSOS Resource Manager decides the assigned resource instance;
- vendor plugins open the concrete resource selected by the `assignedId` returned by ESSOS; and
- GStreamer context is used where this specification requires usage signalling that must be visible to elements other than the sink that received a direct property.

## 7. Video decoder and video plane resource model

### 7.1 General requirements

All AV players supporting dual video decode shall use the application-managed GStreamer path for the concurrent-playback use case.

The platform shall support resource selection between a high-specification video decoder path and a low-specification video decoder path.

### 7.2 `res-usage` signalling and precedence

The middleware shall propagate the `res-usage` value through GStreamer context in addition to setting the `res-usage` property on westeros sink.

This dual signalling exists to support both of these cases:

- DPI9-capable platforms, where vendor plugins rely on GStreamer context to choose the appropriate decoder resource before data flow begins.
- Legacy platforms, where westeros sink property handling remains the deciding mechanism.

When the GStreamer-context value and the westeros sink property differ, the westeros sink property value shall take precedence.

### 7.3 Resource-class mapping

For platforms implementing asymmetric video resources:

- `res-usage = 7` indicates the high-specification decoder profile using full resolution, full quality, and full performance.
- `res-usage = 0` indicates the low-specification decoder profile.

The `res-usage` value shall be established before the pipeline transitions from `NULL` to `READY`, so that decoder selection can complete when the resource-owning element first requests the video decoder.

### 7.4 ESSOS interaction

The video resource-owning element, following the westeros-sink pattern, shall create its ESSOS Resource Manager connection once for the lifetime of the playback pipeline and shall request the video decoder during the GStreamer `NULL` to `READY` transition.

ESSOS Resource Manager shall return the appropriate video-decoder instance identifier through `assignedId` in the resource request result, either immediately from the request call or asynchronously through the registered notify callback.

The vendor video plugin shall use the returned `assignedId` to open the concrete video-decoder instance.

If ESSOS later issues a revoke event for that decoder, the owning element shall stop using the decoder, release any vendor-side resources associated with it, and call `EssRMgrReleaseResource()` before another player is granted the same decoder.

When playback is torn down and ESSOS communication is no longer required, the owning element shall destroy its ESSOS Resource Manager connection.

For legacy AV players not using the application-managed GStreamer path:

- only one video decoder is typically requested;
- the default usage value is the high-specification profile; and
- the higher-specification decoder identifier is therefore expected.

### 7.5 Video plane relationship

The high-specification video decoder is expected to pair with the high-specification video plane and PQ pipeline.

The low-specification decoder and associated plane may omit support for Dolby Vision or other PQ features on some platforms. That limitation shall be reflected in platform capability reporting.

## 8. Audio decoder resource model

### 8.1 General requirements

All AV players supporting dual audio decode shall use the application-managed GStreamer path for the concurrent-playback use case.

Audio decoder resources are treated as symmetric. This specification does not require an extension to an audio-usage enumeration analogous to the video `res-usage` profile.

### 8.2 ESSOS Resource Manager configuration

The ESSOS Resource Manager configuration shall advertise two available audio-decoder resources.

### 8.3 Example ESSOS configuration entries

The essrmgr.conf configuration file is expected to contain two audio resource entries in the following form:

```text
audio: hardware
audio: hardware
```

### 8.4 ESSOS configuration and allocation

The ESSOS resource-manager configuration shall be extended to advertise both available audio-decoder resources.

When middleware requests an audio decoder:

- the first request shall receive the first available audio-decoder `assignedId`;
- the second request shall receive the second available audio-decoder `assignedId`; and
- no further usage-class distinction is required for symmetric audio decoders.

### 8.5 Vendor plugin behaviour

The audio sink or audio-decoder plugin that consumes ESSOS resource-manager results shall use the returned `assignedId` to decide which hardware audio-decoder instance to open.

The mapping from `assignedId` to the actual hardware decoder instance remains private to the vendor implementation.

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

### 10.2 ESSOS Resource Manager configuration

The ESSOS Resource Manager configuration shall advertise two SVP allocator resources for the dual-decode use case.

### 10.3 Example ESSOS configuration entries

For a symmetric configuration, the essrmgr.conf file is expected to contain two SVP resource entries in the following form:

```text
svpa: hardware
svpa: hardware
```

### 10.4 Asymmetric SVP resources

On platforms where SVP allocator resources are asymmetric, middleware shall signal SVP usage through GStreamer context.

The recommended logical usage values are:

- `EssRMgrSVPAUse_fullResolution` for the UHD-capable pool; and
- `EssRMgrSVPAUse_none` for the FHD-capable pool.

On such platforms:

- ESSOS resource-manager configuration shall define both the UHD SVP pool resource and the FHD SVP pool resource;
- the application-managed GStreamer path shall propagate SVP usage through GStreamer context before the first secure-buffer consumer binds the allocator;
- the first component that consumes the SVP buffer, such as a decryptor or SVP payload element, shall pass the usage to the vendor-side SVP integration; and
- vendor code shall use the usage value together with the assigned resource identifier to bind the correct allocator resource.

An asymmetric configuration may, for example, contain entries in the following form in essrmgr.conf:

```text
svpa: hardware
svpa: hardware, limitedQuality
```

The SVP resource-owning element shall create its ESSOS connection once for the lifetime of the playback pipeline, request the SVP allocator during the initial pipeline-establishment path before secure playback depends on that allocator, and handle grant or revoke using the same ESSOS request/callback/release pattern used for video.

### 10.5 Symmetric SVP resources

On platforms where SVP allocator resources are symmetric:

- the SVP usage flag need not be signalled through GStreamer context for allocator selection;
- ESSOS resource-manager configuration shall define the allocator resources symmetrically; and
- the assigned ESSOS identifier alone is sufficient for vendor-resource binding.

Even on symmetric platforms, the SVP-owning element shall keep a persistent ESSOS connection for the lifetime of the playback pipeline, release the allocator with `EssRMgrReleaseResource()` when playback stops or a revoke event is received, and destroy the ESSOS connection only when resource-manager communication is no longer needed.

### 10.6 Legacy behaviour

For legacy AV players not using the application-managed GStreamer path:

- only one SVP allocator is typically requested; and
- the default expectation is the higher-specification allocator when such differentiation exists.

### 10.7 Extensions to the OCDM Adaptor, gst-svp-ext-common, and gst-svp-ext-soc

The OCDM adaptor, `gst-svp-ext-common`, and `gst-svp-ext-soc` shall carry the minimum set of attributes required to resolve the correct secure-resource instance for the active playback session.

At a minimum, the secure-resource metadata shall include:

- the logical usage class for the resource, such as primary or secondary video or SVP pool;
- the selected ESSOS `assignedId` for the concrete resource instance;
- the pipeline or process ownership identifier used to distinguish the entering and exiting sessions; and
- any platform-specific key or allocator identifier required by the vendor implementation.

The integration shall ensure that the resource-owning element requests the allocator or decryptor state in the same order used by the platform's existing secure-playback flow, and shall not allow a later session to reuse stale resource state from an earlier owner.

### 10.8 GStreamer context structure for carrying SVP pool information

Where the platform requires usage information to be visible to elements other than the sink that received the direct property, the SVP usage information shall be carried in GStreamer context using a stable structure keyed by logical resource type.

The structure shall include at least the following fields:

- `usage` or equivalent logical resource class, such as `primary`, `secondary`, `fullResolution`, or `standardResolution`;
- `assigned-id` or equivalent assigned ESSOS resource identifier;
- `resource-type`, such as `video`, `audio`, or `svp`; and
- `pipeline-role`, identifying the primary or secondary playback instance.

A representative example is shown below:

```text
GstStructure: "svp-info"
  usage = "fullResolution"
  assigned-id = 3
  resource-type = "svp"
  pipeline-role = "primary"
```

The value passed in GStreamer context shall be read before the secure buffer is bound to the allocator, and the platform shall reject or fail positively when the context does not match an available resource instance.

## 11. DRM resource model

### 11.1 General requirements

Dual-DRM support applies to both video and audio paths. In a dual-decode implementation, the platform may therefore require up to four concurrent DRM sessions: one for the primary video path, one for the primary audio path, one for the secondary video path, and one for the secondary audio path.

DRM resources are not managed by ESSOS Resource Manager.

### 11.2 Provisioning expectations

The platform shall provision the relevant key identifiers for the playback session before the GStreamer pipeline is constructed.

This applies equally to encrypted video and encrypted audio data. Encrypted media passed during decryption shall carry sufficient key-identification information for the DRM implementation to select the correct decryption key.


## 12. AV player transitions and resource acquisition

### 12.1 General transition rules

Resource management during AV player transitions shall prevent mixed ownership of decoder, plane, audio-decoder, or SVP resources.

For OTT applications using the GStreamer-managed playback model, the application itself shall coordinate activation and inactivation of the exiting and entering pipelines. The entering application shall stop the exiting pipeline before attempting to acquire the same video, audio, or SVP resources for the next playback session.

This is the default behaviour for OTT application switches. There is no delayed-release path for transitions between GStreamer-managed OTT applications because the application process is responsible for ordering its own teardown and acquisition flow.

ESSOS Resource Manager remains the fallback and arbitration mechanism when non-GStreamer applications, such as HDMI In or Composite In, continue to run while a GStreamer-based OTT application is being torn down. In that case the orderly-release path through ESSOS is still required so that a non-GStreamer application can acquire or retain AV resources before the GStreamer application has fully released them.

In dual-decode scenarios, if ESSOS Resource Manager revokes one video resource during a transition, the platform shall revoke all video resources owned by the exiting player together. This is required to prevent video from the exiting player being mixed with video from the entering player during handover.

Audio-mixer behaviour remains process-specific and shall follow the ownership and isolation rules defined in Section 9.

Within one AV pipeline, `EssRMgrCreate()` shall be called once during playback setup. Following the westeros-sink resource-manager pattern, the resource-owning element shall request its ESSOS-managed resource during the first transition from GStreamer `NULL` to `READY`. The grant may be delivered immediately through the request result or later through `EssRMgrNotifyCB`, which carries the relevant `EssRMgrEvent`. `EssRMgrDestroy()` shall be called only when ESSOS resource-manager communication is no longer required for that pipeline.

### 12.2 GStreamer-managed OTT application to GStreamer-managed OTT application

For transitions between two GStreamer-managed OTT applications:

- the application process is responsible for ordering teardown of the exiting playback pipeline and initialisation of the entering playback pipeline;
- the exiting application shall release video decoders, audio decoders, and SVP allocators before the entering application acquires them; and
- the entering application shall wait for release confirmation before activating the next resource acquisition.

There is no delayed-release path for OTT application-to-OTT application transitions. The resource-management model is application-driven and shall be implemented as an orderly release and re-acquisition sequence.

Orderly-release path, where the exiting application releases resources before the entering application requests them:

```mermaid
sequenceDiagram
    autonumber
    participant AM as OTT Application
    participant APP1 as Entering GStreamer<br/>Application Process
    participant G1 as GStreamer pipeline<br/>A/B
    participant V1 as Entering Video<br/>Element A/B
    participant A1 as Entering Audio<br/>Element A/B
    participant S1 as Entering Decryptor/svp payload element<br/>OCDM adaptor - gst-svp-ext-common<br/>gst-svp-ext-soc A/B
    participant RM as ESSOS Resource<br/>Manager
    participant APP2 as Exiting GStreamer<br/>Application Process
    participant G2 as GStreamer pipeline<br/>A/B
    participant V2 as Exiting Video<br/>Element A/B
    participant A2 as Exiting Audio<br/>Element A/B
    participant S2 as Exiting Decryptor/svp payload element<br/>OCDM adaptor - gst-svp-ext-common<br/>gst-svp-ext-soc A/B

    Note over V2,S2: EssRMgrCreate() already completed for exiting player Pipeline A/B during initial pipeline setup
    Note over V1,S1: EssRMgrCreate() will be done once for entering player Pipeline A/B during this pipeline setup, not per transition

    AM->>APP2: stop exiting OTT application
    APP2->>G2: stop exiting pipeline A/B
    G2->>V2: stop playback and release video resources A/B
    V2->>RM: EssRMgrReleaseResource(video A/B)
    G2->>A2: release audio resources A/B
    A2->>RM: EssRMgrReleaseResource(audio A/B)
    G2->>S2: release SVP resources A/B
    S2->>RM: EssRMgrReleaseResource(svp A/B)
    APP2-->>AM: resource release complete

    AM->>APP1: activate entering OTT application
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

### 12.3 GStreamer-managed OTT application to non-GStreamer-based AV player

For transitions from a GStreamer-managed OTT application to a non-GStreamer-based AV player, the required behaviour is equivalent to the GStreamer-to-GStreamer case from a resource-safety perspective, except that the application is no longer the only actor.

The platform shall support an orderly release path where:

- the exiting GStreamer application releases AV resources before the entering non-GStreamer player requests them; or
- if the non-GStreamer player is launched before the GStreamer application has fully torn down, ESSOS Resource Manager shall arbitrate the handover and revoke the exiting GStreamer resources if necessary.

Orderly-release path, where the exiting GStreamer-based player releases resources before the entering non-GStreamer player requests them:

```mermaid
sequenceDiagram
    autonumber
    participant AM as OTT Application<br/>Manager
    participant APP2 as Exiting GStreamer<br/>Application Process
    participant G2 as GStreamer pipeline<br/>A/B
    participant V2 as Exiting Video<br/>Element A/B
    participant A2 as Exiting Audio<br/>Element A/B
    participant S2 as Exiting Decryptor/svp payload element<br/>OCDM adaptor - gst-svp-ext-common<br/>gst-svp-ext-soc A/B
    participant RM as ESSOS Resource<br/>Manager
    participant APP1 as Non-GStreamer App
    participant G1 as Platform pipeline<br/>A
    participant V1 as Entering Video<br/>Element A
    participant A1 as Entering Audio<br/>Element A
    participant S1 as Entering Decryptor/svp payload element<br/>OCDM adaptor - gst-svp-ext-common<br/>gst-svp-ext-soc A

    Note over V2,S2: EssRMgrCreate() already completed for exiting player pipeline during initial pipeline setup
    Note over V1,S1: EssRMgrCreate() is done once when the entering player pipeline is first established

    AM->>APP2: inactivate GStreamer-based OTT application
    APP2->>G2: inactivate exiting pipeline A/B
    G2->>V2: release video resources A/B
    V2->>RM: EssRMgrReleaseResource(video A/B)
    G2->>A2: release audio resources A/B
    A2->>RM: EssRMgrReleaseResource(audio A/B)
    G2->>S2: release SVP resources A/B
    S2->>RM: EssRMgrReleaseResource(svp A/B)
    APP2-->>AM: release complete

    AM->>APP1: launch non-GStreamer player
    APP1->>G1: activate pipeline A
    G1->>V1: initialise pipeline
    V1->>RM: EssRMgrCreate(notifyCB)
    G1->>V1: transition NULL -> READY
    V1->>RM: EssRMgrRequestResource(video, notifyCB)
    RM-->>V1: request result or notifyCB(EssRMgrEvent::Granted, assignedId)
    G1->>A1: initialise audio path
    A1->>RM: EssRMgrCreate(notifyCB)
    G1->>A1: transition NULL -> READY
    A1->>RM: EssRMgrRequestResource(audio, notifyCB)
    RM-->>A1: request result or notifyCB(EssRMgrEvent::Granted, assignedId)
    G1->>S1: initialise SVP path if required
    S1->>RM: EssRMgrCreate(notifyCB)
    G1->>S1: transition NULL -> READY
    S1->>RM: EssRMgrRequestResource(svp, notifyCB)
    RM-->>S1: request result or notifyCB(EssRMgrEvent::Granted, assignedId)
```

### 12.4 Non-GStreamer-based AV player to GStreamer-managed OTT application

For transitions from a non-GStreamer-based player to a GStreamer-managed OTT application:

- the exiting non-GStreamer player shall release AV resources through the current platform path when it is torn down;
- the entering GStreamer-based application shall not rely on a delayed-release path; and
- if the non-GStreamer player is still running during the GStreamer application's acquisition attempt, ESSOS Resource Manager shall arbitrate the handover and revoke the exiting non-GStreamer resources before the GStreamer application is granted the same AV resources.

The orderly-release path is still required because non-GStreamer applications such as HDMI In or Composite In may be launched before the GStreamer application is able to tear down and release AV resources.

Orderly-release path, where the exiting non-GStreamer player releases resources before the entering GStreamer application requests them:

```mermaid
sequenceDiagram
    autonumber
    participant AM as OTT Application<br/>Manager
    participant APP2 as Non-GStreamer App
    participant G2 as Platform pipeline<br/>A
    participant V2 as Exiting Video<br/>Element A
    participant A2 as Exiting Audio<br/>Element A
    participant S2 as Exiting Decryptor/svp payload element<br/>OCDM adaptor - gst-svp-ext-common<br/>gst-svp-ext-soc A
    participant RM as ESSOS Resource<br/>Manager
    participant APP1 as Entering GStreamer<br/>Application Process
    participant G1 as GStreamer pipeline<br/>A/B
    participant V1 as Entering Video<br/>Element A/B
    participant A1 as Entering Audio<br/>Element A/B
    participant S1 as Entering Decryptor/svp payload element<br/>OCDM adaptor - gst-svp-ext-common<br/>gst-svp-ext-soc A/B

    Note over V2,S2: Exiting non-GStreamer app owns AV resources through the current platform path
    Note over V1,S1: Entering GStreamer pipeline performs EssRMgrCreate() once during startup, then requests resources

    AM->>APP2: stop exiting non-GStreamer player
    APP2->>G2: stop exiting pipeline A
    G2->>V2: release video resource
    V2->>RM: EssRMgrReleaseResource(video)
    G2->>A2: release audio resource
    A2->>RM: EssRMgrReleaseResource(audio)
    G2->>S2: release SVP resource
    S2->>RM: EssRMgrReleaseResource(svp)
    APP2-->>AM: resource release complete

    AM->>APP1: prepare incoming GStreamer-based OTT application
    APP1->>G1: prepare entering pipeline A/B
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

    G1->>S1: initialise SVP path A/B if required
    S1->>RM: EssRMgrCreate(notifyCB)
    G1->>S1: transition NULL -> READY
    S1->>RM: EssRMgrRequestResource(svp A/B, notifyCB)
    RM-->>S1: request result or notifyCB(EssRMgrEvent::Granted, assignedId)
```

### 12.5 Non-GStreamer-based AV player to non-GStreamer-based AV player

For transitions between two non-GStreamer-based AV players, release occurs through the existing platform method.

This specification does not require application-managed GStreamer coordination in that case, but the platform shall still preserve exclusive ownership of the relevant AV resources.

The transition shall work according to the current non-GStreamer model already in place on the platform.

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
| 5 | GStreamer context and sink property signalling coexist, with the direct sink property taking precedence on conflict. |
| 6 | ESSOS `assignedId` values are used by vendor plugins to open the selected video-decoder and audio-decoder instances. |
| 7 | Audio decoder resources are treated symmetrically and are represented as multiple configured resources in ESSOS. |
| 8 | Audio mixer ownership for main and secondary playback inputs is process-scoped, and an entering process is isolated from stale playback audio injected by an exiting process. |
| 9 | Asymmetric SVP platforms signal SVP usage and provide separate UHD and FHD allocator resources in ESSOS configuration. |
| 10 | Symmetric SVP platforms do not require usage-based allocator selection and may define allocator resources symmetrically. |
| 11 | DRM-session provisioning occurs before pipeline construction and supports the platform's declared concurrent playback model. |
| 12 | Transition handling prevents mixed ownership that would produce overlapping or blended video from exiting and entering players. |
| 13 | Audio from an exiting process is ignored once the mixer is in use by the entering process, consistent with process-scoped mixer ownership. |
| 14 | ESSOS Resource Manager support is available as the mandatory fallback resource-management model for cross-application transitions when the application resource manager does not provide smooth handover. |

Failure of any mandatory gate means DPI9 concurrent playback is unsupported on that implementation or supported only within a narrower capability set explicitly declared by the platform.

## 15. Clarifications and implementation notes

The following points should be documented by each platform implementation where relevant:

- the exact ESSOS resource identifiers used for high-specification and low-specification video resources;
- whether the platform's SVP resources are asymmetric or symmetric;
- any platform-specific limit on concurrent DRM sessions during steady state or transition;
- any restriction on concurrent Dolby or other high-cost audio decode combinations.
