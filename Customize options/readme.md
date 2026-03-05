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
- 
- `peakUp / peakDown` → peak indicator sensitivity

[⬆ Back to top](#top)

---

### Miscellaneous section

[⬆ Back to top](#top)

---

### TTS clock section

[⬆ Back to top](#top)

---






