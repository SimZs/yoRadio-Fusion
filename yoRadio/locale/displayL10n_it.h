#ifndef dsp_full_loc
#define dsp_full_loc

#include <pgmspace.h>
#include "../src/core/config.h"
#include "../myoptions.h"

const char mon[] PROGMEM = "Lu";
const char tue[] PROGMEM = "Ma";
const char wed[] PROGMEM = "Me";
const char thu[] PROGMEM = "Gi";
const char fri[] PROGMEM = "Ve";
const char sat[] PROGMEM = "Sa";
const char sun[] PROGMEM = "Do";

const char monf[] PROGMEM = "Lunedi";
const char tuef[] PROGMEM = "Martedi";
const char wedf[] PROGMEM = "Mercoledi";
const char thuf[] PROGMEM = "Giovedi";
const char frif[] PROGMEM = "Venerdi";
const char satf[] PROGMEM = "Sabato";
const char sunf[] PROGMEM = "Domenica";

const char jan[] PROGMEM = "Gennaio";
const char feb[] PROGMEM = "Febbraio";
const char mar[] PROGMEM = "Maezo";
const char apr[] PROGMEM = "Aprile";
const char may[] PROGMEM = "Maggio";
const char jun[] PROGMEM = "Giugno";
const char jul[] PROGMEM = "Luglio";
const char aug[] PROGMEM = "Agosto";
const char sep[] PROGMEM = "Settembre";
const char octc[] PROGMEM = "Ottobre";
const char nov[] PROGMEM = "Novembre";
const char decc[] PROGMEM = "Dicembre";

// ============================================================
// WIND DIRECTIONS – SHORT
// ============================================================
const char wn_N_s[]      PROGMEM = "NORD";
const char wn_NNE_s[]    PROGMEM = "NNE";
const char wn_NE_s[]     PROGMEM = "NE";
const char wn_ENE_s[]    PROGMEM = "ENE";
const char wn_E_s[]      PROGMEM = "EST";
const char wn_ESE_s[]    PROGMEM = "ESE";
const char wn_SE_s[]     PROGMEM = "SE";
const char wn_SSE_s[]    PROGMEM = "SSE";
const char wn_S_s[]      PROGMEM = "SUD";
const char wn_SSW_s[]    PROGMEM = "SSW";
const char wn_SW_s[]     PROGMEM = "SW";
const char wn_WSW_s[]    PROGMEM = "WSW";
const char wn_W_s[]      PROGMEM = "OVEST";
const char wn_WNW_s[]    PROGMEM = "WNW";
const char wn_NW_s[]     PROGMEM = "NW";
const char wn_NNW_s[]    PROGMEM = "NNW";

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
const char wn_SSW_l[] PROGMEM = "sud-sud-ovest";
const char wn_SW_l[]  PROGMEM = "sud-ovest";
const char wn_WSW_l[] PROGMEM = "ovest-sud-ovest";
const char wn_W_l[]   PROGMEM = "ovest";
const char wn_WNW_l[] PROGMEM = "ovest-nord-ovest";
const char wn_NW_l[]  PROGMEM = "nord-ovest";
const char wn_NNW_l[] PROGMEM = "nord-nord-ovest";

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
const char* const mnths[]   PROGMEM = { jan, feb, mar, apr, may, jun, jul, aug, sep, octc, nov, decc };

const char    const_PlReady[]    PROGMEM = "[Pronto]";
const char  const_PlStopped[]    PROGMEM = "[Stop]";
const char  const_PlConnect[]    PROGMEM = "";
const char  const_DlgVolume[]    PROGMEM = "VOLUME";
const char    const_DlgLost[]    PROGMEM = "* PERSO *";
const char  const_DlgUpdate[]    PROGMEM = "* AGGIORNAMENTO *";
const char const_DlgNextion[]    PROGMEM = "* NEXTION *";
const char const_getWeather[]    PROGMEM = "";
const char  const_waitForSD[]    PROGMEM = "INDICE SD";

const char        apNameTxt[]    PROGMEM = "NOME AP";
const char        apPassTxt[]    PROGMEM = "PASSWORD";
const char       bootstrFmt[]    PROGMEM = "Connessione a %s";
const char        apSettFmt[]    PROGMEM = "PAGINA IMPOSTAZIONI SU: HTTP://%s/";

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
  "%s, %.1f°C · PERCEPITA: %.1f°C · PRESSIONE: %d hPa · UMIDITA: %d%% · VENTO: %.1f km/h [%s]";
#else
const char weatherFmtLong[] PROGMEM =
  "%s, %.1f°C · PRESSIONE: %d hPa · UMIDITA: %d%%";
#endif

static inline const char* getWeatherFmt() {
  return config.store.shortWeather ? weatherFmtShort : weatherFmtLong;
}

const char     weatherUnits[]    PROGMEM = "metric";   /* standard, metric, imperial */
const char      weatherLang[]    PROGMEM = "it";       /* https://openweathermap.org/current#multi */

#endif
