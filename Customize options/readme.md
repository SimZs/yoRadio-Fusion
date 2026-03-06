## 🎨 Customize page
<a name="top"></a>

- [THEME](#theme-section)
- [LAYOUT](#layout-section)
- [BANDS](#bands-section)
- [DYNAMICS](#dynamics-section)
- [SMOOTHING](#smoothing-section)
- [MISC](#miscellaneous-section)
- [TTS](#tts-clock-section)

---

### Theme section

Upload a custom theme file to recolor the display.

<img src="https://github.com/user-attachments/assets/6a7480d5-325a-48cc-afc7-cd572ccd4d37" width="530"/>

If you want to create or edit a theme file, use the online editor via the link below the button.

<img src="https://github.com/user-attachments/assets/e8f9a77b-0f1b-4118-abe6-a4c218d37460" width="530" />

Replaces the border around the meta information bar with a thin, elegant separator line.

<img src="https://github.com/user-attachments/assets/4f14f693-5ad8-4314-93ec-cb2b2781e473" width="530" />

Enables a shadowed background behind the clock.

<img src="https://github.com/user-attachments/assets/cfa7a98b-c39d-4c3a-a0df-510f221d868c" width="530"/>

Switches the display to a grayscale (monochrome) theme.

<img src="https://github.com/user-attachments/assets/3ad9a74c-ff49-4d23-b4cf-0f6dfee4fc84" width="530"/>

After the configured idle time (bl down interval), the backlight will automatically dim down to the selected percentage (bl down level).

<img width="530" src="https://github.com/user-attachments/assets/56468b2f-5f13-4462-8e1d-9d7d14ed9b8d" />

[⬆ Back to top](#top)

---

### Layout section

Select from up to **four different VU meter layouts**.

On smaller displays only **three layouts are available**, because the default layout would not fit on the screen.

<img width="530" src="https://github.com/user-attachments/assets/1c41c1b6-976d-4bec-8021-92b1cbde0427" />

[⬆ Back to top](#top)

---

### Bands section

To configure the VU meter, it must first be enabled.  

You can also enable it directly in this section.

The following parameters can be adjusted:

- **Number of bands**
- **Band height** (in the default layout this corresponds to the band width)
- **Gap** – spacing between bands
- **Fade speed**

<img width="530" src="https://github.com/user-attachments/assets/ddd69c75-60e4-473e-b77b-5dfeb98017ca" />

For the **Tricolor VU** layout you can also configure:

- **Mid color range** (percentage)
- **Upper color range** (percentage)

<img width="530" src="https://github.com/user-attachments/assets/ea4b2e58-f01f-464c-b351-6bb228984cf3" />

Additional options:

- **MID Band color** can be disabled or recolored
- **Peak indicator** can be disabled or recolored

<img width="530" src="https://github.com/user-attachments/assets/63d7cbf8-f699-49e5-aaa3-2ffdc4c596e3" />

- **Band labels** (at the start of each band) can be customized:
  - adjustable **size**
  - adjustable **color**
  - setting the width to **0 or 1 disables the labels**

<img width="530" src="https://github.com/user-attachments/assets/54177b59-1017-409e-bbed-5d56d3632c72" />

All parameters except colors are stored per layout.

Only these options allow direct color configuration because all other colors are controlled by the theme.  

Themes can be customized and uploaded separately.

[⬆ Back to top](#top)

---

### Dynamics section

**VU Meter Calibration**

yoRadio Fusion includes a fully tunable VU engine with dynamic range shaping, soft-knee compression, smoothing and gain controls.  
These parameters allow the VU meter to behave naturally across different hardware, audio sources and display sizes.

**Floor (%)**  
Noise threshold. Below this level the VU does not move.  
Lower it to make very quiet passages visible.

**Ceil (%)**  
Upper limit (headroom). Above this value the signal is compressed.  
`100%` = no ceiling, full dynamic range.

**Expo (γ)** – Curve steepness  
- `1.0` → linear  
- `> 1.0` → compresses low end, emphasizes peaks  
- `< 1.0` → expands low end, makes quiet parts more “alive”

**Gain**  
Overall multiplier.  
Increase if the VU appears too low, decrease if it constantly maxes out.

**Knee (%)**  
Soft entry threshold.  
Higher values → smoother and less “snappy” transitions.

**Processing Order in the Engine**

`normalize(floor..ceil) → smoothstep(knee) → powf(expo) → * gain`

⚡ Quick Setup (4 Steps)

1. **Floor**  
   Lower (or upper) it until you see movement even in quiet sections.  
   - Small displays: 20–40%  
   - Large VUs: 10–25%

2. **Ceil**  
   Start with **100%**.  
   If peaks slam into the top, reduce to **90–95%**.

3. **Expo**  
   - VU moves only at the top → decrease (`1.6 → 1.2`)  
   - Too much low-end activity → increase (`1.0 → 1.3–1.6`)

4. **Gain**  
   Adjust last to fit the full range.  
   Increase if it's too low, decrease if it hits the ceiling.

5. **Knee** Fine Tuning
  - **0–5%** → fast, snappy, more aggressive  
  - **10–20%** → smoother, less jittery motion

[⬆ Back to top](#top)

---

### Smoothing section

**Smoothing Controls**  

- `alphaUp` → smaller = slower rise, larger = faster

- `alphaDown` → larger = slower decay

- `peakUp / peakDown` → peak indicator sensitivity

[⬆ Back to top](#top)

---

### Miscellaneous section

1. **Date Format**

Select from **five built-in date formats**.

Note: On **320×240 displays** only the first **two options** are available, as the remaining formats are too long.

<img width="530" src="https://github.com/user-attachments/assets/6f8eb0a8-58c0-4069-968d-8d3c8adec0cd" />

<hr width="75%">

2. **Show nameday**

Enable or disable the display of the **current nameday**.

When enabled, a **list icon** appears at the end of the date.  
The nameday is displayed as a **scrolling text**.

<img width="530" src="https://github.com/user-attachments/assets/5043de91-b756-4440-9405-e807aabd7a2f" />

<hr width="75%">

3. **Clock Font**

The firmware includes **seven built-in clock fonts**.

<img width="530" src="https://github.com/user-attachments/assets/eb5fcba0-8039-4b38-906e-4944582ab0e9" />

<hr width="75%">

4. **Meta Station Name Skip**

When enabled, the **station name** is taken from the **playlist entry** instead of the stream metadata.

<img width="530" src="https://github.com/user-attachments/assets/f3733ce3-34e0-495c-889b-4853563fc3f1" />

<hr width="75%">

5. **Short Weather Info**

When enabled, weather information is displayed in a **short format without scrolling**.

<img width="530" src="https://github.com/user-attachments/assets/dfccb6d6-81c3-4411-984b-ef9567ca5d70" />

<hr width="75%">

6. **Weather Icon Set**

The firmware includes **two built-in weather icon sets**.

<img width="530" src="https://github.com/user-attachments/assets/25bd9c78-29ba-4492-a8fb-88db9627633b" />

<hr width="75%">

7. **Direct Channel Change**

When enabled, the selected station in the **playlist** will start automatically after the configured **return time**,

without requiring an additional click or touch.

<img width="530" src="https://github.com/user-attachments/assets/f7b0e940-faf7-40a2-89a9-3f8be9091be1" />

<hr width="75%">

8. **12 Hours Clock**

Switches the clock display to **12-hour format**.

<img width="530" src="https://github.com/user-attachments/assets/225b2f25-72cd-447d-9bc4-7f00a40d59a8" />

<hr width="75%">

9. **Moving Cursor Playlist Mode**

When enabled, the **cursor moves within the playlist**, 

and page scrolling occurs only when the cursor reaches the bottom of the list.

<img width="530" src="https://github.com/user-attachments/assets/96cdb700-dce4-4449-9afa-cb52ce61fa9a" />

<hr width="75%">

10. **Stall Watchdog**

Stops playback in **internet radio mode** when the playback buffer runs out, then automatically restarts the stream.

<img width="530" src="https://github.com/user-attachments/assets/17f7cc1a-c014-4972-ab98-12d6c3be7574" />

[⬆ Back to top](#top)

---

### TTS clock section

**Enable clock (TTS)**

When enabled, the radio announces the **current time** at the configured interval.

To use this feature, the **Google TTS language** must be set in the `myoptions.h` file before uploading the firmware.

Example:

```cpp
#define CLOCK_TTS_LANGUAGE "hu" // Default language for TTS
```

<img width="530" src="https://github.com/user-attachments/assets/899bd3ea-f5c4-414e-b362-7f1832bf1c14" />

<hr width="75%">

The **Enable TTS During Playback** option controls whether the time announcement

should also occur while a stream is playing, or only when playback is idle.

<img width="530" src="https://github.com/user-attachments/assets/a317ae7c-6c51-4bfb-af3e-8603667f150d" />

<hr width="75%">

During the configured Do Not Disturb (DND) time window, the TTS clock will not activate.

<img width="530" src="https://github.com/user-attachments/assets/17d80a74-4c4f-4141-b3ee-806861b7dc9f" />

Due to limitations of the audio library, **TTS Clock is disabled in SD mode and DLNA mode**.

[⬆ Back to top](#top)

---






