#ifndef dsp_full_loc
#define dsp_full_loc

#include <pgmspace.h>
#include "../src/core/config.h"
#include "../myoptions.h"

const char mon[] PROGMEM = "lu";
const char tue[] PROGMEM = "ma";
const char wed[] PROGMEM = "mi";
const char thu[] PROGMEM = "jo";
const char fri[] PROGMEM = "vi";
const char sat[] PROGMEM = "sâ";
const char sun[] PROGMEM = "du";

const char monf[] PROGMEM = "luni";
const char tuef[] PROGMEM = "marți";
const char wedf[] PROGMEM = "miercuri";
const char thuf[] PROGMEM = "joi";
const char frif[] PROGMEM = "vineri";
const char satf[] PROGMEM = "sâmbătă";
const char sunf[] PROGMEM = "duminică";

const char jan[]  PROGMEM = "ianuarie";
const char feb[]  PROGMEM = "februarie";
const char mar[]  PROGMEM = "martie";
const char apr[]  PROGMEM = "aprilie";
const char may[]  PROGMEM = "mai";
const char jun[]  PROGMEM = "iunie";
const char jul[]  PROGMEM = "iulie";
const char aug[]  PROGMEM = "august";
const char sep[]  PROGMEM = "septembrie";
const char octc[] PROGMEM = "octombrie";
const char nov[]  PROGMEM = "noiembrie";
const char decc[] PROGMEM = "decembrie";

// ============================================================
// WIND DIRECTIONS – SHORT
// ============================================================
const char wn_N_s[]   PROGMEM = "N";
const char wn_NNE_s[] PROGMEM = "NNE";
const char wn_NE_s[]  PROGMEM = "NE";
const char wn_ENE_s[] PROGMEM = "ENE";
const char wn_E_s[]   PROGMEM = "E";
const char wn_ESE_s[] PROGMEM = "ESE";
const char wn_SE_s[]  PROGMEM = "SE";
const char wn_SSE_s[] PROGMEM = "SSE";
const char wn_S_s[]   PROGMEM = "S";
const char wn_SSW_s[] PROGMEM = "SSV";
const char wn_SW_s[]  PROGMEM = "SV";
const char wn_WSW_s[] PROGMEM = "VSV";
const char wn_W_s[]   PROGMEM = "V";
const char wn_WNW_s[] PROGMEM = "VNV";
const char wn_NW_s[]  PROGMEM = "NV";
const char wn_NNW_s[] PROGMEM = "NNV";

const char *const wind_short[] PROGMEM = {
  wn_N_s, wn_NNE_s, wn_NE_s, wn_ENE_s,
  wn_E_s, wn_ESE_s, wn_SE_s, wn_SSE_s,
  wn_S_s, wn_SSW_s, wn_SW_s, wn_WSW_s,
  wn_W_s, wn_WNW_s, wn_NW_s, wn_NNW_s, wn_N_s
};

// ============================================================
// WIND DIRECTIONS – LONG
// ============================================================
const char wn_N_l[]   PROGMEM = "nord";
const char wn_NNE_l[] PROGMEM = "nord-nord-est";
const char wn_NE_l[]  PROGMEM = "nord-est";
const char wn_ENE_l[] PROGMEM = "est-nord-est";
const char wn_E_l[]   PROGMEM = "est";
const char wn_ESE_l[] PROGMEM = "est-sud-est";
const char wn_SE_l[]  PROGMEM = "sud-est";
const char wn_SSE_l[] PROGMEM = "sud-sud-est";
const char wn_S_l[]   PROGMEM = "sud";
const char wn_SSW_l[] PROGMEM = "sud-sud-vest";
const char wn_SW_l[]  PROGMEM = "sud-vest";
const char wn_WSW_l[] PROGMEM = "vest-sud-vest";
const char wn_W_l[]   PROGMEM = "vest";
const char wn_WNW_l[] PROGMEM = "vest-nord-vest";
const char wn_NW_l[]  PROGMEM = "nord-vest";
const char wn_NNW_l[] PROGMEM = "nord-nord-vest";

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

const char const_PlReady[] PROGMEM = "[gata]";
const char const_PlStopped[] PROGMEM = "[stop]";
const char const_PlConnect[] PROGMEM = "";
const char const_DlgVolume[] PROGMEM = "Volum";
const char const_DlgLost[] PROGMEM = "* CONEXIUNE PIERDUTĂ *";
const char const_DlgUpdate[] PROGMEM = "* ACTUALIZARE *";
const char const_DlgNextion[] PROGMEM = "* NEXTION *";
const char const_getWeather[] PROGMEM = "";
const char const_waitForSD[] PROGMEM = "INDEX SD";

const char apNameTxt[] PROGMEM = "WiFi AP";
const char apPassTxt[] PROGMEM = "Parolă";
const char bootstrFmt[] PROGMEM = "Conectare: %s";
const char apSettFmt[] PROGMEM = "Radio disponibil la: HTTP://%s/";

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
  "%s, %.1f°C · resimțit: %.1f°C · presiune: %d hPa · umiditate: %d%% · viteză vânt: %.1f km/h [%s]";
#else
const char weatherFmtLong[] PROGMEM =
  "%s, %.1f°C · %d hPa · %d%%";
#endif

static inline const char* getWeatherFmt() {
  return config.store.shortWeather ? weatherFmtShort : weatherFmtLong;
}

const char weatherUnits[] PROGMEM = "metric";
const char weatherLang[]  PROGMEM = "ro";

#endif
