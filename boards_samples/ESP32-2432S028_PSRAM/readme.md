# ESP32-2432S028 (CYD) – Hardware Modifications

This folder contains the required hardware modification notes and wiring diagrams
for using the **ESP32-2432S028 (CYD)** board with yoRadio Fusion.

⚠️ **Important:**  
The CYD panel **cannot be used in its factory configuration**.  
Three hardware modifications are required:

1. Add an external **8 MB PSRAM chip**
2. Remove the onboard **FM8002A** audio amplifier
3. Wire an external **PCM5102A DAC**, **rotary encoder**, and **IR receiver**

The PSRAM I use can be purchased here:

https://www.aliexpress.com/item/1005006722662043.html?spm=a2g0o.order_list.order_list_main.136.6098180273OrXF

All steps are described below.

---

## 🔧 1. Adding External PSRAM (8 MB)

The ESP32-2432S028 does not include PSRAM by default.  
yoRadio Fusion requires PSRAM for stable audio buffering and display operation.

### ✔ Required component
- 8 MB **SPIRAM / PSRAM** chip  
  – electrically compatible with **Espressif ESP-PSRAM32**  
  – select correct **voltage version** (1.8 V or 3.6 V depending on module)

### ✔ Wiring instructions

Solder the PSRAM chip **directly onto the Flash chip's pads**:

| PSRAM pin | Connect to |
|----------|-------------|
| 2 | Flash pin 2 |
| 3 | Flash pin 3 |
| 4 | Flash pin 4 |
| 5 | Flash pin 5 |
| 7 | Flash pin 7 |
| 8 | Flash pin 8 (VDD_SDIO 3.3 V) |


Additional connections:

- **PSRAM pin 1** → **GPIO16**  
- Add a **10 kΩ pull-up resistor** between **PSRAM pin 1** and **pin 8 (3.3 V)**  
- **PSRAM pin 6** → **GPIO17**

> 📌 These GPIO connections must match the ESP32 OPI PSRAM configuration used by yoRadio Fusion.



---

## 🔥 2. Removing the onboard FM8002A DAC

The CYD board includes an onboard **FM8002A Class-D amplifier**.  
It conflicts with the I2S output used by yoRadio Fusion.

### ✔ Required modification  
**Desolder and remove the FM8002A chip completely.**

This frees the I2S pins for the external DAC.

---

## 🎧 3. Wiring the PCM5102A DAC, Encoder and IR Receiver

After the FM8002A is removed, you must wire:

### ✔ PCM5102A (external DAC)
Connect according to the wiring diagram in this folder:

- **I2S BCK**
- **I2S LRCK**
- **I2S DATA**
- **5V / 3.3V power**
- **GND**

### ✔ Rotary Encoder
- Connect **CLK**, **DT**, **SW**, and **GND**  
- Use the pinout shown in `wiring_diagram.png` (included in this directory)

### ✔ IR Receiver (VS1838B / TSOP4838)
- **OUT** → IR input pin (as defined in `myoptions.h`)  
- **VCC** → 3.3 V  
- **GND**

---

## 💾 SD Card Support (Required File Replacement)

To enable SD card functionality, you **must replace** the following files in the  
main yoRadio firmware source:

- `sdmanager.cpp`
- `sdmanager.h`

Use the versions included in this folder.  

---

## 📦 Included Files

- `wiring_diagram.png` – DAC + encoder + IR wiring  
- `Arduino_setup.png` – recommended Arduino IDE settings  
- Additional board-specific notes

---

## ⚠️ Notes

- This board **must have PSRAM** to run yoRadio Fusion  
- Ensure **Arduino IDE → PSRAM = Enabled**  
- Perform a **Full Flash Erase** on the first upload  
- Use the provided wiring diagram to avoid pin conflicts

