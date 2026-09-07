/*************************************************************************************
    NV3007 428x142 display configuration.
    Initial layout adapted from the supplied NV3007 configuration for Fusion v0.6.9.
*************************************************************************************/

#ifndef displayNV3007_142conf_h
#define displayNV3007_142conf_h

#include "../../core/config.h"

#define DSP_WIDTH       428
#define DSP_HEIGHT      142
#define TFT_FRAMEWDT    4
#define MAX_WIDTH       (DSP_WIDTH - TFT_FRAMEWDT * 2)

#define bootLogoTop     28

/* SCROLLING TEXT */
const ScrollConfig metaConf     PROGMEM = {{ TFT_FRAMEWDT + 46, 4, 3, WA_CENTER }, 140, true, MAX_WIDTH - 92, 5000, 5, 30 };
const ScrollConfig title1Conf   PROGMEM = {{ TFT_FRAMEWDT, 30, 2, WA_LEFT }, 140, true, MAX_WIDTH - 165, 5000, 4, 30 };
const ScrollConfig title2Conf   PROGMEM = {{ TFT_FRAMEWDT, 50, 2, WA_LEFT }, 140, true, MAX_WIDTH - 165, 5000, 4, 30 };
const ScrollConfig playlistConf PROGMEM = {{ TFT_FRAMEWDT, 112, 2, WA_LEFT }, 140, true, MAX_WIDTH, 1000, 4, 30 };
const ScrollConfig apTitleConf  PROGMEM = {{ TFT_FRAMEWDT, TFT_FRAMEWDT, 3, WA_CENTER }, 140, false, MAX_WIDTH, 0, 4, 20 };
const ScrollConfig apSettConf   PROGMEM = {{ TFT_FRAMEWDT, DSP_HEIGHT - 18, 2, WA_LEFT }, 140, false, MAX_WIDTH, 0, 4, 30 };
const ScrollConfig weatherConf  PROGMEM = {{ TFT_FRAMEWDT, 89, 5, WA_LEFT }, 140, false, MAX_WIDTH, 0, 2, 30 };

/* BACKGROUNDS */
const FillConfig metaBGConf     PROGMEM = {{ 0, 0, 0, WA_LEFT }, DSP_WIDTH, 29, false };
const FillConfig metaBGConfLine PROGMEM = {{ 3, 28, 0, WA_CENTER }, DSP_WIDTH - 6, 1, true };
const FillConfig metaBGConfInv  PROGMEM = {{ 0, 29, 0, WA_LEFT }, DSP_WIDTH, 1, false };
const FillConfig volbarConf     PROGMEM = {{ TFT_FRAMEWDT, DSP_HEIGHT - TFT_FRAMEWDT - 3, 0, WA_LEFT }, MAX_WIDTH, 4, true };
const FillConfig playlBGConf    PROGMEM = {{ 0, 107, 0, WA_LEFT }, DSP_WIDTH, 24, false };
const FillConfig heapbarConf    PROGMEM = {{ TFT_FRAMEWDT, DSP_HEIGHT - 2, 0, WA_LEFT }, MAX_WIDTH, 2, false };

/* WIDGETS */
const WidgetConfig bootstrConf PROGMEM = { 0, DSP_HEIGHT - 20, 5, WA_CENTER };
const WidgetConfig bitrateConf PROGMEM = { TFT_FRAMEWDT + 120, DSP_HEIGHT - 20, 2, WA_RIGHT };
const WidgetConfig iptxtConf   PROGMEM = { TFT_FRAMEWDT, DSP_HEIGHT - 18, 0, WA_LEFT };
const WidgetConfig voltxtConf  PROGMEM = { 210, DSP_HEIGHT - 18, 0, WA_LEFT };
const WidgetConfig rssiConf    PROGMEM = { TFT_FRAMEWDT, DSP_HEIGHT - 18, 0, WA_RIGHT };
const WidgetConfig numConf     PROGMEM = { TFT_FRAMEWDT, 95, 0, WA_CENTER };
const WidgetConfig apNameConf  PROGMEM = { 0, 36, 2, WA_CENTER };
const WidgetConfig apName2Conf PROGMEM = { 0, 56, 2, WA_CENTER };
const WidgetConfig apPassConf  PROGMEM = { 0, 83, 2, WA_CENTER };
const WidgetConfig apPass2Conf PROGMEM = { 0, 103, 2, WA_CENTER };
const WidgetConfig bootWdtConf PROGMEM = { 0, DSP_HEIGHT - 40, 1, WA_CENTER };
const ProgressConfig bootPrgConf PROGMEM = { 90, 10, 4 };

inline BitrateConfig getfullbitrateConf() {
  return {{ 320, 78, 2, WA_LEFT }, 50 };
}

inline StationNumConfig getstationNumConf() {
  return {{ TFT_FRAMEWDT, 4, 2, WA_LEFT }, 42 };
}

inline PlayModeConfig getplayModeConf() {
  return {{ DSP_WIDTH - TFT_FRAMEWDT - 42, 4, 2, WA_LEFT }, 42 };
}

inline VUBandsConfig getbandsConf() {
  switch (config.store.vuLayout) {
    case 1: return { 200, config.store.vuBarHeightStr, 15, config.store.vuBarGapStr, config.store.vuBarCountStr, config.store.vuFadeSpeedStr };
    case 2: return { 200, config.store.vuBarHeightBbx, 15, config.store.vuBarGapBbx, config.store.vuBarCountBbx, config.store.vuFadeSpeedBbx };
    case 3: return { MAX_WIDTH, config.store.vuBarHeightStd, config.store.vuBarGapStd, 3, config.store.vuBarCountStd, config.store.vuFadeSpeedStd };
    default: return { config.store.vuBarHeightDef, 80, 6, config.store.vuBarGapDef, config.store.vuBarCountDef, config.store.vuFadeSpeedDef };
  }
}

inline WidgetConfig getvuConf() {
  switch (config.store.vuLayout) {
    case 1:
    case 2:
    case 3: return { TFT_FRAMEWDT, 110, 1, WA_CENTER };
    default: return { TFT_FRAMEWDT, DSP_HEIGHT - 27, 1, WA_LEFT };
  }
}

inline WidgetConfig getclockConf() {
  return { TFT_FRAMEWDT, 69, 1, WA_RIGHT }; //69
}

static constexpr ScrollConfig kDateBase = {
  { TFT_FRAMEWDT, 70, 5, WA_LEFT }, 192, false, MAX_WIDTH, 5000, 1, 50
};

inline ScrollConfig getDateConf(uint8_t layout) {
  (void)layout;
  return kDateBase;
}

inline ScrollConfig getDateConf() {
  return getDateConf(config.store.vuLayout);
}

const char numtxtFmt[] PROGMEM = "%d";
const char rssiFmt[] PROGMEM = "WiFi %ddBm";
const char iptxtFmt[] PROGMEM = "\010 %s";
const char voltxtFmt[] PROGMEM = "\023\025%d%%";
const char bitrateFmt[] PROGMEM = "%d kBs";

inline MoveConfig getclockMove() {
  return { TFT_FRAMEWDT, 69, -1 };
}

inline MoveConfig getdateMove() {
  const ScrollConfig dc = getDateConf();
  const WidgetConfig cc = getclockConf();
  const MoveConfig cm = getclockMove();
  MoveConfig move;
  move.x = (uint16_t)((int16_t)dc.widget.left + (int16_t)cm.x - (int16_t)cc.left);
  move.y = (uint16_t)((int16_t)dc.widget.top + (int16_t)cm.y - (int16_t)cc.top);
  move.width = dc.width;
  return move;
}

const MoveConfig weatherMove   PROGMEM = { TFT_FRAMEWDT, 89, MAX_WIDTH - 110 };
const MoveConfig weatherMoveVU PROGMEM = { TFT_FRAMEWDT, 89, MAX_WIDTH - 110 };

#endif
