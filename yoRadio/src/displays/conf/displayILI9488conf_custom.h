/*************************************************************************************
    ILI9488 480X320 displays configuration file.
*************************************************************************************/

#ifndef displayILI9488conf_h
#define displayILI9488conf_h

#include "../../core/config.h"

#define DSP_WIDTH       480
#define DSP_HEIGHT      320
#define TFT_FRAMEWDT    10
#define MAX_WIDTH       DSP_WIDTH-TFT_FRAMEWDT*2

#if BITRATE_FULL
  #define TITLE_FIX 44
#else
  #define TITLE_FIX 0
#endif
#define bootLogoTop     60

/* SROLLS  */
// meta_fs=4 → DejaVuSansBold16 → font_h=26px, dim=52, boxH=26
// metaBG: top=0, height=50 → box_top=(50-26)/2=12
// metaConf szűkítve: bal +54px (52+2gap), jobb -54px → width=352
const ScrollConfig metaConf     PROGMEM = {{ TFT_FRAMEWDT+53,  12, 4, WA_CENTER }, 140, true,  MAX_WIDTH-116, 5000, 7, 40 };
const ScrollConfig title1Conf   PROGMEM = {{ TFT_FRAMEWDT, 49, 4, WA_CENTER   }, 140, true,  MAX_WIDTH, 5000, 7, 40 };
const ScrollConfig title2Conf   PROGMEM = {{ TFT_FRAMEWDT, 80, 4, WA_CENTER   }, 140, false, MAX_WIDTH, 5000, 7, 40 };  // textsize=1 → DejaVuSans11
const ScrollConfig playlistConf PROGMEM = {{ TFT_FRAMEWDT, 146, 1, WA_LEFT  }, 140, true,  MAX_WIDTH, 1000, 7, 40 };
const ScrollConfig apTitleConf  PROGMEM = {{ TFT_FRAMEWDT, TFT_FRAMEWDT, 4, WA_CENTER }, 140, false, MAX_WIDTH, 0, 7, 40 };
const ScrollConfig apSettConf   PROGMEM = {{ TFT_FRAMEWDT, 320-TFT_FRAMEWDT-16, 5, WA_CENTER }, 140, false, MAX_WIDTH, 0, 7, 40 };
const ScrollConfig weatherConf  PROGMEM = {{ 10, 113, 3, WA_CENTER }, 140, false, MAX_WIDTH+20, 0, 3, 60 };

/* BACKGROUNDS  */
const FillConfig   metaBGConf     PROGMEM = {{ 0, 0, 0, WA_LEFT }, DSP_WIDTH, 50, false };
const FillConfig   metaBGConfLine PROGMEM = {{3, 45, 0, WA_CENTER}, DSP_WIDTH - 6, 1, true};
const FillConfig   metaBGConfInv  PROGMEM = {{ 0, 50, 0, WA_LEFT }, DSP_WIDTH, 2, false };
const FillConfig   volbarConf     PROGMEM = {{ 0, DSP_HEIGHT-19, 0, WA_LEFT }, DSP_WIDTH, 8, true };
const FillConfig  playlBGConf    PROGMEM = {{ 0, 138, 0, WA_LEFT }, DSP_WIDTH, 36, false };
const FillConfig  heapbarConf   PROGMEM = {{ 0, DSP_HEIGHT-7, 0, WA_LEFT }, DSP_WIDTH, 6, false };

/* WIDGETS  */
const WidgetConfig bootstrConf    PROGMEM = { 0, 235, 2, WA_CENTER };
const WidgetConfig bitrateConf    PROGMEM = { 0, 63, 2, WA_RIGHT };
const WidgetConfig voltxtConf     PROGMEM = { 0, DSP_HEIGHT-38, 2, WA_CENTER };
const WidgetConfig  iptxtConf     PROGMEM = { TFT_FRAMEWDT, DSP_HEIGHT-38, 2, WA_LEFT };
const WidgetConfig   rssiConf     PROGMEM = { TFT_FRAMEWDT-5, DSP_HEIGHT-38, 2, WA_RIGHT };
const WidgetConfig numConf        PROGMEM = { 0, 200, 70, WA_CENTER };
const WidgetConfig apNameConf     PROGMEM = { TFT_FRAMEWDT, 88, 3, WA_CENTER };
const WidgetConfig apName2Conf    PROGMEM = { TFT_FRAMEWDT, 120, 3, WA_CENTER };
const WidgetConfig apPassConf     PROGMEM = { TFT_FRAMEWDT, 173, 3, WA_CENTER };
const WidgetConfig apPass2Conf    PROGMEM = { TFT_FRAMEWDT, 205, 3, WA_CENTER };

const WidgetConfig bootWdtConf    PROGMEM = { 0, 216, 2, WA_CENTER };
const ProgressConfig bootPrgConf  PROGMEM = { 90, 14, 4 };

/* BITRÁTA */
inline BitrateConfig getfullbitrateConf() {
  switch (config.store.vuLayout) {
    case 1: return {{TFT_FRAMEWDT+5, 210, 2, WA_LEFT}, 45 };
    case 2: return {{TFT_FRAMEWDT+5, 210, 2, WA_LEFT}, 45 };
    case 3: return {{TFT_FRAMEWDT+5, 210, 2, WA_LEFT}, 45 };
    default: return {{DSP_WIDTH-TFT_FRAMEWDT-95, 150, 2, WA_RIGHT}, 45 };
  }
}

/* ÁLLOMÁS SORSZÁM (outline box, bal – meta sor) */
// metaBG: top=0, height=50, box_h=26 → box_top=12
inline StationNumConfig getstationNumConf() {
  return {{ 3, 12, 2, WA_LEFT }, 55 };
}

/* LEJÁTSZÁS MÓD (filled box, jobb – meta sor) */
inline PlayModeConfig getplayModeConf() {
  return {{ DSP_WIDTH - 3 - 55, 12, 2, WA_LEFT }, 55 };
}

/* BANDS  */
inline VUBandsConfig getbandsConf() {
  switch (config.store.vuLayout) {
    case 1: return { 220, config.store.vuBarHeightStr, 20, config.store.vuBarGapStr,  config.store.vuBarCountStr,  config.store.vuFadeSpeedStr };
    case 2: return { 220, config.store.vuBarHeightBbx, 20, config.store.vuBarGapBbx,  config.store.vuBarCountBbx,  config.store.vuFadeSpeedBbx };
    case 3: return { 460, config.store.vuBarHeightStd, config.store.vuBarGapStd, 3, config.store.vuBarCountStd,  config.store.vuFadeSpeedStd };
    default: return { config.store.vuBarHeightDef, 132, 4, config.store.vuBarGapDef,  config.store.vuBarCountDef,  config.store.vuFadeSpeedDef };
  }
}

inline WidgetConfig getvuConf() {
  switch (config.store.vuLayout) {
    case 1: case 2: case 3: return { TFT_FRAMEWDT, DSP_HEIGHT - 70, 1, WA_CENTER };
    default: return { TFT_FRAMEWDT, 138, 1, WA_LEFT };
  }
}

inline WidgetConfig getclockConf() {
  switch (config.store.vuLayout) {
    case 1: case 2: case 3: return { TFT_FRAMEWDT+80, 205, 70, WA_RIGHT };
    default: return { TFT_FRAMEWDT+90, 245, 70, WA_RIGHT };
  }
}

inline WidgetConfig getWeatherIconConf() {
  switch (config.store.vuLayout) {
    case 1: case 2: case 3: return { TFT_FRAMEWDT+8, 138, 2, WA_LEFT };
    default: return { DSP_WIDTH-TFT_FRAMEWDT-65, 200, 2, WA_RIGHT };
  }
}

static constexpr ScrollConfig kDateBase = {
  { TFT_FRAMEWDT+65, 245, 2, WA_RIGHT }, 128, false, 304, 5000, 1, 50 };

static inline uint16_t dateTopByLayout(uint8_t ly) {
  switch (ly) { case 1: case 2: case 3: return 215; default: return 255; }
}
static inline uint16_t dateLeftByLayout(uint8_t ly) {
  switch (ly) { case 1: case 2: case 3: return TFT_FRAMEWDT+90; default: return TFT_FRAMEWDT+95; }
}

inline ScrollConfig getDateConf(uint8_t ly) {
  ScrollConfig c = kDateBase;
  c.widget.top  = dateTopByLayout(ly);
  c.widget.left = dateLeftByLayout(ly);
  return c;
}
inline ScrollConfig getDateConf() { return getDateConf(config.store.vuLayout); }

/* STRINGS  */
const char         numtxtFmt[]    PROGMEM = "%d";
const char           rssiFmt[]    PROGMEM = "WiFi %ddBm";
const char          iptxtFmt[]    PROGMEM = "IP: %s";
const char         voltxtFmt[]    PROGMEM = "Vol: %d%%";
const char        bitrateFmt[]    PROGMEM = "%d kBs";

/* MOVES  */
inline MoveConfig getclockMove() {
  switch (config.store.vuLayout) {
    case 1: case 2: case 3: return { TFT_FRAMEWDT+70, 205, -1 };
    default: return { TFT_FRAMEWDT+90, 245, -1 };
  }
}

inline MoveConfig getdateMove() {
  const ScrollConfig& dc = getDateConf();
  const auto cc = getclockConf();
  const auto cm = getclockMove();
  MoveConfig m;
  m.x = (uint16_t)((int16_t)dc.widget.left + (int16_t)cm.x - (int16_t)cc.left);
  m.y = (uint16_t)((int16_t)dc.widget.top  + (int16_t)cm.y - (int16_t)cc.top);
  m.width = dc.width;
  return m;
}

const MoveConfig   weatherMove    PROGMEM = { TFT_FRAMEWDT, 113, MAX_WIDTH };
const MoveConfig   weatherMoveVU  PROGMEM = { TFT_FRAMEWDT, 113, MAX_WIDTH };


// STATUS WIDGETEK (SPEECH / BLFADE / LSTRIP)
// idx: 0=SPEECH, 1=BLFADE, 2=LSTRIP
// Default layout (vuLayout==0): az óra felett, a fullbitrate widgettel egy sorban,
//   tőle balra elhelyezve egymás mellé.
// Egyéb layoutok (1-3): képernyő jobb szélén, a dátum pozíciójától felfelé,
//   egymás alatt.
inline StatusWidgetConfig getstatusConf(uint8_t idx) {
  const uint16_t W  = 55;   // doboz szélessége px
  const uint16_t H  = 18;   // doboz magassága px
  const uint8_t  TS = 6;    // textsize

  if (config.store.vuLayout == 0) {
    // Default layout: fullbitrate default: left=DSP_WIDTH-TFT_FRAMEWDT-95, top=125, dim=45
    // rightEdge: fullbitrate bal széle - kis hézag
    uint16_t rightEdge = DSP_WIDTH - TFT_FRAMEWDT - 140;
    uint16_t left = (uint16_t)(rightEdge - (uint16_t)((2 - idx) * (W + 20)));
    return {{ (uint16_t)(left - 35), 150, TS, WA_LEFT }, W, H };
  } else {
    // Layout 1-3: jobb szélen, dátumtól felfelé egymás alatt
    // idx=0 (SPEECH) legfelül, idx=2 (LSTRIP) legalul (legközelebb a dátumhoz)
    uint16_t baseTop = dateTopByLayout(config.store.vuLayout);
    uint16_t top = (uint16_t)(baseTop - (uint16_t)((3 - idx) * (H + 10)));
    return {{ (uint16_t)(DSP_WIDTH - TFT_FRAMEWDT - W), (uint16_t)(top + 20), TS, WA_LEFT }, W, H };
  }
}

#endif