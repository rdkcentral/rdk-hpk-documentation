# SoC Video Frame Capture Specification

**Status:** Draft for review.

**Specification version:** 0.7

## Revision history

| Version | Status | What changed | What reviewers should check |
|---|---|---|---|
| 0.7 | Draft for review | Added mandatory EGL/OpenGL ES and Vulkan requirements and clarified the logical capture reservation model. | Graphics import and lifetime, retention-capacity admission, logical slot and object keys, acquisition-specific FDs, and stable backing while locked. |
| 0.6 | Draft for review | Changed ICaptureSession delivery from GstContext to Gst property based on feedback that GstContext is pipeline-wide, not element-specific. Added westeros-sink-soc.h contract changes for property handling. Uprev to v0.6. | Property-based delivery approach, westeros-sink-soc.h contract changes, Section 6 restructuring to 5 focused sections |
| 0.5 | Draft for review | Added ICaptureSession delivery mechanism documentation, clarifying comments to Westeros example showing platform-dependent factory selection, and factory function example. No API contract changes. | Delivery mechanism documentation, example clarity, and version bump. |
| 0.4 | Draft for review | Kept the existing platform video sink as playbin's selected sink, supplied the capture session directly to that sink, permitted direct or private-helper capture integration, and added sink-compatibility checks. | Existing-sink association, property handling, scheduled-output attachment, preservation of normal sink behavior and properties, reference ownership, teardown, and integration tests. |
| 0.3 | Draft for review | Expanded frame representation and acquisition safety, made pool and FD ownership explicit, and defined the complete per-pipeline GStreamer integration with construction and test examples. | Plane/object layout, acquisition tokens, failure invariants, process-local FD ownership, pool provisioning, playbin/session association, reference ownership, teardown, and conformance tests. |
| 0.2 | Draft for review | Changed frame acquisition so the HAL returns each newly selected frame once instead of repeatedly returning the same slot. Clarified that several previously returned frames may remain locked at the same time. | Frame acquisition and release, behavior when there is no newer frame, slot exhaustion, flush, and teardown. |
| 0.1 | Initial draft | Introduced the capture session, DMA-BUF pool, GStreamer attachment, frame selection, and lifetime contract. | Complete specification. |

### What changed in version 0.7

- Required direct import of every advertised capture format, modifier, and concrete plane/object layout through both EGL/OpenGL ES and Vulkan.
- Added modifier-aware DMA-BUF import, external-texture and YCbCr sampling, and correctly interpreted RGB(A) requirements.
- Defined EGL and Vulkan FD ownership, failure rollback, graphics-resource destruction, and HAL release ordering.
- Added a vendor graphics capability declaration without extending the HAL or public API.
- Moved the frame/plane/object conceptual model into Section 7 before the concrete representation examples.
- Clarified that `CapturePool` admits concurrent retention and describes a fixed layout; it does not require a separate copied-frame pool or permanently assigned DMA-BUF backing.
- Uprev to v0.7.

### What changed in version 0.6

- Changed ICaptureSession delivery from GstContext to Gst property based on feedback that GstContext is pipeline-wide, not element-specific.
- Added westeros-sink-soc.h contract changes for property handling, including required struct fields and contract function.
- Restructured Section 6 from 7 sections to 5 focused sections.
- Uprev to v0.6.

### What changed in version 0.5

- Added documentation for ICaptureSession delivery mechanism, specifying that SoC vendor provides a shared library with a factory function.
- Added factory function example showing how MW calls the vendor factory to obtain ICaptureSession.
- No API contract changes; interface signatures and method behaviors remain unchanged.

### What changed in version 0.4

- The normal platform policy resolves and creates the concrete video sink before `READY`; the same sink remains assigned to playbin's `video-sink` property.
- MW supplies the pipeline-local `ICaptureSession` directly to that sink through the `rdk-capture-session` property.
- The platform sink implements capture directly or owns a private helper attached to its native scheduled-output path while preserving normal presentation and the existing sink contract.
- The revision provides a complete MW lifecycle example, non-normative Westeros property plumbing, a runnable plumbing prototype requirement, and aligned conformance tests.

### What changed in version 0.3

- The pool keeps the indexed DMA-BUF object and plane layout introduced in 0.2, but no longer exports FDs at pool creation. Each successful acquisition exports one MW-process-local FD for every DMA-BUF object referenced by that slot.
- Each acquisition now carries a unique token. `releaseFrame()` validates the slot and token so a stale frame cannot release a newer lock after slot reuse.
- `CapturedFrame` is non-copyable and move-constructible, with deleted move assignment. FD ownership remains separate from the one required frame release.
- Failed HAL calls leave outputs unchanged, close descriptors created by the failed attempt, roll back temporary locks, and create no new caller obligation. Different slots cannot expose overlapping byte ranges.
- The previous explicit `gst_frame_capture_attach()`/`detach()` side channel is replaced by one explicitly selected `framecapture` video sink or sink-bin per playbin. A pipeline-local GObject property supplies that sink with its borrowed `ICaptureSession`.
- `createPool()` admits retention capacity and the fixed layout before GStreamer enters `READY`. `READY` validates compatibility and connects native scheduled output to the accepted capture contract.
- Section 6 adds complete construction and rollback code plus a vendor integration-test template covering property validation, reference ownership, scheduled-output teardown, and concurrent-pipeline isolation.

### What changed in version 0.2

- `acquireCurrentFrame()` now returns `NO_NEW_FRAME` when the current scheduler selection has already been returned. In this case it does not change `outFrame` or create another lock.
- A successful acquisition still locks the returned slot. Different acquired frames may keep different slots locked concurrently until MW releases them.
- The HAL decides whether a frame is new from its scheduler state, not by comparing reusable slot indices.
- The flush, slot-exhaustion, pipeline-teardown, and late-release rules now follow the new acquisition behavior.
- The conformance checks now cover repeated and concurrent acquisition, multiple locked slots, slot reuse, and post-teardown release.

---

## 0. Contract boundary

This specification defines the in-process contract between **MW** and the **SoC vendor**:

```text
MW
 ├── knows the pipeline output requirement before graph construction
 ├── requests capture capacity and retains the returned layout
 ├── supplies the session to the existing platform video sink
 └── owns returned FDs and matching frame releases

SoC vendor
 ├── implements ICaptureSession
 ├── integrates capture with the existing platform video sink
 ├── connects the capture session to native scheduled output
 └── provides EGL/OpenGL ES and Vulkan import for the fixed capture layout
```

MW means the middleware component that calls the HAL. This specification covers graphics import and texture sampling of an acquired frame; later application composition and display are outside its scope.

---

## 1. Purpose

This specification defines how the SoC vendor exposes decoded clear-video frames to MW.

`ICaptureSession` admits the capture capacity and tracks logical slots while frames are retained. The SoC vendor chooses how those slots are backed from its private native output.

This adds to the existing SoC decode stack; it does not replace it.

## 2. Roles and responsibilities

| Role | Definition | Responsibilities |
|------|------------|------------------|
| **MW** | The middleware that calls the HAL. | - Determines each pipeline's output requirement before graph construction<br>- Obtains one session and calls `createPool()` once to request concurrent retention capacity<br>- Retains the returned immutable layout<br>- Obtains the pipeline's existing platform video sink, sets its session property, and assigns that same sink to playbin<br>- Closes returned FDs and releases acquired frames<br>- Keeps the session alive until sink teardown and all releases are complete |
| **SoC vendor** | The provider of the decoder, capture, and graphics import implementation. | - Implements `ICaptureSession` and the advertised fixed layout<br>- Integrates capture with the existing platform video sink, directly or through a private helper<br>- Owns native queue and backing management<br>- Observes the native scheduled-output frame selection<br>- Provides EGL/OpenGL ES and Vulkan direct import and sampling for the fixed layout<br>- Preserves normal presentation and the platform sink contract<br>- Exports FDs atomically and preserves locked frame contents<br>- Keeps decode, audio, STC, and scheduled selection running when capture slots are exhausted<br>- Tears down safely and accepts required late releases |

### 2.1 Vendor delivery information

For integration and review, the SoC vendor supplies:

1. support for the `rdk-capture-session` GObject property on westerossink;
2. a runnable plumbing prototype showing property receipt before `READY`, playbin assignment, normal presentation, and teardown;
3. the concrete `CaptureCapabilities` values;
4. the fixed pool layout and supported slot counts;
5. evidence that pipeline-local association, native selection, locking, FD export, exhaustion, sink teardown, late release, normal presentation, and existing sink behavior meet this specification; and
6. any platform constraint that makes Video Frame Capture unsupported; and
7. **ICaptureSession delivery mechanism**: The SoC vendor provides a shared library with a factory function to create ICaptureSession instances. The factory function signature is:

```cpp
ICaptureSession* createCaptureSession(const CaptureConfig& config);
```

The library naming convention is platform-specific (e.g., `libcapture-<vendor>.so`). MW loads the library using the platform's normal library discovery mechanism (e.g., `dlopen()`), resolves the factory symbol, and calls it to obtain an ICaptureSession instance. The application retains ownership of the session and must destroy it (via the vendor-provided destruction function or by calling the factory's `destroyCaptureSession()` method) after the pipeline has reached NULL and all acquired frames have been released. The session lifetime must exceed the sink's READY state; the sink borrows the session pointer and does not take ownership.

8. **WesterosSinkSoC ICaptureSession integration**: The SoC vendor must extend their westeros-sink-soc.h contract to support ICaptureSession delivery via the `rdk-capture-session` GObject property as specified in Section 6.3. This includes:
   - Adding `captureSession` and `captureSessionEnabled` fields to `GstWesterosSinkSoc`
   - Implementing `gst_westeros_sink_soc_set_capture_session()`
   - Connecting capture during NULL→READY
   - Disconnecting capture during READY→NULL
9. a graphics capability matrix for the fixed capture layout, declaring:
   - EGL/OpenGL ES extension or core-equivalent functionality;
   - Vulkan extension or core-equivalent functionality;
   - EGL-supported DRM formats and modifiers, including the reported external-only status;
   - Vulkan `VkFormat` and DRM-modifier pairs, sampled-image support, external-memory importability, disjoint-plane support where applicable, and sampler-YCbCr conversion support;
   - direct support for the exact plane count and order, offsets, strides, and shared, separate, or mixed DMA-BUF object mapping returned by `createPool()`; and
   - YCbCr matrix, full or narrow range, and chroma-siting support for the advertised capture format.

The graphics capability matrix is integration evidence. It does not add fields to `CaptureCapabilities`, `CapturePool`, `ICaptureSession`, or any public API.

The vendor also provides whatever private decoder or scheduled-output access the sink's capture integration needs. That access must not interfere with native scheduling, STC progression, audio presentation, normal playback, sink properties, decoder discovery, subtitle integration, or secondary-video behavior.

## 3. Scope

### What this specification defines

- The capture session and fixed pool layout.
- Association with native scheduled output and its frame selection.
- Frame-plane, DMA-BUF object, crop, format, modifier, and timestamp metadata.
- Atomic frame acquisition, FD ownership, frame locks, and release.
- Direct EGL/OpenGL ES and Vulkan import, texturing, graphics-resource lifetime, and cleanup for the advertised capture layout.
- Synchronization, slot exhaustion, sink teardown, and late release.

### 3.1 Terminology

The contract uses these terms exclusively:

| Term | Meaning |
|---|---|
| **Current selection** | The frame chosen by native scheduled output for the current STC/display position. |
| **Frame** | The current selection's timestamp, visible region, slot, and exported FDs. One successful acquisition creates one frame lock. |
| **Slot** | One logical position in the bounded set of frames that may be retained concurrently. It is not necessarily a physical buffer or native queue index. |
| **Slot layout** | The fixed plane description used when a frame is bound to a logical slot. |
| **Frame plane** | One DRM image plane and its byte range and stride within a DMA-BUF object. It owns no FD. |
| **DMA-BUF object** | One logical object entry in the fixed layout. |
| **DMA-BUF object index** | The key that joins frame planes to an acquisition's exported FD. It is not persistent physical allocation identity. |
| **Exported DMA-BUF** | The acquisition-specific binding between one object index and one valid MW-process FD. On successful acquisition, `fd >= 0` is owned and must be closed, and `-1` is invalid. The FD is not a frame lock. |
| **Pool** | The concurrent retention capacity and immutable object, slot, and plane layout accepted for one session. `CapturePool` contains no FDs and does not require a second native queue or copied frames. |

The document does not use “buffer” as a synonym for either slot or frame. “Decoder output buffer” is used only when referring to a vendor-native object outside the capture-pool contract.

## 4. Mandatory ownership model

The following behavior is normative; native queue and backing management remain private to the SoC vendor:

> `createPool()` confirms that the requested number of captured frames can be retained safely. How the vendor provides that capacity is private and may use existing native buffers; a separate capture-buffer allocation is not required. `ICaptureSession` owns the logical slot and lock state.

MW owns the session from the caller side and releases each acquired frame exactly once. The SoC vendor owns the slot state and backing implementation.

The backing bound to an acquired frame must remain unchanged until its matching `releaseFrame()`. After successful release, a later acquisition may reuse the logical slot with different backing and newly exported FDs. Pipeline teardown must not invalidate an outstanding acquired frame; resources needed to complete its late release remain valid until the session is closed.

Capture-slot exhaustion must not block decode, native scheduled selection, audio presentation, or STC progression.

FD lifetime and frame-content lifetime are separate. MW owns and explicitly closes every FD returned by a successful acquisition. Closing an FD does not release the frame lock, and an open FD does not preserve stable contents after release. MW completes every downstream use before calling `releaseFrame()`; after release, it must not access that frame and the SoC may reuse the slot.

A successful acquisition also guarantees that decoder writes, cache maintenance, and SoC-side synchronization are complete, so the selected slot is safe to read. The HAL returns no graphics fence. MW completes downstream use before release, while any decoder, capture, cache, or native fence handling remains private to the SoC implementation.

## 5. Session lifecycle and selection state

### 5.1 Pipeline-scoped lifetime

MW knows whether a pipeline requires frame capture before graph construction. Each capture-enabled pipeline has one dedicated `ICaptureSession`. Before the GStreamer integration in Section 6, MW calls `createPool()` to confirm the required concurrent retention capacity and obtain the immutable layout defined in Section 9.

A session belongs to one pipeline and is never reassociated or reused. Its lifetime extends beyond sink teardown until the final acquisition opportunity is resolved or abandoned and every acquired frame is released.

### 5.2 Selection state

The session passively tracks the native STC scheduler's current captured selection, whether that selection has already been acquired, and every active frame lock. Selection identity is independent of reusable slot identity: a released slot may later hold a different selection that returns `OK` once.

A slot is writable only when it has no frame lock and no write in progress. Before reusing an unlocked current slot, the session atomically invalidates the old selection so concurrent acquisition returns `NO_FRAME` rather than observing an overwrite. Section 9 defines acquisition and release semantics.

### 5.3 Post-teardown behavior

The platform sink first completes the disconnection contract in Section 6.4. The session, final current selection, and locked slots then remain valid while MW completes these obligations:

1. An unacquired final selection remains available once; transient `NO_RESOURCES` leaves it available for retry.
2. An already acquired final selection returns `NO_NEW_FRAME`, and no valid final selection returns `NO_FRAME`.
3. The session continues accepting `releaseFrame()` for every acquired frame, whose contents remain valid until release.
4. MW destroys the session only after the final acquisition opportunity is resolved or abandoned and every frame lock is released.

The selection does not advance after sink teardown.

## 6. GStreamer integration and scheduled-output behavior

### 6.1 Required integration model

The platform video sink is westerossink. MW creates the westerossink instance, assigns it to playbin's `video-sink` property, sets the `rdk-capture-session` property on the westerossink instance, and drives it to READY to validate capture integration. Capture integrates with the existing surface and video-plane output.

The platform sink integrates capture directly or owns a private helper attached to its native scheduled-output path. The helper may be a private object or child GStreamer element. The public pipeline topology remains unchanged.

For every returned frame, the vendor implementation uses the same STC and scheduling rules as native scheduled output. It publishes only a selection established after decoder consumption. `CapturedFrame.presentationTimeNs` is the selected frame's PTS in that scheduler timeline.

Capture preserves normal presentation and the platform sink's existing contract, including its properties, surface output, decoder discovery, subtitle integration, secondary-video behavior, and scheduling. A pipeline not requesting capture uses the same platform sink without a capture session and is otherwise unaffected.

Each capture-enabled pipeline has one independent relationship:

```text
playbin instance
  -> westerossink instance
       -> capture integration (direct or private helper)
            -> ICaptureSession
                 -> admitted capture reservation and layout
```

Concurrent or dual video uses one complete relationship per pipeline. Sink A receives session A and can access only reservation A; sink B receives session B and can access only reservation B. The SoC implementation does not choose among active playbins. If a vendor-private graph contains multiple scheduled-output branches, the platform sink associates the session with the exact branch represented by that sink instance.

### 6.2 Session-property contract

The property name is `rdk-capture-session` with type G_TYPE_POINTER and value type `ICaptureSession *`. The pointer is opaque to GStreamer and borrowed by the sink. MW keeps it valid until sink teardown completes and retains the session longer when final acquisition or late release remains possible.

MW sets the property directly on the westerossink instance before any state transition that can start decoder output. The sink accepts exactly one non-null session. Reapplying the same pointer is idempotent; a null pointer or attempt to replace it with a different session is invalid.

Before entering `READY`, the sink validates that `createPool()` completed successfully and that native scheduled output can satisfy the accepted retention capacity and fixed layout. Invalid configuration or incompatible setup makes the transition fail with a GStreamer error, rolls back partial capture setup, and starts no capture writes.

The property does not own the session, create a pool, or carry the `CapturePool` description. Private access to backing and scheduler state remains inside the paired vendor implementations.

### 6.3 WesterosSinkSoC contract

Each SoC-specific `westeros-sink-soc.h` must extend the `GstWesterosSinkSoc` struct with the following fields:

```cpp
struct _GstWesterosSinkSoc
{
    // ... existing fields ...

    // ICaptureSession support
    void *captureSession;              // Borrowed ICaptureSession pointer
    gboolean captureSessionEnabled;   // Whether capture session is active
};
```

Each SoC-specific `westeros-sink-soc.c` must implement the following contract function:

```cpp
gboolean gst_westeros_sink_soc_set_capture_session(
    GstWesterosSink *sink,
    void *session);
```

**Semantics**:
- Input: `sink` - WesterosSink instance, `session` - ICaptureSession pointer (borrowed, not owned)
- Return: TRUE if session accepted, FALSE if rejected
- Must validate that session is non-null
- Must store session in `sink->soc.captureSession`
- Must set `sink->soc.captureSessionEnabled = TRUE`
- Must reject if session is already set (idempotent: reapplying same session is OK, different session is error)

The common `westeros-sink.c` must implement a property handler for `rdk-capture-session`:

```cpp
static void gst_westeros_sink_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
    GstWesterosSink *sink = GST_WESTEROS_SINK(object);

    switch (prop_id)
    {
        case PROP_RDK_CAPTURE_SESSION:
        {
            gpointer session = g_value_get_pointer(value);
            if (!gst_westeros_sink_soc_set_capture_session(sink, session))
            {
                GST_ERROR_OBJECT(sink, "Failed to set capture session");
            }
            break;
        }
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
            break;
    }
}
```

The property must be registered in the class_init function.

**Timing requirements**:
- **Connect**: During NULL→READY, after pool creation and before decoder starts, in `gst_westeros_sink_soc_null_to_ready()`
- **Disconnect**: During READY→NULL, before decoder teardown, in `gst_westeros_sink_soc_ready_to_null()`

This timing matches Section 6.2: READY validates compatibility and connects native scheduled output to the accepted capture contract.

**Session lifetime**: The ICaptureSession pointer is borrowed by the sink. MW keeps it valid until sink teardown completes and retains the session longer when final acquisition or late release remains possible. The sink does not take ownership of the session.

### 6.4 Middleware lifecycle

The complete construction and teardown order is:

```plantuml
@startuml
participant MW
participant "playbin instance" as Playbin
participant "westerossink instance" as Sink
participant "native scheduled output +\nprivate capture integration" as Output
participant ICaptureSession as Session

MW -> Session: getCapabilities()
MW -> Session: createPool(slotCount, outPool)
Session --> MW: OK + immutable pool description
MW -> Playbin: create playbin instance
MW -> Sink: create westerossink instance
MW -> Playbin: set video-sink = Sink
MW -> Sink: set rdk-capture-session property
MW -> Sink: set READY and wait for completion
Sink -> Output: configure native output and capture integration
Output -> Session: connect scheduled output to capture session
Sink --> MW: READY validation succeeded
MW -> Playbin: later activate configured pipeline
Output -> Output: present and capture the same STC-selected frame

MW -> Playbin: teardown / set NULL and wait
MW -> Sink: ensure NULL and wait
Sink -> Output: stop new capture writes; complete or cancel active write
Sink -> Output: disconnect capture before destroying native output
Sink --> MW: NULL transition completed
MW -> Playbin: clear video-sink property
note over MW,Session
MW retains Session for final acquisition
and outstanding releaseFrame() obligations.
end note
@enduml
```

The application selects Video Frame Capture through its media API before pipeline construction. It does not manipulate GStreamer objects or pass `ICaptureSession` directly. MW performs the integration using one session and the westerossink instance.

MW creates the westerossink instance, assigns it to playbin's `video-sink` property, sets the `rdk-capture-session` property on the westerossink instance, then drives it to READY to validate capture integration. Success proves that the sink accepted the property and can connect native scheduled output to the accepted capture contract. It does not claim that playbin has prerolled or activated a media source.

The caller retains `session` and `outPool` after success. `outPool` is the immutable layout used to interpret acquired frames; it is not passed to the sink. A failure after `createPool()` returns the sink to `NULL`, and the caller discards that session and creates a fresh one for any retry.

Pipeline teardown takes playbin to `NULL`, which destroys the westerossink instance. MW retains the session until teardown completes, the final acquisition opportunity is resolved, and every acquired frame has been released.

**Obtaining ICaptureSession from vendor library**: MW loads the vendor-provided shared library using the platform's normal library discovery mechanism (e.g., `dlopen()`), resolves the factory symbol, and calls it to obtain an ICaptureSession instance. The application retains ownership of the session and must destroy it after the pipeline has reached NULL and all acquired frames have been released.

### 6.5 Prototype and conformance testing

A vendor can use the same helper shape with a test `ICaptureSession` and an instrumented westerossink. At minimum, an integration test verifies:

```cpp
TEST(FrameCaptureIntegration, PreservesPlatformSinkAndAssociatesSession)
{
    TestCaptureSession session;
    GstElement *playbin = gst_element_factory_make("playbin", nullptr);
    CapturePool pool;

    ASSERT_NE(playbin, nullptr);

    ASSERT_TRUE(configureFrameCaptureOutput(
        playbin, session, 4, pool));

    GstElement *configuredSink = nullptr;
    g_object_get(playbin, "video-sink", &configuredSink, nullptr);
    ASSERT_NE(configuredSink, nullptr);
    ASSERT_STREQ(GST_ELEMENT_NAME(configuredSink), "westerossink");

    EXPECT_EQ(session.createPoolCallCount(), 1u);
    EXPECT_EQ(session.requestedSlotCount(), 4u);
    EXPECT_EQ(testCaptureSession(configuredSink), &session);
    EXPECT_TRUE(testScheduledOutputConnected(configuredSink));
    EXPECT_TRUE(testNormalPresentationActive(configuredSink));
    EXPECT_TRUE(existingSinkPropertiesUnchanged(configuredSink));

    ASSERT_TRUE(teardownFrameCaptureOutput(playbin));

    gst_object_unref(configuredSink);
    gst_object_unref(playbin);
}
```

The test functions are vendor test hooks, not production APIs. Additional tests create two playbins with two westerossink instances and two sessions and verify that each sink reaches only its own session and pool. Failure tests cover null session property, repeated property with a different session, incompatible `READY` setup, and rollback to `NULL` without capture writes.

The vendor supplies a runnable plumbing prototype using westerossink or an instrumented build of westerossink. The prototype demonstrates property receipt on the westerossink instance before `READY`, successful `READY` validation, assignment to playbin, normal presentation, and confirmed `NULL` teardown. It need not implement frame export while validating this plumbing.

The integration suite also verifies that normal scheduled presentation continues while capture is enabled; existing sink properties retain their values and effects; decoder discovery, subtitle integration, secondary-video behavior, geometry, visibility, and immediate-output behavior remain usable; capture-slot exhaustion omits capture without blocking presentation; and teardown disconnects capture before scheduled output is destroyed. Final acquisition and late `releaseFrame()` remain valid after sink teardown.


## 7. Capture value types

```cpp
enum class CaptureStatus : uint32_t
{
    OK,
    NO_NEW_FRAME,
    UNSUPPORTED,
    INVALID_ARGUMENT,
    NO_RESOURCES,
    NO_FRAME,
    FATAL_ERROR,
};

constexpr uint32_t kInvalidCaptureSlotIndex = std::numeric_limits<uint32_t>::max();
constexpr uint64_t kInvalidAcquisitionToken = 0;

struct Size
{
    uint32_t width;
    uint32_t height;
};

struct Rectangle
{
    uint32_t x;
    uint32_t y;
    Size size;
};

struct CaptureCapabilities
{
    uint32_t size{sizeof(CaptureCapabilities)};
    uint32_t drmFormat;
    uint64_t drmModifier;
    Size maximumContentSize;
    uint32_t maximumSlots;
};

struct DmaBufObject
{
    uint32_t objectIndex;
    uint64_t sizeBytes;
};

struct FramePlane
{
    uint32_t planeIndex;
    uint32_t dmaBufObjectIndex;
    uint64_t offsetBytes;
    uint64_t lengthBytes;
    uint32_t strideBytes;
};

struct CaptureSlotLayout
{
    uint32_t slotIndex;
    std::vector<FramePlane> planes;
};

struct ExportedDmaBuf
{
    uint32_t dmaBufObjectIndex;
    int fd{-1};
};

struct CapturePool
{
    Size backingSize;
    std::vector<DmaBufObject> dmaBufObjects;
    std::vector<CaptureSlotLayout> slotLayouts;
};

struct CapturedFrame
{
    uint32_t slotIndex{kInvalidCaptureSlotIndex};
    uint64_t acquisitionToken{kInvalidAcquisitionToken};
    int64_t presentationTimeNs;
    Rectangle visibleRegion;
    std::vector<ExportedDmaBuf> exportedDmaBufs;

    CapturedFrame() : presentationTimeNs{0}, visibleRegion{} {}
    CapturedFrame(const CapturedFrame &) = delete;
    CapturedFrame &operator=(const CapturedFrame &) = delete;
    CapturedFrame(CapturedFrame &&other) noexcept;
    CapturedFrame &operator=(CapturedFrame &&other) noexcept = delete;
    ~CapturedFrame() = default;
};
```

The value types follow these rules:

- `backingSize` gives the fixed dimensions supported by each logical slot; it does not identify physical backing.
- `DmaBufObject.sizeBytes` gives the minimum size of the object bound to that object key for an acquisition; it does not pre-create or identify a physical allocation.
- MW owns each successful `fd >= 0`, closes it explicitly, and sets it to `-1`. `-1` means invalid. Destructors do not close FDs.
- `CapturedFrame` is non-copyable. A valid `slotIndex` and matching `acquisitionToken` mean exactly one `releaseFrame()` is owed, independently of FD ownership.
- The HAL assigns a unique non-zero `acquisitionToken` for each successful acquisition. The token is opaque; MW must not interpret or reconstruct it.
- Moving a frame through the move constructor transfers its metadata, FDs, token, and release obligation without `dup()` or `close()`. The source FDs become `-1`, its `slotIndex` becomes `kInvalidCaptureSlotIndex`, and its `acquisitionToken` becomes `kInvalidAcquisitionToken`.
- Move assignment is deleted to avoid undefined behavior when the destination is non-empty. To reuse an existing instance, callers must release its frame lock, close and invalidate its FDs, return it to the empty state, and pass it to `acquireCurrentFrame()`; transferring ownership to a different object requires the move constructor.
- Pool `slotIndex` and `objectIndex` values equal their positions in `slotLayouts` and `dmaBufObjects`. They are unique and contiguous from zero; no pool slot may use `kInvalidCaptureSlotIndex`. They are layout and bookkeeping keys, not native queue indices, FD identities, or persistent allocation identities.
- `planeIndex` follows the plane order defined by the reported DRM format. Import code maps that order to its graphics API.
- Each plane references one valid object. Validate its range without overflow: `offsetBytes <= sizeBytes` and `lengthBytes <= sizeBytes - offsetBytes`. The range and stride must fit the advertised layout.
- For any two planes that belong to different slots and can be bound within the same physical DMA-BUF object, their half-open byte ranges `[offsetBytes, offsetBytes + lengthBytes)` must not overlap. Planes within one slot may share an object as defined by that format. Binding simultaneously locked slots to distinct physical allocations also provides the required content isolation.
- For every `OK` frame, validate `visibleRegion` without overflow: `x <= backingSize.width`, `size.width <= backingSize.width - x`, `y <= backingSize.height`, and `size.height <= backingSize.height - y`.
- Plane and object metadata remain fixed for the pool lifetime. Storage not exposed as a DRM plane remains part of its DMA-BUF object.
- On `OK`, `exportedDmaBufs` contains exactly one entry for each distinct object referenced by the selected slot, with no duplicate or unrelated object. The selected slot's planes are resolved by joining `FramePlane.dmaBufObjectIndex` to `ExportedDmaBuf.dmaBufObjectIndex` in that frame.
- Each locked frame has its own stable object-to-FD bindings. After release and slot reuse, the same logical object keys may bind different physical allocations and newly exported FDs.

Plane count, object count, and FD count are independent.

A `CapturedFrame` is empty only when its `slotIndex` is invalid, its `acquisitionToken` is invalid, and every FD is `-1`. Closing all FDs does not make a frame empty while its slot index and token remain valid; the frame lock still requires release.

FDs are exported only when a frame is acquired. MW owns and closes them. Closing an FD does not release the frame lock, and `releaseFrame()` does not inspect FD values.

### 7.1 Conceptual model

The relationships are directional and do not imply one-to-one cardinality:

```
Frame (selected content and timing)
  └── Slot layout
       ├── Frame plane 0 ──references──> DMA-BUF object A ──exported as──> FD for A
       ├── Frame plane 1 ──references──> DMA-BUF object A ──same object, no second FD
       └── Frame plane 2 ──references──> DMA-BUF object B ──exported as──> FD for B
```

- A **Frame** is decoded content written to one reusable slot at a specific STC selection.
- Its **slot layout** contains one logical record for each DRM plane.
- Each **frame plane** references exactly one **DMA-BUF object** role and describes its byte range and stride within the object bound for that acquisition.
- Several planes may reference the same object at different offsets, or each plane may reference a different object. Mixed arrangements are valid.
- A successful acquisition exports one FD for each distinct object referenced by the selected slot, never one FD per plane by implication.
- Neither the process-local FD value nor `dmaBufObjectIndex` is physical allocation identity. `dmaBufObjectIndex` joins the fixed plane layout to that acquisition's exported handle records.

### 7.2 Valid frame representations

The following examples use illustrative process-local FD values. Each object appears once in `dmaBufObjects`; each successful acquisition exports each referenced object once.

**Example A — one plane, one object, one FD**

```text
plane 0 ──> object 0 ──> FD 10
```

```cpp
dmaBufObjects = {{.objectIndex = 0, .sizeBytes = S}};
planes = {{.planeIndex = 0, .dmaBufObjectIndex = 0,
           .offsetBytes = 0, .lengthBytes = S, .strideBytes = P}};
exportedDmaBufs = {{.dmaBufObjectIndex = 0, .fd = 10}};
```

**Example B — two planes sharing one object and one FD**

```text
plane 0 ──> object 0 @ offset 0 ──> FD 10
plane 1 ──> object 0 @ offset N ──> same FD 10
```

```cpp
dmaBufObjects = {{.objectIndex = 0, .sizeBytes = S}};
planes = {
    {.planeIndex = 0, .dmaBufObjectIndex = 0,
     .offsetBytes = 0, .lengthBytes = N, .strideBytes = P0},
    {.planeIndex = 1, .dmaBufObjectIndex = 0,
     .offsetBytes = N, .lengthBytes = S - N, .strideBytes = P1},
};
exportedDmaBufs = {{.dmaBufObjectIndex = 0, .fd = 10}};
```

**Example C — two planes, two objects, two FDs**

```text
plane 0 ──> object 0 ──> FD 10
plane 1 ──> object 1 ──> FD 11
```

```cpp
dmaBufObjects = {
    {.objectIndex = 0, .sizeBytes = S0},
    {.objectIndex = 1, .sizeBytes = S1},
};
planes = {
    {.planeIndex = 0, .dmaBufObjectIndex = 0,
     .offsetBytes = 0, .lengthBytes = S0, .strideBytes = P0},
    {.planeIndex = 1, .dmaBufObjectIndex = 1,
     .offsetBytes = 0, .lengthBytes = S1, .strideBytes = P1},
};
exportedDmaBufs = {
    {.dmaBufObjectIndex = 0, .fd = 10},
    {.dmaBufObjectIndex = 1, .fd = 11},
};
```

**Example D — shared and separate objects in one frame**

```text
plane 0 ──> object 0 @ offset 0 ──> FD 10
plane 1 ──> object 0 @ offset N ──> same FD 10
plane 2 ──> object 1 @ offset 0 ──> FD 11
```

```cpp
dmaBufObjects = {
    {.objectIndex = 0, .sizeBytes = S0},
    {.objectIndex = 1, .sizeBytes = S1},
};
planes = {
    {.planeIndex = 0, .dmaBufObjectIndex = 0,
     .offsetBytes = 0, .lengthBytes = N, .strideBytes = P0},
    {.planeIndex = 1, .dmaBufObjectIndex = 0,
     .offsetBytes = N, .lengthBytes = S0 - N, .strideBytes = P1},
    {.planeIndex = 2, .dmaBufObjectIndex = 1,
     .offsetBytes = 0, .lengthBytes = S1, .strideBytes = P2},
};
exportedDmaBufs = {
    {.dmaBufObjectIndex = 0, .fd = 10},
    {.dmaBufObjectIndex = 1, .fd = 11},
};
```

### 7.3 DRM NV12 linear example

For this example, `CaptureCapabilities` reports `DRM_FORMAT_NV12` and `DRM_FORMAT_MOD_LINEAR`; the pool supplies the plane/object layout shown below. Linux `drm_fourcc.h` defines NV12 as a two-plane format and the modifier as linear storage. An NV12 slot may represent plane 0 (luma Y) and plane 1 (interleaved chroma UV) using either Example B, where both planes reference one DMA-BUF object at different offsets and acquisition exports one FD, or Example C, where each plane references a separate object and acquisition exports two object-indexed FDs. The modifier does not determine DMA-BUF object or FD cardinality; the plane-to-object mapping in the slot layout does. These examples define representational capability, while an implementation must still advertise and provide an arrangement importable through every required graphics path.

### 7.4 Fixed capture layout

Each implementation supports one fixed capture contract. `getCapabilities()` reports its format, modifier, and limits. `createPool()` reports the concrete object, slot, and plane layout. The two records belong to the same session and together describe the layout; neither duplicates the other's fields.

The implementation need not support every shared-object, separate-object, or mixed arrangement shown in the examples. Its returned pool must faithfully represent its one fixed format/modifier arrangement and be usable through every required graphics path.

For the requested slot count, `CapturePool` describes:

- the actual logical slot capacity in `backingSize`;
- each DMA-BUF object's index and size; and
- each slot's indexed planes, object references, offsets, lengths, and strides.

Every slot supports `backingSize`; that size must not exceed `maximumContentSize`. `slotLayouts.size()` equals the requested count, which must not exceed `maximumSlots`. If different slots can be bound within the same physical DMA-BUF object, `createPool()` must validate that their referenced byte ranges do not overlap. Alternatively, distinct physical allocations for simultaneously locked slots provide the required content isolation.

The format, modifier, dimensions, plane topology, offsets, strides, object sizes and roles, and slot count remain fixed until the session is closed. Physical DMA-BUF identity and FD values are acquisition-specific and may change after release. Resolution or crop changes update `CapturedFrame.visibleRegion`; they do not replace the slot reservation.

If content exceeds `CaptureCapabilities.maximumContentSize` or cannot be represented by the implemented capture layout, capture returns `UNSUPPORTED` or `FATAL_ERROR` and stops updating its current selection. It does not silently disable capture while continuing only normal platform-sink output; MW chooses any later pipeline reconstruction. Unrelated normal-output pipelines remain available. Runtime capture-slot replacement is outside this specification. One `ICaptureSession` has one logical slot reservation for its complete lifetime.

## 8. Graphics import and texturing requirements

### 8.1 Common requirements

A conforming implementation supports both EGL/OpenGL ES and Vulkan import paths for its fixed capture contract. In each path, the imported resource directly references the acquired frame's DMA-BUF backing. An intermediate copy or conversion into different backing does not satisfy this requirement.

Both paths combine the immutable `CaptureCapabilities` and `CapturePool` layout with the selected frame's acquisition-specific object-to-FD bindings. They support the exact DRM format and modifier; plane count and DRM plane order; every plane's object index, offset, length, and stride; and the shared-object, separate-object, or mixed DMA-BUF mapping. Import does not assume persistent DMA-BUF identity across acquisitions. A combination is supported only when it can be sampled, not merely queried, allocated, or imported.

For a YUV capture layout, sampling returns correctly interpreted RGB(A) using the source matrix, full or narrow sample range, and chroma siting. The graphics implementation performs that interpretation during external-texture or sampler-YCbCr sampling; the consumer is not required to sample raw YUV planes or select the conversion.

The importing graphics resources and the frame lock have the same content-validity boundary. Graphics work that can access the frame completes before those resources are destroyed, and every importing graphics resource is destroyed before the matching `releaseFrame()` permits slot reuse. Successful HAL acquisition already guarantees that decoder writes, cache maintenance, and SoC-side synchronization are complete as specified in Section 4; this contract adds no native-fence extension requirement.

Extension functionality promoted into a core API version satisfies the corresponding requirement. The vendor capability declaration identifies whether each requirement is provided by an extension or by core functionality.

### 8.2 EGL DMA-BUF image import

The EGL implementation provides the functionality defined by:

- `EGL_EXT_image_dma_buf_import`, including the `EGL_LINUX_DMA_BUF_EXT` image target and the DRM format and plane FD, offset, and pitch attributes;
- `EGL_EXT_image_dma_buf_import_modifiers`, including per-plane modifier low/high attributes and `eglQueryDmaBufFormatsEXT()` / `eglQueryDmaBufModifiersEXT()`; and
- EGL image creation and destruction through `eglCreateImage()` / `eglDestroyImage()`, or the equivalent `eglCreateImageKHR()` / `eglDestroyImageKHR()` functionality defined by `EGL_KHR_image_base`.

`EGL_LINUX_DMA_BUF_EXT` is the image target defined by `EGL_EXT_image_dma_buf_import`; it is not an extension name.

MW constructs the image attributes from the capture records as follows:

- `CaptureCapabilities.drmFormat` supplies `EGL_LINUX_DRM_FOURCC_EXT`;
- each DRM plane uses the FD for its `dmaBufObjectIndex`, repeating that FD in the attribute list when several planes reference the same DMA-BUF object;
- `FramePlane.offsetBytes` and `strideBytes` supply the matching plane OFFSET and PITCH attributes;
- `CaptureCapabilities.drmModifier` supplies every plane's `EGL_DMA_BUF_PLANE*_MODIFIER_LO_EXT` and `EGL_DMA_BUF_PLANE*_MODIFIER_HI_EXT` attributes; and
- the source colour metadata available to the vendor integration supplies `EGL_YUV_COLOR_SPACE_HINT_EXT`, `EGL_SAMPLE_RANGE_HINT_EXT`, `EGL_YUV_CHROMA_HORIZONTAL_SITING_HINT_EXT`, and `EGL_YUV_CHROMA_VERTICAL_SITING_HINT_EXT`.

`eglQueryDmaBufFormatsEXT()` and `eglQueryDmaBufModifiersEXT()` report the advertised format and modifier. Their result also identifies whether the combination is external-only; external-only support is valid because Section 8.3 requires the external-texture path.

EGL image creation does not transfer ownership of the supplied DMA-BUF FDs. After `eglCreateImage()` or `eglCreateImageKHR()` returns, MW closes every supplied FD copy that is no longer needed for another import attempt, on both success and failure. A successful EGL image retains the imported backing until `eglDestroyImage()` or `eglDestroyImageKHR()`.

A failed EGL import closes every MW-owned FD and destroys every EGL image created by that attempt. It leaves no graphics resource associated with the frame.

### 8.3 OpenGL ES external-texture binding and sampling

The OpenGL ES implementation provides:

- `GL_OES_EGL_image` and `glEGLImageTargetTexture2DOES()`;
- `GL_OES_EGL_image_external`, `GL_TEXTURE_EXTERNAL_OES`, and `samplerExternalOES`;
- `GL_OES_EGL_image_external_essl3` for ESSL 3 external-texture shaders; and
- the core texture operations `glGenTextures()`, `glBindTexture()`, `glTexParameteri()`, and `glDeleteTextures()`.

MW creates one external texture for each EGL image used to expose the frame. All resources in one frame use `GL_TEXTURE_EXTERNAL_OES`. After binding that target, MW associates the EGL image with the texture by calling `glEGLImageTargetTexture2DOES()` and configures texture parameters valid for an external texture. Sampling through `samplerExternalOES` returns the interpreted RGB(A) values required by Section 8.1.

The resource order is:

1. create the EGL image from the acquired DMA-BUF layout;
2. create and bind the GL texture, then associate the EGL image with `glEGLImageTargetTexture2DOES()`;
3. retain the frame and its EGL/GL resources until all graphics work using them has completed;
4. delete the texture with `glDeleteTextures()`;
5. destroy the EGL image with `eglDestroyImage()` or `eglDestroyImageKHR()`; and
6. call `releaseFrame()` only after the texture and EGL image no longer reference the frame.

Failure after partial construction deletes created textures, destroys created EGL images, closes all MW-owned FDs, and preserves the frame lock until that rollback is complete.

### 8.4 Vulkan DMA-BUF image import and sampling

The Vulkan implementation provides the functionality defined by:

- `VK_KHR_external_memory`;
- `VK_KHR_external_memory_fd`;
- `VK_EXT_external_memory_dma_buf`, including `VK_EXTERNAL_MEMORY_HANDLE_TYPE_DMA_BUF_BIT_EXT`;
- `VK_EXT_image_drm_format_modifier`;
- `VK_KHR_get_memory_requirements2` and `VK_KHR_bind_memory2`, or their core equivalents; and
- `VK_KHR_sampler_ycbcr_conversion`, or Vulkan 1.1 core functionality, for YUV capture formats.

The complete query, import, and texturing path provides:

- `vkEnumerateDeviceExtensionProperties()` or core-version checks to establish the required functionality;
- `vkGetPhysicalDeviceFeatures2()` to confirm sampler-YCbCr conversion support where required;
- `vkGetPhysicalDeviceFormatProperties2()` with `VkDrmFormatModifierPropertiesListEXT` to identify sampled-image support for the DRM modifier;
- `vkGetPhysicalDeviceImageFormatProperties2()` with external-memory and DRM-modifier query structures to validate DMA-BUF import for the required image usage;
- `vkCreateImage()` with `VkExternalMemoryImageCreateInfo` and `VkImageDrmFormatModifierExplicitCreateInfoEXT` populated from the concrete modifier plane layouts;
- `vkGetMemoryFdPropertiesKHR()` for each DMA-BUF handle to be imported;
- `vkGetImageMemoryRequirements2()` for whole-image or disjoint-plane memory requirements;
- `vkAllocateMemory()` with `VkImportMemoryFdInfoKHR` and `VK_EXTERNAL_MEMORY_HANDLE_TYPE_DMA_BUF_BIT_EXT`;
- `vkBindImageMemory2()`, including `VkBindImagePlaneMemoryInfo` for disjoint multi-planar images where required;
- `vkCreateImageView()`; and
- for YUV images, `vkCreateSamplerYcbcrConversion()` with `VkSamplerYcbcrConversionCreateInfo` populated from the source matrix, range, component mapping, and chroma siting, `VkSamplerYcbcrConversionInfo` on the image-view and sampler creation paths, and `vkCreateSampler()`.

The queried format/modifier properties include sampled-image usage and the exact shared, separate, or mixed allocation arrangement represented by the capture pool. DRM image planes, DMA-BUF objects, Vulkan format planes, and DRM-modifier memory planes are distinct concepts; their counts are not assumed to be equal. The vendor declares the `VkFormat` and binding model that represent its fixed DRM format, modifier, and concrete pool layout.

A successful `vkAllocateMemory()` carrying `VkImportMemoryFdInfoKHR` transfers ownership of that FD to the Vulkan implementation. MW does not close or reuse that FD afterward. When allocation fails before ownership transfers, MW closes the still-owned FD. If one DMA-BUF object must be imported into more than one Vulkan memory allocation, each ownership-transferring import uses its own valid duplicated FD.

The Vulkan resource order is:

1. query support and create the external, modifier-aware image;
2. import and bind the memory required for each DMA-BUF object or Vulkan memory plane;
3. create the required sampler-YCbCr conversion, image view, and sampler;
4. retain the frame and all Vulkan resources until submitted graphics work using them has completed;
5. destroy samplers and image views that depend on the conversion or image with `vkDestroySampler()` and `vkDestroyImageView()`;
6. destroy the conversion with `vkDestroySamplerYcbcrConversion()` after its dependent objects;
7. destroy the image with `vkDestroyImage()`;
8. free imported memory with `vkFreeMemory()`; and
9. call `releaseFrame()` only after every importing Vulkan resource has been destroyed or freed.

Failure after partial construction unwinds resources in reverse dependency order. It destroys every created Vulkan object, frees every allocated memory object, closes every FD whose ownership did not transfer, and preserves the frame lock until rollback is complete.

### 8.5 Vendor capability declaration

The vendor supplies one matrix row for its advertised capture format, modifier, and concrete layout. The row declares:

- the EGL format/modifier query result and external-only flag;
- `GL_OES_EGL_image`, external-texture, and ESSL 3 support;
- the Vulkan `VkFormat`, modifier, sampled-image features, external-memory importability, disjoint-plane support where applicable, and sampler-YCbCr conversion support;
- support for the concrete plane count/order, offsets, strides, and plane-to-DMA-BUF-object mapping; and
- confirmation that the layout is directly importable, sampleable as correctly interpreted RGB(A), and releasable through both graphics paths.

This matrix is integration evidence. It does not add runtime capability fields or replace the normative requirements above.

## 9. Session interface

```cpp
class ICaptureSession
{
public:
    virtual ~ICaptureSession() = default;

    virtual CaptureStatus getCapabilities(CaptureCapabilities &out) const = 0;

    virtual CaptureStatus createPool(
        uint32_t slotCount,
        CapturePool &outPool) = 0;

    virtual CaptureStatus acquireCurrentFrame(
        CapturedFrame &outFrame) = 0;

    virtual CaptureStatus releaseFrame(
        CapturedFrame &frame) = 0;
};
```

`getCapabilities()` reports exactly the fixed `drmFormat`, `drmModifier`, `maximumContentSize`, and `maximumSlots` for the session. It does not report plane count/order, DMA-BUF objects, plane mappings, offsets, lengths, strides, or the concrete capture dimensions. `OK` fully populates `out`; every non-`OK` result leaves it unchanged.

`createPool()` accepts `slotCount` only when `slotCount > 0` and `slotCount <= maximumSlots`; otherwise it returns `INVALID_ARGUMENT` and leaves `outPool` unchanged. The call may complete successfully only once per session; a later call returns `INVALID_ARGUMENT`. On `OK`, it synchronously admits the requested logical retention capacity and guarantees that up to `slotCount` distinct captured selections can remain locked without stalling normal playback. It returns the concrete immutable pool layout: `backingSize`, DMA-BUF object indices/sizes, and each slot's plane indices, object references, offsets, lengths, and strides. It does not repeat format or modifier, contains no FDs, and need not bind slots to physical backing or allocate additional frame buffers. Every plane range must pass the overflow-safe validation in Section 7. `backingSize` must not exceed `maximumContentSize`, and the returned layout must be valid for the same session's format and modifier. Every non-`OK` result leaves `outPool` unchanged, releases any partial admission, and creates no pool.

The capability result and pool belong to the same session and are interpreted together. Format, modifier, and limits remain fixed for that session. MW keeps both records while it needs to interpret the pool; no extra capability fields or duplicated pool fields are implied.

The SoC vendor chooses how to provide the slots. `maximumSlots` is the greatest count that preserves the resources needed for normal playback.

`acquireCurrentFrame()` requires an empty `outFrame`: `slotIndex == kInvalidCaptureSlotIndex`, `acquisitionToken == kInvalidAcquisitionToken`, and every `ExportedDmaBuf.fd == -1`. These are the exact empty-state conditions defined in Section 7; any other FD value, including a negative value other than `-1`, is non-empty. A valid slot index and matching token mean that output still owes a release even if all its FDs are already closed and set to `-1`. Passing a non-empty output returns `INVALID_ARGUMENT` without changing it.

For an empty output, the method examines the current native selection. A new selection is temporarily locked and bound to a free logical slot or equivalent admitted bookkeeping entry. One FD for each distinct object required by that frame is exported, and the complete result—including the valid slot index and release obligation—is moved into `outFrame` before returning `OK`. That logical-to-native binding remains unchanged until release. Every returned FD is valid in the MW process, is independently owned by MW before the call returns, and remains valid if the vendor implementation closes any descriptor from which it was obtained. The vendor may obtain, duplicate, export, or transport the descriptor through in-process or vendor-service-specific mechanisms; those mechanisms and their descriptor lifetimes are private and are not part of this interface. No valid selection returns `NO_FRAME`; an already returned selection returns `NO_NEW_FRAME`. Every non-`OK` result leaves `outFrame` unchanged and creates no release obligation.

If descriptor creation/export fails because a transient resource is unavailable, including descriptor exhaustion, `acquireCurrentFrame()` returns `NO_RESOURCES`. Before returning, the vendor implementation explicitly closes every FD created for that attempt, removes the temporary lock, leaves the selection not returned, and does not move any result into `outFrame`. No matching release is required and the selection remains retriable. An unrecoverable session/backing failure returns `FATAL_ERROR` with the same all-or-nothing cleanup. `FATAL_ERROR` is not retriable; capture is unavailable for the remainder of the session. An implementation that cannot roll back atomically is non-conformant.

`createPool()` is the synchronous admission point for the logical retention capacity represented by `outPool`. After it returns `OK`, sink validation at `READY` must not require another fallible capacity-admission step. The vendor may use existing native backing or dedicated backing. The call does not pre-create or export caller-owned DMA-BUF FDs: for each successful acquisition, `acquireCurrentFrame()` exports the FD handles needed to access the selected frame's DMA-BUF objects. A vendor may bind a selected native allocation to a logical slot only when that scheduler selection is captured, but the capacity and fixed layout were already admitted by `createPool()`. No newly created caller-owned FD handles from a failed attempt survive any non-`OK` result; any pre-existing value in `outFrame` remains the caller's unchanged responsibility.

On `OK`, MW owns every descriptor in `outFrame.exportedDmaBufs` and must close each one explicitly. Descriptor closure neither releases nor satisfies the HAL frame lock. The lock protects frame contents until the caller invokes the one matching `releaseFrame()` after all use has ended.

The SoC implementation determines whether a native scheduler selection is new using private scheduler identity or equivalent internal state, not `slotIndex`; a released slot may later contain a different selection. The new-selection check, temporary lock, complete descriptor export, returned-state commit, and move into `outFrame` form one atomic success transaction. A failed attempt rolls back and does not consume the selection. Later acquisition of the same current selection returns `NO_NEW_FRAME`. Reset, flush, seek, or equivalent decoder/scheduled-output processing may invalidate the current selection and comparison state without releasing acquired frames; acquisition then returns `NO_FRAME` until decoder consumption establishes another native selection.

Every `OK` acquisition establishes one lock for one distinct captured selection. Several acquired frames may lock different slots concurrently, but there is at most one live lock per slot and one successful acquisition per selection. MW may share access through references or one shared owner around the non-copyable record and calls `releaseFrame()` once when final use ends.

`releaseFrame(CapturedFrame &frame)` requires a valid slot index and a matching acquisition token. The HAL validates that the token corresponds to the current lock for that slot; a mismatched token indicates a stale or reconstructed frame and returns `INVALID_ARGUMENT`. On `OK`, it releases that slot lock, ends the logical-to-native binding, and sets `frame.slotIndex` to `kInvalidCaptureSlotIndex` and `frame.acquisitionToken` to `kInvalidAcquisitionToken`. Only then may the vendor make the native frame eligible for queue reuse or bind the slot's object roles to different backing. It does not inspect or close FDs, so MW must still close any open FDs separately. A successful release makes the frame invalid for content access even if an FD remains open.

An invalid, moved-from, out-of-range, stale, or currently unlocked slot index, or a mismatched acquisition token, returns `INVALID_ARGUMENT`. Every non-`OK` result leaves both the frame and its existing lock state unchanged, so a valid frame retains exactly one release obligation. A slot cannot be reused while its lock remains active.

The **SoC vendor** supplies both `ICaptureSession` and the GStreamer integration. Scheduler notifications, writable-slot management, decoder handles, and copy/conversion details remain private between those components.

Capture has no independent start/stop state or selection policy. It follows valid native scheduler selections after decoder consumption. A valid selection remains acquirable while scheduled output is paused at stationary STC; a state transition alone does not manufacture or advance one.

## 10. Conformance checklist

An SoC implementation is conformant only when all of the following are true:

| Area | Requirement | Sections |
|---|---|---|
| Delivery and association | MW creates the westerossink instance, assigns it to playbin's `video-sink` property, sets the `rdk-capture-session` property on it, and drives it to READY. MW supplies exactly one pipeline-local session property directly to that sink before state transition. Capture is implemented directly in the sink or by a private helper attached to its scheduled-output path. | §6 |
| Plumbing prototype | A runnable prototype proves property receipt on westerossink before `READY`, assignment to playbin, normal presentation, existing property behavior, and confirmed `NULL` teardown. | §6 |
| Concurrent pipelines | Each concurrent pipeline has a distinct westerossink instance, session, capture reservation, and layout. Each sink publishes selections only through its own pipeline's session. | §§5, 6 |
| Capacity and layout | `createPool()` admits the requested concurrent retention capacity before `READY` and returns its concrete immutable, bounds-checked object/slot/plane description. Existing native backing may satisfy that admission. Logical slots and object indices are bookkeeping and layout keys. Different slots have isolated contents, and caller-owned FDs are exported only at acquisition. | §§4, 6, 7, 9 |
| Native selection | Capture follows the same native scheduled-output selection used for normal presentation. It returns `NO_FRAME` without a valid selection, `OK` once for a new selection, and `NO_NEW_FRAME` for a repeated selection. | §§5, 6, 9 |
| Graphics import and texturing | Both EGL/OpenGL ES and Vulkan directly import and sample the exact advertised format, modifier, plane layout, and DMA-BUF object mapping. Sampling yields correctly interpreted RGB(A). EGL and Vulkan FD ownership is observed, partial construction rolls back completely, importing resources are destroyed after graphics completion and before HAL frame release, and the vendor supplies the graphics capability matrix. | §8 |
| Existing sink contract | Capture preserves normal presentation, sink properties, surface output, decoder discovery, subtitle integration, secondary-video behavior, geometry, visibility, immediate-output behavior, and scheduling. | §6 |
| Frame identity, FD ownership and rollback | Valid slot identity and matching acquisition token mean one release is owed. Move constructor transfers token, slot index, and FDs, invalidating the source. HAL assigns unique non-zero token per acquisition. Acquisition requires invalid slot and token values and every FD exactly `-1`. Move assignment is deleted. MW closes one FD per distinct object. Export failure rolls back and leaves output unchanged. | §§4, 7, 9 |
| Frame locks and synchronization | One `OK` creates one lock with stable physical backing and object-to-FD bindings. Successful release ends that binding and invalidates the slot identity while leaving FD ownership unchanged. A later acquisition may then bind the logical slot and object roles to different backing. | §§4, 5, 7–9 |
| Exhaustion and playback | Slot exhaustion may omit capture selections but must not stop decode, native scheduled selection, audio, or STC. | §§4–6 |
| Sink teardown | Final acquisition, transient retry, outstanding frame validity, late release, and session close follow the ordered teardown contract. Scheduled output is disconnected safely during sink state teardown. | §§5, 6 |
| HAL and sink contract tests | Tests cover missing/invalid/replaced session property, two concurrent pipelines without cross-association, preserved sink properties and presentation, invalid/default/moved/released frame identity, move constructor, acquisition token validation, acquisition-output reuse, capability/pool pairing, layout bounds and cross-slot non-overlap, rollback, repeated/concurrent acquisition, exhaustion, sink teardown, final acquisition, and late release. Graphics conformance evidence is the capability matrix required by Section 8.5. | — |

Failure of any mandatory gate means Video Frame Capture is unsupported on that implementation; it does not permit a weakened lifetime or playback-continuity contract.

