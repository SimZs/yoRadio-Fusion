#ifndef dsp_full_loc
#define dsp_full_loc

#include <pgmspace.h>
#include "../src/core/config.h"
#include "../myoptions.h"

const char mon[] PROGMEM = "po";
const char tue[] PROGMEM = "ut";
const char wed[] PROGMEM = "st";
const char thu[] PROGMEM = "štt";
const char fri[] PROGMEM = "pi";
const char sat[] PROGMEM = "so";
const char sun[] PROGMEM = "ne";

const char monf[] PROGMEM = "pondelok";
const char tuef[] PROGMEM = "utorok";
const char wedf[] PROGMEM = "streda";
const char thuf[] PROGMEM = "štvrtok";
const char frif[] PROGMEM = "piatok";
const char satf[] PROGMEM = "sobota";
const char sunf[] PROGMEM = "nedeľa";

const char jan[] PROGMEM = "január";
const char feb[] PROGMEM = "február";
const char mar[] PROGMEM = "marec";
const char apr[] PROGMEM = "apríl";
const char may[] PROGMEM = "máj";
const char jun[] PROGMEM = "jún";
const char jul[] PROGMEM = "júl";
const char aug[] PROGMEM = "august";
const char sep[] PROGMEM = "september";
const char octt[] PROGMEM = "október";
const char nov[] PROGMEM = "november";
const char decc[] PROGMEM = "december";

// ============================================================
// WIND DIRECTIONS – SHORT
// ============================================================
const char wn_N_s[]      PROGMEM = "SEV";
const char wn_NNE_s[]    PROGMEM = "SSV";
const char wn_NE_s[]     PROGMEM = "SV";
const char wn_ENE_s[]    PROGMEM = "VSV";
const char wn_E_s[]      PROGMEM = "V";
const char wn_ESE_s[]    PROGMEM = "VVJ";
const char wn_SE_s[]     PROGMEM = "JV";
const char wn_SSE_s[]    PROGMEM = "JJV";
const char wn_S_s[]      PROGMEM = "JUH";
const char wn_SSW_s[]    PROGMEM = "JJZ";
const char wn_SW_s[]     PROGMEM = "JZ";
const char wn_WSW_s[]    PROGMEM = "ZJZ";
const char wn_W_s[]      PROGMEM = "ZAP";
const char wn_WNW_s[]    PROGMEM = "ZSZ";
const char wn_NW_s[]     PROGMEM = "SZ";
const char wn_NNW_s[]    PROGMEM = "SSZ";

const char *const wind_short[] PROGMEM = {
  wn_N_s, wn_NNE_s, wn_NE_s, wn_ENE_s,
  wn_E_s, wn_ESE_s, wn_SE_s, wn_SSE_s,
  wn_S_s, wn_SSW_s, wn_SW_s, wn_WSW_s,
  wn_W_s, wn_WNW_s, wn_NW_s, wn_NNW_s, wn_N_s
};

// ============================================================
// WIND DIRECTIONS – LONG
// ============================================================
const char wn_N_l[]   PROGMEM = "severný";
const char wn_NNE_l[] PROGMEM = "severoseverovýchodný";
const char wn_NE_l[]  PROGMEM = "severovýchodný";
const char wn_ENE_l[] PROGMEM = "východoseverovýchodný";
const char wn_E_l[]   PROGMEM = "východný";
const char wn_ESE_l[] PROGMEM = "východojuhovýchodný";
const char wn_SE_l[]  PROGMEM = "juhovýchodný";
const char wn_SSE_l[] PROGMEM = "juhovýchodojuhový";
const char wn_S_l[]   PROGMEM = "južný";
const char wn_SSW_l[] PROGMEM = "juhojuhozápadný";
const char wn_SW_l[]  PROGMEM = "juhozápadný";
const char wn_WSW_l[] PROGMEM = "západojuhozápadný";
const char wn_W_l[]   PROGMEM = "západný";
const char wn_WNW_l[] PROGMEM = "západoseverozápadný";
const char wn_NW_l[]  PROGMEM = "severozápadný";
const char wn_NNW_l[] PROGMEM = "severoseverozápadný";

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

const char* const dow[]     PROGMEM = { sun, mon, tue, wed, thu, fri, sat };
const char* const dowf[]    PROGMEM = { sunf, monf, tuef, wedf, thuf, frif, satf };
const char* const mnths[]   PROGMEM = { jan, feb, mar, apr, may, jun, jul, aug, sep, octt, nov, decc };

const char    const_PlReady[]    PROGMEM = "[pripravené]";
const char  const_PlStopped[]    PROGMEM = "[zastavené]";
const char  const_PlConnect[]    PROGMEM = "";
const char  const_DlgVolume[]    PROGMEM = "hlasitosť";
const char    const_DlgLost[]    PROGMEM = "* stratený signál *";
const char  const_DlgUpdate[]    PROGMEM = "* obnova *";
const char const_DlgNextion[]    PROGMEM = "* NEXTION *";
const char const_getWeather[]    PROGMEM = "";
const char  const_waitForSD[]    PROGMEM = "INDEX SD";

const char        apNameTxt[]    PROGMEM = "Nazov AP";
const char        apPassTxt[]    PROGMEM = "HESLO";
const char       bootstrFmt[]    PROGMEM = "Pripája sa %s";
const char        apSettFmt[]    PROGMEM = "Stránka s nastaveniami: HTTP://%s/";

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
  "%s, %.1f°C · pocitová: %.1f°C · Tlak: %d hPa · Vlhkosť: %d%% · Vietor: %.1f km/h [%s]";
#else
const char weatherFmtLong[] PROGMEM =
  "%s, %.1f°C · Tlak: %d hPa · vlhkosť: %d%%";
#endif

static inline const char* getWeatherFmt() {
  return config.store.shortWeather ? weatherFmtShort : weatherFmtLong;
}

const char     weatherUnits[]    PROGMEM = "metric";   /* standard, metric, imperial */
const char      weatherLang[]    PROGMEM = "sk";       /* https://openweathermap.org/current#multi */

#endif
