# SoC Media Metrics GStreamer Specification

**Status:** Draft for review.

**Specification version:** 0.4

## Revision history

| Version | Status | What changed | What reviewers should check |
|---|---|---|---|
| 0.4 | Draft for review | Added exact cumulative corrupted-frame count to the video frame snapshot already introduced in 0.3. | Corruption identity/deduplication, overlap with presentation outcomes, event consistency, and transport. |
| 0.3 | Draft for review | Added exact cumulative rendered and dropped video-frame counters for Cobalt/YouTube PAT and WPE-WebKit W3C reporting. | Counter property, presentation-point ownership, hidden-frame exclusions, seek/discard behavior, and conformance tests. |
| 0.2 | Draft for review | Made all seven defined metrics part of the required SoC contract and removed per-platform metric capability reporting. | Required metric coverage, topology limits, buffer-underflow reporting, client selection, and conformance tests. |
| 0.1 | Initial draft | Introduced the GStreamer message format, metric meanings, producer ownership, collector behavior, and conformance rules. | Complete specification. |

### What changed in version 0.4

- Added mandatory cumulative `corrupted` alongside `rendered` and `dropped` so Cobalt, WPE and Prime can expose their existing corruption fields without relying on potentially filtered or missed notifications.
- Defined corruption as one frame-level classification per distinct affected frame; repeated hardware indications for one frame do not increment it again.
- Clarified that corruption may overlap either rendered or dropped and is never added to W3C total video frames.
- Required the cumulative counter and `MEDIA_METRIC_VIDEO_DECODE_ERROR` occurrences to originate from the same authoritative observations.

### What changed in version 0.3

- Added a mandatory read-only cumulative snapshot containing distinct rendered and dropped video-frame counts for direct SoC-rendered video.
- Defined W3C total frames as rendered plus dropped; submitted, demuxed, decoder-processed and codec-hidden pictures are not valid substitutes.
- Defined seek, flush and `discardUntilPosition()` exclusions so application-directed removal does not appear as poor playback.
- Required one coherent snapshot that remains monotonic across normal playback-state and presentation-timeline changes.
- Kept per-drop occurrence messages for event consumers; the cumulative snapshot is independent of client event subscription and delivery loss.

### What changed in version 0.2

- Every defined media metric is now required on a conforming SoC; vendors no longer advertise a supported subset.
- The `media-metrics-capabilities` property and other metric-capability queries have been removed.
- Buffer-underflow reporting is now required and uses the same started/resolved episode model as the other episode metrics.
- A metric is required only where its condition can occur inside the active SoC pipeline. For example, the SoC does not report frame repetition performed later by an application renderer after Video Frame Capture handoff.
- Applications can choose which metrics Rialto delivers, but where that filtering happens is an implementation choice for RialtoServer rather than a new public vendor-element control.
- Fatal decoder failure handling is intentionally separate from this frame-level metrics contract.
- The conformance matrix and required tests now cover every defined metric in each applicable pipeline topology.

Reviewers who already reviewed an earlier version can focus on the areas named in each later row. Versions 0.3 and 0.4 do not change the event message field names, value types, units, or producer-ownership model.

---

## 1. Purpose

This specification defines the media metric messages that SoC-supplied GStreamer elements must post and the cumulative video frame counters that scheduled video output must expose.

```text
hardware statistics or callbacks
    → SoC GStreamer element
    → media-pipeline-metric GstMessage
    → GstPipeline bus
    → metrics collector
```

Elements post the messages defined by this specification. The collector receives one event stream and does not read SoC-specific interfaces.

Vendors modify GStreamer elements that own or observe hardware state.

## 2. Scope

### In scope

- The custom GStreamer bus messages required from SoC elements.
- Exact field names, GTypes, units, and lifecycle behavior.
- Element-to-media-source association.
- Individual occurrences and episode boundaries.
- Coherent cumulative rendered and dropped video frame counters.
- Collector responsibilities.
- Conformance requirements.

### Out of scope

- Use or transport of metrics after the pipeline collector receives them.
- Measurements made by components downstream of the SoC media pipeline.

## 3. Design principles

1. Elements post **messages** to `GstBus`; GStreamer events remain pad-flow control and are not the metrics transport.
2. Every frame drop or decode error is represented by one occurrence message.
3. Every episode metric uses a started/resolved message pair.
4. `GST_MESSAGE_SRC(message)` identifies the concrete producing element. The collector maps it to the corresponding audio or video stream.
5. Every defined metric type is mandatory for a conforming SoC media HAL; no metric-capability list is exposed.
6. Optional PTS carries the best available stream position under the semantics of its observation kind; zero remains a valid PTS.
7. Messages use ordinary queued `GstBus` delivery. Posting must not block decode or presentation.
8. `GST_BUS_ASYNC` is not the delivery mode: a bus sync handler must not return `GST_BUS_ASYNC` for metric messages. If a sync handler is installed, it permits these messages to enter the normal bus queue by returning `GST_BUS_PASS`.
9. Messages are ordered only per producing element; cross-element ordering is not implied.

## 4. Pipeline association

The collector is created for each GStreamer pipeline. It registers every producing element against the corresponding audio or video stream.

Registration is keyed by GStreamer object identity, not element name or factory name. The collector resolves `GST_MESSAGE_SRC(message)` to that registration. The metric type defines the observation semantics; the message does not expose a decoder-versus-renderer classification.

Messages from an unregistered source are ignored and diagnosed. Messages received after registration has been removed are ignored.

## 5. Occurrence metric requirement

Every frame drop or decode error is posted as one `OCCURRENCE` message. An occurrence message does not carry a count.

Each observed occurrence produces exactly one message. How an SoC implementation converts native callbacks or cumulative counters into individual messages is outside the normative contract. Appendix B gives non-normative implementation guidance.

## 6. Custom message envelope

Every observation is a `GST_MESSAGE_ELEMENT` whose `GstStructure` name is:

```text
media-pipeline-metric
```

This is a platform media-pipeline contract. Any compatible pipeline collector may consume it.

The producer calls `gst_element_post_message()` from the observing element. A successful call means that GStreamer accepted the message for ordinary queued bus delivery; it does not mean that the collector handled it before the call returned. Collection runs outside the posting thread. A `GstBusSyncHandler`, if present for unrelated pipeline needs, returns `GST_BUS_PASS` for `media-pipeline-metric` messages without performing metric work. It must not return `GST_BUS_ASYNC`.

### 6.1 Required fields on every message

| Field | GType | Meaning |
|---|---|---|
| `metric` | `G_TYPE_UINT` | `MediaMetricType` value. |
| `observation-kind` | `G_TYPE_UINT` | `MediaMetricObservationKind` value. |

`pts-ns` is an optional `G_TYPE_INT64` field containing the PTS of the affected frame or sample when known. When the exact PTS is unknown, it contains the current stream position at detection. It is omitted when unavailable; zero is a valid PTS and is not an unavailable sentinel. Section 7 defines its meaning for each observation kind.

Unknown fields are ignored. An unknown metric or observation kind causes the message to be rejected without affecting playback. Additive evolution uses new fields or enum values. An incompatible future contract uses a different `GstStructure` name.

### 6.2 Metric enum

```cpp
enum MediaMetricType : uint32_t
{
    MEDIA_METRIC_VIDEO_FRAME_DROP = 1,
    MEDIA_METRIC_AUDIO_FRAME_DROP = 2,
    MEDIA_METRIC_VIDEO_DECODE_ERROR = 3,
    MEDIA_METRIC_AUDIO_DECODE_ERROR = 4,
    MEDIA_METRIC_VIDEO_FRAME_REPEAT = 5,
    MEDIA_METRIC_AUDIO_GAP = 6,
    MEDIA_METRIC_BUFFER_UNDERFLOW = 7,
};
```

Values are append-only and must not be renumbered.

### 6.3 Observation-kind enum

```cpp
enum MediaMetricObservationKind : uint32_t
{
    MEDIA_METRIC_OBSERVATION_OCCURRENCE = 1,
    MEDIA_METRIC_OBSERVATION_EPISODE_STARTED = 2,
    MEDIA_METRIC_OBSERVATION_EPISODE_RESOLVED = 3,
};
```

## 7. Fields by observation kind

### 7.1 Occurrence

An occurrence message has no additional required fields. It represents exactly one frame drop or decode error. When present, `pts-ns` is the affected frame or sample PTS when known; otherwise it is the best available current stream PTS at detection.

### 7.2 Episode started

No `count` or `duration-ns` field is present. When present, `pts-ns` is the PTS at which the episode started.

There may be only one active episode for a metric, source, and producing element. A duplicate start is non-conformant.

### 7.3 Episode resolved

Required field:

| Field | GType | Meaning |
|---|---|---|
| `duration-ns` | `G_TYPE_UINT64` | Complete episode duration in nanoseconds. |

Video-repeat and audio-gap resolutions additionally require `count`, the total affected frames or samples during the completed episode. Buffer-underflow resolution does not carry `count`. When present, `pts-ns` repeats the episode-start PTS so resolution is self-describing if the start message was lost. A collector may accept a resolved message without receiving the corresponding start.

## 8. Metric-specific requirements

### 8.1 Video frame drop

Required form: `OCCURRENCE`.

The metric contains all pipeline-observable video frames dropped before capture handoff or SoC presentation. The producer integration may use one authoritative aggregate or several demonstrably disjoint producers. Failures during content parsing or header processing that produce no frame are decode errors, not frame drops.

A producer reports only drops observed within the SoC media pipeline. Drops after frame handoff are outside this specification.

### 8.2 Audio frame drop

Required form: `OCCURRENCE`.

The metric contains decoded audio frames discarded before output and any audio frames intentionally skipped by the decoder under load. Content-authored silence is not a drop. A decode failure that produces no sample is a decode error.

### 8.3 Video decode error

Required form: `OCCURRENCE`.

The source is the element owning the hardware video decoder or a combined element owning the complete decode path. The metric counts frame-level failures. Fatal decoder failure behavior is specified separately from this metrics contract.

### 8.4 Audio decode error

Required form: `OCCURRENCE`.

The source is the element owning the hardware audio decoder or a combined element owning the complete decode path.

### 8.5 Video frame repeat episode

Required forms: `EPISODE_STARTED` and `EPISODE_RESOLVED`.

The producer controls video output and reused the preceding frame because the next frame was unavailable. Structured-cadence repeats such as 3:2 or 2:2 pulldown are excluded.

The HAL implements this metric for pipeline topologies in which the SoC renderer controls final video output. No repeat event is expected for repetition occurring after a Video Frame Capture handoff because that downstream renderer is outside the SoC media pipeline; this topology qualification is not an unsupported capability.

### 8.6 Audio gap episode

Required forms: `EPISODE_STARTED` and `EPISODE_RESOLVED`.

The audio-output element posts start when it emits its first null/substitute frame in place of expected decoded output. It posts resolved when the first real frame is emitted after the substitution run.

Content-authored silence is excluded. Gap PTS/duration properties supplied by upstream code are not evidence that the output element observed a gap.

### 8.7 Buffer-underflow episode

Required forms: `EPISODE_STARTED` and `EPISODE_RESOLVED`.

The message source owns the affected audio or video decoder. Underflow means that decoder lacked required input data. Pipeline queues and renderer elements do not post this metric.

The resolved message supplies total duration and no count. A one-shot signal without a recovery observation cannot provide an exact episode duration.

Buffer-underflow episode observation is mandatory. The decoder-owning producer emits the pair whenever the defined starvation condition occurs.

## 9. Authoritative message producers

`GST_MESSAGE_SRC(message)` is part of the contract. A metric is posted by the element that owns the observation, not an arbitrary pipeline bin. No decoder-versus-renderer field is carried.

| Metric | Authoritative producer |
|---|---|
| Video frame drop | One element owning the aggregate video-drop observation, or several elements reporting disjoint occurrences |
| Audio frame drop | One element owning the aggregate audio-drop observation, or several elements reporting disjoint occurrences |
| Video decode error | Element owning the hardware video decoder |
| Audio decode error | Element owning the hardware audio decoder |
| Video frame repeat | Element controlling video output selection |
| Audio gap | Element emitting null/substitute audio output |
| Video buffer underflow | Element owning the affected video decoder |
| Audio buffer underflow | Element owning the affected audio decoder |

A combined vendor element may post several metric types because it implements several internal functions. It does not expose those internal stages in the message schema.

For frame drops, the integration selects one model for the lifetime of a source:

1. one authoritative element posts an aggregate covering all pipeline-observable drops; or
2. several elements post only demonstrably disjoint occurrences.

If a combined element posts an aggregate observation stream, child elements do not additionally post the same occurrences.

A forwarding bin may relay only when a child cannot post directly. The integration registers that forwarding source as authoritative for the metric and media source.

Non-conformant behavior includes:

- a pipeline-wide element posting without authoritative source association;
- a parser reporting a decode error merely because it rejected malformed input;
- a renderer duplicating decoder-owned drops inferred from PTS gaps;
- an upstream element reporting audio gap from a discontinuity hint;
- a queue or renderer reporting decoder underflow; and
- two elements posting the same occurrence.

## 10. Mandatory support and client selection

A conforming SoC media HAL provides authoritative producers for all seven `MediaMetricType` values. There is no `media-metrics-capabilities` property, custom capability query or format/pacing-qualified metric list.

Mandatory support means the integration can observe and emit the specified message whenever the condition occurs within the active SoC pipeline topology. It does not require an event for a condition that cannot occur or is outside that topology. For example, repetition performed by NRDP after a Video Frame Capture handoff is downstream and produces no SoC video-repeat message.

The Rialto client API defaults to an empty metric subscription and permits runtime replacement by type. How RialtoServer realizes that selection is outside this producer-message contract: it may privately control native observation, filter bus messages in the collector, or filter normalized events before IPC. A vendor element is not required to expose public enable properties. Regardless of the selected implementation, enabling any defined type must not fail because the SoC lacks observation support.

## 11. Cumulative video frame counters

For a direct SoC-rendered video path, the element owning final scheduled video output exposes a read-only `stats` property:

```text
Property: stats
GType:    GST_TYPE_STRUCTURE
Access:   readable
```

Each successful read returns one coherent structure containing at least:

```text
rendered:  G_TYPE_UINT64
dropped:   G_TYPE_UINT64
corrupted: G_TYPE_UINT64
```

`rendered` counts distinct presentation-eligible video frames actually presented by the SoC renderer. It increments when presentation is committed at the same output point that owns first-frame presentation. It excludes codec-hidden/non-display pictures, repeated display refreshes of one frame, decoded reference pictures never output, and preroll frames never displayed.

`dropped` counts distinct presentation-eligible video frames irreversibly discarded before presentation. It includes predecode lateness/load-shedding drops, decoded frames discarded before output, and renderer/sink drops caused by missed presentation deadlines. One frame increments `dropped` at most once. A vendor may assemble this aggregate from disjoint decoder and renderer observations, but the property is the single authoritative result and must not double-count a frame observed at several stages.

`corrupted` counts distinct frames for which the authoritative decoder reports a frame-level decode failure or corruption condition. Multiple native error indications associated with one frame increment it once. Corruption is a classification rather than a presentation outcome: a corrupted frame may also increment `rendered` when concealed output is presented or `dropped` when no output is presented. `corrupted` is not added to `rendered + dropped` when deriving W3C total video frames.

Frames intentionally removed by seek or flush, frames belonging only to a superseded seek target, and frames suppressed because their position is below an active `discardUntilPosition()` boundary do not falsely increment the counters. A paused frame actually presented through explicit frame rendering increments `rendered` once. Structured cadence and renderer repeats do not create another rendered frame.

All three counters start at zero when a new source presentation path is created and are monotonically non-decreasing until that path is destroyed. Seek, discard, pause/play, underflow, playback-speed changes, presentation-timeline changes, decoder recreation within the same source path and EOS do not reset them. During seek or discard, reads remain valid and return the accumulated snapshot; counters advance only for genuine presentation outcomes.

The snapshot is independent of client metric subscription and message delivery. For a loss-free controlled interval, the increase in `dropped` equals the number of `MEDIA_METRIC_VIDEO_FRAME_DROP` occurrences generated for the same source and interval, and the increase in `corrupted` equals the number of `MEDIA_METRIC_VIDEO_DECODE_ERROR` occurrences. No per-rendered-frame message is required.

For Video Frame Capture, the application/NRDP renderer owns final presentation after handoff. The SoC `stats` property is not an authoritative W3C displayed-frame count for that topology; Rialto reports the public frame-counter query as unavailable and the downstream renderer maintains its own counters.

## 12. Collector requirements

The pipeline-scoped collector:

1. includes `GST_MESSAGE_ELEMENT` in its bus-dispatch selection and consumes queued `media-pipeline-metric` messages on a dedicated dispatcher context, not in the posting thread;
2. validates each message and maps message-source object identity to the corresponding audio or video stream;
3. verifies that the source is currently registered and authoritative for that metric and media source;
4. timestamps accepted observations using Rialto's monotonic clock;
5. dispatches one normalized notification for each occurrence;
6. tracks episode state and accepts self-describing resolutions;
7. prevents overlap between aggregate and child producers;
8. clears accumulated client-facing totals and active episodes on source removal; and
9. dispatches normalized metric notifications on the collector's execution context.

The collector performs no SoC-specific polling, native counter differencing, counter-wrap interpretation, or decoder-versus-renderer classification.

## 13. Conformance requirements

A vendor implementation is conformant only when:

- every defined frame-drop or decode-error metric emits one `OCCURRENCE` message per observed occurrence in its applicable pipeline topology;
- every defined episode metric emits started/resolved messages whenever its condition occurs in its applicable topology;
- occurrence messages do not carry `count`;
- all required and present optional fields have exact GTypes and units;
- native cumulative reset/wrap does not create false occurrences;
- each message source is authoritative for the metric and media source;
- metric messages use ordinary queued bus delivery: a sync handler does no metric work, returns `GST_BUS_PASS`, and never returns `GST_BUS_ASYNC` for them;
- posting does not wait for collector handling or otherwise block streaming threads beyond normal bus queue insertion;
- flush, seek, source replacement, and decoder recreation do not create false occurrences;
- repeats exclude structured cadence;
- audio gaps exclude content silence;
- underflow is decoder-specific;
- producers do not report behavior in downstream components; and
- malformed or unsupported messages can be ignored without affecting playback;
- direct SoC-rendered video exposes coherent `rendered`, aggregate `dropped` and frame-deduplicated `corrupted` `G_TYPE_UINT64` counters through the output element's read-only `stats` property;
- cumulative `corrupted` and `MEDIA_METRIC_VIDEO_DECODE_ERROR` use the same authoritative frame-level observations;
- submitted, decoder-processed, codec-hidden, repeated, seek-flushed and discard-suppressed frames do not falsely increment those counters; and
- counter values remain monotonic across seek, discard and normal state transitions for the source-path lifetime.

Required tests cover:

1. every defined metric type in each applicable SoC pipeline topology;
2. individual occurrence messages and expansion of native batched observations;
3. complete episodes and self-describing resolutions;
4. duplicate starts and mismatched resolutions;
5. omitted, zero-valued, exact, episode-start, and current-position PTS;
6. unknown enums and incorrect GTypes;
7. unregistered message sources;
8. source removal with messages in flight;
9. ordinary queue delivery through the collector's `GST_MESSAGE_ELEMENT` dispatcher selection, including verification that the posting thread does not wait for handling and a sync handler returns `GST_BUS_PASS` rather than `GST_BUS_ASYNC`;
10. aggregate-versus-child duplicate prevention;
11. coherent rendered/dropped/corrupted reads and `rendered + dropped` W3C totals;
12. codec-hidden/non-display pictures excluded from presentation totals;
13. predecode and missed-deadline drops included once without cross-stage duplication;
14. multiple native error indications for one frame increment `corrupted` once;
15. corrupted-and-rendered and corrupted-and-dropped overlap cases;
16. no false increments or resets across seek, superseding seek, flush and discard; and
17. Video Frame Capture reported as unavailable because final presentation occurs downstream.

## 14. Open decisions

1. Whether the structures and enums should be published as a small C header shared with vendor plugins.
2. Maximum permitted polling interval for native counters.

---

# Appendix A — Normative message catalogue

This appendix is the complete producer checklist. It is normative, not illustrative.

## A.1 Common envelope

Every message is posted as a `GST_MESSAGE_ELEMENT` with this structure:

```text
media-pipeline-metric {
    metric:             G_TYPE_UINT = <MediaMetricType>,
    observation-kind:   G_TYPE_UINT = <MediaMetricObservationKind>,
    pts-ns:             G_TYPE_INT64 = <best available stream PTS>  [optional]
}
```

The source passed to `gst_message_new_element()` is the authoritative producing element. The producer checks the return from `gst_element_post_message()` and diagnoses a failed post locally. Successfully posted messages are the delivery boundary; the schema does not add sequence or acknowledgement state that could detect but not recover a message discarded during bus flushing or teardown.

## A.2 Video frame drop occurrences

```text
media-pipeline-metric {
    <common envelope>,
    metric:             MEDIA_METRIC_VIDEO_FRAME_DROP,
    observation-kind:   MEDIA_METRIC_OBSERVATION_OCCURRENCE
}
```

Use the affected-frame PTS when known; otherwise use the best available current stream PTS or omit `pts-ns`.

## A.3 Audio frame drop occurrences

```text
media-pipeline-metric {
    <common envelope>,
    metric:             MEDIA_METRIC_AUDIO_FRAME_DROP,
    observation-kind:   MEDIA_METRIC_OBSERVATION_OCCURRENCE
}
```

Content-authored silence does not produce this message.

## A.4 Video decode-error occurrences

```text
media-pipeline-metric {
    <common envelope>,
    metric:             MEDIA_METRIC_VIDEO_DECODE_ERROR,
    observation-kind:   MEDIA_METRIC_OBSERVATION_OCCURRENCE
}
```

The source owns the video decoder. Fatal decoder failure behavior is outside this metrics contract.

## A.5 Audio decode-error occurrences

```text
media-pipeline-metric {
    <common envelope>,
    metric:             MEDIA_METRIC_AUDIO_DECODE_ERROR,
    observation-kind:   MEDIA_METRIC_OBSERVATION_OCCURRENCE
}
```

The source owns the audio decoder.

## A.6 Video frame-repeat episode

### A.6.1 Started

```text
media-pipeline-metric {
    <common envelope>,
    metric:             MEDIA_METRIC_VIDEO_FRAME_REPEAT,
    observation-kind:   MEDIA_METRIC_OBSERVATION_EPISODE_STARTED,
    pts-ns:             G_TYPE_INT64 = <PTS of first unready repeat>  [optional]
}
```

No `count` or `duration-ns` is present.

### A.6.2 Resolved

```text
media-pipeline-metric {
    <common envelope>,
    metric:             MEDIA_METRIC_VIDEO_FRAME_REPEAT,
    observation-kind:   MEDIA_METRIC_OBSERVATION_EPISODE_RESOLVED,
    pts-ns:             G_TYPE_INT64 = <same PTS as started>  [optional],
    count:              G_TYPE_UINT64 = <total unready repeats, greater than zero>,
    duration-ns:        G_TYPE_UINT64 = <complete episode duration>
}
```

A single-repeat episode still produces both messages. Structured-cadence repeats produce neither.

## A.7 Audio-gap episode

### A.7.1 Started

```text
media-pipeline-metric {
    <common envelope>,
    metric:             MEDIA_METRIC_AUDIO_GAP,
    observation-kind:   MEDIA_METRIC_OBSERVATION_EPISODE_STARTED,
    pts-ns:             G_TYPE_INT64 = <PTS of first substituted audio frame>  [optional]
}
```

No `count` or `duration-ns` is present.

### A.7.2 Resolved

```text
media-pipeline-metric {
    <common envelope>,
    metric:             MEDIA_METRIC_AUDIO_GAP,
    observation-kind:   MEDIA_METRIC_OBSERVATION_EPISODE_RESOLVED,
    pts-ns:             G_TYPE_INT64 = <same PTS as started>  [optional],
    count:              G_TYPE_UINT64 = <substituted audio frames, greater than zero>,
    duration-ns:        G_TYPE_UINT64 = <complete episode duration>
}
```

Content-authored silence produces neither message.

## A.8 Buffer-underflow episode

### A.8.1 Started

```text
media-pipeline-metric {
    <common envelope>,
    metric:             MEDIA_METRIC_BUFFER_UNDERFLOW,
    observation-kind:   MEDIA_METRIC_OBSERVATION_EPISODE_STARTED,
    pts-ns:             G_TYPE_INT64 = <best available decoder stream PTS>  [optional]
}
```

The source owns the affected audio or video decoder.

### A.8.2 Resolved

```text
media-pipeline-metric {
    <common envelope>,
    metric:             MEDIA_METRIC_BUFFER_UNDERFLOW,
    observation-kind:   MEDIA_METRIC_OBSERVATION_EPISODE_RESOLVED,
    pts-ns:             G_TYPE_INT64 = <same PTS as started>  [optional],
    duration-ns:        G_TYPE_UINT64 = <complete episode duration>
}
```

No `count` field is present.

## A.9 Complete mandatory producer matrix

| Metric | Mandatory messages in applicable topology |
|---|---|
| `MEDIA_METRIC_VIDEO_FRAME_DROP` | Video frame-drop `OCCURRENCE` |
| `MEDIA_METRIC_AUDIO_FRAME_DROP` | Audio frame-drop `OCCURRENCE` |
| `MEDIA_METRIC_VIDEO_DECODE_ERROR` | Video decode-error `OCCURRENCE` |
| `MEDIA_METRIC_AUDIO_DECODE_ERROR` | Audio decode-error `OCCURRENCE` |
| `MEDIA_METRIC_VIDEO_FRAME_REPEAT` | Repeat started and resolved |
| `MEDIA_METRIC_AUDIO_GAP` | Audio gap started and resolved |
| `MEDIA_METRIC_BUFFER_UNDERFLOW` | Underflow started and resolved |

Every row is required by the HAL contract. Additive custom metrics require a new governed enum value and corresponding normative message definition; they are not advertised dynamically through a capability property.

---

# Appendix B — Adapting native metric sources

This appendix is non-normative. It shows how an SoC implementation might produce the required `OCCURRENCE` messages from common native interfaces. The normative output is defined by Section 5 and Appendix A.

## B.1 One callback per occurrence

```text
native callback reports one occurrence
    → post one OCCURRENCE
```

No additional counting state is required.

## B.2 Batched native callback

```text
native callback reports N new occurrences
    → post N OCCURRENCE messages
```

The expansion keeps native batching private to the SoC implementation. Each posted message has the best stream PTS available for that occurrence; when the native source supplies only one PTS for the batch, each expanded message may carry that same PTS.

## B.3 Cumulative native counter

```text
read current native counter
    → compare with the previous native value
    → handle native reset or wrap
    → calculate the positive difference N
    → post N OCCURRENCE messages
```

The element stores only the state needed to calculate new occurrences. It does not maintain totals for users of the metric stream. An unchanged counter produces no message.

The SoC implementation is best placed to perform this conversion because it knows:

- which native API supplies the counter;
- when and how often the counter may be read safely;
- when the counter resets;
- the counter width and wrap behavior;
- whether native counters overlap; and
- which native causes belong to each metric.

## B.4 Polling

If the hardware provides only a cumulative counter, the SoC element may poll it. The polling interval must be short enough to meet the agreed reporting requirement and must not interfere with decoding or presentation. The element expands every positive difference into individual occurrence messages before posting them.
