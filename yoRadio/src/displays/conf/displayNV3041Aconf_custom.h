/*************************************************************************************
    NV3041A 480x272 – DejaVu Sans fontokhoz igazított koordináták
    
    yAdvance értékek (DejaVu, FT_RENDER_MODE_MONO, 96dpi):
      DejaVuSansBold14 = 23px  → metaConf   (textsize=3)
      DejaVuSansBold10 = 17px  → title1Conf (textsize=2)
      DejaVuSans11     = 18px  → title2Conf (textsize=1 ← változott!)
      DejaVuSans8      = 13px  → ip/rssi/vol%
    
    Minden más (VU layoutok, óra, időjárás, dátum, moves) változatlan.
*************************************************************************************/

#ifndef displayNV3041Aconf_h
#define displayNV3041Aconf_h

#include "../../core/config.h"

#define DSP_WIDTH       480
#define DSP_HEIGHT      272
#define TFT_FRAMEWDT    10
#define MAX_WIDTH       DSP_WIDTH-TFT_FRAMEWDT*2

#if BITRATE_FULL
  #define TITLE_FIX 44
#else
  #define TITLE_FIX 0
#endif
#define bootLogoTop     60

#ifndef BATTERY_OFF
  #define BatX      325
  #define BatY      DSP_HEIGHT-38
  #define BatFS     2
  #define ProcX     375
  #define ProcY     DSP_HEIGHT-38
  #define ProcFS    2
  #define VoltX     230
  #define VoltY     DSP_HEIGHT-38
  #define VoltFS    2
#endif

/* SCROLLOK */
/* top = szövegzóna teteje; baseline eltolást a widget kezeli */
const ScrollConfig metaConf     PROGMEM = {{ TFT_FRAMEWDT+53,  2, 4, WA_CENTER }, 140, true,  MAX_WIDTH-112, 5000, 7, 40 };
const ScrollConfig title1Conf   PROGMEM = {{ TFT_FRAMEWDT, 35, 2, WA_CENTER   }, 140, true,  MAX_WIDTH, 5000, 7, 40 };
const ScrollConfig title2Conf   PROGMEM = {{ TFT_FRAMEWDT, 56, 1, WA_CENTER   }, 140, false, MAX_WIDTH, 5000, 7, 40 };  // textsize=1 → DejaVuSans11
const ScrollConfig playlistConf PROGMEM = {{ TFT_FRAMEWDT, 146, 1, WA_LEFT  }, 140, true,  MAX_WIDTH, 1000, 7, 40 };
const ScrollConfig apTitleConf  PROGMEM = {{ TFT_FRAMEWDT, TFT_FRAMEWDT, 4, WA_CENTER }, 140, false, MAX_WIDTH, 0, 7, 40 };
const ScrollConfig apSettConf   PROGMEM = {{ TFT_FRAMEWDT, 320-TFT_FRAMEWDT-16, 5, WA_LEFT }, 140, false, MAX_WIDTH, 0, 7, 40 };
const ScrollConfig weatherConf  PROGMEM = {{ 10, 80, 1, WA_CENTER }, 140, false, MAX_WIDTH+20, 0, 3, 60 };

/* HÁTTEREK */
const FillConfig metaBGConf     PROGMEM = {{ 0,  0, 0, WA_LEFT }, DSP_WIDTH, 30, false };   // 23px + 2px
const FillConfig metaBGConfLine PROGMEM = {{ 3, 29, 0, WA_CENTER}, DSP_WIDTH - 6, 1, true};
const FillConfig metaBGConfInv  PROGMEM = {{ 0, 30, 0, WA_LEFT }, DSP_WIDTH, 2, false };
const FillConfig volbarConf     PROGMEM = {{ TFT_FRAMEWDT, DSP_HEIGHT-TFT_FRAMEWDT-8, 0, WA_LEFT }, MAX_WIDTH, 8, true };
const FillConfig playlBGConf    PROGMEM = {{ 0, 138, 0, WA_LEFT }, DSP_WIDTH, 36, false };
const FillConfig heapbarConf    PROGMEM = {{ 0, DSP_HEIGHT-2, 0, WA_LEFT }, DSP_WIDTH, 2, false };

/* WIDGETEK */
const WidgetConfig bootstrConf  PROGMEM = { 0, 243, 5, WA_CENTER };
const WidgetConfig bitrateConf  PROGMEM = { 6, 62, 2, WA_RIGHT };
const WidgetConfig voltxtConf   PROGMEM = { 0, DSP_HEIGHT-38, 5, WA_CENTER };
const WidgetConfig iptxtConf    PROGMEM = { TFT_FRAMEWDT, DSP_HEIGHT-38, 5, WA_LEFT };
const WidgetConfig rssiConf     PROGMEM = { TFT_FRAMEWDT, DSP_HEIGHT-38, 5, WA_RIGHT };
const WidgetConfig numConf      PROGMEM = { 0, 170, 1, WA_CENTER };
const WidgetConfig apNameConf   PROGMEM = { TFT_FRAMEWDT, 88, 3, WA_CENTER };
const WidgetConfig apName2Conf  PROGMEM = { TFT_FRAMEWDT, 120, 3, WA_CENTER };
const WidgetConfig apPassConf   PROGMEM = { TFT_FRAMEWDT, 173, 3, WA_CENTER };
const WidgetConfig apPass2Conf  PROGMEM = { TFT_FRAMEWDT, 205, 3, WA_CENTER };
const WidgetConfig bootWdtConf  PROGMEM = { 0, 216, 1, WA_CENTER };
const ProgressConfig bootPrgConf PROGMEM = { 90, 14, 4 };

/* BITRÁTA, VU, ÓRA, IDŐJÁRÁS, DÁTUM, MOVES – mind változatlan */
inline BitrateConfig getfullbitrateConf() {
  switch (config.store.vuLayout) {
    case 1: return {{TFT_FRAMEWDT+5, 170, 2, WA_LEFT}, 40 };
    case 2: return {{TFT_FRAMEWDT+5, 170, 2, WA_LEFT}, 40 };
    case 3: return {{TFT_FRAMEWDT+5, 170, 2, WA_LEFT}, 40 };
    default: return {{DSP_WIDTH-TFT_FRAMEWDT-70, 108, 2, WA_RIGHT}, 38 };
  }
}

/* ÁLLOMÁS SORSZÁM (outline box, bal – meta sor) */
// metaBG: top=0, height=50, box_h=26 → box_top=12
inline StationNumConfig getstationNumConf() {
  return {{ 3, 2, 2, WA_LEFT }, 52 };
}

/* LEJÁTSZÁS MÓD (filled box, jobb – meta sor) */
inline PlayModeConfig getplayModeConf() {
  return {{ DSP_WIDTH - 3 - 52, 2, 2, WA_LEFT }, 52 };
}

inline VUBandsConfig getbandsConf() {
  switch (config.store.vuLayout) {
    case 1: return { 220, config.store.vuBarHeightStr, 20, config.store.vuBarGapStr,  config.store.vuBarCountStr,  config.store.vuFadeSpeedStr };
    case 2: return { 220, config.store.vuBarHeightBbx, 20, config.store.vuBarGapBbx,  config.store.vuBarCountBbx,  config.store.vuFadeSpeedBbx };
    case 3: return { 460, config.store.vuBarHeightStd, config.store.vuBarGapStd, 3, config.store.vuBarCountStd,  config.store.vuFadeSpeedStd };
    default: return { config.store.vuBarHeightDef, 135, 6, config.store.vuBarGapDef,  config.store.vuBarCountDef,  config.store.vuFadeSpeedDef };
  }
}
inline WidgetConfig getvuConf() {
  switch (config.store.vuLayout) {
    case 1: return { TFT_FRAMEWDT, DSP_HEIGHT - 70, 1, WA_CENTER };
    case 2: return { TFT_FRAMEWDT, DSP_HEIGHT - 70, 1, WA_CENTER };
    case 3: return { TFT_FRAMEWDT, DSP_HEIGHT - 70, 1, WA_CENTER };
    default: return { TFT_FRAMEWDT, 110, 1, WA_LEFT };
  }
}
inline WidgetConfig getclockConf() {
  switch (config.store.vuLayout) {
    case 1: return { TFT_FRAMEWDT+90, 170, 70, WA_RIGHT };
    case 2: return { TFT_FRAMEWDT+90, 170, 70, WA_RIGHT };
    case 3: return { TFT_FRAMEWDT+90, 170, 70, WA_RIGHT };
    default: return { TFT_FRAMEWDT+95, 213, 70, WA_RIGHT };
  }
}
inline WidgetConfig getWeatherIconConf() {
  switch (config.store.vuLayout) {
    case 1: return { TFT_FRAMEWDT+8, 102, 2, WA_LEFT };
    case 2: return { TFT_FRAMEWDT+8, 102, 2, WA_LEFT };
    case 3: return { TFT_FRAMEWDT+8, 102, 2, WA_LEFT };
    default: return { DSP_WIDTH-TFT_FRAMEWDT-65, 156, 2, WA_RIGHT };
  }
}
static constexpr ScrollConfig kDateBase = {
  { TFT_FRAMEWDT+95, 205, 1, WA_RIGHT }, 128, false, 304, 5000, 1, 50 };
static inline uint16_t dateTopByLayout(uint8_t ly) {
  switch (ly) { case 1: case 2: case 3: return 175; default: return 200; }
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

const char numtxtFmt[] PROGMEM = "%d";
const char   rssiFmt[] PROGMEM = "WiFi %ddBm";
const char  iptxtFmt[] PROGMEM = "IP: %s";
const char voltxtFmt[] PROGMEM = "Vol: %d%%";
const char bitrateFmt[] PROGMEM = "%d kBs";

inline MoveConfig getclockMove() {
  switch (config.store.vuLayout) {
    case 1: case 2: case 3: return { TFT_FRAMEWDT+90, 170, -1 };
    default: return { TFT_FRAMEWDT+95, 213, -1 };
  }
}
inline MoveConfig getdateMove() {
  const ScrollConfig& dc = getDateConf();
  const auto cc = getclockConf(); const auto cm = getclockMove();
  MoveConfig m;
  m.x = (uint16_t)((int16_t)dc.widget.left + (int16_t)cm.x - (int16_t)cc.left);
  m.y = (uint16_t)((int16_t)dc.widget.top  + (int16_t)cm.y - (int16_t)cc.top);
  m.width = dc.width;
  return m;
}
const MoveConfig weatherMove   PROGMEM = { TFT_FRAMEWDT, 80, MAX_WIDTH };
const MoveConfig weatherMoveVU PROGMEM = { TFT_FRAMEWDT, 80, MAX_WIDTH };

// STATUS WIDGETEK (SPEECH / BLFADE / LSTRIP)
// idx: 0=SPEECH, 1=BLFADE, 2=LSTRIP
// Default layout (vuLayout==0): az óra felett, a fullbitrate widgettel egy sorban,
//   tőle balra elhelyezve egymás mellé.
// Egyéb layoutok (1-3): képernyő jobb szélén, a dátum pozíciójától felfelé,
//   egymás alatt.
inline StatusWidgetConfig getstatusConf(uint8_t idx) {
  const uint16_t W  = 55;   // doboz szélessége px
  const uint16_t H  = 18;   // doboz magassága px
  const uint8_t  TS = 6;    // textsize (legkisebb yoScrollFont szint)

  if (config.store.vuLayout == 0) {
    // Default layout: fullbitrate bal széle = DSP_WIDTH-TFT_FRAMEWDT-73
    // 3 doboz tőle balra, jobbról balra: idx=2 legközelebb, idx=0 legtávolabb
    uint16_t rightEdge = DSP_WIDTH - TFT_FRAMEWDT - 175;
    uint16_t left = (uint16_t)(rightEdge - (uint16_t)((2 - idx) * (W + 20)));
    return {{ (uint16_t)(left), 108, TS, WA_LEFT }, W, H };
  } else {
    // Layout 1-3: jobb szélen, dátumtól felfelé egymás alatt
    // idx=0 (SPEECH) legfelül, idx=2 (LSTRIP) legalul (legközelebb a dátumhoz)
    uint16_t baseTop = dateTopByLayout(config.store.vuLayout);
    uint16_t top = (uint16_t)(baseTop - (uint16_t)((3 - idx) * (H + 10)));
    return {{ (uint16_t)(DSP_WIDTH - TFT_FRAMEWDT - W), (uint16_t)(top + 20), TS, WA_LEFT }, W, H };
  }
}

#endif
