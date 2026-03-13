# LED Strip Plugin for yoRadio Fusion

Adds NeoPixel ([WS2812B](https://www.aliexpress.com/item/2036819167.html)) LED strip support to yoRadio Fusion with multiple visual effects that react to music.

---

## Hardware Requirements

| | Minimum | Recommended |
|---|---|---|
| LED count | 24 | 144 |
| LED type | WS2812B / NeoPixel (GRB) | WS2812B / NeoPixel (GRB) |
| Data pin | any free GPIO | GPIO 48 (default) |

> **Note:** LED count must be an even number. All symmetric effects split the strip at the midpoint.  
> The fire effect cooling parameters are tuned for 144 LEDs — on shorter strips the flames will be faster and taller, which still looks good.

---

## Installation

### 1. Enable the plugin

Add the following lines to your `options.h` (or board-specific config):

```cpp
#define USE_LEDSTRIP_PLUGIN
#define LEDSTRIP_PIN    48      // GPIO pin connected to LED data line
#define LEDSTRIP_COUNT  144     // total number of LEDs (must be even)
```

Without `USE_LEDSTRIP_PLUGIN` the plugin is completely excluded from the build — no overhead for users who don't need it.

### 2. Flash

Upload files in this order:

1. **SPIFFS** (web UI files: `customize.html.gz`, `script.js.gz`)
2. **Firmware**

On first boot after a firmware update the config is automatically migrated — existing settings are preserved, LED strip settings are initialized to defaults.

---

## Web UI Settings

The settings are available on the customize page.

### Customize → **strip**

The strip tab appears only when `USE_LEDSTRIP_PLUGIN` is defined.

| Control | Description |
|---|---|
| **Enable LED strip** | Master on/off switch. When off, the strip goes dark immediately |
| **Enable during screensaver** | When off, the strip goes dark while the screensaver is active. |
| **Effect** | Select the active visual effect (see below). |
| **Brightness** | Strip brightness, 0–100%. |

Click **Apply** to save brightness changes. The effect selector & chechkboxes saves immediately on change.

---

## Effects

### 0 — VU Meter *(default)*
Stereo VU meter, symmetric from center outward. Left channel on the left half, right channel on the right half.
- Color gradient: cyan → green → yellow → red (low to high level)
- Peak markers with white dot and subtle glow
- Falls back to a slow rainbow when no audio signal is present

### 1 — Rainbow Flow
A flowing rainbow that reacts to the music beat.
- The lit portion of the strip grows and shrinks with the volume
- Strong transients cause a sudden hue jump — giving a clear beat feel
- Without audio signal: full strip, slow steady rotation

### 2 — Fire
Classic heat-map fire effect, mirrored symmetrically from the center outward.
- Color palette: black → dark red → orange → yellow
- Louder music → less cooling → taller flames
- Strong beats inject extra sparks at the base

### 3 — Analog
Dual analog-style meter. Both channels grow left to right.
- Left channel: left half of the strip (left edge = silence, midpoint = full)
- Right channel: right half of the strip (midpoint = silence, right edge = full)
- White bar with a red peak indicator
- Peak hold with ~0.5 s smooth decay

---

## Status Indicators

The strip also reacts to system events regardless of the selected effect:

| Event | Color |
|---|---|
| Boot | Dim white pulse |
| Connecting to WiFi | Blue pulse |
| Buffering | Orange pulse |
| Connected | Blue flash |
| Playback started | Green flash |
| Station changed | Cyan flash |
| Track changed | White flash |
| Volume change | Full-strip level bar (brief) |
| Screensaver | Slow color-breathing (if enabled) |

---
