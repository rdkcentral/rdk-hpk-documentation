# SoC Audio Delay GStreamer Specification

**Status:** Draft for review.

**Specification version:** 0.1

## Revision history

| Version | Status | What changed | What reviewers should check |
|---|---|---|---|
| 0.1 | Initial draft | Initial specification defining audio delay requirements. | Complete specification. |

### What changed in version 0.1

- Initial specification defining audio delay requirements.
- Generic requirements without prescribing specific GST property names or elements.
- Implementation examples provided as non-normative guidance.

---

## 1. Purpose

This specification defines the requirements for audio presentation delay. Audio presentation delay enables adjustment of audio timing for AV sync compensation, particularly useful when audio and video have different latency characteristics.

```text
Middleware control
    → SoC GStreamer element (decoder or sink)
    → audio presentation timing adjustment
```

Vendors implement audio delay through SoC-specific GStreamer element properties. This specification defines the generic requirements without prescribing specific property names or elements.

## 2. Scope

### In scope

- Generic requirements for audio presentation delay behavior.
- Units: nanoseconds, signed integer.
- State rules: valid in paused and playing states.
- Behavior: positive values delay, negative values advance, zero removes adjustment.
- Critical requirement: delay must not change the STC priming point.
- Implementation examples showing typical patterns.

### Out of scope

- Specific GST property names or element types (SoC-specific).
- Unit conversion formulas (SoC-specific).
- SoC-specific HAL interfaces or APIs.

## 3. Requirements

### 3.1 Control interface

Middleware provides a control to set and query audio presentation delay:

- **Unit:** nanoseconds (signed integer)
- **Scope:** Pipeline-level (applies to the entire audio path)
- **Default:** zero (no delay)

### 3.2 Value behavior

- **Positive values:** Audio is presented later than the original timing
- **Negative values:** Audio is presented earlier than the original timing
- **Zero:** No delay adjustment (original presentation timing)

### 3.3 State rules

The delay control is valid in the following states:

- **Paused:** Valid; delay can be set while paused
- **Playing:** Valid; delay can be set while playing
- **Other states:** Invalid; the operation must fail

### 3.4 STC priming point requirement

The implementation must satisfy the audio delay requirement without changing the STC priming point. This ensures that delay adjustments do not affect the system time clock or other timing-critical components.

### 3.5 Behavior requirements

- Delay adjustments must take effect immediately when set
- The delay value must persist across state transitions (pause/play)
- The current delay value must be queryable
- Delay must not introduce audio gaps or discontinuities beyond the intended offset

## 4. Implementation examples (non-normative)

The following examples illustrate how SoC vendors may implement audio delay. These are non-normative and provided for guidance only. The specific mechanism is SoC-specific.

### 4.1 Example: Decoder property

A SoC may implement audio delay through a property on the audio decoder element:

```text
Middleware
    → Set decoder property (e.g., audio-pts-offset)
    → Decoder applies timing offset during decode
    → Audio presentation timing adjusted
```

### 4.2 Example: Audio sink property

A SoC may implement audio delay through a property on the audio sink element:

```text
Middleware
    → Set sink property (e.g., render-delay or equivalent)
    → Sink applies delay during rendering
    → Audio presentation timing adjusted
```

## 5. Conformance

A conforming SoC implementation must:

1. Accept audio delay values in nanoseconds
2. Apply the delay without changing the STC priming point
3. Support positive, negative, and zero values
4. Allow delay to be set in paused and playing states
5. Return the current delay value when queried
6. Not prescribe specific GST property names or elements in the normative sections
