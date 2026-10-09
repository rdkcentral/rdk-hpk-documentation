# SoC Media Metrics GStreamer Specification

**Status:** Draft for review.

**Specification version:** 0.5

## Revision history

| Version | Status | What changed | What reviewers should check |
|---|---|---|---|
| 0.5 | Draft for review | Defined complete episode lifecycle, decoder-underflow recovery, cross-metric overlap, deliberate-drop exclusions, and event/counter consistency. | Episode boundaries and restart, native decoder/output conditions, overlapping metrics, and counter alignment. |
| 0.4 | Draft for review | Added exact cumulative corrupted-frame count to the video frame snapshot already introduced in 0.3. | Corruption identity/deduplication, overlap with presentation outcomes, event consistency, and transport. |
| 0.3 | Draft for review | Added exact cumulative rendered and dropped video-frame counters for direct SoC-rendered video. | Counter property, presentation-point ownership, hidden-frame exclusions, deliberate-removal behavior, and conformance tests. |
| 0.2 | Draft for review | Made all seven defined metrics part of the required SoC contract and removed per-platform metric capability reporting. | Required metric coverage, topology limits, buffer-underflow reporting, client selection, and conformance tests. |
| 0.1 | Initial draft | Introduced the GStreamer message format, metric meanings, producer ownership, collector behavior, and conformance rules. | Complete specification. |

### What changed in version 0.5

- Defined natural and lifecycle-boundary resolution for every episode metric.
- Kept underflow decoder-scoped, with immediate start and input-recovery resolution.
- Allowed underflow, video-repeat, and audio-gap episodes to overlap independently.
- Excluded deliberate decoder/output control and pacing removals from drop events and counters.
- Aligned video-drop events with `dropped` and video decode-error events with `corrupted`.

### What changed in version 0.4

- Added mandatory cumulative `corrupted` alongside `rendered` and `dropped` so exact corruption totals do not depend on message delivery.
- Defined corruption as one frame-level classification per distinct affected frame; repeated hardware indications for one frame do not increment it again.
- Clarified that corruption may overlap either rendered or dropped and is not added to the presentation-outcome total.
- Required the cumulative counter and `MEDIA_METRIC_VIDEO_DECODE_ERROR` occurrences to originate from the same authoritative observations.

### What changed in version 0.3

- Added a mandatory read-only cumulative snapshot containing distinct rendered and dropped video-frame counts for direct SoC-rendered video.
- Defined total presentation outcomes as rendered plus dropped; submitted, demuxed, decoder-processed and codec-hidden pictures are not valid substitutes.
- Excluded deliberate decoder/output removal from poor-playback counts.
- Required one coherent snapshot that remains monotonic across decoder/output lifecycle changes.
- Kept per-drop occurrence messages; the cumulative snapshot is independent of message delivery.

### What changed in version 0.2

- Every defined media metric is now required on a conforming SoC; vendors no longer advertise a supported subset.
- The `media-metrics-capabilities` property and other metric-capability queries have been removed.
- Buffer-underflow reporting is now required and uses the same started/resolved episode model as the other episode metrics.
- A metric is required only where its condition can occur inside the active SoC pipeline. Repetition performed by a downstream renderer after frame handoff is not a SoC repeat metric.
- Vendor elements expose the complete required observation stream; downstream filtering does not change the vendor contract.
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

### 7.4 Common episode lifecycle

Only one episode may be active for one metric, media source, and producing element. Different metric types are independent and may be active at the same time.

An episode ends by natural recovery or when its decoder/output observation epoch ends. The producer posts `EPISODE_RESOLVED` before it stops being authoritative when any of these boundaries occurs:

- native scheduled output enters paused operation;
- EOS is accepted by the affected decoder or output path;
- decoder flush or reset begins;
- decoder input/output is invalidated while establishing a new decode position;
- source or codec configuration is replaced; or
- the source path is removed or torn down.

For a boundary resolution, `duration-ns` ends at the boundary. Repeat and gap `count` includes only affected outputs observed before the boundary. Optional `pts-ns` remains the episode-start PTS. A resolution means that the episode ended; it does not necessarily mean natural recovery.

The next decoder/output epoch is armed independently. Its first qualifying observation may start a new episode immediately. The boundary operation itself does not start an episode unless the new epoch separately meets that metric's start condition. The producer resolves active episodes before it is unregistered; collector cleanup is a defensive fallback, not a replacement for the required resolution.

### 7.5 Cross-metric overlap

Episode metrics describe different observations and may overlap. One decoder starvation may produce buffer underflow and also video repeat or audio gap. Each qualifying metric is reported independently; there is no precedence or mutual exclusion. Duplicate starts remain prohibited for the same metric, source, and producer.

## 8. Metric-specific requirements

### 8.1 Video frame drop

Required form: `OCCURRENCE`.

The metric contains unintended quality-loss removal of presentation-eligible video frames inside the SoC pipeline, including load shedding and missed presentation deadlines. The producer integration may use one authoritative aggregate or several demonstrably disjoint producers.

Do not report frames deliberately removed for decoder flush/reset cleanup, establishment of a new decode position, source/codec reconfiguration, EOS cleanup, or configured non-unity pacing. A frame discarded before decode is not a decode error. Drops after frame handoff are outside this specification.

### 8.2 Audio frame drop

Required form: `OCCURRENCE`.

The metric contains unintended quality-loss removal of decoded audio frames before output, including decoder skipping under load. It excludes content-authored silence and frames deliberately removed for decoder flush/reset cleanup, establishment of a new decode position, normal source/codec reconfiguration, EOS cleanup, or configured non-unity pacing. A decode failure that produces no sample is a decode error.

### 8.3 Video decode error

Required form: `OCCURRENCE`.

The source is the element owning the hardware video decoder or a combined element owning the complete decode path. Count every authoritative frame-level decoder failure, including failures during recovery or source/codec reconfiguration. Data deliberately discarded before decode is not a decode error. Fatal decoder failure behavior is specified separately.

### 8.4 Audio decode error

Required form: `OCCURRENCE`.

The source is the element owning the hardware audio decoder or a combined element owning the complete decode path. Count every authoritative sample/frame decode failure, including failures during recovery or source/codec reconfiguration. Data deliberately discarded before decode is not a decode error.

### 8.5 Video frame repeat episode

Required forms: `EPISODE_STARTED` and `EPISODE_RESOLVED`.

Start on the first unintended reuse of the preceding frame because the required next frame is unavailable during active scheduled output. Resolve when scheduled output first selects/presents a new real frame.

Exclude structured cadence, a frame held while output is paused, the final frame held after EOS, and a frame held while decoder/output state is invalid during flush, reset, or decode-position establishment. This metric applies only when the SoC controls final video output; repetition after frame handoff is downstream.

### 8.6 Audio gap episode

Required forms: `EPISODE_STARTED` and `EPISODE_RESOLVED`.

Start when audio output emits its first null/substitute frame in place of expected real output. Resolve when the first real frame is emitted after the substitution run.

Exclude content-authored silence, paused silence, post-EOS silence, and silence emitted only while decoder/output state is invalid during flush, reset, or decode-position establishment. Substitute output during an active source/codec reconfiguration remains reportable. Upstream gap hints are not evidence that output observed a gap.

### 8.7 Buffer-underflow episode

Required forms: `EPISODE_STARTED` and `EPISODE_RESOLVED`.

The message source owns the affected audio or video decoder. Start immediately on the first observation that the decoder actively requires input and required input is unavailable; there is no minimum starvation threshold. Resolve naturally when required input is first available or accepted again. The resolved message supplies total duration and no count.

Do not start underflow when the decoder is not requesting input, after EOS, or while decoder state is invalid during flush/reset or decode-position establishment. During active source/codec reconfiguration, underflow remains reportable if the newly active decoder requests input and none is available. Pipeline queues and renderer elements do not post this metric.

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

## 10. Mandatory support

A conforming SoC media HAL provides authoritative producers for all seven `MediaMetricType` values. There is no metric capability property or custom capability query.

Mandatory support means the integration emits each specified message whenever its condition occurs inside the active SoC pipeline topology. A condition occurring only after frame handoff to a downstream renderer produces no SoC metric. Vendor elements provide the complete required observation stream; downstream filtering does not change this contract.

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

`dropped` uses exactly the same frame identity and inclusion/exclusion rules as `MEDIA_METRIC_VIDEO_FRAME_DROP`. It counts unintended quality-loss removal such as load shedding and missed deadlines, once per frame. Deliberate flush/reset cleanup, decode-position establishment, source/codec reconfiguration cleanup, EOS cleanup, and configured non-unity pacing do not increment it. A vendor may combine disjoint decoder and renderer observations, but must not count one frame twice.

`corrupted` counts distinct frames for which the authoritative decoder reports a frame-level decode failure or corruption condition. It uses the same frame classification as `MEDIA_METRIC_VIDEO_DECODE_ERROR`; multiple indications for one frame increment it once. A corrupted frame may also increment `rendered` when concealed output is presented or `dropped` when no output is presented. `corrupted` is not added to `rendered + dropped`, which is the total presentation-outcome count.

A frame actually presented while scheduled output is paused increments `rendered` once. Structured cadence and repeated refresh of one frame do not create another rendered frame.

All three counters start at zero when a source presentation path is created and remain monotonic until that path is destroyed. Decoder flush/reset, decode-position changes, source/codec reconfiguration, output pause/resume, underflow, configured pacing changes, decoder recreation within the same source path, and EOS do not reset them. Reads remain valid during those conditions; counters advance only for genuine presentation outcomes or decoder failures.

For a loss-free controlled interval, the increase in `dropped` equals the number of `MEDIA_METRIC_VIDEO_FRAME_DROP` occurrences for the same source and interval, and the increase in `corrupted` equals the number of `MEDIA_METRIC_VIDEO_DECODE_ERROR` occurrences. No rendered-frame message is required.

After Video Frame Capture handoff, final presentation occurs downstream. The SoC `stats` property is therefore not authoritative for displayed-frame totals in that topology, and the frame-counter interface must report unavailable.

## 12. Collector requirements

The pipeline-scoped collector:

1. includes `GST_MESSAGE_ELEMENT` in its bus-dispatch selection and consumes queued `media-pipeline-metric` messages on a dedicated dispatcher context, not in the posting thread;
2. validates each message and maps message-source object identity to the corresponding audio or video stream;
3. verifies that the source is currently registered and authoritative for that metric and media source;
4. timestamps accepted observations using the collector's monotonic clock;
5. dispatches one normalized notification for each occurrence;
6. tracks episode state independently for each metric, source, and producer, including overlapping metrics;
7. accepts natural and boundary resolutions and rejects duplicate starts for the same episode key;
8. prevents duplicate observations between aggregate and child producers;
9. expects producers to resolve active episodes before unregister/removal, then clears any residual state defensively without synthesizing another resolution; and
10. dispatches normalized notifications on the collector's execution context.

The collector performs no SoC-specific polling, native counter differencing, counter-wrap interpretation, or decoder-versus-renderer classification.

## 13. Conformance requirements

A vendor implementation is conformant only when:

- every qualifying drop or decode failure emits one `OCCURRENCE` with the required types and units;
- every episode follows the common lifecycle and its metric-specific start/resolve rules;
- lifecycle boundaries resolve active episodes before the producer becomes inactive or is removed;
- underflow starts immediately on active decoder demand without input and resolves on input recovery;
- repeat, gap, and underflow may overlap, while duplicate starts for one episode key remain invalid;
- paused output, EOS, decoder flush/reset, and invalid decode-position state do not create false episodes;
- deliberate decoder/output control and pacing removals create neither a drop occurrence nor a `dropped` increment;
- actual decoder failures during recovery or reconfiguration still create decode-error occurrences;
- each message source is authoritative and aggregate/child observations do not duplicate an occurrence;
- messages use ordinary non-blocking queued bus delivery;
- malformed or unsupported messages can be ignored without affecting playback;
- direct SoC-rendered video exposes coherent `rendered`, `dropped`, and frame-deduplicated `corrupted` counters;
- video-drop occurrences and `dropped` use identical frame identity and exclusions;
- video decode-error occurrences and `corrupted` use identical frame identity and deduplication; and
- counters remain monotonic for the source-path lifetime across decoder/output lifecycle changes.

Required tests cover:

1. every metric type in each applicable SoC topology and exact field types;
2. occurrence expansion from individual, batched, and cumulative native observations;
3. immediate decoder-underflow start, input-recovery resolution, and later restart;
4. repeat start on first unintended reuse and resolution on first new real frame;
5. audio-gap start on first substitute and resolution on first real frame;
6. boundary resolution at scheduled-output pause, EOS, decoder flush/reset, decode-position invalidation, source/codec reconfiguration, source removal, and teardown;
7. boundary duration/count cutoff and restart in the next decoder/output epoch;
8. source/codec reconfiguration that produces reportable underflow and audio gap;
9. no false episodes from paused/EOS held frames or silence, structured cadence, or invalid reset output;
10. overlapping video underflow/repeat and audio underflow/gap, with no duplicate same-metric start;
11. unknown enums, incorrect GTypes, unregistered sources, and messages in flight during removal;
12. ordinary non-blocking queued bus delivery and aggregate-versus-child duplicate prevention;
13. no drop event or `dropped` increment for deliberate flush/reset, decode-position establishment, reconfiguration, EOS cleanup, or non-unity pacing removal;
14. one drop occurrence and one `dropped` increment for each qualifying quality-loss video drop;
15. actual decoder failures during recovery/reconfiguration and exclusion of pre-decoder deliberate discard;
16. decode-error/`corrupted` delta equality and frame-level deduplication;
17. coherent rendered/dropped/corrupted reads and `rendered + dropped` presentation-outcome totals;
18. codec-hidden/non-display pictures excluded from presentation totals;
19. corrupted-and-rendered and corrupted-and-dropped overlap; and
20. frame-counter unavailability when final presentation occurs downstream after frame handoff.

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

Start, natural resolution, lifecycle-boundary resolution, and exclusions follow §§7.4 and 8.5.

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

Start, natural resolution, lifecycle-boundary resolution, switch behavior, and exclusions follow §§7.4 and 8.6.

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

Immediate decoder-demand start, input-recovery resolution, lifecycle-boundary resolution, and exclusions follow §§7.4 and 8.7. No debounce threshold applies.

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
