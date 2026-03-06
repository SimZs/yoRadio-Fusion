<p align="center">
<img width="1536" alt="bitkép" src="https://github.com/user-attachments/assets/ef6b7091-67af-4ebe-9bdc-6cf28449cfff" />
</p>
<a name="top"></a>

# yoRadio Fusion – ESP32 Web Radio

This project is based on **ёRadio v0.9.720** by e2002:  
https://github.com/e2002/yoradio  
📌 *Reading the original documentation is strongly recommended.*

## 📑 Table of Contents

- [Features](#-features)
- [Supported Hardware](#-supported-hardware)
- [Supported Displays](#-supported-displays)
- [Getting Started](#-getting-started)
- [Version History](#-version-history--changelog)
- [Credits](#-credits)
  
---

## 🚀 Features
- High-bitrate audio playback (IDF-modified audio pipeline included)  
- Modern Web UI with configuration for VU, clock fonts, themes, timers  
- Theme upload + customizable theme colors and [custom](https://github.com/SimZs/yoRadio-Fusion/tree/main/Customize%20options) features
- Multi-language and regional support: EN, RU, UA, SK, RO, PL, NL, IT, GR, DE, CZ, HU (Developed by Tamás Várai)
- Multilingual nameday system: UA, PL, NL, GR, DE, CZ, HU (Developed by Tamás Várai)
- Play audio files on [DLNA](https://github.com/SimZs/yoRadio-Fusion/tree/main/DLNA%20setup) home network via web UI

[⬆ Back to top](#top)

---

## 🖥 Supported Hardware

- ESP32-S3 with 4–16 MB flash and PSRAM
- External DACs (PCM5102A, **CS4344** depending on setup)
- TF/SD modules, IR receivers, rotary encoders
- **CST816** / GT911 / XPT2046 / Ft6x36 touch controllers

[⬆ Back to top](#top)

---

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

[⬆ Back to top](#top)

---

## 🚀 Getting Started
<a name="subtop"></a>
<details>
<summary>💻 Firmware Installation</summary>

 ---

Follow these steps to set up the development environment for **yoRadio**.

Note: Simply click on the images to enlarge them.

### 1. Install Git

Download and install Git for Windows:  
https://git-scm.com/download/win

Install with default settings.

During installation select: ***Git from the command line and also from 3rd-party software***

<a href="https://github.com/user-attachments/assets/f785b709-42ab-462a-8a2a-07365c137898">
  <img src="https://github.com/user-attachments/assets/f785b709-42ab-462a-8a2a-07365c137898" width="400">
</a>

---

### 2. Install Python

Download Python:
https://www.python.org/downloads/

Install with default settings.

During installation enable: ***Add Python to PATH***

<a href="https://github.com/user-attachments/assets/6ef444fd-642f-4256-a2ae-715526230ef9">
  <img src="https://github.com/user-attachments/assets/6ef444fd-642f-4256-a2ae-715526230ef9" width="400">
</a>

---

### 3. Install Visual Studio Code

Download VSCode:
https://code.visualstudio.com/

Install with default settings.

---

### 4. Install PlatformIO

Open VSCode and install the PlatformIO IDE extension.

Steps: VSCode → Extensions → search "PlatformIO IDE" → Install

<a href="https://github.com/user-attachments/assets/a742fceb-4126-4d36-aa05-3f6ef462ba91">
  <img src="https://github.com/user-attachments/assets/a742fceb-4126-4d36-aa05-3f6ef462ba91" width="400">
</a>

After installation restart VSCode.

<a href="https://github.com/user-attachments/assets/7216e432-b2e5-4aba-afcd-fed0c00f2eba">
  <img src="https://github.com/user-attachments/assets/7216e432-b2e5-4aba-afcd-fed0c00f2eba" width="400">
</a>

After restarting VSCode, wait until PlatformIO finishes installing the required toolchains and dependencies.

<a href="https://github.com/user-attachments/assets/306eb958-1c4a-4460-b51f-1f5e6a6496f7">
  <img src="https://github.com/user-attachments/assets/306eb958-1c4a-4460-b51f-1f5e6a6496f7" width="400">
</a>

---

### 5. Download & unzip firmware

Click the green **Code** button at the top of this page and select **Download ZIP**.

Extract the downloaded ZIP file to a folder on your computer.

Create a project folder on your system: (for example: C:\platformio)

From the extracted archive navigate to: yoRadio-main/yoRadio

Copy the **entire contents of this folder** into: C:\platformio

Only the **yoRadio** folder is required. The other folders and files from the ZIP archive are not needed.

In the **yoRadio-main** archive open the folder: **Setup samples**

Select the folder that matches your display type (for example: **ILI9341**).

From the selected folder copy the file: **platformio.ini** into the project folder: **C:\platformio\yoRadio**

Copy your own **myoptions.h** file into the project folder: **C:\platformio\yoRadio**, or use the one in the Setup samples folder, but adapt it to your own hardware!

**The project folder is now ready for use!**

---

### 6. Back to VSCode PlatformIO

In VSCode click the **PlatformIO icon** (ant head) in the left sidebar.

Select **Pick a folder** and open: C:\platformio\yoRadio

<a href="https://github.com/user-attachments/assets/41ff6090-ea1b-4b8f-8124-35ab3ff44cbb">
  <img src="https://github.com/user-attachments/assets/41ff6090-ea1b-4b8f-8124-35ab3ff44cbb" width="400">
</a>

After opening the project, wait until PlatformIO finishes preparing the environment and installing all required tools and dependencies for the project.

This process may take **5–10 minutes** on the first run.

<a href="https://github.com/user-attachments/assets/56afa0d7-f8e1-432d-a8b9-19814817e61f">
  <img src="https://github.com/user-attachments/assets/56afa0d7-f8e1-432d-a8b9-19814817e61f" width="400">
</a>

The setup is complete when there are no more PlatformIO messages in the bottom status bar or in the notification panel on the right side of VSCode.

---

### 7. Install the pioarduino Platform

The environment is now ready, however **yoRadio Fusion uses the pioarduino platform**, which PlatformIO does not install automatically during the first setup.

Close **VSCode**, then start it again.

When the project loads, VSCode will detect the missing **pioarduino** platform and prompt you to install it.

Click **Install** and wait until the installation completes.

<a href="https://github.com/user-attachments/assets/b54b6369-de44-412f-8393-35d9c97a4cbc">
  <img src="https://github.com/user-attachments/assets/b54b6369-de44-412f-8393-35d9c97a4cbc" width="400">
</a>

After the installation is complete, restart **VSCode**.

After restarting VSCode, the **PlatformIO icon** (ant head) will temporarily change to an **IC icon**.

This indicates that PlatformIO is installing the **Arduino framework and toolchains** required by the project.

Please wait until the installation finishes. This process may take **20–25 minutes**, so please be patient.

<a href="https://github.com/user-attachments/assets/90b15a64-2678-4258-bc03-c425492bf3e8">
  <img src="https://github.com/user-attachments/assets/90b15a64-2678-4258-bc03-c425492bf3e8" width="400">
</a>

---

### 8. Final configuration

After the installation finishes, close **VSCode**.

The next step is to apply three modifications required for **language support** , **high-bitrate audio streaming**
and **configure WiFi**.

### 1. High Bitrate Audio Fix

After installing **pioarduino**, the following folder will exist:

C:\Users\ **your_username** \.platformio\packages\framework-arduinoespressif32-libs\esp32s3\lib\

In this folder replace the following files:

liblwip.a  
libesp_netif.a

with the files located in this repository:

yoRadio-main/Audio_IDF_MOD/

This modification is required for **smooth playback of high-bitrate audio streams**.

For more information, please read: [This documentation](https://github.com/VaraiTamas/yoRadio/blob/main/Lib_tools/LIB_TOOLS.md)

### 2. Display Font Fix

In the project folder you will find a directory similar to:

C:\platformio\yoRadio\.pio\libdeps\yoradio-esp32s3n16r8-ili9341\ **Adafruit GFX Library** \

Replace the file:

glcdfont.c

with the file located in:

C:\platformio\yoRadio\locale\glcdfont\ **your_language** \glcdfont.c

This modification ensures **correct display of texts and icons on the screen**.

### Important

The folders inside **.pio/libdeps/** are created based on the environment defined in **platformio.ini**:

For this reason the example folder: **yoradio-esp32s3n16r8-ili9341** may have a different name 
depending on the selected hardware configuration. 

If you use **multiple hardware configurations**, multiple folders will appear inside **libdeps**, 
and the **font modification must be applied in each of them** at first time.

### 3. Configure WiFi

Navigate to the following folder:

C:\platformio\yoRadio\data\data\

Open the file:

wifi.csv

⚠️ Do not use Excel. 

Open the file with **Notepad++** or directly in the **PlatformIO editor**.

Replace the default values: 'yourssid yourpassword'

with your own WiFi network credentials.  

The SSID and password must be separated by a **TAB**.

If you want to use the device in multiple locations, you can add additional WiFi networks on new lines.

Save the file & close.

---

### 9. Build and Upload the Firmware

Open **VSCode** and click the **Build icon** in the PlatformIO bottom toolbar.

<a href="https://github.com/user-attachments/assets/2ab0f6ff-afd3-4029-af18-a6c87c628e97">
  <img src="https://github.com/user-attachments/assets/2ab0f6ff-afd3-4029-af18-a6c87c628e97" width="400">
</a>

Wait until the first compilation finishes successfully.

Connect your device to the computer using a **USB cable**.

Open a **PlatformIO Terminal**

<a href="https://github.com/user-attachments/assets/756aa022-0720-487e-8eac-5818e338eaa4">
  <img src="https://github.com/user-attachments/assets/756aa022-0720-487e-8eac-5818e338eaa4" width="400">
</a>

and run the following commands:

**1.** Erease all flash: `pio run -t erase`

**2.** Upload firmware: `pio run -t upload`

**3.** Upload SPIFFS: `pio run -t uploadfs`

⚠️ The first upload may take several minutes.

**USE & ENJOY!**

---

### 10. Future Firmware Updates

For later updates you can simply use the **Upload icon** in PlatformIO.

<a href="https://github.com/user-attachments/assets/53d39483-74b4-4fd9-902d-8e68786d98a4">
  <img src="https://github.com/user-attachments/assets/53d39483-74b4-4fd9-902d-8e68786d98a4" width="400">
</a>

The firmware can also be uploaded **via HTTP OTA** from the **WebUI interface**, 
on the Settings page, in the Update Firmware menu:

<a href="https://github.com/user-attachments/assets/0ff3e556-5947-4564-b653-7c2d466fc426">
  <img src="https://github.com/user-attachments/assets/0ff3e556-5947-4564-b653-7c2d466fc426" width="400">
</a>

[⬆ Collapse](#subtop)

---

</details>


<details>
<summary>📦 Hardware Requirements</summary>

- ESP32-S3 board with 4M, 8M, 16M flash memory  
- PSRAM recommended  

</details>

<details>
<summary>🔌 Hardware Setup</summary>

- Display wiring  
- Audio DAC connection  
- Rotary encoder / buttons  

</details>

<details>
<summary>🔗 Useful Links & Tools</summary>

---

### YoRadio-Controller

***Android application*** to access the ëRadio web radio server.

Supported Android versions: SDK 24–36 (Android 7 Nougat and above).

Developed by: Tibor Botfai

https://github.com/gidano/YoRadio-Controller

---

### Font & Icon creation tools

**image2cpp**

A simple tool to change images into byte arrays (I used it to create weather icons)

https://javl.github.io/image2cpp/

**Converting fonts from TrueType to Adafruit GFX**

https://rop.nl/truetype2gfx/

**Adafruit GFX Pixel font customiser**

https://tchapi.github.io/Adafruit-GFX-Font-Customiser/

---

### Theme editors

Online: https://vip-cxema.org/index.php/online-kalkulyatory/yoradio-redaktor-tem

Offline: https://github.com/andrasdaradici/YoRadio-Theme-Editor/releases

[⬆ Collapse](#getting-started)

</details>

[⬆ Back to top](#top)

---

## 📌 Version History / Changelog

## v0.5.8 – 2026-03-06

**Full Erase Needed before upload!!**

 • Added **Czech language** support (Thnx to: Miroslav Tlustý)
 
 • Added **VU label indicator**
 
 • Added **Simple Weather icon set**
 
 • Added **scrollable playlist**
 
 • Added **Backlight fading plugin**
 
 • Added **Stall watchdog switch** (Thnx to: Andrzej Jaroszuk)

 • Added a custom **Fusion logo** on bootscreen for all supported displays
 
 • Added **encoder control for DLNA mode** (Web → DLNA → SD) (Thnx to: Scott Barber)
 
 • **DLNA bug fixes** and improved DLNA server connection stability
 
 • **WebUI player page** visual improvements (Thnx to: Scott Barber)
 
 • **Imperial weather data display** improvements
 
 • Updated **AudioI2S library** to version **3.4.4** (Thnx to: Tamás Várai)
 
 • New features are now accessible from the **WebUI** (see the **Customize page** for detailed descriptions)
 
 • **SDManager chaos resolved** – manual modification of SDManager files is no longer required
 
 • **OTA partition layout** applied to support firmware updates via OTA (Thnx to: nvalw)

 • Added **PlatformIO step-by-step installation guide** [Getting started](https://github.com/SimZs/yoRadio-Fusion/blob/main/README.md#-getting-started) section --> Firmware installation

```
Hardware descriptions and setup guides will be added progressively as time allows

Custom Fusion logo graphic created by Scott Barber & Gemini & chatGPT 😄

I don't plan to change the config_struct in the near future, 

so upcoming updates should be possible via OTA, without requiring a full erase.
``` 

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

[⬆ Back to top](#top)

---

## 🙌 Credits

Based on ёRadio by e2002.
Extended, refactored, and localized by the yoRadio Fusion contributors.
