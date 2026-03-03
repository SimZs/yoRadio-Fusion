<p align="center">
<img width="1536" alt="bitkép" src="https://github.com/user-attachments/assets/ef6b7091-67af-4ebe-9bdc-6cf28449cfff" />
</p>

# yoRadio Fusion – ESP32 Web Radio

This project is based on **ёRadio v0.9.720** by e2002:  
https://github.com/e2002/yoradio  
📌 *Reading the original documentation is strongly recommended.*

---

## 🚀 Features
- High-bitrate audio playback (IDF-modified audio pipeline included)  
- Modern Web UI with configuration for VU, clock fonts, themes, timers  
- Theme upload + customizable theme colors and custom features
- Multi-language and regional support: EN, RU, UA, SK, RO, PL, NL, IT, GR, DE, CZ, HU (Developed by Tamás Várai)
- Multilingual nameday system: UA, PL, NL, GR, DE, CZ, HU (Developed by Tamás Várai)
- Play audio files on DLNA home network via web UI

---

## 🖥 Supported Hardware

- ESP32-S3 with 4–16 MB flash and PSRAM
- External DACs (PCM5102A, **CS4344** depending on setup)
- TF/SD modules, IR receivers, rotary encoders
- **CST816** / GT911 / XPT2046 / Ft6x36 touch controllers

## 📺 Supported Displays

- NV3041A 480×272 (Guition JC4827W543C)
- GC9A01 Round 240x240
- ILI9488, ILI9486 480x320
- ILI9341 320x240
- ST7796 480x320
- ST7789 320x240
- ST7789 284x76
- ST7789 240x240
- ST7789 320x170
- ST7735 160x128

---

## 🚀 Getting Started

<details>
<summary>📦 Requirements</summary>

- ESP32-S3 board  
- PSRAM recommended  
- PlatformIO

</details>

<details>
<summary>🔌 Hardware Setup</summary>

- Display wiring  
- Audio DAC connection  
- Rotary encoder / buttons  

</details>

<details>
<summary>💻 Firmware Build</summary>

PlatformIO build steps here...

</details>

<details>
<summary>🔗 Useful Links & Tools</summary>

- Image converters  
- SVG editor  
- Flash tools  

</details>

---

## 📌 Version History / Changelog

## v0.4.4 – 2026-01-31

 • This build adds **DLNA playlist browsing and playback** support to yoRadio.
 
 ➡️ [Read the DLNA documentation] ([DLNA setup/readme.md](https://github.com/SimZs/yoRadio-Fusion/tree/main/DLNA%20setup))

## v0.4.2 – 2026-01-26

 • Clock widget – alignment of the seconds fonts to match the clock font
 
 • IRremoteESP8266 library update (special thanks to Sebastian Stolowski)

## v0.3.9 – 2026-01-16

 • optimizing websocket messages and http endpoint communications
 
 • new display type added: ST7735 (black_conf) 160x128px

  ![Blue_pcb](https://github.com/user-attachments/assets/7e7879b1-d911-456d-a215-66f7a8eb9dd6)
 
 • Pressure correction converts sea-level pressure to local pressure on WebUI

 <img width="585" height="716" alt="2026-01-16_13-03-01" src="https://github.com/user-attachments/assets/0f920142-4e18-43fe-b076-318acc7c285a" />

 To ensure correct display of air pressure data, the pressure slope correction factor has been moved to the WebUI.
 
 (Pressure slope: linear gradient, typically ~0.11–0.12 hPa per meter (within normal weather ranges, at low to medium altitudes))
 

## v0.3.5 – 2025-12-08

 • SD reading BUG fix (special thanks to Tamás Várai)
 
 • Audio library version update (special thanks to Tamás Várai)
 
 • Browser v2 integration (special thanks to Mirosław Bubka)
 
 • TTS clock BUG fix
 
 • Weather icon placement on 480×xxx and 320×xxx displays

 ![JC4827W543_weather](https://github.com/user-attachments/assets/a4e87d51-e22a-4b37-8219-0c410be00149)

---

## 🙌 Credits

Based on ёRadio by e2002.
Extended, refactored, and localized by the yoRadio Fusion contributors.
