#ifndef dsp_full_loc
#define dsp_full_loc

#include <pgmspace.h>
#include "../src/core/config.h"
#include "../myoptions.h"

const char mon[] PROGMEM = "pond";
const char tue[] PROGMEM = "ute";
const char wed[] PROGMEM = "str";
const char thu[] PROGMEM = "čtv";
const char fri[] PROGMEM = "pát";
const char sat[] PROGMEM = "sob";
const char sun[] PROGMEM = "ned";

const char monf[] PROGMEM = "pondělí";
const char tuef[] PROGMEM = "úterý";
const char wedf[] PROGMEM = "středa";
const char thuf[] PROGMEM = "čtvrtek";
const char frif[] PROGMEM = "pátek";
const char satf[] PROGMEM = "sobota";
const char sunf[] PROGMEM = "neděle";

const char jan[] PROGMEM = "ledna";
const char feb[] PROGMEM = "února";
const char mar[] PROGMEM = "března";
const char apr[] PROGMEM = "dubna";
const char may[] PROGMEM = "května";
const char jun[] PROGMEM = "června";
const char jul[] PROGMEM = "července";
const char aug[] PROGMEM = "srpna";
const char sep[] PROGMEM = "září";
const char oct[] PROGMEM = "října";
const char nov[] PROGMEM = "listopadu";
const char dec[] PROGMEM = "prosince";

// ============================================================
// WIND DIRECTIONS – SHORT
// ============================================================
const char wn_N_s[]      PROGMEM = "S";
const char wn_NNE_s[]    PROGMEM = "SSV";
const char wn_NE_s[]     PROGMEM = "SV";
const char wn_ENE_s[]    PROGMEM = "VSV";
const char wn_E_s[]      PROGMEM = "V";
const char wn_ESE_s[]    PROGMEM = "VJV";
const char wn_SE_s[]     PROGMEM = "JE";
const char wn_SSE_s[]    PROGMEM = "JJE";
const char wn_S_s[]      PROGMEM = "J";
const char wn_SSW_s[]    PROGMEM = "JJZ";
const char wn_SW_s[]     PROGMEM = "JZ";
const char wn_WSW_s[]    PROGMEM = "ZJZ";
const char wn_W_s[]      PROGMEM = "Z";
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
const char wn_N_l[]   PROGMEM = "sever";
const char wn_NNE_l[] PROGMEM = "severoseverovýchod";
const char wn_NE_l[]  PROGMEM = "severovýchod";
const char wn_ENE_l[] PROGMEM = "východoseverovýchod";
const char wn_E_l[]   PROGMEM = "východ";
const char wn_ESE_l[] PROGMEM = "východojihovýchod";
const char wn_SE_l[]  PROGMEM = "jihovýchod";
const char wn_SSE_l[] PROGMEM = "jihojihovýchod";
const char wn_S_l[]   PROGMEM = "jih";
const char wn_SSW_l[] PROGMEM = "jihojihozápad";
const char wn_SW_l[]  PROGMEM = "jihozápad";
const char wn_WSW_l[] PROGMEM = "západojihozápad";
const char wn_W_l[]   PROGMEM = "západ";
const char wn_WNW_l[] PROGMEM = "západoseverozápad";
const char wn_NW_l[]  PROGMEM = "severozápad";
const char wn_NNW_l[] PROGMEM = "severoseverozápad";

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
const char* const mnths[]   PROGMEM = { jan, feb, mar, apr, may, jun, jul, aug, sep, oct, nov, dec };

const char    const_PlReady[]    PROGMEM = "[PŘIPRAVENO]";
const char  const_PlStopped[]    PROGMEM = "[ZASTAVENO]";
const char  const_PlConnect[]    PROGMEM = "";
const char  const_DlgVolume[]    PROGMEM = "HLASITOST";
const char    const_DlgLost[]    PROGMEM = "* ZTRATENÝ SIGNÁL *";
const char  const_DlgUpdate[]    PROGMEM = "* AKTUALIZACE *";
const char const_DlgNextion[]    PROGMEM = "* NEXTION *";
const char const_getWeather[]    PROGMEM = "";
const char  const_waitForSD[]    PROGMEM = "INDEX SD";

const char        apNameTxt[]    PROGMEM = "Název AP";
const char        apPassTxt[]    PROGMEM = "HESLO";
const char       bootstrFmt[]    PROGMEM = "Pripojuje se %s";
const char        apSettFmt[]    PROGMEM = "STRÁNKA S NASTAVENÍMI: HTTP://%s/";

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
  "%s, %.1f°C · pocitová: %.1f°C · Tlak: %d hPa · Vlhkost: %d%% · Vítr: %.1f km/h [%s]";
#else
const char weatherFmtLong[] PROGMEM =
  "%s, %.1f°C · %d hPa · %d%%";
#endif

static inline const char* getWeatherFmt() {
  return config.store.shortWeather ? weatherFmtShort : weatherFmtLong;
}

const char     weatherUnits[]    PROGMEM = "metric";   /* standard, metric, imperial */
const char      weatherLang[]    PROGMEM = "cz";       /* https://openweathermap.org/current#multi */

#endif
