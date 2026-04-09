#include "../../core/options.h"

#if (TS_MODEL==TS_MODEL_CST328)

#include "touchscreen_cst328.h"
#include <Wire.h>

#define CST_I2C Wire
#define CST328_ADDR 0x1A

#define REG_TOUCH_DATA   0xD000
#define REG_TOUCH_COUNT  0xD005

// ===== DEBUG =====
#define CST328_DEBUG      0
#define CST328_DEBUG_RAW  0   // testare citire raw

CST328_Adapter ts;

// ================= I2C =================
static bool cst328_read(uint16_t reg, uint8_t *data, uint8_t len) {
  CST_I2C.beginTransmission(CST328_ADDR);
  CST_I2C.write(reg >> 8);
  CST_I2C.write(reg & 0xFF);
  if (CST_I2C.endTransmission(false) != 0) return false;

  if (CST_I2C.requestFrom((uint8_t)CST328_ADDR, len) != len) return false;

  for (uint8_t i = 0; i < len; i++) data[i] = CST_I2C.read();
  return true;
}

static void cst328_clear() {
  CST_I2C.beginTransmission(CST328_ADDR);
  CST_I2C.write(0xD0);
  CST_I2C.write(0x05);
  CST_I2C.write(0);
  CST_I2C.endTransmission();
}

// ================= INIT =================
bool CST328_Adapter::begin(uint16_t w, uint16_t h, bool flip) {
  _w = w;
  _h = h;
  _flip = flip;

  CST_I2C.begin(TS_SDA, TS_SCL, 400000);

#if (TS_RST >= 0)
  pinMode(TS_RST, OUTPUT);
  digitalWrite(TS_RST, HIGH);
  delay(50);
  digitalWrite(TS_RST, LOW);
  delay(10);
  digitalWrite(TS_RST, HIGH);
  delay(50);
#endif

#if (TS_INT >= 0)
  pinMode(TS_INT, INPUT_PULLUP);
#endif

#if CST328_DEBUG
  Serial.println("[CST328] init OK");
#endif

  return true;
}

void CST328_Adapter::setRotation(uint8_t r) {
  _rot = r & 3;
  _flip = (_rot == 2);   // important: sincronizare cu YoRadio
}

// ================= READ =================
void CST328_Adapter::read() {
  isTouched = false;

  static bool lastTouch = false;
  static uint32_t lastPrint = 0;

  uint8_t count;
  if (!cst328_read(REG_TOUCH_COUNT, &count, 1)) return;

  uint8_t pts = count & 0x0F;

  // ===== NO TOUCH =====
  if (pts == 0) {
    if (lastTouch) {
#if CST328_DEBUG
      Serial.println("[CST328] RELEASE");
#endif
    }
    lastTouch = false;
    cst328_clear();
    return;
  }

  lastTouch = true;

  uint8_t buf[8];
  if (!cst328_read(REG_TOUCH_DATA, buf, 8)) return;

  cst328_clear();

  uint8_t status = buf[0] & 0x0F;
  if (status != 0x06) {
#if CST328_DEBUG
    Serial.println("[CST328] IGNORE");
#endif
    return;
  }

  bool allowPrint = (millis() - lastPrint > 50);
  if (allowPrint) lastPrint = millis();

#if CST328_DEBUG_RAW
  if (allowPrint) {
    Serial.print("[BUF] ");
    for (int i = 0; i < 8; i++) {
      Serial.printf("[%d]=%02X ", i, buf[i]);
    }
    Serial.println();
  }
#endif

  // ===== RAW (CORECT) =====
  uint16_t rawX = ((uint16_t)buf[1] << 4) | ((buf[3] & 0xF0) >> 4);
  uint16_t rawY = ((uint16_t)buf[2] << 4) | (buf[3] & 0x0F);

#if CST328_DEBUG
  if (allowPrint) {
    Serial.printf("[RAW] X=%d Y=%d\n", rawX, rawY);
  }
#endif

  // ===== SCALE DIRECT (fără map) =====
  uint16_t x = (rawX * _w) / 240;
  uint16_t y = (rawY * _h) / 320;

  // clamp
  if (x >= _w) x = _w - 1;
  if (y >= _h) y = _h - 1;

  // ===== AXE STANDARD YO RADIO =====
#if TS_SWAP_XY
  { uint16_t t = x; x = y; y = t; }
#endif

#if TS_INVERT_X
  x = (_w - 1) - x;
#endif

#if TS_INVERT_Y
  y = (_h - 1) - y;
#endif

  // ===== FLIP DIN UI (CRITIC) =====
  if (_flip) {
    x = (_w - 1) - x;
    y = (_h - 1) - y;
  }

#if CST328_DEBUG
  if (allowPrint) {
    Serial.printf("[FINAL] X=%d Y=%d flip=%d\n", x, y, _flip);
  }
#endif

  // ===== OUTPUT =====
  this->points[0].x = x;
  this->points[0].y = y;

  isTouched = true;
}

// ================= STATE =================
bool CST328_Adapter::touched() const {
  return isTouched;
}

#endif