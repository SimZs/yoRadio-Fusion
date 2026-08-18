/*************************************************************************************
    ST7735 160x128 displays configuration file.
*************************************************************************************/
#ifndef displayST7735conf_h
#define displayST7735conf_h

#include "../../core/config.h"

#define DSP_WIDTH       160
#define TFT_FRAMEWDT    4
#define MAX_WIDTH       DSP_WIDTH-TFT_FRAMEWDT*2

#if BITRATE_FULL
  #define TITLE_FIX 24
#else
  #define TITLE_FIX 0
#endif
#define bootLogoTop     15

/* SROLLS  */
// meta_fs=2 → DejaVuSansBold12 → font_h=19px, dim=38, boxH=19
// metaBG: top=0, height=22, box_h=19 → box_top=(22-19)/2=1
// metaConf szűkítve: bal +40px (dim=38 + 2px gap), jobb -40px
// new meta width = 152 - 80 = 72px – szűk, de DLNA=4 karakter fs=2-vel ~28px, belefér középre
const ScrollConfig metaConf       PROGMEM = {{ TFT_FRAMEWDT, TFT_FRAMEWDT, 1, WA_CENTER }, 140, true, MAX_WIDTH-8, 5000, 3, 30 };
const ScrollConfig title1Conf     PROGMEM = {{ TFT_FRAMEWDT, 30, 0, WA_LEFT }, 140, true, MAX_WIDTH, 5000, 3, 30 };
const ScrollConfig title2Conf     PROGMEM = {{ TFT_FRAMEWDT, 45, 0, WA_LEFT }, 140, true, MAX_WIDTH, 5000, 3, 30 };
const ScrollConfig playlistConf   PROGMEM = {{ TFT_FRAMEWDT, 56, 0, WA_LEFT }, 140, true, MAX_WIDTH, 1000, 4, 30 };
const ScrollConfig apTitleConf    PROGMEM = {{ TFT_FRAMEWDT, TFT_FRAMEWDT, 2, WA_CENTER }, 140, false, MAX_WIDTH, 0, 4, 30 };
const ScrollConfig apSettConf     PROGMEM = {{ TFT_FRAMEWDT, 102, 0, WA_LEFT }, 140, false, MAX_WIDTH, 0, 3, 30 };
const ScrollConfig weatherConf    PROGMEM = {{ TFT_FRAMEWDT, 45, 0, WA_CENTER }, 140, true, MAX_WIDTH, 0, 2, 50 };

/* BACKGROUNDS  */
const FillConfig   metaBGConf     PROGMEM = {{ 0, 0, 0, WA_LEFT }, DSP_WIDTH, 29, false };
const FillConfig   metaBGConfLine PROGMEM = {{ 1, 28, 0, WA_CENTER }, DSP_WIDTH - 2, 1, true };
const FillConfig   metaBGConfInv  PROGMEM = {{ 0, 29, 0, WA_LEFT }, DSP_WIDTH, 1, false };
const FillConfig   volbarConf     PROGMEM = {{ TFT_FRAMEWDT, 118, 0, WA_LEFT }, MAX_WIDTH, 5, true };
const FillConfig  playlBGConf     PROGMEM = {{ 0, 52, 0, WA_LEFT }, DSP_WIDTH, 22, false };
const FillConfig  heapbarConf     PROGMEM = {{ 0, 127, 0, WA_LEFT }, DSP_WIDTH, 1, false };

/* WIDGETS  */
const WidgetConfig bootstrConf    PROGMEM = { 0, 113, 0, WA_CENTER };
const WidgetConfig bitrateConf    PROGMEM = { TFT_FRAMEWDT, 26, 0, WA_RIGHT };
const WidgetConfig voltxtConf     PROGMEM = { TFT_FRAMEWDT, 108, 0, WA_LEFT };
const WidgetConfig  iptxtConf     PROGMEM = { TFT_FRAMEWDT, 108, 0, WA_CENTER };
const WidgetConfig   rssiConf     PROGMEM = { TFT_FRAMEWDT, 108, 0, WA_RIGHT };
const WidgetConfig numConf        PROGMEM = { 0, 86, 0, WA_CENTER };
const WidgetConfig apNameConf     PROGMEM = { 0, 40, 0, WA_CENTER };
const WidgetConfig apName2Conf    PROGMEM = { 0, 54, 0, WA_CENTER };
const WidgetConfig apPassConf     PROGMEM = { 0, 74, 0, WA_CENTER };
const WidgetConfig apPass2Conf    PROGMEM = { 0, 88, 0, WA_CENTER };

const WidgetConfig bootWdtConf    PROGMEM = { 0, 90, 1, WA_CENTER };
const ProgressConfig bootPrgConf  PROGMEM = { 90, 14, 4 };

/* BITRÁTA */
inline BitrateConfig getfullbitrateConf() {
  switch (config.store.vuLayout) {
    case 1: return {{TFT_FRAMEWDT, 65, 1, WA_RIGHT}, 26 };
    case 2: return {{TFT_FRAMEWDT, 65, 1, WA_RIGHT}, 26 };
    case 3: return {{TFT_FRAMEWDT, 65, 1, WA_RIGHT}, 26 };
    default: return {{DSP_WIDTH-TFT_FRAMEWDT-52, 58, 1, WA_LEFT}, 26 };
  }
}

/* ÁLLOMÁS SORSZÁM (outline box, bal – meta sor) */
// metaBG: top=0, height=22, box_h=19 → box_top=1
inline StationNumConfig getstationNumConf() {
  return {{ TFT_FRAMEWDT, 1, 2, WA_LEFT }, 38 };
}

/* LEJÁTSZÁS MÓD (filled box, jobb – meta sor) */
inline PlayModeConfig getplayModeConf() {
  return {{ DSP_WIDTH - TFT_FRAMEWDT - 38, 1, 2, WA_LEFT }, 38 };
}

/* BANDS  */
inline VUBandsConfig getbandsConf() {
  switch (config.store.vuLayout) {
    case 1: return { 73, config.store.vuBarHeightStr, 6, config.store.vuBarGapStr,  config.store.vuBarCountStr,  config.store.vuFadeSpeedStr };
    case 2: return { 73, config.store.vuBarHeightBbx, 6, config.store.vuBarGapBbx,  config.store.vuBarCountBbx,  config.store.vuFadeSpeedBbx };
    case 3: return { 149, config.store.vuBarHeightStd, config.store.vuBarGapStd, 3, config.store.vuBarCountStd,  config.store.vuFadeSpeedStd };
    default: return { config.store.vuBarHeightDef, 44, 2, config.store.vuBarGapDef,  config.store.vuBarCountDef,  config.store.vuFadeSpeedDef };
  }
}

inline WidgetConfig getvuConf() {
  switch (config.store.vuLayout) {
    case 1: case 2: case 3: return { TFT_FRAMEWDT, 95, 1, WA_CENTER };
    default: return { TFT_FRAMEWDT, 58, 1, WA_LEFT };
  }
}

inline WidgetConfig getclockConf() {
  switch (config.store.vuLayout) {
    case 1: case 2: case 3: return { 8, 80, 0, WA_RIGHT };
    default: return { 4, 93, 0, WA_RIGHT };
  }
}

static constexpr ScrollConfig kDateBase = {
  { TFT_FRAMEWDT, 95, 0, WA_RIGHT }, 128, false, 200, 5000, 1, 50 };

static inline uint16_t dateTopByLayout(uint8_t ly) {
  switch (ly) { case 1: case 2: case 3: return 82; default: return 93; }
}
static inline uint16_t dateLeftByLayout(uint8_t ly) {
  switch (ly) { case 1: case 2: case 3: return 8; default: return TFT_FRAMEWDT; }
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
const char           rssiFmt[]    PROGMEM = "%d";
const char          iptxtFmt[]    PROGMEM = "%s";
const char         voltxtFmt[]    PROGMEM = "%d%%";
const char        bitrateFmt[]    PROGMEM = "%d";

/* MOVES  */
inline MoveConfig getclockMove() {
  switch (config.store.vuLayout) {
    case 1: case 2: case 3: return { 8, 80, -1 };
    default: return { 4, 93, -1 };
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

const MoveConfig   weatherMove    PROGMEM = {TFT_FRAMEWDT, 45, MAX_WIDTH};
const MoveConfig   weatherMoveVU  PROGMEM = {TFT_FRAMEWDT, 45, MAX_WIDTH};

#endif
