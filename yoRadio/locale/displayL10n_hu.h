#ifndef dsp_full_loc
#define dsp_full_loc

#include <pgmspace.h>
#include "../src/core/config.h"
#include "../myoptions.h"

const char mon[] PROGMEM = "hé";
const char tue[] PROGMEM = "ke";
const char wed[] PROGMEM = "sz";
const char thu[] PROGMEM = "cs";
const char fri[] PROGMEM = "pé";
const char sat[] PROGMEM = "SZ";
const char sun[] PROGMEM = "VA";

const char monf[] PROGMEM = "hétfő";
const char tuef[] PROGMEM = "kedd";
const char wedf[] PROGMEM = "szerda";
const char thuf[] PROGMEM = "csütörtök";
const char frif[] PROGMEM = "péntek";
const char satf[] PROGMEM = "szombat";
const char sunf[] PROGMEM = "vasárnap";

const char jan[]  PROGMEM = "január";
const char feb[]  PROGMEM = "február";
const char mar[]  PROGMEM = "március";
const char apr[]  PROGMEM = "április";
const char may[]  PROGMEM = "május";
const char jun[]  PROGMEM = "június";
const char jul[]  PROGMEM = "július";
const char aug[]  PROGMEM = "augusztus";
const char sep[]  PROGMEM = "szeptember";
const char octc[] PROGMEM = "október";
const char nov[]  PROGMEM = "november";
const char decc[] PROGMEM = "december";

// ============================================================
// WIND DIRECTIONS – SHORT
// ============================================================
const char wn_N_s[]   PROGMEM = "É";
const char wn_NNE_s[] PROGMEM = "É-ÉK";
const char wn_NE_s[]  PROGMEM = "ÉK";
const char wn_ENE_s[] PROGMEM = "K-ÉK";
const char wn_E_s[]   PROGMEM = "K";
const char wn_ESE_s[] PROGMEM = "K-DK";
const char wn_SE_s[]  PROGMEM = "DK";
const char wn_SSE_s[] PROGMEM = "DK-D";
const char wn_S_s[]   PROGMEM = "D";
const char wn_SSW_s[] PROGMEM = "D-DNY";
const char wn_SW_s[]  PROGMEM = "DNY";
const char wn_WSW_s[] PROGMEM = "NY-DNY";
const char wn_W_s[]   PROGMEM = "NY";
const char wn_WNW_s[] PROGMEM = "NY-ÉNY";
const char wn_NW_s[]  PROGMEM = "ÉNY";
const char wn_NNW_s[] PROGMEM = "ÉNY-NY";

const char *const wind_short[] PROGMEM = {
  wn_N_s, wn_NNE_s, wn_NE_s, wn_ENE_s,
  wn_E_s, wn_ESE_s, wn_SE_s, wn_SSE_s,
  wn_S_s, wn_SSW_s, wn_SW_s, wn_WSW_s,
  wn_W_s, wn_WNW_s, wn_NW_s, wn_NNW_s, wn_N_s
};

// ============================================================
// WIND DIRECTIONS – LONG
// ============================================================
const char wn_N_l[]   PROGMEM = "észak";
const char wn_NNE_l[] PROGMEM = "észak-északkelet";
const char wn_NE_l[]  PROGMEM = "északkelet";
const char wn_ENE_l[] PROGMEM = "kelet-északkelet";
const char wn_E_l[]   PROGMEM = "kelet";
const char wn_ESE_l[] PROGMEM = "kelet-délkelet";
const char wn_SE_l[]  PROGMEM = "délkelet";
const char wn_SSE_l[] PROGMEM = "délkelet-dél";
const char wn_S_l[]   PROGMEM = "dél";
const char wn_SSW_l[] PROGMEM = "dél-délnyugat";
const char wn_SW_l[]  PROGMEM = "délnyugat";
const char wn_WSW_l[] PROGMEM = "nyugat-délnyugat";
const char wn_W_l[]   PROGMEM = "nyugat";
const char wn_WNW_l[] PROGMEM = "nyugat-északnyugat";
const char wn_NW_l[]  PROGMEM = "északnyugat";
const char wn_NNW_l[] PROGMEM = "északnyugat-nyugat";

const char *const wind_long[] PROGMEM = {
  wn_N_l, wn_NNE_l, wn_NE_l, wn_ENE_l,
  wn_E_l, wn_ESE_l, wn_SE_l, wn_SSE_l,
  wn_S_l, wn_SSW_l, wn_SW_l, wn_WSW_l,
  wn_W_l, wn_WNW_l, wn_NW_l, wn_NNW_l, wn_N_l
};

// ============================================================
// WIND TABLE SELECTOR
// ============================================================
static inline const char *const *getWindTable() {
  return config.store.shortWeather ? wind_short : wind_long;
}

const char *const dow[]  PROGMEM = { sun, mon, tue, wed, thu, fri, sat };
const char *const dowf[] PROGMEM = { sunf, monf, tuef, wedf, thuf, frif, satf };
const char *const mnths[] PROGMEM = { jan, feb, mar, apr, may, jun, jul, aug, sep, octc, nov, decc };

const char const_PlReady[] PROGMEM = "[kész]";
const char const_PlStopped[] PROGMEM = "[stop]";
const char const_PlConnect[] PROGMEM = "";
const char const_DlgVolume[] PROGMEM = "Hangerő";
const char const_DlgLost[] PROGMEM = "* LESZAKADT *";
const char const_DlgUpdate[] PROGMEM = "* FRISSÍTES *";
const char const_DlgNextion[] PROGMEM = "* NEXTION *";
const char const_getWeather[] PROGMEM = "";
const char const_waitForSD[] PROGMEM = "INDEX SD";

const char apNameTxt[] PROGMEM = "WiFi AP";
const char apPassTxt[] PROGMEM = "Jelszó";
const char bootstrFmt[] PROGMEM = "Csatlakozás: %s";
const char apSettFmt[] PROGMEM = "A rádió elérhetősége: HTTP://%s/";


#if (DSP_MODEL == DSP_ILI9341) || (DSP_MODEL == DSP_ST7735) || (DSP_MODEL == DSP_ST7789) || (DSP_MODEL == DSP_ST7789_76)
const char weatherFmtShort[] PROGMEM =
  "%d hPa · %d%% RH · %.1f km/h";
#elif (DSP_MODEL == DSP_ST7789_240) || (DSP_MODEL==DSP_GC9A01) || (DSP_MODEL==DSP_GC9A01A) || (DSP_MODEL==DSP_GC9A01_I80)
const char weatherFmtShort[] PROGMEM =
  "%d hPa · %d%% RH";
#else
const char weatherFmtShort[] PROGMEM =
  "%d hPa · %d%% RH · %.1f km/h [%s]";
#endif

#if EXT_WEATHER
const char weatherFmtLong[] PROGMEM =
  "%s, %.1f°C · hőérzet: %.1f°C · légnyomás: %d hPa · páratartalom: %d%% · szélsebesség: %.1f km/h [%s]";
#else
const char weatherFmtLong[] PROGMEM =
  "%s, %.1f°C · %d hPa · %d%%";
#endif

static inline const char* getWeatherFmt() {
  return config.store.shortWeather ? weatherFmtShort : weatherFmtLong;
}

const char weatherUnits[] PROGMEM = "metric";
const char weatherLang[]  PROGMEM = "hu";

#endif
