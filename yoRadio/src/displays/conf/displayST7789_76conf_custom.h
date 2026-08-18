/*************************************************************************************
    ST7789 284x76 displays configuration file.
*************************************************************************************/

#ifndef displayST7789_76conf_h
#define displayST7789_76conf_h

#include "../../core/config.h"

#define DSP_WIDTH       284
#define DSP_HEIGHT      76
#define TFT_FRAMEWDT    2
#define MAX_WIDTH       DSP_WIDTH-TFT_FRAMEWDT*2
#define INFO_SCROLL_WIDTH (DSP_WIDTH/2+50)

#define bootLogoTop     4

#define HIDE_IP
#define HIDE_VOL

#ifndef BATTERY_OFF
  #define BatX      113
  #define BatY      51
  #define BatFS     1
  #define ProcX     137
  #define ProcY     51
  #define ProcFS    1
  #define VoltX     125
  #define VoltY     40
  #define VoltFS    1
#endif

/* SROLLS  */
// meta_fs=2 → DejaVuSansBold12 → font_h=19px, dim=38, boxH=19
// metaBG: top=0, height=18px – a sor pontosan 18px, boxH=19 → szorosan illeszkedik
// box_top=0 (a -1 helyett: a keret 1px kilóghat, a fillRect már kezel 0-t)
// metaConf szűkítve: bal +40px (dim=38 + 2px gap), jobb -40px
const ScrollConfig metaConf     PROGMEM = {{ TFT_FRAMEWDT+40, 2, 6, WA_CENTER }, 140, true, MAX_WIDTH-80, 5000, 2, 25 };
const ScrollConfig title1Conf     PROGMEM = {{ TFT_FRAMEWDT, 19, 0, WA_LEFT }, 140, false, DSP_WIDTH/2+23, 5000, 2, 25 };
const ScrollConfig title2Conf     PROGMEM = {{ TFT_FRAMEWDT, 32, 0, WA_LEFT }, 140, false, DSP_WIDTH/2+23, 5000, 2, 25 };
const ScrollConfig playlistConf   PROGMEM = {{ TFT_FRAMEWDT, 30, 0, WA_LEFT }, 140, false, MAX_WIDTH, 500, 2, 25 };
const ScrollConfig apTitleConf   PROGMEM = {{ TFT_FRAMEWDT, TFT_FRAMEWDT, 2, WA_CENTER }, 140, false, MAX_WIDTH, 0, 2, 25 };
const ScrollConfig apSettConf   PROGMEM = {{ TFT_FRAMEWDT+10, 54, 0, WA_LEFT }, 140, false, MAX_WIDTH-(TFT_FRAMEWDT+20), 0, 2, 25 };
const ScrollConfig weatherConf PROGMEM = {{ TFT_FRAMEWDT, DSP_HEIGHT-15, 0, WA_LEFT }, 140, false, MAX_WIDTH, 0, 1, 25 };

/* BACKGROUNDS  */
const FillConfig   metaBGConf     PROGMEM = {{ 0, 0,  0, WA_LEFT }, DSP_WIDTH, 17, false };
const FillConfig   metaBGConfLine PROGMEM = {{ 1, 17, 0, WA_CENTER }, DSP_WIDTH - 2, 1, true };
const FillConfig   metaBGConfInv  PROGMEM = {{ 0, 19, 0, WA_LEFT }, DSP_WIDTH, 1,  false };
const FillConfig   volbarConf     PROGMEM = {{ TFT_FRAMEWDT, DSP_HEIGHT-6, 0, WA_LEFT }, DSP_WIDTH-TFT_FRAMEWDT*2, 3, true };
const FillConfig  playlBGConf     PROGMEM = {{ 0, 26, 0, WA_LEFT }, DSP_WIDTH, 12, false };
const FillConfig  heapbarConf    PROGMEM = {{ 0, 74, 0, WA_LEFT }, DSP_WIDTH, 2, false };

/* WIDGETS  */
const WidgetConfig bootstrConf  PROGMEM = { 0, DSP_HEIGHT-20, 0, WA_CENTER };
const WidgetConfig bitrateConf   PROGMEM = { TFT_FRAMEWDT, 20, 0, WA_RIGHT };
const WidgetConfig  iptxtConf     PROGMEM = { TFT_FRAMEWDT, 40, 1, WA_LEFT };
const WidgetConfig   rssiConf      PROGMEM = { TFT_FRAMEWDT, 50, 1, WA_LEFT };
const WidgetConfig numConf      PROGMEM = { TFT_FRAMEWDT, 56, 35, WA_CENTER };
const WidgetConfig apNameConf     PROGMEM = { TFT_FRAMEWDT, 26, 0, WA_CENTER };  // "SSID : mynet"
const WidgetConfig apName2Conf    PROGMEM = { TFT_FRAMEWDT, 26, 0, WA_LEFT };    // nem jelenik meg
const WidgetConfig apPassConf     PROGMEM = { TFT_FRAMEWDT, 40, 0, WA_CENTER };  // "Pass : mypass"
const WidgetConfig apPass2Conf    PROGMEM = { TFT_FRAMEWDT, 40, 0, WA_LEFT };    // nem jelenik meg

const WidgetConfig bootWdtConf  PROGMEM = { 0, DSP_HEIGHT-23, 1, WA_CENTER };
const ProgressConfig bootPrgConf  PROGMEM = { 90, 10, 4 };

/* ÁLLOMÁS SORSZÁM (outline box, bal – meta sor) */
// metaBG height=18, boxH=19 – szorosan illeszkedik, box_top=0
inline StationNumConfig getstationNumConf() {
  return {{ TFT_FRAMEWDT, 0, 0, WA_LEFT }, 30 };
}

/* LEJÁTSZÁS MÓD (filled box, jobb – meta sor) */
inline PlayModeConfig getplayModeConf() {
  return {{ DSP_WIDTH - TFT_FRAMEWDT - 30, 0, 0, WA_LEFT }, 30 };
}

/* BANDS  */
inline VUBandsConfig getbandsConf() {
  switch (config.store.vuLayout) {
    case 1: return { DSP_WIDTH/2-TFT_FRAMEWDT-6, config.store.vuBarHeightStr, 10, config.store.vuBarGapStr,  config.store.vuBarCountStr,  config.store.vuFadeSpeedStr };
    case 2: return { DSP_WIDTH/2-TFT_FRAMEWDT-6, config.store.vuBarHeightBbx, 10, config.store.vuBarGapBbx,  config.store.vuBarCountBbx,  config.store.vuFadeSpeedBbx };
    case 3: return { DSP_WIDTH-TFT_FRAMEWDT*2, config.store.vuBarHeightStd, config.store.vuBarGapStd, 3, config.store.vuBarCountStd,  config.store.vuFadeSpeedStd };
    default: return { config.store.vuBarHeightDef, 80, 6, config.store.vuBarGapDef,  config.store.vuBarCountDef,  config.store.vuFadeSpeedDef };
  }
}

inline WidgetConfig getvuConf() {
  switch (config.store.vuLayout) {
    case 1: case 2: case 3: return { TFT_FRAMEWDT, DSP_HEIGHT-15, 1, WA_CENTER };
    default: return { TFT_FRAMEWDT, DSP_HEIGHT-15, 1, WA_LEFT };
  }
}

inline WidgetConfig getclockConf() {
  switch (config.store.vuLayout) {
    case 1: case 2: case 3: default: return { 0, 54, 35, WA_RIGHT };
  }
}

static constexpr ScrollConfig kDateBase = {
  { TFT_FRAMEWDT, 46, 0, WA_LEFT }, 192, false, INFO_SCROLL_WIDTH, 5000, 1, 50 };

static inline uint16_t dateTopByLayout(uint8_t ly) {
  (void)ly; return 46;
}
static inline uint16_t dateLeftByLayout(uint8_t ly) {
  (void)ly; return TFT_FRAMEWDT;
}

inline ScrollConfig getDateConf(uint8_t ly) {
  ScrollConfig c = kDateBase;
  c.widget.top  = dateTopByLayout(ly);
  return c;
}
inline ScrollConfig getDateConf() { return getDateConf(config.store.vuLayout); }

/* STRINGS  */
const char         numtxtFmt[]    PROGMEM = "%d";
const char           rssiFmt[]    PROGMEM = "%d";
const char          iptxtFmt[]    PROGMEM = "%s";
const char        bitrateFmt[]    PROGMEM = "%d kBs";

/* MOVES  */
inline MoveConfig getclockMove() {
  switch (config.store.vuLayout) {
    case 1: case 2: case 3: default: return { 0, 54, -1 };
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

const MoveConfig   weatherMove    PROGMEM = {TFT_FRAMEWDT, 46, INFO_SCROLL_WIDTH };
const MoveConfig   weatherMoveVU  PROGMEM = {TFT_FRAMEWDT, 46, INFO_SCROLL_WIDTH };

#endif
