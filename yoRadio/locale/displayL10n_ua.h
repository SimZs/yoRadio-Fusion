#ifndef dsp_full_loc
#define dsp_full_loc

#include <pgmspace.h>
#include "../src/core/config.h"
#include "../myoptions.h"

const char mon[] PROGMEM = "пн";
const char tue[] PROGMEM = "вт";
const char wed[] PROGMEM = "ср";
const char thu[] PROGMEM = "чт";
const char fri[] PROGMEM = "пт";
const char sat[] PROGMEM = "сб";
const char sun[] PROGMEM = "нд";

const char monf[] PROGMEM = "понеділок";
const char tuef[] PROGMEM = "вівторок";
const char wedf[] PROGMEM = "середа";
const char thuf[] PROGMEM = "четвер";
const char frif[] PROGMEM = "п'ятниця";
const char satf[] PROGMEM = "субота";
const char sunf[] PROGMEM = "неділя";

const char jan[] PROGMEM = "січня";
const char feb[] PROGMEM = "лютого";
const char mar[] PROGMEM = "березня";
const char apr[] PROGMEM = "квітня";
const char may[] PROGMEM = "травня";
const char jun[] PROGMEM = "червня";
const char jul[] PROGMEM = "липня";
const char aug[] PROGMEM = "серпня";
const char sep[] PROGMEM = "вересня";
const char octt[] PROGMEM = "жовтня";
const char nov[] PROGMEM = "листопада";
const char decc[] PROGMEM = "грудня";

// ============================================================
// WIND DIRECTIONS – SHORT
// ============================================================
const char wn_N_s[]      PROGMEM = "Пн";
const char wn_NNE_s[]    PROGMEM = "ПнПнСх";
const char wn_NE_s[]     PROGMEM = "ПнСх";
const char wn_ENE_s[]    PROGMEM = "СхПнСх";
const char wn_E_s[]      PROGMEM = "Сх";
const char wn_ESE_s[]    PROGMEM = "СхПдСх";
const char wn_SE_s[]     PROGMEM = "ПдЗх";
const char wn_SSE_s[]    PROGMEM = "ПдПдСх";
const char wn_S_s[]      PROGMEM = "Пд";
const char wn_SSW_s[]    PROGMEM = "ПдПдЗх";
const char wn_SW_s[]     PROGMEM = "ПдЗх";
const char wn_WSW_s[]    PROGMEM = "ЗхПдЗх";
const char wn_W_s[]      PROGMEM = "Зх";
const char wn_WNW_s[]    PROGMEM = "ЗхПнЗх";
const char wn_NW_s[]     PROGMEM = "ПнЗх";
const char wn_NNW_s[]    PROGMEM = "ПнПнЗх";

const char *const wind_short[] PROGMEM = {
  wn_N_s, wn_NNE_s, wn_NE_s, wn_ENE_s,
  wn_E_s, wn_ESE_s, wn_SE_s, wn_SSE_s,
  wn_S_s, wn_SSW_s, wn_SW_s, wn_WSW_s,
  wn_W_s, wn_WNW_s, wn_NW_s, wn_NNW_s, wn_N_s
};

// ============================================================
// WIND DIRECTIONS – LONG
// ============================================================
const char wn_N_l[]   PROGMEM = "північний";
const char wn_NNE_l[] PROGMEM = "північнопівнічно-східний";
const char wn_NE_l[]  PROGMEM = "північно-східний";
const char wn_ENE_l[] PROGMEM = "східнопівнічно-східний";
const char wn_E_l[]   PROGMEM = "східний";
const char wn_ESE_l[] PROGMEM = "східнопівденно-східний";
const char wn_SE_l[]  PROGMEM = "південно-східний";
const char wn_SSE_l[] PROGMEM = "південнопівденно-східний";
const char wn_S_l[]   PROGMEM = "південний";
const char wn_SSW_l[] PROGMEM = "південнопівденно-західний";
const char wn_SW_l[]  PROGMEM = "південно-західний";
const char wn_WSW_l[] PROGMEM = "західнопівденно-західний";
const char wn_W_l[]   PROGMEM = "західний";
const char wn_WNW_l[] PROGMEM = "західнопівнічно-західний";
const char wn_NW_l[]  PROGMEM = "північно-західний";
const char wn_NNW_l[] PROGMEM = "північнопівнічно-західний";

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

const char    const_PlReady[]    PROGMEM = "[готовий]";
const char  const_PlStopped[]    PROGMEM = "[зупинено]";
const char  const_PlConnect[]    PROGMEM = "";
const char  const_DlgVolume[]    PROGMEM = "ГУЧНІСТЬ";
const char    const_DlgLost[]    PROGMEM = "ВИМКНЕНО";
const char  const_DlgUpdate[]    PROGMEM = "ОНОВЛЕННЯ";
const char const_DlgNextion[]    PROGMEM = "NEXTION";
const char const_getWeather[]    PROGMEM = "";
const char  const_waitForSD[]    PROGMEM = "ІНДЕКС SD";

const char        apNameTxt[]    PROGMEM = "ТОЧКА ДОСТУПУ";
const char        apPassTxt[]    PROGMEM = "ГАСЛО";
const char       bootstrFmt[]    PROGMEM = "З'єднуюсь з %s";
const char        apSettFmt[]    PROGMEM = "НАЛАШТУВАННЯ: HTTP://%s/";

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
  "%s, %.1f°C · відчувається: %.1f\011C · тиск: %d гПа · вологість: %d%% · вітер: %.1f км/год [%s]";
#else
const char weatherFmtLong[] PROGMEM =
  "%s, %.1f°C · тиск: %d hPa · вологість: %d%%";
#endif

static inline const char* getWeatherFmt() {
  return config.store.shortWeather ? weatherFmtShort : weatherFmtLong;
}

const char     weatherUnits[]    PROGMEM = "metric";   /* standard, metric, imperial */
const char      weatherLang[]    PROGMEM = "ua";       /* https://openweathermap.org/current#multi */

#endif
