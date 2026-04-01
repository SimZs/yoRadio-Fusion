// v0.9.555
#ifndef dsp_full_loc
#define dsp_full_loc

#include <pgmspace.h>
#include "../src/core/config.h"
#include "../myoptions.h"

const char mon[] PROGMEM = "ma";
const char tue[] PROGMEM = "di";
const char wed[] PROGMEM = "wo";
const char thu[] PROGMEM = "do";
const char fri[] PROGMEM = "vr";
const char sat[] PROGMEM = "za";
const char sun[] PROGMEM = "zo";

const char monf[] PROGMEM = "maan";
const char tuef[] PROGMEM = "din";
const char wedf[] PROGMEM = "woe";
const char thuf[] PROGMEM = "don";
const char frif[] PROGMEM = "vrij";
const char satf[] PROGMEM = "zat";
const char sunf[] PROGMEM = "zon";

const char jan[] PROGMEM = "januari";
const char feb[] PROGMEM = "februari";
const char mar[] PROGMEM = "maart";
const char apr[] PROGMEM = "april";
const char may[] PROGMEM = "mei";
const char jun[] PROGMEM = "juni";
const char jul[] PROGMEM = "juli";
const char aug[] PROGMEM = "augustus";
const char sep[] PROGMEM = "september";
const char octc[] PROGMEM = "oktober";
const char nov[] PROGMEM = "november";
const char decc[] PROGMEM = "december";

// ============================================================
// WIND DIRECTIONS – SHORT
// ============================================================
const char wn_N_s[]   PROGMEM = "N";
const char wn_NNE_s[] PROGMEM = "NNO";
const char wn_NE_s[]  PROGMEM = "NO";
const char wn_ENE_s[] PROGMEM = "ONO";
const char wn_E_s[]   PROGMEM = "O";
const char wn_ESE_s[] PROGMEM = "OZO";
const char wn_SE_s[]  PROGMEM = "ZO";
const char wn_SSE_s[] PROGMEM = "ZZO";
const char wn_S_s[]   PROGMEM = "Z";
const char wn_SSW_s[] PROGMEM = "ZZW";
const char wn_SW_s[]  PROGMEM = "ZW";
const char wn_WSW_s[] PROGMEM = "WZW";
const char wn_W_s[]   PROGMEM = "W";
const char wn_WNW_s[] PROGMEM = "WNW";
const char wn_NW_s[]  PROGMEM = "NW";
const char wn_NNW_s[] PROGMEM = "NNW";

const char *const wind_short[] PROGMEM = {
  wn_N_s, wn_NNE_s, wn_NE_s, wn_ENE_s,
  wn_E_s, wn_ESE_s, wn_SE_s, wn_SSE_s,
  wn_S_s, wn_SSW_s, wn_SW_s, wn_WSW_s,
  wn_W_s, wn_WNW_s, wn_NW_s, wn_NNW_s, wn_N_s
};

// ============================================================
// WIND DIRECTIONS – LONG
// ============================================================
const char wn_N_l[] PROGMEM = "noord";
const char wn_NNE_l[] PROGMEM = "noord noordwest";
const char wn_NE_l[] PROGMEM = "noordoost";
const char wn_ENE_l[] PROGMEM = "oost noordoost";
const char wn_E_l[] PROGMEM = "oost";
const char wn_ESE_l[] PROGMEM = "oost zuidoost";
const char wn_SE_l[] PROGMEM = "zuidoost";
const char wn_SSE_l[] PROGMEM = "zuid zuidoost";
const char wn_S_l[] PROGMEM = "zuid";
const char wn_SSW_l[] PROGMEM = "zuid zuidwest";
const char wn_SW_l[] PROGMEM = "zuidwest";
const char wn_WSW_l[] PROGMEM = "west zuidwest";
const char wn_W_l[] PROGMEM = "west";
const char wn_WNW_l[] PROGMEM = "west noordwest";
const char wn_NW_l[] PROGMEM = "noordwest";
const char wn_NNW_l[] PROGMEM = "noord noordwest";

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

const char *const dow[] PROGMEM = {sun, mon, tue, wed, thu, fri, sat};
const char *const dowf[] PROGMEM = {sunf, monf, tuef, wedf, thuf, frif, satf};
const char *const mnths[] PROGMEM = {jan, feb, mar, apr, may, jun, jul, aug, sep, octc, nov, decc};

const char const_PlReady[] PROGMEM = "[gereed]";
const char const_PlStopped[] PROGMEM = "[gestopt]";
const char const_PlConnect[] PROGMEM = "";
const char const_DlgVolume[] PROGMEM = "volume";
const char const_DlgLost[] PROGMEM = "* verbinding verbroken *";
const char const_DlgUpdate[] PROGMEM = "* update *";
const char const_DlgNextion[] PROGMEM = "* NEXTION *";
const char const_getWeather[] PROGMEM = "";
const char const_waitForSD[] PROGMEM = "INDEX SD";

const char apNameTxt[] PROGMEM = "WiFi AP";
const char apPassTxt[] PROGMEM = "wachtwoord";
const char bootstrFmt[] PROGMEM = "verbinden met: %s";
const char apSettFmt[] PROGMEM = "instellingen: HTTP://%s/";

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
  "%s, %.1f°C · gevoelstemperatuur: %.1f°C · luchtdruk: %d hPa · luchtvochtigheid: %d%% · wind: %.1f km/h [%s]";
#else
const char weatherFmtLong[] PROGMEM =
  "%s, %.1f°C · %d hPa · %d%%";
#endif

static inline const char* getWeatherFmt() {
  return config.store.shortWeather ? weatherFmtShort : weatherFmtLong;
}

const char weatherUnits[] PROGMEM = "metric"; /* standard, metric, imperial */
const char weatherLang[] PROGMEM = "nl";      /* https://openweathermap.org/current#multi */

#endif