#include "../core/options.h"

#if DSP_MODEL==DSP_NV3007_142

#include <initializer_list>
#include "dspcore.h"
#include "../core/config.h"

#ifndef DEF_SPI_FREQ
  #define DEF_SPI_FREQ 20000000UL
#endif

#ifndef NV3007_COL_OFFSET
  #define NV3007_COL_OFFSET 12
#endif

#ifndef NV3007_ROW_OFFSET
  #define NV3007_ROW_OFFSET 0
#endif

namespace {

void sendCommand(Adafruit_ST7789* display, uint8_t command,
                 std::initializer_list<uint8_t> data = {}) {
  if (data.size() == 0) {
    display->sendCommand(command);
    return;
  }

  uint8_t buffer[8];
  uint8_t length = 0;
  for (uint8_t value : data) buffer[length++] = value;
  display->sendCommand(command, buffer, length);
}

void initNV3007(Adafruit_ST7789* display) {
  sendCommand(display, 0xFF, {0xA5});

  sendCommand(display, 0x9A, {0x08});
  sendCommand(display, 0x9B, {0x08});
  sendCommand(display, 0x9C, {0xB0});
  sendCommand(display, 0x9D, {0x16});
  sendCommand(display, 0x9E, {0xC4});
  sendCommand(display, 0x8F, {0x55, 0x04});
  sendCommand(display, 0x84, {0x90});
  sendCommand(display, 0x83, {0x7B});
  sendCommand(display, 0x85, {0x33});

  sendCommand(display, 0x60, {0x00});
  sendCommand(display, 0x70, {0x00});
  sendCommand(display, 0x61, {0x02});
  sendCommand(display, 0x71, {0x02});
  sendCommand(display, 0x62, {0x04});
  sendCommand(display, 0x72, {0x04});
  sendCommand(display, 0x6C, {0x29});
  sendCommand(display, 0x7C, {0x29});
  sendCommand(display, 0x6D, {0x31});
  sendCommand(display, 0x7D, {0x31});
  sendCommand(display, 0x6E, {0x0F});
  sendCommand(display, 0x7E, {0x0F});
  sendCommand(display, 0x66, {0x21});
  sendCommand(display, 0x76, {0x21});
  sendCommand(display, 0x68, {0x3A});
  sendCommand(display, 0x78, {0x3A});
  sendCommand(display, 0x63, {0x07});
  sendCommand(display, 0x73, {0x07});
  sendCommand(display, 0x64, {0x05});
  sendCommand(display, 0x74, {0x05});
  sendCommand(display, 0x65, {0x02});
  sendCommand(display, 0x75, {0x02});
  sendCommand(display, 0x67, {0x23});
  sendCommand(display, 0x77, {0x23});
  sendCommand(display, 0x69, {0x08});
  sendCommand(display, 0x79, {0x08});
  sendCommand(display, 0x6A, {0x13});
  sendCommand(display, 0x7A, {0x13});
  sendCommand(display, 0x6B, {0x13});
  sendCommand(display, 0x7B, {0x13});
  sendCommand(display, 0x6F, {0x00});
  sendCommand(display, 0x7F, {0x00});

  sendCommand(display, 0x50, {0x00});
  sendCommand(display, 0x52, {0xD6});
  sendCommand(display, 0x53, {0x08});
  sendCommand(display, 0x54, {0x08});
  sendCommand(display, 0x55, {0x1E});
  sendCommand(display, 0x56, {0x1C});

  sendCommand(display, 0xA0, {0x2B, 0x24, 0x00});
  sendCommand(display, 0xA1, {0x87});
  sendCommand(display, 0xA2, {0x86});
  sendCommand(display, 0xA5, {0x00});
  sendCommand(display, 0xA6, {0x00});
  sendCommand(display, 0xA7, {0x00});
  sendCommand(display, 0xA8, {0x36});
  sendCommand(display, 0xA9, {0x7E});
  sendCommand(display, 0xAA, {0x7E});

  sendCommand(display, 0xB9, {0x85});
  sendCommand(display, 0xBA, {0x84});
  sendCommand(display, 0xBB, {0x83});
  sendCommand(display, 0xBC, {0x82});
  sendCommand(display, 0xBD, {0x81});
  sendCommand(display, 0xBE, {0x80});
  sendCommand(display, 0xBF, {0x01});
  sendCommand(display, 0xC0, {0x02});

  sendCommand(display, 0xC1, {0x00});
  sendCommand(display, 0xC2, {0x00});
  sendCommand(display, 0xC3, {0x00});
  sendCommand(display, 0xC4, {0x33});
  sendCommand(display, 0xC5, {0x7E});
  sendCommand(display, 0xC6, {0x7E});
  sendCommand(display, 0xC8, {0x33, 0x33});
  sendCommand(display, 0xC9, {0x68});
  sendCommand(display, 0xCA, {0x69});
  sendCommand(display, 0xCB, {0x6A});
  sendCommand(display, 0xCC, {0x6B});
  sendCommand(display, 0xCD, {0x33, 0x33});
  sendCommand(display, 0xCE, {0x6C});
  sendCommand(display, 0xCF, {0x6D});
  sendCommand(display, 0xD0, {0x6E});
  sendCommand(display, 0xD1, {0x6F});

  sendCommand(display, 0xAB, {0x03, 0x67});
  sendCommand(display, 0xAC, {0x03, 0x6B});
  sendCommand(display, 0xAD, {0x03, 0x68});
  sendCommand(display, 0xAE, {0x03, 0x6C});
  sendCommand(display, 0xB3, {0x00});
  sendCommand(display, 0xB4, {0x00});
  sendCommand(display, 0xB5, {0x00});
  sendCommand(display, 0xB6, {0x32});
  sendCommand(display, 0xB7, {0x7E});
  sendCommand(display, 0xB8, {0x7E});

  sendCommand(display, 0xE0, {0x00});
  sendCommand(display, 0xE1, {0x03, 0x0F});
  sendCommand(display, 0xE2, {0x04});
  sendCommand(display, 0xE3, {0x01});
  sendCommand(display, 0xE4, {0x0E});
  sendCommand(display, 0xE5, {0x01});
  sendCommand(display, 0xE6, {0x19});
  sendCommand(display, 0xE7, {0x10});
  sendCommand(display, 0xE8, {0x10});
  sendCommand(display, 0xEA, {0x12});
  sendCommand(display, 0xEB, {0xD0});
  sendCommand(display, 0xEC, {0x04});
  sendCommand(display, 0xED, {0x07});
  sendCommand(display, 0xEE, {0x07});
  sendCommand(display, 0xEF, {0x09});
  sendCommand(display, 0xF0, {0xD0});
  sendCommand(display, 0xF1, {0x0E});
  sendCommand(display, 0xF9, {0x17});
  sendCommand(display, 0xF2, {0x2C, 0x1B, 0x0B, 0x20});
  sendCommand(display, 0xE9, {0x29});
  sendCommand(display, 0xEC, {0x04});

  sendCommand(display, 0x35, {0x00});
  sendCommand(display, 0x44, {0x00, 0x10});
  sendCommand(display, 0x46, {0x10});
  sendCommand(display, 0x3A, {0x05});
  sendCommand(display, 0xFF, {0x00});

  sendCommand(display, 0x11);
  delay(120);
  sendCommand(display, 0x29);
  delay(20);
}

}  // namespace

#if DSP_HSPI
DspCore::DspCore(): Adafruit_ST7789(&SPI2, TFT_CS, TFT_DC, TFT_RST) {}
#else
DspCore::DspCore(): Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST) {}
#endif

void DspCore::initDisplay() {
  init(142, 428);
  initNV3007(this);

  // Adafruit_ST7789 assumes a 240x320 controller RAM and derives invalid
  // offsets for a 142x428 panel. NV3007 uses 168x428 RAM; the visible
  // 142-pixel strip starts near the center of its column address range.
  setColRowStart(NV3007_COL_OFFSET, NV3007_ROW_OFFSET);
  _colstart2 = NV3007_COL_OFFSET;
  _rowstart2 = NV3007_ROW_OFFSET;

  if (DEF_SPI_FREQ > 0) setSPISpeed(DEF_SPI_FREQ);
  invert();
  cp437(true);
  flip();
  setTextWrap(false);
  setTextSize(1);
  fillScreen(0x0000);
}

void DspCore::clearDsp(bool black) {
  fillScreen(black ? 0 : config.theme.background);
}

void DspCore::flip() {
  setRotation(config.store.flipscreen ? 3 : 1);
}

void DspCore::invert() {
  invertDisplay(config.store.invertdisplay);
}

void DspCore::sleep() {
  enableSleep(true);
  delay(150);
  enableDisplay(false);
  delay(150);
}

void DspCore::wake() {
  enableDisplay(true);
  delay(150);
  enableSleep(false);
  delay(150);
}

#endif
