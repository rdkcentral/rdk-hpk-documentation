# SoC Video Frame Capture Specification

**Status:** Draft for review.

**Specification version:** 0.3

## Revision history

| Version | Status | What changed | What reviewers should check |
|---|---|---|---|
| 0.3 | Draft for review | Separated capability limits from concrete pool layout; defined explicit FD and frame-lock ownership, invalid lock identity, non-copyable/movable frame output, vendor playbin integration, native selection behavior, plane validation, and fixed-layout conformance. | Capability/pool pairing, move and release state, output reuse, FD ownership versus frame locks, native selection, attachment, teardown, and rollback. |
| 0.2 | Draft for review | Changed frame acquisition so the HAL returns each newly selected frame once instead of repeatedly returning the same slot. Clarified that several previously returned frames may remain locked at the same time. | Frame acquisition and release, behavior when there is no newer frame, slot exhaustion, flush, and teardown. |
| 0.1 | Initial draft | Introduced the capture session, DMA-BUF pool, GStreamer attachment, frame selection, and lifetime contract. | Complete specification. |

### What changed in version 0.3

- Frame planes reference DMA-BUF objects by index; the API can represent shared-object and multi-object layouts.
- `getCapabilities()` reports format, modifier and limits; `createPool()` reports the concrete plane/object layout. Pool metadata contains no FDs.
- Acquisition exports one MW-owned FD for each distinct object referenced by the selected slot.
- `CapturedFrame` is non-copyable. Move transfers both FD ownership and the frame-release obligation; invalid slot identity means no release is owed.
- `getCapabilities()` returns `CaptureStatus` and leaves output unchanged on failure.
- The vendor playbin policy creates and attaches `framecapture` to native scheduled output.
- Native selection, paused behavior, slot exhaustion, teardown, plane-range validation, and late release are fully defined.

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
 ├── calls ICaptureSession
 └── owns returned FDs and matching frame releases

SoC vendor
 ├── implements ICaptureSession
 ├── provides framecapture and the playbin policy
 └── connects capture to native scheduled output
```

MW means the middleware component that calls the HAL. Transport and rendering after the HAL returns a frame are not part of this specification.

---

## 1. Purpose

This specification defines how the SoC vendor exposes decoded clear-video frames to MW.

`ICaptureSession` provisions the capture slots and tracks their ownership while frames are retained. The SoC vendor chooses how those slots are backed and supplied to the scheduled-output path.

This adds to the existing SoC decode stack; it does not replace it.

## 2. Roles and responsibilities

| Role | Definition | Responsibilities |
|------|------------|------------------|
| **MW** | The middleware that calls the HAL. | - Obtains one session for each capture-enabled pipeline<br>- Calls the HAL methods with valid inputs<br>- Closes returned FDs and releases acquired frames<br>- Keeps the session and pool valid until detach and all releases are complete<br>- Uses the existing playback integration to make the session and pool available to the vendor playbin policy |
| **SoC vendor** | The provider of the decoder and capture implementation. | - Implements `ICaptureSession` and the advertised fixed layout<br>- Provides and registers `framecapture` and the playbin policy<br>- Attaches capture to native scheduled output and follows its frame selection<br>- Exports FDs atomically and preserves locked frame contents<br>- Keeps normal playback running when capture slots are exhausted<br>- Detaches safely and accepts required late releases |

## 3. Scope

### What this specification defines

- The capture session and fixed pool layout.
- Association with native scheduled output and its frame selection.
- Frame-plane, DMA-BUF object, crop, format, modifier, and timestamp metadata.
- Atomic frame acquisition, FD ownership, frame locks, and release.
- Synchronization, slot exhaustion, detach, teardown, and late release.

### 3.1 Terminology

The contract uses these terms exclusively:

| Term | Meaning |
|---|---|
| **Current selection** | The frame chosen by native scheduled output for the current STC/display position. |
| **Frame** | The current selection's timestamp, visible region, slot, and exported FDs. One successful acquisition creates one frame lock. |
| **Slot layout** | The fixed plane layout for one reusable pool slot. |
| **Frame plane** | One DRM image plane and its byte range and stride within a DMA-BUF object. It owns no FD. |
| **DMA-BUF object** | One DMA-BUF allocation. One object may back one or more planes or slots. |
| **Exported DMA-BUF** | One MW-owned raw FD for a DMA-BUF object. `fd >= 0` is owned and must be closed; `-1` is invalid. The FD is not a frame lock. |
| **Pool** | The DMA-BUF object metadata and slot layouts created for one session. Pool metadata contains no FDs. |

The document does not use “buffer” as a synonym for either slot or frame. “Decoder output buffer” is used only when referring to a vendor-native object outside the capture-pool contract.

### 3.2 Conceptual model

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
- Each **frame plane** references exactly one **DMA-BUF object** and describes its byte range and stride within that allocation.
- Several planes may reference the same object at different offsets, or each plane may reference a different object. Mixed arrangements are valid.
- A successful acquisition exports one FD for each distinct object referenced by the selected slot, never one FD per plane by implication.
- The process-local FD value is not object identity. `dmaBufObjectIndex` is the join key between plane layout and exported handle records.

## 4. Mandatory ownership model

The following behavior is normative; the allocation strategy is an SoC vendor decision:

> `ICaptureSession` provisions the capture slots and owns their retention/lock state. The SoC vendor may back them with decoder DPB buffers, a separate capture pool, or another implementation. Regardless of strategy, a locked slot's contents must remain valid and must not be overwritten until its matching `releaseFrame()`. Pipeline teardown must not invalidate an outstanding acquired frame; resources needed to complete its late release remain valid until the session is closed.

MW owns the session from the caller side and releases each acquired frame exactly once. The SoC vendor owns the slot state and backing implementation.

The decoder and capture path must not overwrite a locked slot or invalidate an acquired frame during teardown. Capture-slot exhaustion must not block normal decode, presentation, audio, or STC progression. The backing strategy is a vendor choice.

## 5. Session provision and lifecycle

### 5.1 Session provision and initialization

MW obtains one vendor `ICaptureSession` for each capture-enabled video pipeline. The existing playback integration makes the session and pool available to the vendor playbin policy; this specification does not define that handoff.

**Pipeline scope:** Each video pipeline using Video Frame Capture has a dedicated `ICaptureSession`. The session tracks one set of capture-slot reservations and is attached only to that pipeline. A detached session must not be attached to another video pipeline; a new pipeline requires a new session.

**Close timing:** MW keeps the session alive after detach while a final selection may still be acquired or frame locks remain. MW destroys the session only after that final opportunity is resolved and all acquired frames are released. A session is never reused for another pipeline.

### 5.2 Capture activity model

Capture is a **passive observer** of the decoder's native STC scheduler. Before the decoder has consumed sufficient data and scheduled output has established a valid display selection, acquisition returns `NO_FRAME`. Once a valid native selection exists, capture follows that selection without applying a separate state-based policy.

While native scheduled output is paused, STC and display position are stationary. A valid current selection remains acquirable once and then yields `NO_NEW_FRAME`; paused operation alone does not guarantee that decoder consumption has established a selection. Decoder activity while transitioning to or operating paused may establish one.

Reset, flush, seek, or equivalent decoder/scheduled-output processing may invalidate the current selection and comparison state without releasing acquired frames. Acquisition then returns `NO_FRAME` until further decoder consumption establishes another native selection. During active scheduled output, selections advance according to native scheduling.

## 6. Scheduled-output association and frame selection

MW makes the session and pool available through the existing playback integration. The vendor playbin policy must attach them to the SoC element that owns or exposes native STC-based frame selection. This specification does not define the handoff.

The vendor playbin policy identifies scheduled output, creates/inserts `framecapture`, invokes the typed attachment API in Section 9.2, and detaches before teardown. For every returned frame, the vendor implementation uses the same STC and scheduling rules as native presentation. It can publish only a selection established after decoder consumption and never selects a frame independently from the native scheduler.

`CapturedFrame.presentationTimeNs` is the selected frame's PTS in the scheduler timeline. The session retains the most recent valid selection until a newer one is captured or native decoder/scheduled-output processing invalidates it. Invalidation does not release previously acquired frames.

How `framecapture` reaches the scheduler and obtains pixels remains private to the SoC vendor's playbin policy, element, and `ICaptureSession` implementation. Merely creating or adding `framecapture` does not establish association; typed attachment must complete. The bridge borrows the session and pool and may be destroyed with the pipeline after detach.

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
    int64_t presentationTimeNs;
    Rectangle visibleRegion;
    std::vector<ExportedDmaBuf> exportedDmaBufs;

    CapturedFrame() : presentationTimeNs{0}, visibleRegion{} {}
    CapturedFrame(const CapturedFrame &) = delete;
    CapturedFrame &operator=(const CapturedFrame &) = delete;
    CapturedFrame(CapturedFrame &&other) noexcept;
    CapturedFrame &operator=(CapturedFrame &&other) noexcept;
    ~CapturedFrame() = default;
};
```

The value types follow these rules:

- `backingSize` describes the capacity of each logical slot; it does not require a particular physical allocation strategy.
- MW owns each successful `fd >= 0`, closes it explicitly, and sets it to `-1`. `-1` means invalid. Destructors do not close FDs.
- `CapturedFrame` is non-copyable. A valid `slotIndex` means exactly one `releaseFrame()` is owed, independently of FD ownership.
- Moving a frame transfers its metadata, FDs, and release obligation without `dup()` or `close()`. The source FDs become `-1` and its `slotIndex` becomes `kInvalidCaptureSlotIndex`.
- Move assignment requires an empty destination: invalid `slotIndex` and no owned FDs. It must not discard an existing lock obligation or FD.
- Pool `slotIndex` and `objectIndex` values equal their positions in `slotLayouts` and `dmaBufObjects`. They are unique and contiguous from zero; no pool slot may use `kInvalidCaptureSlotIndex`.
- `planeIndex` follows the plane order defined by the reported DRM format. Import code maps that order to its graphics API.
- Each plane references one valid object. Validate its range without overflow: `offsetBytes <= sizeBytes` and `lengthBytes <= sizeBytes - offsetBytes`. The range and stride must fit the advertised layout.
- Plane and object metadata remain fixed for the pool lifetime. Storage not exposed as a DRM plane remains part of its DMA-BUF object.
- On `OK`, `exportedDmaBufs` contains exactly one entry for each distinct object referenced by the selected slot, with no duplicate or unrelated object. Object index—not FD value—is the join key.

Plane count, object count, and FD count are independent.

A `CapturedFrame` is empty only when its `slotIndex` is invalid and every FD is `-1`. Closing all FDs does not make a frame empty while its slot index remains valid; the frame lock still requires release.

FDs are exported only when a frame is acquired. MW owns and closes them. Closing an FD does not release the frame lock, and `releaseFrame()` does not inspect FD values.

### 7.1 Valid frame representations

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

### 7.2 DRM NV12 linear example

For this example, `CaptureCapabilities` reports `DRM_FORMAT_NV12` and `DRM_FORMAT_MOD_LINEAR`; the pool supplies the plane/object layout shown below. Linux `drm_fourcc.h` defines NV12 as a two-plane format and the modifier as linear storage. An NV12 slot may represent plane 0 (luma Y) and plane 1 (interleaved chroma UV) using either Example B, where both planes reference one DMA-BUF object at different offsets and acquisition exports one FD, or Example C, where each plane references a separate object and acquisition exports two object-indexed FDs. The modifier does not determine DMA-BUF object or FD cardinality; the plane-to-object mapping in the slot layout does. These examples define representational capability, while an implementation must still advertise and provide an arrangement importable through every required graphics path.

## 8. Session interface

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

`getCapabilities()` reports exactly the fixed `drmFormat`, `drmModifier`, `maximumContentSize`, and `maximumSlots` for the session. It does not report plane count/order, DMA-BUF objects, plane mappings, offsets, lengths, strides, or actual pool backing size. `OK` fully populates `out`; `UNSUPPORTED` or `FATAL_ERROR` leaves it unchanged.

`createPool()` accepts a valid `slotCount` and returns the concrete pool layout: `backingSize`, DMA-BUF object indices/sizes, and each slot's plane indices, object references, offsets, lengths, and strides. It does not repeat format or modifier and contains no FDs. Every plane range must pass the overflow-safe validation in Section 7. `backingSize` must not exceed `maximumContentSize`, and the returned layout must be valid for the same session's format and modifier.

The capability result and pool belong to the same session and are interpreted together. Format, modifier, and limits remain fixed for that session. MW keeps both records while it needs to interpret the pool; no extra capability fields or duplicated pool fields are implied.

The SoC vendor chooses how to provide the slots. `maximumSlots` is the greatest count that preserves the resources needed for normal playback.

`acquireCurrentFrame()` requires an empty `outFrame`: invalid slot index and no owned FDs. A valid slot index means that output still owes a release even if all its FDs are already closed. Passing a non-empty output returns `INVALID_ARGUMENT` without changing it.

For an empty output, the method examines the current native selection. A new selection is temporarily locked, all required FDs are exported, and the complete result—including the valid slot index and release obligation—is moved into `outFrame` before returning `OK`. No valid selection returns `NO_FRAME`; an already returned selection returns `NO_NEW_FRAME`. Every non-`OK` result leaves `outFrame` unchanged and creates no release obligation.

If descriptor creation/export fails because a transient resource is unavailable, including descriptor exhaustion, `acquireCurrentFrame()` returns `NO_RESOURCES`. Before returning, the vendor implementation explicitly closes every FD created for that attempt, removes the temporary lock, leaves the selection not returned, and does not move any result into `outFrame`. No matching release is required and the selection remains retriable. An unrecoverable session/backing failure returns `FATAL_ERROR` with the same all-or-nothing cleanup. `FATAL_ERROR` is not retriable; capture is unavailable for the remainder of the session. An implementation that cannot roll back atomically is non-conformant.

`createPool()` establishes the logical slots and their storage metadata, but does not require backing allocation or contain/pre-create DMA-BUF FDs. For each `OK`, `acquireCurrentFrame()` creates/exports the FD handles needed to access the selected slot's DMA-BUF objects. No newly created caller-owned FD handles from a failed attempt survive any non-`OK` result; any pre-existing value in `outFrame` remains the caller's unchanged responsibility.

On `OK`, MW owns every descriptor in `outFrame.exportedDmaBufs` and must close each one explicitly. Descriptor closure neither releases nor satisfies the HAL frame lock. The lock protects frame contents until the caller invokes the one matching `releaseFrame()` after all use has ended.

The SoC implementation determines whether a native scheduler selection is new using private scheduler identity or equivalent internal state, not `slotIndex`; a released slot may later contain a different selection. The new-selection check, temporary lock, complete descriptor export, returned-state commit, and move into `outFrame` form one atomic success transaction. A failed attempt rolls back and does not consume the selection. Later acquisition of the same current selection returns `NO_NEW_FRAME`. Reset, flush, seek, or equivalent decoder/scheduled-output processing may invalidate the current selection and comparison state without releasing acquired frames; acquisition then returns `NO_FRAME` until decoder consumption establishes another native selection.

Every `OK` acquisition establishes one lock for one distinct captured selection. Several acquired frames may lock different slots concurrently, but there is at most one live lock per slot and one successful acquisition per selection. MW may share access through references or one shared owner around the non-copyable record and calls `releaseFrame()` once when final use ends.

`releaseFrame(CapturedFrame &frame)` requires a valid slot index. It releases that slot lock and, on `OK`, sets `frame.slotIndex` to `kInvalidCaptureSlotIndex`. It does not inspect or close FDs, so MW must still close any open FDs separately. A successful release makes the frame invalid for content access even if an FD remains open.

An invalid, moved-from, out-of-range, stale, or currently unlocked slot index returns `INVALID_ARGUMENT`. Every non-`OK` result leaves the frame unchanged. A slot cannot be reused while its lock remains active.

The **SoC vendor** supplies both `ICaptureSession` and the GStreamer integration. Scheduler notifications, writable-slot management, decoder handles, and copy/conversion details remain private between those components.

Capture has no independent start/stop state or selection policy. It follows valid native scheduler selections after decoder consumption. A valid selection remains acquirable while scheduled output is paused at stationary STC; a state transition alone does not manufacture or advance one.

No factory is implied by this interface.

## 9. GStreamer-to-session contract

The vendor playbin policy is the GStreamer integration path. Once MW makes the session and pool available, the policy creates `framecapture`, identifies native scheduled output and STC, and attaches them through the common typed API. The vendor implementation must:

```text
make the framecapture factory and playbin policy available
create and add framecapture during playbin pipeline construction
identify the native scheduled-output path and its STC
accept and validate the CapturePool metadata
obtain pixels for native scheduler selections after decoder consumption
invoke detach before scheduled-output teardown
update the session's current frame: slot index, PTS, and visible region
```

The existing playback integration provides the session and pool; this specification does not define the handoff.

### 9.1 Element and policy delivery

The SoC delivery registers a GStreamer element factory named `framecapture` and provides the required playbin policy. Pad topology and links are vendor-private.

During pipeline construction, the playbin policy creates `framecapture`, identifies native scheduled output, and completes typed attachment before decoder output starts. If the element or policy is unavailable, Video Frame Capture is unsupported but normal playback remains usable.

### 9.2 Common attachment API

The **SoC vendor** provides a C++-callable GStreamer integration header containing:

**Caller:** The **SoC vendor's playbin policy** calls `gst_frame_capture_attach()` and `gst_frame_capture_detach()` in process after MW has made the pipeline-scoped session and pool available.

```cpp
CaptureStatus gst_frame_capture_attach(
    GstElement *frameCapture,
    GstElement *vendorVideoElement,
    ICaptureSession *session,
    const CapturePool &pool);

CaptureStatus gst_frame_capture_detach(
    GstElement *frameCapture);
```

`gst_frame_capture_attach()` is called once before decoder output starts. It validates the session and pool, associates them with native scheduled output, and returns only when attachment has completed or failed. A failed attachment must leave normal playback usable.

Before the call, the playbin policy has created `framecapture`, identified scheduled output, and obtained a valid session and pool. On `OK`, capture is fully attached and decoder output may start. MW keeps the session, pool, and scheduled-output element valid until detach returns.

`gst_frame_capture_detach()` is idempotent and synchronous. It prevents new writes, completes or cancels any write in progress, disconnects scheduled output, and returns only when scheduled output can be destroyed safely. It must preserve the final current selection and backing required by outstanding frame locks.

### 9.3 Native decoder and scheduled-output behavior

| Native decoder/scheduled-output condition | Required capture behavior |
|---|---|
| Before sufficient decoder consumption or with no valid native selection | Return `NO_FRAME` |
| Decoder consumption while transitioning to or operating paused | Follow any selection established by the native scheduler; the activity may establish a selection but paused operation alone does not guarantee one |
| Paused with a valid current selection | STC/display position is stationary; return that selection once as `OK`, then `NO_NEW_FRAME` while it remains current |
| Active scheduled output | Follow advancing native scheduler selections into writable slots |
| Reset, flush, seek, or equivalent processing invalidates selection | Preserve outstanding locks and return `NO_FRAME` until decoder consumption establishes another native selection |
| Teardown | Vendor playbin policy calls `gst_frame_capture_detach()` before destroying scheduled output or the session |

These conditions do not create or destroy the pool. Capture never applies a separate state-based frame-selection policy.

### 9.4 Backing strategy

The backing strategy is vendor-private. It must provide the advertised fixed layout, preserve locked contents, and keep normal decode, presentation, audio, and STC running when capture slots are exhausted.


## 10. Current selection and slot ownership

The session tracks one current captured selection, whether that selection has been successfully acquired, and the set of slots locked by previously acquired frames.

A slot is writable only when no frame lock is active and no capture write is in progress. Multiple distinct acquired frames may lock different slots concurrently, but each slot has at most one live lock.

An unlocked current slot may be reused for a newer selection. Before writing it, the session atomically invalidates that current selection, so a concurrent `acquireCurrentFrame()` returns `NO_FRAME` rather than observing storage being overwritten.

When the native scheduler selects a different current frame, the SoC path captures it into a writable slot, completes private synchronization, assigns private selection identity, and makes that slot current and not yet acquired. Selection identity is independent of storage identity: a slot released from an older frame may later contain a new selection that must return `OK`.

`acquireCurrentFrame()` checks the current selection, temporarily locks its slot, exports every required descriptor, and commits the returned state in one atomic success transaction. A descriptor-export failure closes partial descriptors, removes the temporary lock, leaves `outFrame` unchanged, and leaves the selection available for retry. The first committed call for that selection returns `OK`; later calls return `NO_NEW_FRAME` without returning its `slotIndex` again or changing lock state. The slot therefore cannot become writable during a successful selection/export/commit transaction.

`releaseFrame()` removes one lock and invalidates that frame's slot index. Releasing one slot has no effect on other acquired slots. Open FDs remain MW-owned but no longer provide access to stable frame contents.

If no slot is writable when the scheduler advances, the new capture selection is omitted. The previous captured selection remains current; if it was already acquired, `acquireCurrentFrame()` returns `NO_NEW_FRAME`. Video decode, native presentation, audio presentation, and STC progression continue without waiting. Once capacity returns, the next captured selection represents the then-current SoC frame and returns `OK` on its first acquisition; missed intermediate selections are not replayed.

## 11. Detach, teardown, and late release

The session belongs to one video pipeline and is never reused. Teardown proceeds in this order:

1. Stop new capture writes and complete or cancel any write in progress.
2. Detach `framecapture` from scheduled output before destroying the video path.
3. Keep the session, final current selection, and locked slots valid.
4. If the final selection has not been acquired, allow one acquisition. A transient `NO_RESOURCES` result leaves it available for retry.
5. Continue accepting `releaseFrame()` for every acquired frame.
6. MW destroys the session only after the final acquisition opportunity is completed or abandoned and every frame lock is released.

After detach, the selection no longer advances. An unacquired valid selection returns `OK` once; an already acquired selection returns `NO_NEW_FRAME`; no valid selection returns `NO_FRAME`. Locked frame contents remain valid until release.

## 12. FD and frame lifetime

FD lifetime and frame-content lifetime are separate:

- On `OK`, MW owns every returned FD and closes it explicitly.
- Closing an FD does not release the frame lock.
- The frame lock is the only contract that prevents the SoC from overwriting the slot.
- MW completes all use of the frame before calling `releaseFrame()`.
- After release, MW must not access that frame and the SoC may reuse the slot.
- Detach does not invalidate a final current selection or an acquired frame; required backing remains valid until final acquisition and late releases are complete.

## 13. Synchronization

On `OK`, all decoder writes, cache maintenance, and SoC-side synchronization are complete, so the slot is safe to read. MW completes downstream use before release. The HAL does not return a graphics fence; any SoC-internal synchronization is vendor-private.

## 14. Fixed capture layout

Each implementation supports one fixed capture contract. `getCapabilities()` reports its format, modifier, and limits. `createPool()` reports the concrete object, slot, and plane layout. The two records belong to the same session and together describe the layout; neither duplicates the other's fields.

The implementation need not support every shared-object, separate-object, or mixed arrangement shown in the examples. Its returned pool must faithfully represent its one fixed format/modifier arrangement and be usable through every required graphics path.

For the requested slot count, `CapturePool` describes:

- the actual logical slot capacity in `backingSize`;
- each DMA-BUF object's index and size; and
- each slot's indexed planes, object references, offsets, lengths, and strides.

Every slot supports `backingSize`; that size must not exceed `maximumContentSize`. `slotLayouts.size()` equals the requested count, which must not exceed `maximumSlots`.

The capability values and concrete pool layout remain fixed until the session is closed. Resolution or crop changes update `CapturedFrame.visibleRegion`; they do not replace the slot reservation.

If content exceeds `CaptureCapabilities.maximumContentSize` or cannot be represented by the implemented capture layout, capture returns `UNSUPPORTED` or `FATAL_ERROR` and stops updating its current selection. Normal playback remains available. Runtime capture-slot replacement is outside this specification. One `ICaptureSession` has one logical slot reservation for its complete lifetime.

## 15. Vendor delivery information

For integration and review, the SoC vendor supplies:

1. the registered `framecapture` factory and playbin policy;
2. the concrete `CaptureCapabilities` values;
3. the advertised fixed pool layout and supported slot counts;
4. evidence that native selection, locking, FD export, exhaustion, detach, and late release meet this specification; and
5. any platform constraint that makes Video Frame Capture unsupported.

The vendor is not being asked whether decode may stop under slot exhaustion or whether the pool may be destroyed with the decoder. Those are mandatory requirements below.

### 15.1 Decoder access

The SoC vendor provides whatever private decoder access `framecapture` needs. It must not interfere with native scheduling, STC progression, audio presentation, or normal playback.

## 16. Conformance checklist

An SoC implementation is conformant only when all of the following are true:

| Area | Requirement | Sections |
|---|---|---|
| Delivery and attachment | The vendor registers `framecapture`, provides the playbin policy, attaches to native scheduled output before decoder output, and detaches safely before teardown. Existing playback integration provides the session and pool. | §§5, 6, 9 |
| Capabilities and pool | Capabilities report only format, modifier and limits. Pool creation returns the concrete bounds-checked object/slot/plane layout for the same session, within those limits. | §§7, 8, 14 |
| Native selection | Capture follows native scheduled output. It returns `NO_FRAME` without a valid selection, `OK` once for a new selection, and `NO_NEW_FRAME` for a repeated selection. Paused and invalidation behavior follows §9.3. | §§5, 8–10 |
| Frame identity, FD ownership and rollback | Valid slot identity means one release is owed. Move transfers that identity and all FDs, invalidating the source. Acquisition/move-assignment require an empty destination. MW closes one FD per distinct object. Export failure rolls back and leaves output unchanged. | §§7, 8, 12 |
| Frame locks and synchronization | One `OK` creates one lock. Successful release invalidates the slot identity but leaves FD ownership unchanged. Locked contents remain stable until MW completes use and releases the frame. | §§4, 8, 10, 12, 13 |
| Exhaustion and playback | Slot exhaustion may omit capture selections but must not stop decode, presentation, audio, or STC. | §§9, 10 |
| Detach and teardown | Final acquisition, transient retry, outstanding frame validity, late release, and session close follow the ordered teardown contract. | §11 |
| Contract tests | Tests cover every area above, including invalid/default/moved/released frame identity, move-assignment destination checks, acquisition-output reuse, capability/pool pairing, layout bounds, rollback, repeated/concurrent acquisition, exhaustion, detach, final acquisition, and late release. | — |

Failure of any mandatory gate means Video Frame Capture is unsupported on that implementation; it does not permit a weakened lifetime or playback-continuity contract.

