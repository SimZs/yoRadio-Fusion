#include "options.h"
#if (TS_MODEL!=TS_MODEL_UNDEFINED) && (DSP_MODEL!=DSP_DUMMY)
#include "Arduino.h"
#include "touchscreen.h"
#include "config.h"
#include "controls.h"
#include "display.h"
#include "player.h"
#include "../plugins/backlight/backlight.h"

#ifndef TS_X_MIN
  #define TS_X_MIN              400
#endif
#ifndef TS_X_MAX
  #define TS_X_MAX              3800
#endif
#ifndef TS_Y_MIN
  #define TS_Y_MIN              260
#endif
#ifndef TS_Y_MAX
  #define TS_Y_MAX              3800
#endif
#ifndef TS_STEPS
  #define TS_STEPS              40
#endif

#if TS_MODEL==TS_MODEL_XPT2046
  #ifdef TS_SPIPINS
    SPIClass  TSSPI(HSPI);
  #endif
  #include "../Touch/XPT2046/XPT2046_Touchscreen.h"
  XPT2046_Touchscreen ts(TS_CS);
  typedef TS_Point TSPoint;
#elif TS_MODEL==TS_MODEL_GT911
  #include "../Touch/GT911/TAMC_GT911.h"
  TAMC_GT911 ts = TAMC_GT911(TS_SDA, TS_SCL, TS_INT, TS_RST, 0, 0);
  typedef TP_Point TSPoint;
#elif TS_MODEL==TS_MODEL_CST816
  #include "../Touch/CST816/touchscreen_cst816.h"
  typedef TP_Point TSPoint;
#elif TS_MODEL == TS_MODEL_FT6X36
   #include "../Touch/FT6X36/FT6X36.h"
// Global Wire objektum I2C config.cpp --> Wire.begin(TS_SDA, TS_SCL)
FT6X36 ts(&Wire, TS_INT);
// A későbbi kódhoz egységes típusnév
typedef TPoint           TSPoint;
static volatile bool     g_ftTouched = false;
static volatile uint16_t g_ftX = 0;
static volatile uint16_t g_ftY = 0;
static volatile uint32_t g_ftLastMs = 0;

static void ft6x36TouchHandler(TPoint point, TEvent e) {
    // Események: TouchStart, TouchMove, TouchEnd, Tap, Drag...
    if (e == TEvent::TouchStart || e == TEvent::TouchMove || e == TEvent::DragMove || e == TEvent::DragStart) {
        g_ftX = point.x;
        g_ftY = point.y;
        g_ftTouched = true;
        g_ftLastMs = millis();
    } else if (e == TEvent::TouchEnd || e == TEvent::DragEnd || e == TEvent::Tap) {
        g_ftX = point.x;
        g_ftY = point.y;
        g_ftTouched = false;
        g_ftLastMs = millis();
    }
}
#endif

void TouchScreen::init(uint16_t w, uint16_t h){
  
#if TS_MODEL==TS_MODEL_CST816
  ts.begin(w, h, config.store.fliptouch);
#endif
#if TS_MODEL==TS_MODEL_XPT2046
  #ifdef TS_SPIPINS
    TSSPI.begin(TS_SPIPINS);
    ts.begin(TSSPI);
  #else
    #if TS_HSPI
      ts.begin(SPI2);
    #else
      ts.begin();
    #endif
  #endif
  ts.setRotation(config.store.fliptouch?3:1);
#endif
#if TS_MODEL==TS_MODEL_GT911
  ts.begin();
  ts.setRotation(config.store.fliptouch?1:3);
#endif
    #if TS_MODEL == TS_MODEL_FT6X36
    bool ok = ts.begin(FT6X36_DEFAULT_THRESHOLD);
    if (ok) {
        Serial.println("[TOUCH] FT6X36 init OK");
    } else {
        Serial.println("[TOUCH] FT6X36 init FAILED (nincs IC / I2C hiba)");
    }
    ts.registerTouchHandler(ft6x36TouchHandler);
    #endif
  _width  = w;
  _height = h;
#if TS_MODEL==TS_MODEL_GT911
  ts.setResolution(_width, _height);
#endif
}

tsDirection_e TouchScreen::_tsDirection(uint16_t x, uint16_t y) {
  int16_t dX = x - _oldTouchX;
  int16_t dY = y - _oldTouchY;
  if (abs(dX) > 20 || abs(dY) > 20) {
    if (abs(dX) > abs(dY)) {
      if (dX > 0) {
        return TSD_RIGHT;
      } else {
        return TSD_LEFT;
      }
    } else {
      if (dY > 0) {
        return TSD_DOWN;
      } else {
        return TSD_UP;
      }
    }
  } else {
    return TDS_REQUEST;
  }
}

void TouchScreen::flip(){
#if TS_MODEL==TS_MODEL_CST816
  ts.setRotation(config.store.fliptouch ?2:0);
#endif
#if TS_MODEL==TS_MODEL_XPT2046
  ts.setRotation(config.store.fliptouch?3:1);
#endif
#if TS_MODEL==TS_MODEL_GT911
  ts.setRotation(config.store.fliptouch?1:3);
#endif
}

void TouchScreen::loop(){
  uint16_t touchX, touchY;
  static bool wastouched = true;
  static uint32_t touchLongPress;
  static tsDirection_e direct;
  static uint16_t touchVol, touchStation;
  if (!_checklpdelay(20, _touchdelay)) return;
#if TS_MODEL == TS_MODEL_FT6X36
  ts.loop(); // feldolgozza az IRQ-kat és meghívja a touch handlert
#endif
#if TS_MODEL==TS_MODEL_GT911
  ts.read();
#endif
#if TS_MODEL==TS_MODEL_CST816
  ts.read();
#endif
bool istouched = _istouched();
  if(istouched){
  #if TS_MODEL==TS_MODEL_XPT2046
    TSPoint p = ts.getPoint();
    touchX = map(p.x, TS_X_MIN, TS_X_MAX, 0, _width);
    touchY = map(p.y, TS_Y_MIN, TS_Y_MAX, 0, _height);
  #elif TS_MODEL==TS_MODEL_GT911
    TSPoint p = ts.points[0];
    touchX = p.x;
    touchY = p.y;
  #elif TS_MODEL==TS_MODEL_CST816
    TSPoint p = ts.points[0];
    touchX = p.x;
    touchY = p.y;
  #elif TS_MODEL == TS_MODEL_FT6X36
        uint16_t rawX = g_ftX;
        uint16_t rawY = g_ftY;
        // Serial.printf("touchscreen.cpp--> nyers X: %d, nyers Y: %d, store.fliptouch: %d\n", rawX, rawY, config.store.fliptouch);
        if (config.store.fliptouch) { // 180 fokos tükrözés
            touchX = (_width - 1) - rawY;
            touchY = rawX;
        } else { // Alap helyzet
            touchX = rawY;
            touchY = (_height - 1) - rawX;
        }
        #ifdef X_TOUCH_MIRRORING
        touchX = (_width - 1) - touchX;
        #endif
        #ifdef Y_TOUCH_MIRRORING
        touchY = (_height - 1) - touchY;
        #endif
            // Serial.printf("touchscreen.cpp--> touch X: %d, touch Y: %d\n", touchX, touchY);
  #endif
  if (!wastouched) { /*     START TOUCH     */
      _oldTouchX = touchX;
      _oldTouchY = touchY;
      touchVol = touchX;
      touchStation = touchY;
      direct = TDS_REQUEST;
      touchLongPress=millis();
    } else { /*     SWIPE TOUCH     */
      direct = _tsDirection(touchX, touchY);
      switch (direct) {
        case TSD_LEFT:
        case TSD_RIGHT: {
            touchLongPress=millis();
            if(display.mode()==PLAYER || display.mode()==VOL){
              int16_t xDelta = map(abs(touchVol - touchX), 0, _width, 0, TS_STEPS);
              display.putRequest(NEWMODE, VOL);
              if (xDelta>1) {
                controlsEvent((touchVol - touchX)<0);
                touchVol = touchX;
              }
            }
            break;
          }
        case TSD_UP:
        case TSD_DOWN: {
            touchLongPress=millis();
            if(display.mode()==PLAYER || display.mode()==STATIONS){
              int16_t yDelta = map(abs(touchStation - touchY), 0, _height, 0, TS_STEPS);
              display.putRequest(NEWMODE, STATIONS);
              if (yDelta>1) {
                controlsEvent((touchStation - touchY)<0);
                touchStation = touchY;
              }
            }
            break;
          }
        default:
            break;
      }
    }
  }else{
    if (wastouched) {/*     END TOUCH     */
      if (direct == TDS_REQUEST) {
                uint32_t pressTicks = millis() - touchLongPress;
                if (pressTicks < BTN_PRESS_TICKS * 2) { // (1000 ms stations)
                    if (pressTicks > 50) { 
                      if (config.store.blDimEnable) {
                        bool dimmed = backlightPlugin.isDimmed();
                        bool fading = backlightPlugin.isFading();
                        bool justWoke = backlightPlugin.justWoke();
                        if (dimmed || fading) {
                           // Serial.println("touchscreen.cpp--> WAKE ONLY");
                            touchLongPress = millis();
                            backlightPlugin.wake();
                            return;
                        }
                        backlightPlugin.activity();
                        if (justWoke) {
                            //Serial.println("touchscreen.cpp--> SWALLOW AFTER WAKE");
                            return;
                        }
                     }
                        //Serial.println("touchscreen.cpp--> NORMAL CLICK");
                        onBtnClick(EVT_BTNCENTER);
                    }
        }else{
          display.putRequest(NEWMODE, display.mode() == PLAYER ? STATIONS : PLAYER);
        }
      }
      direct = TSD_STAY;
    }
  }
  wastouched = istouched;
}

bool TouchScreen::_checklpdelay(int m, uint32_t &tstamp) {
  if (millis() - tstamp > m) {
    tstamp = millis();
    return true;
  } else {
    return false;
  }
}

bool TouchScreen::_istouched(){
#if TS_MODEL==TS_MODEL_XPT2046
  return ts.touched();
#elif TS_MODEL==TS_MODEL_GT911
  return ts.isTouched;
#elif TS_MODEL==TS_MODEL_CST816
  return ts.isTouched;
#elif TS_MODEL == TS_MODEL_FT6X36
    // “touched”-nek tekintjük, ha a handler szerint érintve van,
    // vagy ha nagyon friss esemény volt (kis hiszterézis).
    if (g_ftTouched) return true;
    return ((uint32_t)(millis() - g_ftLastMs) < 50);
#endif
}

#endif  // TS_MODEL!=TS_MODEL_UNDEFINED
