# SoC Media Metrics GStreamer Specification

**Status:** Draft for review.

**Specification version:** 0.5

## Revision history

| Version | Status | What changed | What reviewers should check |
|---|---|---|---|
| 0.5 | Draft for review | Completed episode lifecycle rules, decoder-underflow recovery, overlapping episode behavior, deliberate-drop exclusions, direct bus delivery, and event/counter consistency. | Episode boundaries and restart, final resolutions before producer-path teardown, native decoder/output conditions, overlapping metrics, and counter alignment. |
| 0.4 | Draft for review | Added exact cumulative corrupted-frame count to the video frame snapshot already introduced in 0.3. | Corruption identity/deduplication, overlap with presentation outcomes, event consistency, and transport. |
| 0.3 | Draft for review | Added exact cumulative rendered and dropped video-frame counters for direct SoC-rendered video. | Counter property, presentation-point ownership, hidden-frame exclusions, deliberate-removal behavior, and conformance tests. |
| 0.2 | Draft for review | Made all seven defined metrics part of the required SoC contract and removed per-platform metric capability reporting. | Required metric coverage, topology limits, buffer-underflow reporting, and conformance tests. |
| 0.1 | Initial draft | Introduced the GStreamer message format, metric meanings, producer ownership, and conformance rules. | Complete specification. |

### What changed in version 0.5

- Defined natural and producer-scoped lifecycle-boundary resolution for every episode metric, with monotonic elapsed duration.
- Required final episode resolutions before an affected producer path stops or is removed.
- Defined video-repeat resolution at first presentation of a new real frame, not frame selection.
- Kept underflow decoder-scoped, with immediate start and input-recovery resolution.
- Allowed underflow, video-repeat, and audio-gap episodes to overlap independently.
- Excluded deliberate decoder/output control and pacing removals from drop events and counters.
- Aligned video-drop events with `dropped` and video decode-error events with `corrupted`.
- Clarified that each authoritative SoC element posts the specified messages directly to its pipeline bus.

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
    → element's GstPipeline bus
```

Vendors modify the GStreamer elements that own or observe hardware state. Each element posts the specified messages to its pipeline bus.

## 2. Scope

### In scope

- The custom GStreamer bus messages required from SoC elements.
- Exact field names, GTypes, units, and lifecycle behavior.
- Authoritative producer ownership for each metric.
- Individual occurrences and episode boundaries.
- Coherent cumulative rendered, dropped, and corrupted video frame counters.
- Mandatory production of all seven metric types wherever their conditions can occur in the active SoC pipeline topology.
- Conformance requirements for SoC message and counter production.

### Out of scope

- Use of metric messages after they have been accepted by the pipeline bus.
- Measurements made by components downstream of the SoC media pipeline.

## 3. Design principles

1. Elements post **messages** to `GstBus`; GStreamer events remain pad-flow control and are not the metrics transport.
2. Every frame drop or decode error is represented by one occurrence message.
3. Every episode metric uses a started/resolved message pair.
4. `GST_MESSAGE_SRC(message)` is the concrete authoritative producing element.
5. Every defined metric type is mandatory for a conforming SoC media HAL; no metric-capability list is exposed.
6. Optional PTS carries the best available stream position under the semantics of its observation kind; zero remains a valid PTS.
7. Messages use ordinary queued `GstBus` delivery. Posting must not block decode or presentation.
8. Messages from one producing element are posted in observation order; no ordering is required between different producers.
9. Before an affected producer path stops or is removed, it posts the final boundary resolutions required by Sections 6 and 7 and then posts no further observations for that producer instance.

## 4. Pipeline delivery

Each authoritative SoC element posts its metric messages to the `GstBus` of the pipeline containing that element. `GST_MESSAGE_SRC(message)` identifies that producing element. The metric type and the producer-ownership rules in Section 8 identify the authoritative observation point; the message does not expose a separate decoder-versus-renderer classification.

Each authoritative producer:

1. posts each occurrence exactly once and does not duplicate one observation between aggregate and child elements;
2. maintains episode state independently for each metric it produces, including overlapping metrics;
3. posts one start and one natural or boundary resolution for each episode;
4. posts messages from that producer in observation order;
5. posts required final boundary resolutions before its affected path stops or is removed;
6. posts no later observations from that producer instance after its path has stopped or been removed;
7. uses non-blocking ordinary queued bus delivery; and
8. diagnoses a failed `gst_element_post_message()` locally without affecting playback.

The SoC implementation performs any native polling, callback adaptation, cumulative-counter differencing, counter-wrap handling, and decoder-versus-renderer selection needed to produce this stream. Appendix A gives non-normative adaptation examples.

No fixed polling interval is mandated. A polling implementation chooses an interval that cannot lose occurrences through native counter wrap or reset, posts every positive counter delta, and posts final observations before the lifecycle boundaries in Section 6.4. Polling must not block decode or presentation. The vendor documents the chosen interval and the native counter range/reset behavior used to justify it.

## 5. Custom message envelope

Every observation is a `GST_MESSAGE_ELEMENT` whose `GstStructure` name is:

```text
media-pipeline-metric
```

The producer calls `gst_element_post_message()` from the observing element. A successful call means that GStreamer accepted the message for ordinary queued bus delivery. Because `gst_element_post_message()` consumes the `GstMessage` on every call, including failure, any retry constructs a fresh message. Message consumption after successful posting is outside this specification.

### 5.1 Required fields on every message

| Field | GType | Meaning |
|---|---|---|
| `metric` | `G_TYPE_UINT` | `MediaMetricType` value. |
| `observation-kind` | `G_TYPE_UINT` | `MediaMetricObservationKind` value. |

`pts-ns` is an optional `G_TYPE_INT64` field containing the PTS of the affected frame or sample when known. When the exact PTS is unknown, it contains the current stream position at detection. It is omitted when unavailable; zero is a valid PTS and is not an unavailable sentinel. Section 6 defines its meaning for each observation kind.

Unknown fields are ignored. An unknown metric or observation kind causes the message to be rejected without affecting playback. Additive evolution uses new fields or enum values. An incompatible future contract uses a different `GstStructure` name.

### 5.2 Metric enum

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

This specification is the authoritative definition of the structure name, field names, GTypes, enum values, and units. A separate shared C header is not required for conformance. A platform may publish one for build-time convenience, but it must reproduce these definitions exactly and cannot change the wire contract.

### 5.3 Observation-kind enum

```cpp
enum MediaMetricObservationKind : uint32_t
{
    MEDIA_METRIC_OBSERVATION_OCCURRENCE = 1,
    MEDIA_METRIC_OBSERVATION_EPISODE_STARTED = 2,
    MEDIA_METRIC_OBSERVATION_EPISODE_RESOLVED = 3,
};
```

## 6. Fields by observation kind

### 6.1 Occurrence

An occurrence message has no additional required fields. It represents exactly one frame drop or decode error. When present, `pts-ns` is the affected frame or sample PTS when known; otherwise it is the best available current stream PTS at detection.

### 6.2 Episode started

No `count` or `duration-ns` field is present. When present, `pts-ns` is the PTS at which the episode started.

There may be only one active episode for a metric, source, and producing element. A duplicate start is non-conformant.

### 6.3 Episode resolved

Required field:

| Field | GType | Meaning |
|---|---|---|
| `duration-ns` | `G_TYPE_UINT64` | Monotonic elapsed time from episode start observation to natural or boundary resolution, in nanoseconds. |

The clock domain is GStreamer's monotonic timestamp returned by `gst_util_get_timestamp()` (nanoseconds, based on the platform monotonic clock), not pipeline running time, PTS, or STC. The producer records this timestamp at episode start and resolution and reports their non-negative difference. An episode that remains active while media PTS/STC is stationary therefore continues accumulating duration.

Video-repeat and audio-gap resolutions additionally require:

| Field | GType | Meaning |
|---|---|---|
| `count` | `G_TYPE_UINT64` | A value greater than zero. For video repeat, the number of unintended repeated video-frame presentations in the episode. For audio gap, the number of substituted audio frames emitted in the episode. |

`count` is measured in frames for both metrics, not audio samples. Buffer-underflow resolution does not carry `count`. When present, `pts-ns` repeats the episode-start PTS so the resolution message is self-describing independently of receipt of the corresponding start message.

### 6.4 Common episode lifecycle

Only one episode may be active for one metric, media source, and producing element. Different metric types are independent and may be active at the same time.

An episode ends by natural recovery or when its authoritative producer's observation epoch ends. A system transition resolves only the episodes owned by producers whose function is stopped, reset, invalidated, reconfigured, or removed. An unaffected producer retains its active episode.

| Condition | Decoder-owned underflow | Video-output repeat | Audio-output gap |
|---|---|---|---|
| Scheduled output pauses | Retain if the decoder still requests unavailable input; resolve only if decoder demand/epoch also ends | Resolve | Resolve |
| Decoder stops requesting input | Resolve | No effect unless video output is also invalidated | No effect unless audio output is also invalidated |
| EOS accepted by the affected path | Resolve when decoder demand ends | Resolve for video output | Resolve for audio output |
| Decoder flush/reset or decode-position invalidation | Resolve the affected decoder epoch | Resolve only if video-output selection is invalidated | Resolve only if audio-output substitution state is invalidated |
| Source/codec reconfiguration | Resolve each affected producer's old epoch | Resolve if video output is affected | Resolve if audio output is affected |
| Source removal or producer-path teardown | Resolve | Resolve | Resolve |

For a boundary resolution, `duration-ns` ends when that producer epoch ends. Repeat and gap `count` includes only affected outputs before that boundary. Optional `pts-ns` remains the episode-start PTS. A resolution means the episode ended; it does not necessarily mean natural recovery.

The next epoch for that producer is armed independently and may start immediately on its first qualifying observation. The transition itself does not start an episode unless the new epoch meets the metric's start condition.

For source removal or producer-path teardown, the producer posts every required final boundary resolution before that affected path stops, then posts no later metric observation from that producer instance.

### 6.5 Cross-metric overlap

Episode metrics describe different observations and may overlap. One decoder starvation may produce buffer underflow and also video repeat or audio gap. Each qualifying metric is reported independently; there is no precedence or mutual exclusion. Duplicate starts remain prohibited for the same metric, source, and producer.

## 7. Metric-specific requirements

### 7.1 Video frame drop

Required form: `OCCURRENCE`.

The metric contains unintended quality-loss removal of presentation-eligible video frames inside the SoC pipeline, including load shedding and missed presentation deadlines. The producer integration may use one authoritative aggregate or several demonstrably disjoint producers.

Do not report frames deliberately removed for decoder flush/reset cleanup, establishment of a new decode position, source/codec reconfiguration, EOS cleanup, or configured non-unity pacing. A frame discarded before decode is not a decode error. Drops after frame handoff are outside this specification.

### 7.2 Audio frame drop

Required form: `OCCURRENCE`.

The metric contains unintended quality-loss removal of decoded audio frames before output, including decoder skipping under load. It excludes content-authored silence and frames deliberately removed for decoder flush/reset cleanup, establishment of a new decode position, normal source/codec reconfiguration, EOS cleanup, or configured non-unity pacing. A decode failure that produces no sample is a decode error.

### 7.3 Video decode error

Required form: `OCCURRENCE`.

The source is the element owning the hardware video decoder or a combined element owning the complete decode path. Count every authoritative frame-level decoder failure, including failures during recovery or source/codec reconfiguration. Data deliberately discarded before decode is not a decode error. Fatal decoder failure behavior is specified separately.

### 7.4 Audio decode error

Required form: `OCCURRENCE`.

The source is the element owning the hardware audio decoder or a combined element owning the complete decode path. Count every authoritative sample/frame decode failure, including failures during recovery or source/codec reconfiguration. Data deliberately discarded before decode is not a decode error.

### 7.5 Video frame repeat episode

Required forms: `EPISODE_STARTED` and `EPISODE_RESOLVED`.

Start on the first unintended reuse of the preceding frame because the required next frame is unavailable during active scheduled output. Resolve when scheduled output first presents a new real frame. Selection alone does not resolve the episode.

Exclude structured cadence, a frame held while output is paused, the final frame held after EOS, and a frame held while decoder/output state is invalid during flush, reset, or decode-position establishment. This metric applies only when the SoC controls final video output; repetition after frame handoff is downstream.

### 7.6 Audio gap episode

Required forms: `EPISODE_STARTED` and `EPISODE_RESOLVED`.

Start when audio output emits its first null/substitute frame in place of expected real output. Resolve when the first real frame is emitted after the substitution run.

Exclude content-authored silence, paused silence, post-EOS silence, and silence emitted only while decoder/output state is invalid during flush, reset, or decode-position establishment. Substitute output during an active source/codec reconfiguration remains reportable. Upstream gap hints are not evidence that output observed a gap.

### 7.7 Buffer-underflow episode

Required forms: `EPISODE_STARTED` and `EPISODE_RESOLVED`.

The message source owns the affected audio or video decoder. Start immediately on the first observation that the decoder actively requires input and required input is unavailable; there is no minimum starvation threshold. Resolve naturally when required input is first available or accepted again. The resolved message supplies total duration and no count.

Do not start underflow when the decoder is not requesting input, after EOS, or while decoder state is invalid during flush/reset or decode-position establishment. Output pause alone does not resolve an active underflow: retain it while the same decoder continues requesting unavailable input, and resolve only when input recovers or decoder demand/epoch ends. During active source/codec reconfiguration, underflow remains reportable if the newly active decoder requests input and none is available. Pipeline queues and renderer elements do not post this metric.

## 8. Authoritative message producers

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

A forwarding bin may relay only when a child cannot post directly. That forwarding bin then becomes the authoritative message source for that metric, and the child does not also post the same observation.

Non-conformant behavior includes:

- a pipeline-wide element posting without authoritative source association;
- a parser reporting a decode error merely because it rejected malformed input;
- a renderer duplicating decoder-owned drops inferred from PTS gaps;
- an upstream element reporting audio gap from a discontinuity hint;
- a queue or renderer reporting decoder underflow; and
- two elements posting the same occurrence.

## 9. Cumulative video frame counters

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

## 10. Conformance requirements

A vendor implementation is conformant only when:

- every qualifying drop or decode failure emits one `OCCURRENCE` with the required types and units;
- every episode follows the common lifecycle and its metric-specific start/resolve rules;
- every `duration-ns` is monotonic elapsed time in one producer clock domain, never media PTS/STC delta;
- lifecycle boundaries resolve active episodes before the producer becomes inactive or is removed;
- underflow starts immediately on active decoder demand without input and resolves on input recovery;
- repeat, gap, and underflow may overlap, while duplicate starts for one episode key remain invalid;
- lifecycle boundaries resolve only episodes owned by affected producers; output-only pause does not resolve decoder underflow while decoder demand continues;
- deliberate decoder/output control and pacing removals create neither a drop occurrence nor a `dropped` increment;
- actual decoder failures during recovery or reconfiguration still create decode-error occurrences;
- each message source is authoritative and aggregate/child observations do not duplicate an occurrence;
- messages use ordinary non-blocking queued bus delivery;
- each affected producer posts required final boundary resolutions before its path stops or is removed and posts no later observations from that producer instance;
- posting failure is diagnosed locally without affecting playback;
- any polling interval is justified against native counter width, wrap/reset behavior, maximum event rate, and lifecycle-boundary delivery, with no lost occurrences;
- direct SoC-rendered video exposes coherent `rendered`, `dropped`, and frame-deduplicated `corrupted` counters;
- video-drop occurrences and `dropped` use identical frame identity and exclusions;
- video decode-error occurrences and `corrupted` use identical frame identity and deduplication; and
- counters remain monotonic for the source-path lifetime across decoder/output lifecycle changes.

Required tests cover:

1. every metric type in each applicable SoC topology and exact field types;
2. occurrence expansion from individual, batched, and cumulative native observations;
3. immediate decoder-underflow start, input-recovery resolution, and later restart;
4. repeat start on first unintended reuse, no resolution on selection alone, and resolution on first presentation of a new real frame;
5. audio-gap start on first substitute and resolution on first real frame;
6. producer-scoped boundary resolution at scheduled-output pause, EOS, decoder flush/reset, decode-position invalidation, source/codec reconfiguration, source removal, and teardown;
7. output-only pause while decoder demand continues retains the same underflow episode;
8. decoder demand stopping during pause resolves underflow, and renewed starved demand starts a new episode;
9. decoder-only reset leaves unaffected output episodes active, while output invalidation resolves them;
10. boundary duration/count cutoff and restart in each affected producer's next epoch;
11. source/codec reconfiguration that produces reportable underflow and audio gap;
12. no false episodes from paused/EOS held frames or silence, structured cadence, or invalid reset output;
13. overlapping video underflow/repeat and audio underflow/gap, with no duplicate same-metric start;
14. exact message source identity and field typing for every authoritative producer;
15. ordinary non-blocking queued bus delivery, local failed-post diagnosis, and aggregate-versus-child duplicate prevention;
16. no drop event or `dropped` increment for deliberate flush/reset, decode-position establishment, reconfiguration, EOS cleanup, or non-unity pacing removal;
17. one drop occurrence and one `dropped` increment for each qualifying quality-loss video drop;
18. actual decoder failures during recovery/reconfiguration and exclusion of pre-decoder deliberate discard;
19. decode-error/`corrupted` delta equality and frame-level deduplication;
20. coherent rendered/dropped/corrupted reads and `rendered + dropped` presentation-outcome totals;
21. codec-hidden/non-display pictures excluded from presentation totals;
22. corrupted-and-rendered and corrupted-and-dropped overlap;
23. frame-counter unavailability when final presentation occurs downstream after frame handoff;
24. required final boundary resolutions are posted before each affected producer path stops or is removed, with no later observation from that producer instance;
25. monotonic episode duration while media PTS/STC is stationary, including decoder underflow retained across output-only pause; and
26. polling at the documented interval under maximum event rate, native counter wrap/reset, and lifecycle teardown without losing occurrences.

---

# Appendix A — Adapting native metric sources

This appendix is non-normative. It shows how an SoC implementation might produce the required `OCCURRENCE` messages from common native interfaces. The normative output is defined by Sections 5–8.

## A.1 One callback per occurrence

```text
native callback reports one occurrence
    → post one OCCURRENCE
```

No additional counting state is required.

## A.2 Batched native callback

```text
native callback reports N new occurrences
    → post N OCCURRENCE messages
```

The expansion keeps native batching private to the SoC implementation. Each posted message has the best stream PTS available for that occurrence; when the native source supplies only one PTS for the batch, each expanded message may carry that same PTS.

## A.3 Cumulative native counter

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

## A.4 Polling

If the hardware provides only a cumulative counter, the SoC element may poll it. The polling interval must be short enough to meet the agreed reporting requirement and must not interfere with decoding or presentation. The element expands every positive difference into individual occurrence messages before posting them.
