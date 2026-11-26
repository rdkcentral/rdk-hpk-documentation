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
        Ctx -->|SVP usage| SVPC
        Ctx -->|Video decoder usage| VE
        Ctx -->|Audio decoder usage| AE
        App --> YAML[Capability YAML]
        DEC --> OCDM[OCDM Adaptor]
        SVPP --> SVPC[gst-svp-ext-common]
        SVPC -->|Resolved SVP usage: API TBC| SVPE[gst-svp-ext-soc<br/>SVP Allocator]
        WSU -->|GstWesterosSink pointer: resUsage| WSOC
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

    VE -->|IPC| RM
    AE -->|IPC| RM
    SVPE -->|IPC| RM

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
- **Resource usage** is resolved by westeros sink for video, the resource-owning audio element for audio, and `gst-svp-ext-common` for SVP. The corresponding SoC implementation requests the resource from ESSOS and binds the granted instance.
- **Vendor plugins** bind the returned `assignedId` values and any GStreamer-context signalling to the correct hardware resources.

### Contract scope

This specification defines the contract between:

- **Application layer / playback owner**, which creates and tears down the GStreamer-owned playback pipeline and owns application-level transition behaviour.
- **SoC vendor**, which provides the decoder, sinks, SVP handling, DRM integration, and resource-aware vendor plugins.
- **Vendor layer team**, which integrates the SoC delivery, capability YAML, and ESSOS resource-manager configuration.

---

## 1. Purpose

This specification defines how concurrent playback extends the legacy AV HAL surface without redesigning the legacy GStreamer pipeline model.

The goal is to support multiple playback pipelines in a single application process while preserving existing single-pipeline behaviour unless explicitly extended by this document.

The contract covers:

- existing resource-usage properties and GStreamer context;
- capability declaration and resource identification;
- selection of high-specification and low-specification video resources;
- audio-decoder and SVP resource allocation rules; and
- behaviour during transitions between GStreamer-based applications and non-GStreamer AV players.

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
- Maximum of two playback sessions in one application process.
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

[TBC - Link to Capability schema to be added]

Capability YAML shall describe the declared resource model and list the supported concurrent-playback features together with the platform-specific resource classes used by the middleware.

It shall distinguish whether dual video decode, dual audio decode, audio mixing, dual SVP support, and dual DRM support are present for the relevant video and audio paths.

Example structure (TBC):

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

Any SoC-vendor GStreamer element with a hardware-resource dependency shall support the full ESSOS interaction model for that dependency: request, grant handling, release, and revoke handling where arbitration requires it.

[TBC - Documentation of ESSOS Resource Manager APIs to be added; see https://github.com/rdkcmf/westeros/blob/master/essos/essos-resmgr.h]

The platform-specific configuration shall document the concrete resource identifiers and any limits that apply to concurrent audio and video acquisition. Where the platform exposes asymmetric pools, the configuration shall clearly distinguish the relevant high-specification and low-specification pools.

## 5. GStreamer context and `res-usage` documentation

This specification does not add a new GStreamer property. It documents the existing `res-usage` flag so that its role in dual-decode resource selection is explicit and consistent across implementations.

### 5.1 Existing `res-usage` flag

`res-usage` is an existing westeros sink property. GStreamer context also carries resource-specific usage values for video, audio, and SVP. This specification introduces no additional GStreamer properties.

| Property | Type | Purpose | Required behaviour |
|---|---|---|---|
| `res-usage` | Enum | Identifies the intended resource profile for the video path. | The property value shall continue to distinguish primary and secondary resource usage and shall participate in video-decoder selection as defined in Section 7. For asymmetric decoder platforms, `res-usage` distinguishes the high-specification UHD path from the lower-specification FHD path. |

### 5.2 Property application timing

- The video property and resource-usage context shall be available before the owning element resolves usage and requests a resource.
- Video usage shall be resolved before the `GstWesterosSink` pointer is passed to `gst_westeros_sink_soc_init()` and before the `NULL` to `READY` resource request. SVP usage shall be resolved before secure-buffer allocation.

### 5.3 Context decision tree and element coverage

The owning component shall resolve usage according to Sections 7, 8, and 10. An absent context value shall retain the applicable element default; it shall not imply secondary or symmetric-resource selection. Every selection path shall request an ESSOS-managed resource and use the granted `assignedId` for hardware binding.

## 6. Resource identification model

Resource selection across middleware and vendor plugins relies on consistent identifiers and usage signalling.

The platform shall provide capability YAML, ESSOS resource-manager configuration, and a stable mapping between logical usage and the assigned `assignedId` values used by vendor plugins.

Usage identifies the requested resource profile, not a granted instance. ESSOS assigns the concrete video, audio, or SVP resource; the vendor implementation binds the returned `assignedId`. Context field names and the SVP handoff API remain TBC where not already defined by the implementation.

## 7. Video decoder and video plane resource model

### 7.1 General requirements

All AV players supporting dual video decode shall use the application-managed GStreamer path for the concurrent-playback use case.

The platform shall support resource selection between a high-specification video decoder path and a low-specification video decoder path.

### 7.2 `res-usage` signalling and precedence

Westeros sink shall resolve video usage in the following order:

| Priority | Condition | Selected usage |
|---|---|---|
| 1 | `res-usage` was explicitly set on this sink instance. | Property value, including an explicitly set default value. |
| 2 | The property was not explicitly set and GStreamer context supplies video `res-usage`. | Context value. |
| 3 | Neither supplies a value. | Existing element default: the most capable video decoder resource. |

Westeros sink shall store the resolved value in `GstWesterosSink.resUsage` and pass the structure pointer to the SoC module through `gst_westeros_sink_soc_init()`. The SoC implementation shall use this value to distinguish primary and secondary decoder profiles when requesting a resource from ESSOS, then bind the granted video-decoder instance. It shall track whether the property was explicitly set rather than infer this from its value.

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
- usage selection shall follow the precedence and default rules in Section 7.2.

### 7.5 Video plane relationship

The high-specification video decoder is expected to pair with the high-specification video plane and PQ pipeline.

The low-specification decoder and associated plane may omit support for Dolby Vision or other PQ features on some platforms. That limitation shall be reflected in platform capability reporting.

## 8. Audio decoder resource model

### 8.1 General requirements

All AV players supporting dual audio decode shall use the application-managed GStreamer path for the concurrent-playback use case.

The audio sink or audio decoder that owns the decoder resource shall resolve audio usage as follows:

| Condition | Selected usage |
|---|---|
| GStreamer context supplies audio resource usage. | Context value. |
| No audio usage value is supplied. | Existing element default: the most capable audio decoder resource. |

The owning element shall request the corresponding resource from ESSOS in both cases. No audio sink property analogous to video `res-usage` is introduced.

### 8.2 ESSOS Resource Manager configuration

The ESSOS Resource Manager configuration shall advertise two available audio-decoder resources.

### 8.3 Example ESSOS configuration entries

The `essrmgr.conf` file is expected to contain two audio resource entries in the following form (TBC):

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

`gst-svp-ext-common` shall resolve SVP usage as follows:

| Condition | Selected usage |
|---|---|
| GStreamer context supplies SVP resource usage. | Context value. |
| No SVP usage value is supplied. | Existing element default: the highest available SVP resource. |

It shall pass the resolved usage to `gst-svp-ext-soc` through an API to be defined (TBC). The SoC module shall request the corresponding resource from ESSOS in both cases and use the granted instance to select the SVP memory pool.

### 10.2 ESSOS Resource Manager configuration

The ESSOS Resource Manager configuration shall advertise two SVP allocator resources for the dual-decode use case.

### 10.3 Example ESSOS configuration entries

For a symmetric configuration, the `essrmgr.conf` file is expected to contain two SVP resource entries in the following form:

```text
svpa: hardware
svpa: hardware
```

### 10.4 Asymmetric SVP resources

On asymmetric platforms, the resolved SVP usage shall distinguish the required memory-pool profile.

The recommended logical usage values are:

- `EssRMgrSVPAUse_fullResolution` for the UHD-capable pool; and
- `EssRMgrSVPAUse_none` for the FHD-capable pool.

On such platforms:

- ESSOS resource-manager configuration shall define both the UHD SVP pool resource and the FHD SVP pool resource;
- `gst-svp-ext-common` shall resolve usage and forward it as defined in Section 10.1; and
- `gst-svp-ext-soc` shall bind the ESSOS-granted resource to the corresponding memory pool.

An asymmetric configuration may, for example, contain entries in `essrmgr.conf` in the following form:

```text
svpa: hardware
svpa: hardware, limitedQuality
```

The SVP resource-owning element shall create its ESSOS connection once for the lifetime of the playback pipeline, request the SVP allocator during the initial pipeline-establishment path before secure playback depends on that allocator, and handle grant or revoke using the same ESSOS request/callback/release pattern used for video.

### 10.5 Symmetric SVP resources

On platforms where SVP allocator resources are symmetric:

- context usage, when supplied, shall still be resolved according to Section 10.1;
- ESSOS resource-manager configuration shall define the allocator resources symmetrically; and
- the granted ESSOS identifier shall identify the pool used for vendor-resource binding.

Even on symmetric platforms, the SVP-owning element shall keep a persistent ESSOS connection for the lifetime of the playback pipeline, release the allocator with `EssRMgrReleaseResource()` when playback stops or a revoke event is received, and destroy the ESSOS connection only when resource-manager communication is no longer needed.

### 10.6 Legacy behaviour

For legacy AV players not using the application-managed GStreamer path:

- only one SVP allocator is typically requested; and
- absent usage signalling, the default is the highest available SVP resource.

### 10.7 Extensions to the OCDM Adaptor, gst-svp-ext-common, and gst-svp-ext-soc

The API for passing resolved SVP usage from `gst-svp-ext-common` to `gst-svp-ext-soc` is TBC. This specification does not define a new function prototype or additional OCDM adaptor API.

### 10.8 GStreamer context structure for carrying SVP pool information

The SVP context structure and field names are TBC. The context shall carry the requested usage; ESSOS supplies the granted resource identifier.

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

For transitions between two GStreamer-based applications:

- the application manager orders teardown of the exiting pipeline and launch of the entering one;
- the exiting pipeline releases video, audio, and SVP resources before the entering pipeline requests them; and
- the entering pipeline waits for the release confirmation before it begins its next resource acquisition.

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
    participant S1 as Entering Decryptor/svp payload element<br/>OCDM adaptor - gst-svp-ext-common<br/>gst-svp-ext-soc A/B
    participant RM as ESSOS Resource<br/>Manager
    participant APP2 as Exiting GStreamer<br/>Application Process
    participant G2 as GStreamer pipeline<br/>A/B
    participant V2 as Exiting Video<br/>Element A/B
    participant A2 as Exiting Audio<br/>Element A/B
    participant S2 as Exiting Decryptor/svp payload element<br/>OCDM adaptor - gst-svp-ext-common<br/>gst-svp-ext-soc A/B

    Note over V2,S2: EssRMgrCreate() already completed for exiting player Pipeline A/B during initial pipeline setup
    Note over V1,S1: EssRMgrCreate() will be done once for entering player Pipeline A/B during this pipeline setup, not per transition

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
    participant S1 as Entering Decryptor/svp payload element<br/>OCDM adaptor - gst-svp-ext-common<br/>gst-svp-ext-soc A/B
    participant RM as ESSOS Resource<br/>Manager
    participant APP2 as Exiting GStreamer<br/>Application Process
    participant G2 as Exiting GStreamer pipeline<br/>A/B
    participant V2 as Exiting Video<br/>Element A/B
    participant A2 as Exiting Audio<br/>Element A/B
    participant S2 as Exiting Decryptor/svp payload element<br/>OCDM adaptor - gst-svp-ext-common<br/>gst-svp-ext-soc A/B

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

The platform shall support an orderly release path, and if the non-GStreamer player arrives before teardown completes, ESSOS Resource Manager may arbitrate the handover and revoke the exiting AV resources. If secure playback resources are active in the exiting path, SVP resources follow the same ESSOS revoke and release behaviour.

Orderly-release path, where the exiting GStreamer-based player releases resources before the entering non-GStreamer player requests them:

```mermaid
sequenceDiagram
    autonumber
    participant AM as Application manager
    participant APP_GST_EXIT as Exiting GStreamer<br/>Application Process
    participant G_GST_EXIT as GStreamer pipeline<br/>A/B
    participant V_GST_EXIT as Exiting Video<br/>Element A/B
    participant A_GST_EXIT as Exiting Audio<br/>Element A/B
    participant S_GST_EXIT as Exiting Decryptor/svp payload element<br/>OCDM adaptor - gst-svp-ext-common<br/>gst-svp-ext-soc A/B
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
    APP_NG->>RM: EssRMgrRequestResource(svp, notifyCB)
    RM-->>APP_NG: request result or notifyCB(EssRMgrEvent::Granted, assignedId)
```

Disorderly-release path, where the entering non-GStreamer player requests AV and SVP resources before the exiting GStreamer-based application has fully released them:

```mermaid
sequenceDiagram
    autonumber
    participant AM as Application manager
    participant RM as ESSOS Resource<br/>Manager
    participant APP_GST_EXIT as Exiting GStreamer<br/>Application Process
    participant G_GST_EXIT as Exiting GStreamer pipeline<br/>A/B
    participant V_GST_EXIT as Exiting Video<br/>Element A/B
    participant A_GST_EXIT as Exiting Audio<br/>Element A/B
    participant S_GST_EXIT as Exiting Decryptor/svp payload element<br/>OCDM adaptor - gst-svp-ext-common<br/>gst-svp-ext-soc A/B
    participant APP_NG as Non-GStreamer<br/>AV Player

    Note over APP_GST_EXIT: Exiting GStreamer pipeline is still tearing down and has not yet fully released all AV/SVP resources
    Note over APP_NG: Entering non-GStreamer player requests video, audio, and SVP resources sequentially

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

    APP_NG->>RM: EssRMgrRequestResource(svp, notifyCB)
    RM->>S_GST_EXIT: revoke or reassign the exiting SVP resource to the entering player
    S_GST_EXIT-->>RM: revoke callback and release in progress
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

The orderly-release path remains required because non-GStreamer applications such as HDMI In or Composite In may still be active while the GStreamer-based application starts. If secure playback resources are active, SVP allocation and arbitration are ESSOS-managed in the same transition model.

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
    participant S_GST_ENTER as Entering Decryptor/svp payload element<br/>OCDM adaptor - gst-svp-ext-common<br/>gst-svp-ext-soc A/B

    Note over APP_NG: Exiting non-GStreamer player is still the current owner of the audio and video resources
    Note over APP_GST_ENTER: Entering GStreamer-based application waits until the previous player has released ownership

    AM->>APP_NG: stop exiting non-GStreamer player
    APP_NG->>RM: EssRMgrReleaseResource(video)
    APP_NG->>RM: EssRMgrReleaseResource(audio)
    APP_NG->>RM: EssRMgrReleaseResource(svp)
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

Disorderly-release path, where the entering GStreamer-based application requests AV and SVP resources before the exiting non-GStreamer-based player has fully released them:

```mermaid
sequenceDiagram
    autonumber
    participant AM as Application manager
    participant APP_GST_ENTER as Entering GStreamer<br/>Application Process
    participant G_GST_ENTER as GStreamer pipeline<br/>A/B
    participant V_GST_ENTER as Entering Video<br/>Element A/B
    participant A_GST_ENTER as Entering Audio<br/>Element A/B
    participant S_GST_ENTER as Entering Decryptor/svp payload element<br/>OCDM adaptor - gst-svp-ext-common<br/>gst-svp-ext-soc A/B
    participant RM as ESSOS Resource<br/>Manager
    participant APP_NG as Exiting Non-GStreamer<br/>AV Player

    Note over APP_NG: Exiting non-GStreamer player still owns the current AV/SVP resources
    Note over APP_GST_ENTER: Entering GStreamer-based application requests AV/SVP resources while the non-GStreamer player is still unwinding

    AM->>APP_GST_ENTER: launch incoming GStreamer-based application before non-GStreamer teardown is complete
    APP_GST_ENTER->>G_GST_ENTER: activate entering pipeline A/B
    G_GST_ENTER->>V_GST_ENTER: request video resources A/B
    V_GST_ENTER->>RM: EssRMgrRequestResource(video A/B, notifyCB)
    RM->>APP_NG: revoke both exiting video resources and reassign them to the entering GStreamer-based application
    APP_NG-->>RM: notifyCB(EssRMgrEvent::Revoked)
    APP_NG->>RM: EssRMgrReleaseResource(video primary)
    APP_NG->>RM: EssRMgrReleaseResource(video secondary)
    RM-->>V_GST_ENTER: request result or notifyCB(EssRMgrEvent::Granted, assignedId)

    G_GST_ENTER->>A_GST_ENTER: request audio resources A/B
    A_GST_ENTER->>RM: EssRMgrRequestResource(audio A/B, notifyCB)
    RM->>APP_NG: revoke or reassign the exiting audio resource to the entering GStreamer-based application
    APP_NG-->>RM: revoke callback and release in progress
    RM-->>A_GST_ENTER: request result or notifyCB(EssRMgrEvent::Granted, assignedId)

    G_GST_ENTER->>S_GST_ENTER: request SVP resources A/B
    S_GST_ENTER->>RM: EssRMgrRequestResource(svp A/B, notifyCB)
    RM->>APP_NG: revoke or reassign the exiting SVP resource to the entering GStreamer-based application
    APP_NG-->>RM: revoke callback and release in progress
    RM-->>S_GST_ENTER: request result or notifyCB(EssRMgrEvent::Granted, assignedId)

    APP_NG->>RM: final release of remaining video, audio, and SVP resources after arbitration
    RM-->>APP_NG: final resource release acknowledged
```

### 12.5 Non-GStreamer-based AV player to non-GStreamer-based AV player

For transitions between two non-GStreamer-based AV players, release occurs through the existing platform method.

This specification does not require application-managed GStreamer coordination in that case, but the platform shall still preserve exclusive ownership of the relevant AV resources.

The transition shall work according to the current non-GStreamer model already in place on the platform.

Orderly-release path, where the exiting non-GStreamer player releases AV and SVP resources before the entering non-GStreamer player requests them:

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
    APP_NG_EXIT->>RM: EssRMgrReleaseResource(svp)
    APP_NG_EXIT-->>AM: resource release complete

    AM->>APP_NG_ENTER: launch entering non-GStreamer player
    APP_NG_ENTER->>RM: EssRMgrRequestResource(video, notifyCB)
    RM-->>APP_NG_ENTER: request result or notifyCB(EssRMgrEvent::Granted, assignedId)
    APP_NG_ENTER->>RM: EssRMgrRequestResource(audio, notifyCB)
    RM-->>APP_NG_ENTER: request result or notifyCB(EssRMgrEvent::Granted, assignedId)
    APP_NG_ENTER->>RM: EssRMgrRequestResource(svp, notifyCB)
    RM-->>APP_NG_ENTER: request result or notifyCB(EssRMgrEvent::Granted, assignedId)
```

Disorderly-release path, where the entering non-GStreamer player requests AV and SVP resources before the exiting non-GStreamer player has fully released them:

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
    RM->>APP_NG_EXIT: revoke both exiting video resources and reassign them to the entering player
    APP_NG_EXIT-->>RM: notifyCB(EssRMgrEvent::Revoked)
    APP_NG_EXIT->>RM: EssRMgrReleaseResource(video primary)
    APP_NG_EXIT->>RM: EssRMgrReleaseResource(video secondary)
    RM-->>APP_NG_ENTER: request result or notifyCB(EssRMgrEvent::Granted, assignedId)

    APP_NG_ENTER->>RM: EssRMgrRequestResource(audio, notifyCB)
    RM->>APP_NG_EXIT: revoke or reassign exiting audio resource
    APP_NG_EXIT-->>RM: revoke callback and release in progress
    RM-->>APP_NG_ENTER: request result or notifyCB(EssRMgrEvent::Granted, assignedId)

    APP_NG_ENTER->>RM: EssRMgrRequestResource(svp, notifyCB)
    RM->>APP_NG_EXIT: revoke or reassign exiting SVP resource
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
| 6 | The owning audio element uses context-supplied usage, or the most capable decoder default when absent, and requests the resource from ESSOS. |
| 7 | Audio mixer ownership for main and secondary playback inputs is process-scoped, and an entering process is isolated from stale playback audio injected by an exiting process. |
| 8 | `gst-svp-ext-common` resolves context-supplied SVP usage or the highest available resource default and passes it to `gst-svp-ext-soc` for ESSOS allocation and pool binding. |
| 9 | Westeros sink resolves video usage in explicit-property, context, then element-default order and passes `GstWesterosSink.resUsage` to the SoC module. |
| 10 | DRM-session provisioning occurs before pipeline construction and supports the platform's declared concurrent playback model. |
| 11 | Transition handling prevents mixed ownership that would produce overlapping or blended video from exiting and entering players. |
| 12 | Audio from an exiting process is ignored once the mixer is in use by the entering process, consistent with process-scoped mixer ownership. |
| 13 | ESSOS Resource Manager support is available for switching between non-GStreamer-based applications and between non-GStreamer-based and GStreamer-based applications, including the revoke-driven arbitration path required when the competing application requests video or audio resources before teardown completes. |
| 14 | Vendor-level validation demonstrates HDMI In, Composite In, and equivalent non-GStreamer contention cases against GStreamer pipelines, including orderly acquisition, revoke-driven arbitration, and clean release. |
| 15 | Hardware-backed vendor elements that consume managed resources demonstrate request, grant, release, and revoke behaviour consistent with the ESSOS model defined in this specification. |

Failure of any mandatory gate means concurrent playback is unsupported on that implementation or supported only within a narrower capability set explicitly declared by the platform.

