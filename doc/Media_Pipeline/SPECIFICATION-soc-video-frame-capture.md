# SoC Video Frame Capture Specification

**Status:** Draft for review.

**Specification version:** 0.91

The key words **MUST**, **MUST NOT**, **SHOULD**, **SHOULD NOT** and **MAY** are interpreted as described in RFC 2119 and RFC 8174 when written in uppercase. Requirements are referenced by section and the conformance checklist.

## 1. Purpose

This specification defines how the SoC vendor exposes decoded clear-video frames to Middleware in a form that can be used for graphics texturing. Later application composition and display are outside its scope.

This specification defines the interface between **Middleware**, the software that manages the media pipeline, and the vendor's **hardware abstraction layer**.

### 1.1 Glossary

| Term | Definition |
|------|------------|
| **API** | Application Programming Interface. |
| **HAL** | Hardware Abstraction Layer, the interface implemented by the vendor library. |
| **FD** | Linux file descriptor, a process-local handle to an open resource. |
| **STC** | System Time Clock, which controls native audio/video pipeline scheduling. |
| **PTS** | Presentation timestamp identifying a frame's position on the media timeline. |
| **DRM** | Direct Rendering Manager, the Linux graphics subsystem whose format codes and modifiers describe image formats and storage layouts. |
| **DMA-BUF** | Linux kernel framework for sharing backing allocations between devices and processes. |
| **Backing allocation** | Memory used to store video image data. |
| **DMA-BUF object** | One backing allocation, which may contain several DRM planes. |
| **DPB** | Decoded Picture Buffer, storage for decoded pictures retained for reference prediction and/or output. |
| **Graphics import** | Creation of graphics resources that reference exported backing allocations for texture sampling. |

### 1.2 Specification-defined terms

| Term | Definition |
|------|------------|
| **Capture session** | The capture interface and state for one media pipeline. |
| **Pool** | The configured slots for holding captured frames, together with their layout. |
| **Pool capacity** | The number of acquired frames that can be held at the same time. |
| **DMA-BUF object ID** | An identifier for one backing allocation, stable until DMA-BUF identities are invalidated. |
| **Exported DMA-BUF** | An object ID paired with a valid FD returned to Middleware for one frame acquisition. |
| **Current selection** | The frame chosen by platform video output for the current STC/display position. |
| **Frame** | A selected decoded video image together with its timing, visible region, colour and backing description. |
| **Frame lock** | A lock that keeps an acquired frame's contents unchanged until Middleware releases it. |
| **Frame plane** | One DRM image plane and its byte range and stride within a DMA-BUF object. |
| **Slot** | Capacity to hold one acquired frame. |
| **Slot layout** | The configured arrangement of image planes for one slot. Each acquired frame describes its actual backing allocations. |

This specification uses “slot” for logical retention capacity and “frame” for the selected image and its description.

## 2. Conceptual Model

**Middleware** decides whether the GStreamer pipeline needs capture before pipeline construction. It obtains a capture session, connects it to the platform sink and sets the pool capacity. For each acquired frame, Middleware keeps its description, manages its FDs and releases the lock after use. Middleware keeps the session alive until teardown and all outstanding frame releases are complete.

- A capture session lets Middleware choose how many frames it can hold for graphics use.
- A **slot** represents capacity to hold one frame.
- A successful acquisition locks the frame's contents until Middleware releases it.
- A returned FD (or set of FDs) gives access to the frame's backing allocations but does not replace that lock.
- Middleware uses **graphics import** to create EGL/OpenGL ES or Vulkan resources that reference the frame's backing allocations.

**The SoC vendor** implements the capture session library and its sink integration. It selects frames from platform video output, manages the backing allocations and exports them safely. It preserves each locked frame's contents.

- The vendor library executes in the Middleware process.
- The vendor decides how to provide the requested number of capture slots.
- Capture adds to the existing decode stack; it does not replace native scheduling or normal playback.
- The vendor library returns FDs for each acquired frame.
- Normal playback continues when capture capacity is occupied.
- The vendor supports direct graphics import for the vendor's supported frame layout.

Section 3 defines the capture session interface. Section 4 explains how the library and pipeline are connected. Section 5 describes what acquisition returns. Section 6 defines ownership and normal frame use. Section 7 covers configuration changes and shutdown. Section 8 defines what the vendor delivers.

## 3. Capture Session interface

The session interface provides four operations:

| Operation | Purpose |
|---|---|
| `getCapabilities()` | Query the configured capture format and pool-capacity limits. |
| `createPool()` | Set the pool capacity once for the session and return its initial layout. |
| `acquireCurrentFrame()` | Lock the selected frame and return its description and FDs. |
| `releaseFrame()` | End the frame lock after graphics use has finished. |

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

`slotCount` is the requested total retention capacity. `out`, `outPool` and `outFrame` are output records supplied by Middleware, not objects owned by the HAL. The `frame` passed to `releaseFrame()` is the acquired record. If the record was moved to another object, use that moved record.

### 3.1 Configured capabilities

Middleware calls `getCapabilities()` after the configured codec/resolution milestone in Section 4.4. An earlier call returns `INVALID_ARGUMENT`. On `OK`, the HAL reports `drmFormat`, `drmModifier`, `maximumContentSize`, `minimumSlots` and `maximumSlots`. The query allocates no pool and exports no FDs. Every non-`OK` result MUST leave `out` unchanged.

The limits MUST satisfy `1 <= minimumSlots <= maximumSlots`. They count retained frames, not DMA-BUF objects. The vendor MUST support every integer in this inclusive range; equal endpoints are valid. A `createPool()` request outside the range returns `INVALID_ARGUMENT`, never a silently adjusted count. Genuine resource failures may return `NO_RESOURCES`. The vendor must not hide unsupported count values by silently adjusting them.

### 3.2 Pool creation

`createPool()` requires configured capabilities and a count within their inclusive bounds. The HAL MUST return `INVALID_ARGUMENT` before configuration, for a count outside those bounds or after any successful pool creation.

The vendor MAY allocate backing initially or on demand, reuse or replace it, use native decoder backing, or combine those strategies. Each strategy must preserve independently retainable contents, importability, and playback continuity. Several slots MAY occupy isolated non-overlapping ranges of one allocation; the slot count does not require that many distinct allocations.

Middleware MUST NOT assume that:

- an object ID remains valid after DMA-BUF identity invalidation;
- one object ID always arrives with the same numeric FD;
- the vendor always reuses or always replaces backing;
- a slot index identifies a physical allocation or native queue position; or
- numeric FD values identify allocations.

Section 5 defines allocation identity and frame metadata.

On `OK`, the HAL commits that count for the session and returns the FD-free initial layout. A separate copied-frame pool is not required. Before accepting capacity, the vendor MUST validate plane ranges and content isolation against native constraints. Pool creation and acquisition must ensure that concurrently locked frames have isolated contents. Frames must use separate physical allocations, or non-overlapping byte ranges within shared allocations. Object and FD counts do not determine the slot count.

Every slot supports `backingSize`, which MUST NOT exceed `maximumContentSize`. `slotLayouts.size()` equals the requested frame count, inclusively between `minimumSlots` and `maximumSlots`.

On every non-`OK` result, the HAL MUST leave `outPool` unchanged. It MUST roll back partial pool creation and authorize no capture writes. Middleware may retry a genuine transient resource failure before the first successful pool creation.

Middleware keeps each capability and layout description as long as it has frames that depend on it. New configuration MUST NOT overwrite metadata held by older retained frames. `maximumSlots` is the greatest reported supported count that preserves resources for normal playback; `minimumSlots` is the least supported count for the configured video.

## 4. Capture library and GStreamer integration

### 4.1 Required integration model

The platform video sink is westerossink. Middleware creates the westerossink instance, assigns it to playbin's `video-sink` property, sets the `rdk-capture-session` property on the westerossink instance, and drives it to READY to validate capture integration. Capture integrates with the existing surface and video-plane output.

The platform sink integrates capture directly or owns a private helper attached to its platform video output path. The helper may be a private object or child GStreamer element. The public pipeline topology remains unchanged.

For every returned frame, the vendor implementation uses the same system time clock (STC) and scheduling rules as platform video output. It publishes only a selection established after decoder consumption. `CapturedFrame.presentationTimeNs` is the frame's media presentation timestamp (PTS) in nanoseconds, representing the media timeline. Stream discontinuities may change the PTS timeline.

Capture preserves normal presentation and the platform sink's existing contract, including its properties, surface output, decoder discovery, subtitle integration, secondary-video behavior, and scheduling. A pipeline not requesting capture uses the same platform sink without a capture session and is otherwise unaffected.

Each capture-enabled pipeline has one independent relationship:

```text
playbin instance
  -> westerossink instance
       -> capture integration (direct or private helper)
            -> ICaptureSession
                 -> admitted capture reservation and layout
```

Concurrent or dual video uses one complete relationship per pipeline. Sink A receives session A and can access only reservation A; sink B receives session B and can access only reservation B. The SoC implementation does not choose among active playbins. If a vendor-private graph has multiple platform video output branches, the platform sink associates the session with the branch that this sink instance represents.

### 4.2 Session-property contract

The property name is `rdk-capture-session` with type G_TYPE_POINTER and value type `ICaptureSession *`. The pointer is opaque to GStreamer and borrowed by the sink. Middleware keeps the pointer valid until sink teardown completes. Middleware must retain the session longer if a final acquisition or late release is still possible.

Middleware sets the property directly on the westerossink instance before any state transition that can start decoder output. The sink accepts exactly one non-null session. Reapplying the same pointer is idempotent; a null pointer or attempt to replace it with a different session is invalid.

During `NULL`→`READY`, the sink accepts the session and configures its platform video output association without requiring a pool. Invalid association or native setup fails the transition with a GStreamer error and rolls back partial setup. After configured `READY`, Middleware queries limits and calls `createPool()`; capture writes and publication remain disabled until pool creation succeeds. Pool-creation failure starts no capture writes and Middleware rolls the pipeline back to `NULL`.

The property does not own the session, create a pool, or carry the `CapturePool` description. Private access to backing and scheduler state remains inside the paired vendor implementations.

### 4.3 WesterosSinkSoC contract

Each SoC-specific `westeros-sink-soc.h` MUST extend the `GstWesterosSinkSoc` struct with the following fields:

```cpp
struct _GstWesterosSinkSoc
{
    // ... existing fields ...

    // ICaptureSession support
    void *captureSession;              // Borrowed ICaptureSession pointer
};
```

Each SoC-specific `westeros-sink-soc.c` MUST implement the following contract function:

```cpp
gboolean gst_westeros_sink_soc_set_capture_session(
    GstWesterosSink *sink,
    void *session);
```

**Semantics**:
- Input: `sink` - WesterosSink instance, `session` - ICaptureSession pointer (borrowed, not owned)
- Return: TRUE if session accepted, FALSE if rejected
- The implementation MUST validate that session is non-null
- The implementation MUST store session in `sink->soc.captureSession`
- The implementation MUST reject if session is already set (idempotent: reapplying same session is OK, different session is error)

The common `westeros-sink.c` MUST implement a property handler for `rdk-capture-session`:

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

The property MUST be registered in the class_init function.

**Timing requirements**:
- **Connect**: During NULL→READY, associate the session with platform video output in `gst_westeros_sink_soc_null_to_ready()`; do not require a pool or start capture writes. Query configured limits and create the pool afterward, before enabling capture.
- **Disconnect**: During READY→NULL, before decoder teardown, in `gst_westeros_sink_soc_ready_to_null()`

This timing separates native association at `READY` from later pool creation, as specified in Section 4.2.

**Session lifetime**: The sink borrows the factory-created object introduced in Section 4.4. Middleware owns it and completes the deletion prerequisites in Section 4.6.

### 4.4 Session creation and threading

Middleware knows whether a pipeline requires capture before graph construction. Each capture-enabled pipeline has one dedicated `ICaptureSession` object. Middleware loads the vendor library, resolves the factory declared in Section 8.1 and obtains the owned pointer. In this example, `session` is that pointer:

```cpp
ICaptureSession* session = createCaptureSession();
```

Middleware supplies `session` to the sink. The sink borrows the pointer and MUST NOT delete the object. This HAL session is separate from the application's public capture object.

Middleware configures the pipeline's codec and resolution, queries the session's limits and calls `createPool()` before enabling capture output.

By configured `READY`, the vendor integration MUST have the native codec and resolution information needed for an accurate `getCapabilities()` query and pool creation. Middleware MUST query after that milestone and create the pool before capture writes or publication begin. Generic GStreamer `READY` does not prove that caps are negotiated or preroll is complete. Early native configuration is an integration requirement. Early public Rialto graphics-support queries remain separate from this HAL query.

Middleware MUST serialize method calls within a session, even when successive calls originate on different threads. The vendor MUST synchronize native scheduler updates with those calls. Several retained frames may coexist, but this does not permit overlapping HAL calls or concurrent mutation of one output record. Setup and destruction MUST NOT overlap other HAL calls or native access to the session. Final shared-frame cleanup MUST use the same serialized release path.

A session belongs to one pipeline and is never reassociated or reused. The session lifetime extends beyond sink teardown. It ends when the final acquisition opportunity is resolved or abandoned, and every acquired frame is released.

### 4.5 Middleware lifecycle

This sequence illustrates successful setup, an optional supported transition and safe teardown. `Session` represents the object pointed to by `session` in Section 4.4. `N` is the admitted count, `outPool` is the initial `CapturePool` output, and `frameA`/`frameB` are initially empty `CapturedFrame` records.

This example retains frame A while acquiring frame B, so it assumes `N >= 2`. With `N == 1`, Middleware finishes use of A and releases it before B can be acquired. Full capacity omits captures without stalling playback. Section 7.1 defines the transition and fatal-error rules.

```mermaid
sequenceDiagram
    participant Middleware
    participant Playbin as playbin
    participant Sink as westerossink
    participant Output as Platform video output
    participant Session as ICaptureSession
    Middleware->>Middleware: Load library and call createCaptureSession()
    Middleware->>Playbin: Create pipeline and assign westerossink
    Middleware->>Sink: Set borrowed rdk-capture-session
    Middleware->>Playbin: Configure codec/resolution and reach READY
    Sink->>Output: Configure native association, no capture writes
    Output->>Session: Connect platform video output
    Sink-->>Middleware: Configured READY
    Middleware->>Session: getCapabilities()
    Session-->>Middleware: Accurate format and inclusive slot limits
    Middleware->>Session: createPool(N, outPool), once
    Session-->>Middleware: OK, initial layout without allocation IDs or FDs
    Middleware->>Playbin: Activate admitted pipeline
    Output->>Session: Publish completed STC-selected frame A
    Middleware->>Session: acquireCurrentFrame(frameA)
    Session-->>Middleware: OK, frame metadata including colour, IDs, FDs, token
    Middleware->>Middleware: Import or reuse compatible resources; close owned FDs
    opt Supported resolution/backing change
        Output->>Session: Update backing and latch identity invalidation
        Note over Middleware,Session: Frame A remains valid and counts against N. No new pool creation.
        Output->>Session: Publish frame B when capacity permits
        Middleware->>Session: acquireCurrentFrame(frameB)
        Session-->>Middleware: OK, invalidation flag and frame B snapshot
        Middleware->>Middleware: Apply invalidation before ID lookup; retain frame A imports
        Middleware->>Middleware: Use frame B colour and layout; finish graphics reads
        Middleware->>Session: releaseFrame(frameB), original slot/token
        Session-->>Middleware: OK
    end
    Middleware->>Playbin: Set NULL and wait
    Sink->>Output: Stop writes; complete or cancel active write
    Output->>Session: Disconnect before destroying platform video output
    Sink-->>Middleware: Teardown complete
    Note over Middleware,Session: Resolve or abandon final acquisition; finish all retained graphics use.
    Middleware->>Session: releaseFrame(frameA), original slot/token
    Session-->>Middleware: OK
    Middleware->>Middleware: Complete any other releases and FD cleanup
    Middleware->>Session: delete session through virtual destructor
    Middleware->>Middleware: Unload library only after dependent cleanup
```

The application selects capture through its media API before pipeline construction. It does not manipulate GStreamer objects or supply the HAL pointer. Middleware performs the setup under Sections 4.2 and 4.4. Association with the sink is not pool creation or media activation.

When Middleware abandons failed pipeline construction, it returns the sink to `NULL` and retires that HAL session. A new pipeline attempt uses a fresh session.

Taking playbin to `NULL` completes native teardown and disconnection. It does not itself destroy a referenced westerossink object; GStreamer object destruction depends on releasing its references. Middleware completes the HAL-session cleanup in Section 4.6 separately from releasing GStreamer objects.

### 4.6 Teardown and deletion

The sink MUST stop capture writes and disconnect native access before tearing down its platform video output. The session's selection no longer advances. The final acquisition opportunity is determined by the selection that remains:

| Final selection state | Acquisition result |
|---|---|
| Valid and not yet acquired | Available once; transient `NO_RESOURCES` leaves it available for retry. |
| Already acquired | `NO_NEW_FRAME`. |
| No valid selection | `NO_FRAME`. |

Teardown does not recover a capture-fatal session; the `FATAL_ERROR` rule in Section 7.1 still applies. In every case, the session MUST continue accepting matching releases for older acquired frames. Their contents remain protected by Section 6.

Before deleting the factory-created `ICaptureSession` object, Middleware MUST:

1. complete sink disconnection and all native writes;
2. resolve or abandon the final acquisition opportunity;
3. finish graphics use and release every outstanding frame lock;
4. close or otherwise complete ownership transfer of all remaining owned FDs; and
5. ensure no HAL call or native access to the session is in progress.

Middleware then destroys the object pointed to by `session`, the owned factory result introduced in Section 4.4:

```cpp
delete session;
session = nullptr;
```

Deletion through `ICaptureSession*` invokes the concrete vendor implementation's destructor because the base destructor is virtual. Middleware MUST keep the vendor library loaded through this deletion and all dependent cleanup.

### 4.7 Prototype and conformance testing

The following example uses vendor-supplied test hooks, not production API functions. `configureFrameCaptureOutput()` performs the setup in Sections 4.2 and 4.4 using the supplied test session. `teardownFrameCaptureOutput()` drives native disconnection and waits for completion. The `test...` helpers observe the instrumented sink's state.

This test fixture advertises a slot-count range containing four. The value four is an example, not a production default. At minimum, a runnable integration test verifies:

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
    GstElementFactory *sinkFactory = gst_element_get_factory(configuredSink);
    ASSERT_NE(sinkFactory, nullptr);
    ASSERT_STREQ(gst_plugin_feature_get_name(GST_PLUGIN_FEATURE(sinkFactory)), "westerossink");

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

Additional tests create two playbins with two westerossink instances and two sessions and verify that each sink reaches only its own session and pool. Failure tests cover null session property, repeated property with a different session, incompatible `READY` setup, and rollback to `NULL` without capture writes.

The vendor supplies a runnable integration example using westerossink or an instrumented build of westerossink. The example demonstrates property receipt on the westerossink instance before `READY`, successful `READY` validation, assignment to playbin, normal presentation, and confirmed `NULL` teardown. It need not implement frame export while validating this plumbing.

The integration suite also verifies that normal scheduled presentation continues while capture is enabled; existing sink properties retain their values and effects; decoder discovery, subtitle integration, secondary-video behavior, geometry, visibility, and immediate-output behavior remain usable; capture-slot exhaustion omits capture without blocking presentation; and teardown disconnects capture before platform video output is destroyed. Final acquisition and late `releaseFrame()` remain valid after sink teardown.

## 5. Captured frames

An acquired frame describes the selected image and how its planes are stored in backing allocations. The description includes the image's timing, visible region, colour, dimensions, format and storage modifier.

```text
Acquired frame
  ├── Timing, visible region, colour and image format
  └── Image planes
       ├── Plane 0 ──> DMA-BUF object A ──> one FD for A
       ├── Plane 1 ──> DMA-BUF object A ──> same FD for A
       └── Plane 2 ──> DMA-BUF object B ──> one FD for B
```

Each plane identifies a backing allocation and gives its byte offset, length and stride. Several planes may share one allocation. Each distinct allocation is exported through one FD for that acquisition. Object IDs link the plane descriptions, allocation sizes and exported FDs within the frame description.

The diagram illustrates one possible arrangement. A vendor can meet this specification with one supported frame layout.

### 5.1 Data types

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
    uint32_t minimumSlots;
    uint32_t maximumSlots;
};

enum class CaptureYcbcrMatrix : uint32_t
{
    UNKNOWN = 0,
    BT601 = 1,
    BT709 = 2,
    BT2020 = 3,
};

enum class CaptureSampleRange : uint32_t
{
    UNKNOWN = 0,
    FULL = 1,
    NARROW = 2,
};

enum class CaptureChromaSiting : uint32_t
{
    UNKNOWN = 0,
    COSITED = 1,
    MIDPOINT = 2,
};

struct CaptureColorDescription
{
    CaptureYcbcrMatrix matrix{CaptureYcbcrMatrix::UNKNOWN};
    CaptureSampleRange range{CaptureSampleRange::UNKNOWN};
    CaptureChromaSiting horizontalSiting{CaptureChromaSiting::UNKNOWN};
    CaptureChromaSiting verticalSiting{CaptureChromaSiting::UNKNOWN};
};

struct DmaBufObject
{
    uint64_t objectId;
    uint64_t sizeBytes;
};

struct FramePlane
{
    uint32_t planeIndex;
    uint64_t dmaBufObjectId;
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
    uint64_t dmaBufObjectId;
    int fd{-1};
};

struct CapturePool
{
    Size backingSize;
    std::vector<CaptureSlotLayout> slotLayouts;
};

struct CapturedFrame
{
    uint32_t slotIndex{kInvalidCaptureSlotIndex};
    uint64_t acquisitionToken{kInvalidAcquisitionToken};
    int64_t presentationTimeNs;
    Rectangle visibleRegion;
    bool dmaBufObjectsFlushed{false};
    CaptureCapabilities capabilities{};
    CaptureColorDescription colorDescription{};
    Size backingSize{};
    std::vector<DmaBufObject> dmaBufObjects;
    std::vector<FramePlane> planes;
    std::vector<ExportedDmaBuf> exportedDmaBufs;

    CapturedFrame() : presentationTimeNs{0}, visibleRegion{} {}
    CapturedFrame(const CapturedFrame &) = delete;
    CapturedFrame &operator=(const CapturedFrame &) = delete;
    CapturedFrame(CapturedFrame &&other) noexcept;
    CapturedFrame &operator=(CapturedFrame &&other) noexcept = delete;
    ~CapturedFrame() = default;
};
```

The value types follow these rules.

**Dimensions and ownership**

- `CaptureCapabilities::size` is that record's size in bytes, not a version for the whole interface.
- Pool `backingSize` gives the configured slot dimension for admitted capacity. Acquired `backingSize` describes the actual returned frame. Section 3.2 requires that pool `backingSize` not exceed `maximumContentSize`. Neither size alone identifies physical backing.
- `DmaBufObject.sizeBytes` gives the size available for range validation in the identified acquired allocation. Reporting it does not allocate or export an FD.
- After closing an FD or completing an ownership-consuming import, Middleware MUST set the corresponding `ExportedDmaBuf::fd` to `-1`. Section 6 defines FD ownership. Destructors do not close FDs.
- `CapturedFrame` is non-copyable. A valid `slotIndex` and matching `acquisitionToken` mean exactly one `releaseFrame()` is owed, independently of FD ownership.
- The HAL must assign a session-unique non-zero `acquisitionToken` to each successful acquisition, without colliding wraparound or reuse. The token is opaque; Middleware must not interpret or reconstruct it.
- When transferring a frame to another object, the metadata, FDs, token, and release obligation move to the new object. The source FDs become `-1`, its `slotIndex` becomes `kInvalidCaptureSlotIndex`, and its `acquisitionToken` becomes `kInvalidAcquisitionToken`.
- You cannot overwrite a frame record that already contains data, because you would lose the FD and the obligation to release the frame. When a frame record is deleted, it does not automatically release the frame lock because the system cannot determine when the GPU has finished using the frame. To reuse a frame record, Middleware must first finish graphics use, release the lock, close or invalidate remaining owned FDs, and reset the record. Transfer ownership to another object using the transfer operation.

The destructor does not release the frame lock because it cannot determine when the GPU has finished using the frame. Middleware must explicitly call releaseFrame() after GPU work completes. Move operations exist only to transfer the release obligation to one owner, not to automatically manage the frame lifetime.

**Identity and complete frame descriptions**

- A pool's `slotIndex` values MUST equal their positions in `slotLayouts`. They are contiguous from zero and cannot equal `kInvalidCaptureSlotIndex`.
- Object IDs are opaque non-zero `uint64_t` values, not vector positions. Zero denotes an unbound allocation in an initial slot template. The HAL must not return zero object IDs in acquired planes.
- Each acquired frame carries its own `capabilities`, `colorDescription`, `backingSize`, `dmaBufObjects` and ordered `planes`, atomically describing the exported pixels and backing. Pool creation returns structural slot templates with object IDs set to zero, not invented allocation identities. Concrete object sizes and plane-to-object bindings are supplied on acquisition.
- An object ID identifies the same allocation until `dmaBufObjectsFlushed` invalidates the identity cache. Colour interpretation is per-frame and may change without rebinding the allocation. Middleware must check the complete import description before reuse. IDs may be reused after invalidation, but never silently rebound before invalidation. Slot reuse and numeric FD reuse do not establish object identity.
- `planeIndex` follows the plane order defined by the reported DRM format. Import code maps that order to its graphics API.

**Range validation**

- Each plane MUST reference one valid object. Middleware and the vendor MUST validate its range without overflow: `offsetBytes <= sizeBytes` and `lengthBytes <= sizeBytes - offsetBytes`. The range and stride MUST fit the advertised layout.
- For any two planes that belong to different slots and are bound within the same physical DMA-BUF object, their byte ranges must not overlap. Planes within one slot may share an object as defined by that format. Binding simultaneously locked slots to distinct physical allocations also provides the required content isolation.
- The vendor and Middleware MUST validate each successful frame's `visibleRegion` without overflow: `x <= backingSize.width`, `size.width <= backingSize.width - x`, `y <= backingSize.height`, and `size.height <= backingSize.height - y`.
- Storage not exposed as a DRM plane remains part of its DMA-BUF object.

**FD export and retained contents**

- On `OK`, `exportedDmaBufs` must contain exactly one entry for each distinct object that the frame's planes reference. The list must not contain duplicates or unrelated objects. Middleware MUST match `FramePlane::dmaBufObjectId`, `DmaBufObject::objectId` and `ExportedDmaBuf::dmaBufObjectId` within that returned frame.
- A later selection may reuse a released allocation and ID with newly exported FDs, or return an unknown ID requiring a new import. Section 6 governs retained contents.
- `dmaBufObjectsFlushed` tells Middleware to discard cached allocation identities before looking up the returned IDs. It is false by default. Section 7.3 defines its latching and delivery rules. Moving transfers the flag and all metadata; the source flag becomes false. Resetting a released, FD-free record clears its flag and old metadata.

Plane count can differ from object count. The exported FD count follows the distinct referenced objects.

An empty `CapturedFrame` satisfies all three conditions:

- `slotIndex == kInvalidCaptureSlotIndex`;
- `acquisitionToken == kInvalidAcquisitionToken`; and
- every `ExportedDmaBuf::fd == -1`.

Any other FD value, including a negative value other than `-1`, is non-empty. Closing all FDs does not make a record empty while its slot and token still represent a live lock. Section 7.4 defines how successful release invalidates those fields.

### 5.2 Valid frame representations

The following examples describe successful acquisition metadata, not pool templates. IDs 101 and 205 and the FD numbers are illustrative. Each object appears once in `dmaBufObjects` and exports once per acquisition.

These initializers use placeholder values: `S`, `S0` and `S1` are allocation sizes in bytes; `P`, `P0` and `P1` are strides in bytes; `planeOffset` is the start of the second plane within a shared allocation. A real implementation supplies values satisfying the range rules above. The snippets illustrate mappings, not standalone programs.

**Example A — one plane, one object, one FD**

```text
plane 0 ──> object 101 ──> FD 10
```

```cpp
dmaBufObjects = {{.objectId = 101, .sizeBytes = S}};
planes = {{.planeIndex = 0, .dmaBufObjectId = 101,
           .offsetBytes = 0, .lengthBytes = S, .strideBytes = P}};
exportedDmaBufs = {{.dmaBufObjectId = 101, .fd = 10}};
```

**Example B — two planes sharing one object and one FD**

```text
plane 0 ──> object 101 @ offset 0 ──> FD 10
plane 1 ──> object 101 @ offset planeOffset ──> same FD 10
```

```cpp
dmaBufObjects = {{.objectId = 101, .sizeBytes = S}};
planes = {
    {.planeIndex = 0, .dmaBufObjectId = 101,
     .offsetBytes = 0, .lengthBytes = planeOffset, .strideBytes = P0},
    {.planeIndex = 1, .dmaBufObjectId = 101,
     .offsetBytes = planeOffset, .lengthBytes = S - planeOffset, .strideBytes = P1},
};
exportedDmaBufs = {{.dmaBufObjectId = 101, .fd = 10}};
```

**Example C — two planes, two objects, two FDs**

```text
plane 0 ──> object 101 ──> FD 10
plane 1 ──> object 205 ──> FD 11
```

```cpp
dmaBufObjects = {
    {.objectId = 101, .sizeBytes = S0},
    {.objectId = 205, .sizeBytes = S1},
};
planes = {
    {.planeIndex = 0, .dmaBufObjectId = 101,
     .offsetBytes = 0, .lengthBytes = S0, .strideBytes = P0},
    {.planeIndex = 1, .dmaBufObjectId = 205,
     .offsetBytes = 0, .lengthBytes = S1, .strideBytes = P1},
};
exportedDmaBufs = {
    {.dmaBufObjectId = 101, .fd = 10},
    {.dmaBufObjectId = 205, .fd = 11},
};
```

**Example D — shared and separate objects in one frame**

```text
plane 0 ──> object 101 @ offset 0 ──> FD 10
plane 1 ──> object 101 @ offset planeOffset ──> same FD 10
plane 2 ──> object 205 @ offset 0 ──> FD 11
```

```cpp
dmaBufObjects = {
    {.objectId = 101, .sizeBytes = S0},
    {.objectId = 205, .sizeBytes = S1},
};
planes = {
    {.planeIndex = 0, .dmaBufObjectId = 101,
     .offsetBytes = 0, .lengthBytes = planeOffset, .strideBytes = P0},
    {.planeIndex = 1, .dmaBufObjectId = 101,
     .offsetBytes = planeOffset, .lengthBytes = S0 - planeOffset, .strideBytes = P1},
    {.planeIndex = 2, .dmaBufObjectId = 205,
     .offsetBytes = 0, .lengthBytes = S1, .strideBytes = P2},
};
exportedDmaBufs = {
    {.dmaBufObjectId = 101, .fd = 10},
    {.dmaBufObjectId = 205, .fd = 11},
};
```

After releasing Example C's frame, a later selection may return IDs 101 and 205 with FDs 30 and 31. Middleware reuses the compatible cached image, closes the unused new FDs, and holds the new frame lock until graphics reads finish. If that acquisition instead has `dmaBufObjectsFlushed == true`, Middleware first invalidates the old cache and imports the returned mapping anew, even if the IDs are numerically unchanged.

### 5.3 DRM NV12 linear example

For this example, `CaptureCapabilities` reports `DRM_FORMAT_NV12` and `DRM_FORMAT_MOD_LINEAR`. Linux `drm_fourcc.h` defines NV12 as two planes: luma Y and interleaved chroma UV. The acquired frame describes their actual storage:

- In Example B, both planes occupy different offsets in one DMA-BUF object. Acquisition exports one FD.
- In Example C, each plane occupies a separate object. Acquisition exports two object-ID-labelled FDs.

The modifier alone does not determine the number of objects or FDs. The returned plane-to-object mapping does. A vendor need not implement both arrangements, but MUST provide direct import through both required graphics paths for whichever layout it returns.

## 6. Acquiring, using and releasing frames

This section defines the ownership and lifetime of captured-frame resources. Native queue management remains private to the vendor.

### 6.1 FD ownership and frame locks

A successful acquisition gives Middleware independently owned FDs and one frame lock. These are separate obligations. Middleware MUST close each FD while it still owns it; a successful ownership-consuming graphics import takes over that obligation.

Middleware MAY forward FDs to another process using `SCM_RIGHTS`. The receiver owns separate descriptors. After successful transfer, Middleware MUST close and invalidate its own copies. It retains the original frame record and token until downstream use finishes. A failed transfer MUST close the owned descriptors and release the failed acquisition exactly once. Section 7.3 defines how pending invalidation survives failed delivery.

The vendor MUST keep acquired contents and backing stable until the matching `releaseFrame()`, including after pipeline teardown. Middleware MUST finish every downstream use, including graphics processing unit (GPU) reads, before that release. Closing an FD does not release the lock. Keeping an FD or cached import open does not preserve the pixels after release.

Middleware MUST call `releaseFrame()` exactly once per successful acquisition. After release it MUST NOT access that frame's contents; the vendor may reuse the slot and backing. A later acquisition creates a new lock even if an existing graphics import can be reused.

### 6.2 Graphics import and caching

Middleware must organize its import cache by the ordered object-ID and plane mapping, and by the complete import description including colour interpretation. Cache entries belong to one session and graphics context/device. Middleware reuses a compatible entry or creates an import when none matches. The portability rules in Section 6.1 prohibit using FD numbers or slot indices as allocation identity.

On a cache hit, Middleware MUST close the newly received FDs because no import consumes them. Multiple objects may form one image; they do not turn one frame into several slots.

Middleware MUST keep at most the admitted number of reusable frame-image entries, evicting unused entries as needed. Older retained entries awaiting invalidation cleanup may temporarily exceed that cache bound. This is a graphics-cache allowance, not extra frame-lock capacity or a total-memory guarantee.

For a YUV layout, graphics sampling MUST return correctly interpreted RGB(A) using `CapturedFrame.colorDescription`. The consumer does not sample raw YUV planes or select the conversion.

For every successful YUV acquisition, the vendor must return an effective matrix, range, and both siting values. If stream metadata is missing, use applicable defined defaults instead of guessing from dimensions. `COSITED` means offset 0, `MIDPOINT` means offset 0.5. `UNKNOWN` is valid for an empty record or for RGB fields that do not apply, not for a successful YUV interpretation. Only matrix variants that both required graphics paths can represent are supported. These fields do not describe HDR transfer, tone mapping, or display-gamut conversion.

Both EGL hints and Vulkan sampler-YCbcr conversion MUST use that frame's description. Middleware MUST include colour interpretation in cache compatibility. A BT601-to-BT709 change, with or without a resolution change, may reuse the allocation ID. It must not reuse an incompatible image or conversion. Older retained frames keep their original description. A colour-only change does not require DMA-BUF identity invalidation. Existing status and atomic rollback rules apply when correct sampling cannot be provided; no separate colour-error protocol is introduced.

When `dmaBufObjectsFlushed` is true, Middleware MUST invalidate the old cache before looking up returned IDs. Section 7.3 defines when this notification becomes pending and how it is delivered.

Middleware MUST destroy unused invalidated entries. It MUST defer destruction of imports used by older retained frames until their graphics use and frame release finish. Those retired entries MUST NOT enter the new cache, even if their numeric IDs match. Capture destruction evicts remaining unused imports. The graphics context/device MUST outlive retained frames and their cleanup.

Extension functionality promoted into a core API version satisfies the corresponding requirement. The vendor capability declaration in Section 8.3 identifies whether each requirement is provided by an extension or by core functionality.

### 6.3 EGL DMA-BUF image import

Middleware constructs the image attributes from the capture records as follows:

- `CaptureCapabilities.drmFormat` supplies `EGL_LINUX_DRM_FOURCC_EXT`;
- each DRM plane uses the FD for its `dmaBufObjectId`, repeating that FD in the attribute list when several planes reference the same DMA-BUF object;
- `FramePlane.offsetBytes` and `strideBytes` supply the matching plane OFFSET and PITCH attributes;
- `CaptureCapabilities.drmModifier` supplies every plane's `EGL_DMA_BUF_PLANE*_MODIFIER_LO_EXT` and `EGL_DMA_BUF_PLANE*_MODIFIER_HI_EXT` attributes; and
- `CapturedFrame.colorDescription` supplies `EGL_YUV_COLOR_SPACE_HINT_EXT`, `EGL_SAMPLE_RANGE_HINT_EXT`, `EGL_YUV_CHROMA_HORIZONTAL_SITING_HINT_EXT`, and `EGL_YUV_CHROMA_VERTICAL_SITING_HINT_EXT`.

`eglQueryDmaBufFormatsEXT()` and `eglQueryDmaBufModifiersEXT()` report the advertised format and modifier. Their result also identifies whether the combination is external-only; external-only support is valid because OpenGL ES external-texture binding is required.

EGL image creation does not transfer ownership of the supplied DMA-BUF FDs. After `eglCreateImage()` or `eglCreateImageKHR()` returns, Middleware closes every supplied FD copy that is no longer needed for another import attempt, on both success and failure. A successful EGL image retains the imported backing until `eglDestroyImage()` or `eglDestroyImageKHR()`.

A failed EGL import closes every Middleware-owned FD and destroys every EGL image created by that attempt. It leaves no graphics resource associated with the frame.

### 6.4 OpenGL ES external-texture binding and sampling

Middleware creates one external texture for each EGL image used to expose the frame. All resources in one frame use `GL_TEXTURE_EXTERNAL_OES`. After binding that target, Middleware associates the EGL image with the texture by calling `glEGLImageTargetTexture2DOES()` and configures texture parameters valid for an external texture. Sampling through `samplerExternalOES` returns the interpreted RGB(A) values required by Section 6.3.

The resource order is:

1. create the EGL image from the acquired DMA-BUF layout;
2. create and bind the GL texture, then associate the EGL image with `glEGLImageTargetTexture2DOES()`;
3. retain the acquired frame until all graphics work reading it has completed;
4. call `releaseFrame()` exactly once, retaining compatible EGL/GL imports in the cache for future acquisitions; and
5. on cache eviction or final cleanup, delete the unused texture with `glDeleteTextures()`, then destroy its EGL image with `eglDestroyImage()` or `eglDestroyImageKHR()`. Older retained frames defer this cleanup as described in Section 6.3.

Failure after partial construction deletes created textures, destroys created EGL images, closes all Middleware-owned FDs, and preserves the frame lock until that rollback is complete.

### 6.5 Vulkan DMA-BUF image import and sampling

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

The queried format/modifier properties include sampled-image usage and the exact shared, separate, or mixed allocation arrangement represented by the capture pool. DRM image planes, DMA-BUF objects, Vulkan format planes, and DRM-modifier memory planes are distinct concepts; their counts are not assumed to be equal. The vendor declares the `VkFormat` and binding model that represent each configured DRM format, modifier and concrete acquired layout.

A successful `vkAllocateMemory()` carrying `VkImportMemoryFdInfoKHR` transfers ownership of that FD to the Vulkan implementation. Middleware does not close or reuse that FD afterward. When allocation fails before ownership transfers, Middleware closes the still-owned FD. If one DMA-BUF object MUST be imported into more than one Vulkan memory allocation, each ownership-transferring import uses its own valid duplicated FD.

The Vulkan resource order is:

1. query support and create the external, modifier-aware image;
2. import and bind the memory required for each DMA-BUF object or Vulkan memory plane;
3. create the required sampler-YCbCr conversion, image view, and sampler;
4. retain the acquired frame until submitted graphics work reading it has completed, then call `releaseFrame()` exactly once; compatible Vulkan imports remain cached for a later acquisition;
5. on eviction or final cleanup, destroy unused samplers and image views with `vkDestroySampler()` and `vkDestroyImageView()`;
6. destroy their conversion with `vkDestroySamplerYcbcrConversion()` after dependent objects;
7. destroy the image with `vkDestroyImage()`; and
8. free imported memory with `vkFreeMemory()`. Imports used by older retained frames are cleaned up only after their final GPU use and release.

Failure after partial construction unwinds resources in reverse dependency order. It destroys every created Vulkan object, frees every allocated memory object, closes every FD whose ownership did not transfer, and preserves the frame lock until rollback is complete.

## 7. Configuration changes and shutdown

### 7.1 Configuration transitions

The admitted count is a session-wide retention commitment. A supported resolution change MAY change backing dimensions, format, modifier, plane layout or colour interpretation, but MUST NOT reduce that count. The vendor handles the transition internally. Middleware MUST NOT repeat `createPool()` after successful pool creation; no replacement-pool cycle or earlier notification API is needed.

Old and new frames share the same total lock limit. Older locks consume capacity until released. When capacity is full or Middleware waits for older releases, this is ordinary exhaustion, not a fatal error. Exhaustion must not stall normal playback. Each acquired frame MUST retain its own import description; Middleware MUST NOT interpret it through another frame's or a refreshed pool description.

The vendor MUST declare the supported configuration and transition envelope. If a transition cannot sustain the admitted capacity or required capture layout, acquisition MUST return `FATAL_ERROR` for the remainder of the session. Existing locked contents and matching releases MUST remain valid, and normal playback MUST continue. The HAL MUST NOT silently clamp capacity or recover by recreating a smaller pool. Reporting an error on a required supported transition does not establish conformance.

### 7.2 Selection state

The session passively tracks the platform video output's current captured selection, whether that selection has already been acquired, and every active frame lock. Selection identity is independent of reusable slot identity: a released slot may later hold a different selection that returns `OK` once.

A slot is writable only when it has no frame lock and no write in progress. Before reusing an unlocked current slot, the session atomically invalidates the old selection so concurrent acquisition returns `NO_FRAME` rather than observing an overwrite. Section 7.3 defines acquisition and release behavior.

### 7.3 Acquisition and delivery

Middleware MUST pass an empty `outFrame`, as defined in Section 5. The HAL MUST reject non-empty output with `INVALID_ARGUMENT` and leave it unchanged. Before initial pool creation, acquisition also returns `INVALID_ARGUMENT` without changing the output.

The HAL examines the current native selection. It returns `NO_FRAME` if no valid selection exists, or `NO_NEW_FRAME` if that selection has already been returned.

For a new selection, the HAL locks it temporarily and binds it to a free admitted slot or equivalent bookkeeping entry. It exports one FD for each distinct referenced object. It then moves the complete frame, including its slot and release obligation, into `outFrame` and returns `OK`. These steps form one atomic success transaction. They are not independent partial successes. The binding must remain unchanged until release.

Every returned FD MUST be valid in the Middleware process and independently owned by Middleware before the call returns. It MUST remain valid if the vendor closes the descriptor from which it was obtained. The vendor's internal export, duplication or service transport mechanism remains private.

Every non-`OK` result MUST leave `outFrame` unchanged and create no release obligation.

If descriptor export fails because a transient resource is unavailable, including FD exhaustion, the HAL MUST return `NO_RESOURCES`. Before returning, the vendor MUST close every FD created for that attempt and remove the temporary lock. The output remains unchanged and the selection is available for retry; Middleware owes no release for that failed attempt.

An unrecoverable session/backing failure returns `FATAL_ERROR` with the same all-or-nothing cleanup. Capture then remains unavailable for the session. Older matching releases remain valid under Section 7.1. An implementation that cannot roll back a failed acquisition atomically is non-conformant.

Whenever a HAL call returns `FATAL_ERROR`, the implementation MUST log the operation and cause where known through the existing platform logging system. Logging adds no error-reporting API and MUST NOT expose protected content or secrets.

Each successful acquisition must return the complete per-frame description defined in Section 5 as one atomic snapshot, including effective colour and concrete object/plane bindings. That snapshot remains valid for the lock after reconfiguration. FD export occurs on every successful acquisition, including graphics-cache hits; a failed attempt leaves no newly created caller-owned FD.

When a backing change invalidates cached allocation identities, the HAL must set `dmaBufObjectsFlushed`. The notification stays pending across `NO_FRAME`, `NO_NEW_FRAME`, `NO_RESOURCES`, and export rollback until a successful acquisition delivers it. If multiple changes occur before delivery, they coalesce to the latest usable configuration.

A pipeline/source flush, seek or presentation discard does not by itself invalidate DMA-BUF identities. The HAL sets the flag only when backing identities/imports were actually invalidated. Section 6.3 defines how Middleware applies it before cache lookup.

Middleware must preserve acquisition order through server-to-client delivery. If inter-process communication (IPC) fails after HAL success, the server must retain pending invalidation and forward it with the next successful delivery. This applies even if the next HAL flag is false. The server cleans up the failed acquisition under Section 6. After a failed client import, the client MUST NOT restore invalidated cache entries. No acknowledgement API is added.

In this context, "server-to-client" refers to Middleware's forwarding to the application client downstream of the in-process HAL call boundary.

The vendor MUST determine whether a selection is new from private scheduler identity or equivalent state, not `slotIndex`. A failed acquisition MUST NOT mark the selection as returned.

Reset, flush, seek or equivalent native processing may invalidate the current selection and comparison state. It MUST NOT release acquired frames. Acquisition then returns `NO_FRAME` until decoder consumption establishes another platform video output selection.

Each `OK` acquisition establishes one lock for one distinct selection, with at most one live lock per slot. Section 7.1 limits the total old/new locks. Middleware may share the record through references or one shared owner; final cleanup must use the serialized release path in Section 4.4.

### 7.4 Release

`releaseFrame(CapturedFrame &frame)` requires a valid slot and its matching acquisition token. The HAL must validate the session-unique token against the original lock, including locks from older configurations. A mismatched token returns `INVALID_ARGUMENT`. Reconfiguration must not make an older lock out of range or confuse it with a newer use of the same slot.

On `OK`, the HAL releases the lock and ends the slot's native binding. It sets `frame.slotIndex` to `kInvalidCaptureSlotIndex` and `frame.acquisitionToken` to `kInvalidAcquisitionToken`. Only then may the vendor reuse the native frame or bind the slot to different backing.

Release does not inspect or close FDs. Middleware MUST close any remaining owned FDs separately. The content-access rule in Section 6.2 applies even if an FD remains open after release.

An invalid, moved-from, out-of-range, or stale slot index returns `INVALID_ARGUMENT`. A currently unlocked slot index or a mismatched acquisition token also returns `INVALID_ARGUMENT`. Every non-`OK` result leaves both the frame and its existing lock state unchanged, so a valid frame retains exactly one release obligation. A slot cannot be reused while its lock remains active.

### 7.5 Readiness and playback continuity

Before returning a successful acquisition, the vendor MUST complete decoder writes, cache maintenance and SoC-side synchronization. The selected contents are then safe to read. Native fence handling remains private; the HAL returns no graphics fence.

Capture-slot exhaustion MUST NOT block decode, native scheduled selection, audio presentation or STC progression. Section 7.1 defines how old and new locks share capacity during transitions.

## 8. Vendor delivery requirements

### 8.1 Capture library and compatibility

The vendor MUST provide a shared library exporting this factory through the platform's agreed symbol-discovery convention:

```cpp
ICaptureSession* createCaptureSession();
```

The factory MUST return an owned `ICaptureSession` object. Section 4 defines how Middleware creates, uses and destroys that object.

The platform SDK, build system, and package integration must prevent incompatible HAL library and Middleware combinations. The library and Middleware MUST use the agreed interface revision and compatible C++/standard-library application binary interfaces (ABIs), runtimes and build settings. They MUST use compatible memory allocation and deallocation rules. Compiler versions may differ if these requirements are met.

These requirements apply to the complete C++ interface. Runtime version negotiation is not required.

Status-returning HAL calls MUST NOT allow exceptions to escape across the library boundary. The implementation reports failures through the existing `CaptureStatus` results.

The library MUST provide capture-failure logging through the existing platform logging system. Section 7.3 defines the fatal-error logging requirements.

### 8.2 Platform integration

The vendor MUST provide the westerossink session property and capture integration defined in Section 4. This includes the native decoder and platform video output connections needed for capture. The integration MUST preserve the existing platform-sink behaviour defined in Section 4.1.

### 8.3 Graphics support

The vendor MUST support direct graphics import for its supported frame layout through both EGL/OpenGL ES and Vulkan.

**EGL/OpenGL ES support**

The EGL implementation provides the functionality defined by:

- `EGL_EXT_image_dma_buf_import`, including the `EGL_LINUX_DMA_BUF_EXT` image target and the DRM format and plane FD, offset, and pitch attributes;
- `EGL_EXT_image_dma_buf_import_modifiers`, including per-plane modifier low/high attributes and `eglQueryDmaBufFormatsEXT()` / `eglQueryDmaBufModifiersEXT()`; and
- EGL image creation and destruction through `eglCreateImage()` / `eglDestroyImage()`, or the equivalent `eglCreateImageKHR()` / `eglDestroyImageKHR()` functionality defined by `EGL_KHR_image_base`.

The OpenGL ES implementation provides:

- `GL_OES_EGL_image` and `glEGLImageTargetTexture2DOES()`;
- `GL_OES_EGL_image_external`, `GL_TEXTURE_EXTERNAL_OES`, and `samplerExternalOES`;
- `GL_OES_EGL_image_external_essl3` for ESSL 3 external-texture shaders; and
- the core texture operations `glGenTextures()`, `glBindTexture()`, `glTexParameteri()`, and `glDeleteTextures()`.

**Vulkan support**

The Vulkan implementation provides the functionality defined by:

- `VK_KHR_external_memory`;
- `VK_KHR_external_memory_fd`;
- `VK_EXT_external_memory_dma_buf`, including `VK_EXTERNAL_MEMORY_HANDLE_TYPE_DMA_BUF_BIT_EXT`;
- `VK_EXT_image_drm_format_modifier`;
- `VK_KHR_get_memory_requirements2` and `VK_KHR_bind_memory2`, or their core equivalents; and
- `VK_KHR_sampler_ycbcr_conversion`, or Vulkan 1.1 core functionality, for YUV capture formats.

**Vendor capability declaration**

The vendor supplies a matrix row for the supported frame layout. Each row declares:

- the EGL/OpenGL ES and Vulkan functionality provided by extensions or equivalent core support;
- the EGL format/modifier query result and external-only flag;
- `GL_OES_EGL_image`, external-texture, and ESSL 3 support;
- the Vulkan `VkFormat`, modifier, sampled-image features, external-memory importability, disjoint-plane support where applicable, and sampler-YCbCr conversion support;
- support for the concrete plane count/order, offsets, strides, and shared, separate or mixed plane-to-DMA-BUF-object mapping;
- supported YCbCr matrices, sample ranges and horizontal/vertical chroma siting; and
- confirmation that the layout is directly importable, sampleable as correctly interpreted RGB(A), and releasable through both graphics paths.

This matrix is integration evidence. It does not add runtime capability fields or replace the normative requirements.

### 8.4 Documentation and evidence

The vendor MUST supply:

- documentation of the supported video configurations, frame layout and pool-capacity limits;
- a description of supported configuration changes and any platform restrictions that prevent capture;
- expected frame-acquisition time, sustainable capture rate, memory requirements and other resource costs for the supported configurations;
- a runnable integration example meeting the requirements in Section 4.5; and
- evidence against the conformance checklist in Section 9.

The implementation returns capability values for the configured pipeline, as described in Section 3.1. The documentation describes supported configurations and limits. These declarations are integration documentation, not additional runtime capability fields. This specification sets no universal numeric performance target.

## 9. Conformance checklist

An SoC implementation is conformant only when all of the following are true:

| Area | Conformance evidence | Sections |
|---|---|---|
| Delivery | Compatible SDK/ABI/runtime, owned factory result and virtual-destructor deletion; declared constraints, performance and useful fatal diagnostics. | 8.1, 8.3, 4.6, 7.3–7.4 |
| Association | Sink property receipt before state change, independent sessions for two pipelines, missing/null/replaced-property rejection and rollback. | 4.1–4.3, 4.7 |
| Configuration and capacity | Accurate configured-READY query, positive inclusive limits, every intermediate count, out-of-range rejection, initial rollback and rejection after one successful pool creation. No writes before pool creation. | 4.4, 3.1–3.2 |
| Layout and backing | Unbound initial templates; complete bounds-checked acquisition metadata. Shared/separate/mixed objects, NV12, isolated ranges across locks and exactly one owned FD per distinct object. | 6, 5 |
| Resolution and colour | Durable total old/new capacity, ordinary exhaustion versus session-fatal unsupported transition, valid per-frame effective colour including BT601-to-BT709. Old contents and releases remain valid. | 6.2, 6.3, 7.3–7.4 |
| Native selection | OK once per new platform video output selection, NO_NEW_FRAME on repeated polls, NO_FRAME without a selection. Paused/final selections follow the lifecycle contract. | 4.6, 4.7, 7.1, 7.3 |
| Identity and atomicity | Nonzero 64-bit IDs/tokens, safe stale/moved/released-record rejection, custom move/reset/output reuse, unchanged failed outputs and complete temporary-lock/FD rollback. | 5, 7.3–7.4 |
| Graphics | Direct exact-layout import and correct RGB(A) sampling in both APIs; colour-compatible cache reuse, owned-FD cleanup on cache hits, GPU completion before release and partial-import rollback. Vendor supplies the matrix. | 8.3, 6.2–6.5 |
| Invalidation | Latched over unsuccessful calls, ordered through failed IPC/import, applied before reused-ID lookup; old retained imports cleaned only after final use/release. Generic pipeline flush is distinct. | 6.3, 7.3 |
| Playback and isolation | Exhaustion never blocks decode, presentation, audio or STC. Existing sink properties, surfaces, decoder discovery, subtitles, secondary video, geometry, visibility and immediate output preserved. | 6, 4 |
| Threading and teardown | Serialized Middleware calls and native synchronization; no teardown race; final acquisition, late releases, library lifetime and deletion after all obligations. Runnable integration example and two-pipeline tests. | 4.4, 4.6, 4.7 |

These are implementation test obligations, not a report of tests run on hardware. Acquisition tests from different threads MUST still serialize calls as required by Section 4.4.

Failure of any mandatory gate means Video Frame Capture is unsupported on that implementation; it does not permit a weakened lifetime or playback-continuity contract.

## Appendix A. Revision history

These entries describe historical changes, not additional requirements. References to earlier section numbers and superseded integration models are historical.

| Version | Status | What changed | What reviewers should check |
|---|---|---|---|
| 0.91 | Draft for review | Resolves the 0.9 review: CapturedFrame rationale, corrected changelog, version reconciliation, cross-reference fixes, IPC clarification. | Design rationale, version authority, checklist references, boundary clarity. |
| 0.9 | Draft for review | Restructured for progressive disclosure, renamed sections, consolidated pool creation, improved readability, replaced jargon. | Reading order, pool strategy, human readability, terminology. |
| 0.81 | Draft for review | Resolves the 0.8 review: effective per-frame colour, explicit C++ ownership/compatibility, durable pool creation and a focused readability pass. | Configured READY, one total retention limit, old-frame validity, actor/ownership clarity and early conceptual orientation. |
| 0.8 | Earlier draft | Configured slot limits, stable allocation IDs, cached imports and invalidation. Timing and transition choices were provisional in that revision. | Historical baseline for the review; the 0.81 contract supersedes its provisional lifecycle. |
| 0.7 | Draft for review | Mandatory EGL/OpenGL ES and Vulkan direct import and logical reservation model. | Exact layout support, graphics lifetime and stable locked backing. |
| 0.6 | Draft for review | Replaced GstContext delivery with a sink property and added westeros-sink-soc.h integration. | Element-local association, property handling and teardown. |
| 0.5 | Draft for review | Library/factory delivery explanation and platform-selected sink example; no method behaviour changes. | Discovery and ownership explanation. |
| 0.4 | Draft for review | Existing platform sink retained; direct or private-helper capture integration. | Normal sink behaviour, scheduled-output attachment and tests. |
| 0.3 | Draft for review | Complete plane/object representation, tokens, transactional FDs and pipeline integration examples. | Failure invariants, FD ownership, construction and late release. |
| 0.2 | Draft for review | One acquisition per scheduler selection, multiple retained locks. | NO_NEW_FRAME, exhaustion, flush and teardown. |
| 0.1 | Initial draft | Session, pool, sink association, selection and lifetime contract. | Complete initial specification. |

### What changed in version 0.9

- Restructured sections for progressive disclosure: purpose, conceptual model, capture session interface, library and GStreamer integration, captured frames, acquisition/use/release, configuration changes and shutdown, vendor delivery, conformance.
- Renamed section 3 to “Capture Session interface” and moved it before integration.
- Moved vendor delivery requirements to section 8, after configuration changes and shutdown.
- Merged backing allocation choices into pool creation (section 3.2).
- Removed duplicate section 5.4; moved pool layout constraints into section 8.2.
- Replaced “native scheduled output” with “platform video output” throughout.
- Clarified backingSize in section 5.1 with cross-reference to section 3.2.
- Rewrote dense sentences for clarity and removed C++ jargon from narrative text.
- Updated all cross-references to new section numbers.


### What changed in version 0.81

- Query configured minimum/maximum frame-slot counts after mandatory native configuration at READY. Every integer in the inclusive range is supported.
- Identify backing using normative nonzero 64-bit IDs; export independently owned FDs on each acquisition. Tokens are session-unique nonzero 64-bit values.
- Cache compatible imports after release without permitting access to released pixels. Return effective per-frame matrix, range and both chroma-siting values, including changes during a session.
- Latch allocation invalidation until successful acquisition; retain old frames with their own descriptions and tokens.
- Replace the earlier provisional query-timing and resolution-change pool-creation assumptions with one durable pool creation and a total old/new lock limit. Unsupported transitions are capture-fatal without invalidating old locks or stopping playback.
- Clarify serialized calls/native synchronization, SDK/ABI policy, factory creation and ordinary deletion. Remove the duplicate enabled flag.
- Move history here, bring the model forward, renumber from 1 and use Mermaid for the embedded lifecycle. No individual requirement IDs.
- Restructure sections for progressive disclosure: purpose, conceptual model, session interface, library and GStreamer integration, captured frames, acquisition/use/release, configuration changes and shutdown, vendor delivery, conformance.

### What changed in version 0.91

- Added explicit rationale for CapturedFrame destructor and move operations in section 5.1.
- Removed duplicated "What changed in version 0.9" block.
- Updated specification version to 0.91 and added 0.9 and 0.91 rows to revision-history table.
- Fixed conformance checklist cross-references: Graphics row now cites 6.2–6.5; Resolution and colour row now cites 6.2, 6.3.
- Clarified IPC language in section 7.3 to distinguish downstream Middleware-to-client forwarding from the in-process HAL boundary.
