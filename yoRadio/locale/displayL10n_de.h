#ifndef dsp_full_loc
#define dsp_full_loc
#include <pgmspace.h>
#include "../myoptions.h"

const char mon[] PROGMEM = "Mo";
const char tue[] PROGMEM = "Di";
const char wed[] PROGMEM = "Mi";
const char thu[] PROGMEM = "Do";
const char fri[] PROGMEM = "Fr";
const char sat[] PROGMEM = "Sa";
const char sun[] PROGMEM = "So";

const char monf[] PROGMEM = "Montag";
const char tuef[] PROGMEM = "Dienstag";
const char wedf[] PROGMEM = "Mittwoch";
const char thuf[] PROGMEM = "Donnerstag";
const char frif[] PROGMEM = "Freitag";
const char satf[] PROGMEM = "Samstag";
const char sunf[] PROGMEM = "Sonntag";

const char jan[] PROGMEM = "Januar";
const char feb[] PROGMEM = "Februar";
const char mar[] PROGMEM = "März";
const char apr[] PROGMEM = "April";
const char may[] PROGMEM = "Mai";
const char jun[] PROGMEM = "Juni";
const char jul[] PROGMEM = "Juli";
const char aug[] PROGMEM = "August";
const char sep[] PROGMEM = "September";
const char octt[] PROGMEM = "Oktober";
const char nov[] PROGMEM = "November";
const char decc[] PROGMEM = "Dezember";

// ============================================================
// WIND DIRECTIONS – SHORT
// ============================================================
const char wn_N_s[] PROGMEM = "NORD";
const char wn_NNE_s[] PROGMEM = "NNO";
const char wn_NE_s[] PROGMEM = "NO";
const char wn_ENE_s[] PROGMEM = "ONO";
const char wn_E_s[] PROGMEM = "OST";
const char wn_ESE_s[] PROGMEM = "OSO";
const char wn_SE_s[] PROGMEM = "SO";
const char wn_SSE_s[] PROGMEM = "SSO";
const char wn_S_s[] PROGMEM = "SÜD";
const char wn_SSW_s[] PROGMEM = "SSW";
const char wn_SW_s[] PROGMEM = "SW";
const char wn_WSW_s[] PROGMEM = "WSW";
const char wn_W_s[] PROGMEM = "WEST";
const char wn_WNW_s[] PROGMEM = "WNW";
const char wn_NW_s[] PROGMEM = "NW";
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
const char wn_N_l[]   PROGMEM = "Nord";
const char wn_NNE_l[] PROGMEM = "Nordnordost";
const char wn_NE_l[]  PROGMEM = "Nordost";
const char wn_ENE_l[] PROGMEM = "Ostnordost";
const char wn_E_l[]   PROGMEM = "Ost";
const char wn_ESE_l[] PROGMEM = "Ostsüdost";
const char wn_SE_l[]  PROGMEM = "Südost";
const char wn_SSE_l[] PROGMEM = "Südsüdost";
const char wn_S_l[]   PROGMEM = "Süd";
const char wn_SSW_l[] PROGMEM = "Südsüdwest";
const char wn_SW_l[]  PROGMEM = "Südwest";
const char wn_WSW_l[] PROGMEM = "Westsüdwest";
const char wn_W_l[]   PROGMEM = "West";
const char wn_WNW_l[] PROGMEM = "Westnordwest";
const char wn_NW_l[]  PROGMEM = "Nordwest";
const char wn_NNW_l[] PROGMEM = "Nordnordwest";

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
const char *const mnths[] PROGMEM = {jan, feb, mar, apr, may, jun, jul, aug, sep, octt, nov, decc};

const char const_PlReady[] PROGMEM = "[bereit]";
const char const_PlStopped[] PROGMEM = "[gestoppt]";
const char const_PlConnect[] PROGMEM = "";
const char const_DlgVolume[] PROGMEM = "Lautstärke";
const char const_DlgLost[] PROGMEM = "* Signal verloren *";
const char const_DlgUpdate[] PROGMEM = "* Aktualisierung *";
const char const_DlgNextion[] PROGMEM = "* NEXTION *";
const char const_getWeather[] PROGMEM = "";
const char const_waitForSD[] PROGMEM = "INDEX SD";

const char apNameTxt[] PROGMEM = "AP NAME";
const char apPassTxt[] PROGMEM = "PASSWORT";
const char bootstrFmt[] PROGMEM = "VERBINDEN %s";
const char apSettFmt[] PROGMEM = "SEITE MIT EINSTELLUNGEN: HTTP://%s/";

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
  "%s, %.1f°C · gefühlte Temperatur: %.1f°C · Druck: %d hPa · Luftfeuchtigkeit: %d%% · Wind: %.1f km/h [%s]";
#else
const char weatherFmtLong[] PROGMEM =
  "%s, %.1f°C · Druck: %d hPa · Luftfeuchtigkeit: %d%%";
#endif

static inline const char* getWeatherFmt() {
  return config.store.shortWeather ? weatherFmtShort : weatherFmtLong;
}

const char weatherUnits[] PROGMEM = "metric"; /* standard, metric, imperial */
const char weatherLang[] PROGMEM = "de";      /* https://openweathermap.org/current#multi */

#endif
