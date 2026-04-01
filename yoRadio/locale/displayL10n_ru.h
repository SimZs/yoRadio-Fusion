#ifndef dsp_full_loc
#define dsp_full_loc
#include <pgmspace.h>
/*************************************************************************************
    HOWTO:
    Copy this file to yoRadio/locale/displayL10n_custom.h
    and modify it
*************************************************************************************/
const char mon[] PROGMEM = "пн";
const char tue[] PROGMEM = "вт";
const char wed[] PROGMEM = "ср";
const char thu[] PROGMEM = "чт";
const char fri[] PROGMEM = "пт";
const char sat[] PROGMEM = "сб";
const char sun[] PROGMEM = "вс";

const char monf[] PROGMEM = "понедельник";
const char tuef[] PROGMEM = "вторник";
const char wedf[] PROGMEM = "среда";
const char thuf[] PROGMEM = "четверг";
const char frif[] PROGMEM = "пятница";
const char satf[] PROGMEM = "суббота";
const char sunf[] PROGMEM = "воскресенье";

const char jan[] PROGMEM = "января";
const char feb[] PROGMEM = "февраля";
const char mar[] PROGMEM = "марта";
const char apr[] PROGMEM = "апреля";
const char may[] PROGMEM = "мая";
const char jun[] PROGMEM = "июня";
const char jul[] PROGMEM = "июля";
const char aug[] PROGMEM = "августа";
const char sep[] PROGMEM = "сентября";
const char octt[] PROGMEM = "октября";
const char nov[] PROGMEM = "ноября";
const char decc[] PROGMEM = "декабря";

// ============================================================
// WIND DIRECTIONS – SHORT
// ============================================================
const char wn_N_s[]      PROGMEM = "СЕВ";
const char wn_NNE_s[]    PROGMEM = "ССВ";
const char wn_NE_s[]     PROGMEM = "СВ";
const char wn_ENE_s[]    PROGMEM = "ВСВ";
const char wn_E_s[]      PROGMEM = "ВОСТ";
const char wn_ESE_s[]    PROGMEM = "ВЮВ";
const char wn_SE_s[]     PROGMEM = "ЮВ";
const char wn_SSE_s[]    PROGMEM = "ЮЮВ";
const char wn_S_s[]      PROGMEM = "ЮЖ";
const char wn_SSW_s[]    PROGMEM = "ЮЮЗ";
const char wn_SW_s[]     PROGMEM = "ЮЗ";
const char wn_WSW_s[]    PROGMEM = "ЗЮЗ";
const char wn_W_s[]      PROGMEM = "ЗАП";
const char wn_WNW_s[]    PROGMEM = "ЗСЗ";
const char wn_NW_s[]     PROGMEM = "СЗ";
const char wn_NNW_s[]    PROGMEM = "ССЗ";

const char *const wind_short[] PROGMEM = {
  wn_N_s, wn_NNE_s, wn_NE_s, wn_ENE_s,
  wn_E_s, wn_ESE_s, wn_SE_s, wn_SSE_s,
  wn_S_s, wn_SSW_s, wn_SW_s, wn_WSW_s,
  wn_W_s, wn_WNW_s, wn_NW_s, wn_NNW_s, wn_N_s
};

// ============================================================
// WIND DIRECTIONS – LONG
// ============================================================
const char wn_N_l[]   PROGMEM = "северный";
const char wn_NNE_l[] PROGMEM = "северо-северо-восточный";
const char wn_NE_l[]  PROGMEM = "северо-восточный";
const char wn_ENE_l[] PROGMEM = "восточно-северо-восточный";
const char wn_E_l[]   PROGMEM = "восточный";
const char wn_ESE_l[] PROGMEM = "восточно-юго-восточный";
const char wn_SE_l[]  PROGMEM = "юго-восточный";
const char wn_SSE_l[] PROGMEM = "юго-юго-восточный";
const char wn_S_l[]   PROGMEM = "южный";
const char wn_SSW_l[] PROGMEM = "юго-юго-западный";
const char wn_SW_l[]  PROGMEM = "юго-западный";
const char wn_WSW_l[] PROGMEM = "западно-юго-западный";
const char wn_W_l[]   PROGMEM = "западный";
const char wn_WNW_l[] PROGMEM = "западно-северо-западный";
const char wn_NW_l[]  PROGMEM = "северо-западный";
const char wn_NNW_l[] PROGMEM = "северо-северо-западный";

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

const char    const_PlReady[]    PROGMEM = "[готов]";
const char  const_PlStopped[]    PROGMEM = "[остановлено]";
const char  const_PlConnect[]    PROGMEM = "";
const char  const_DlgVolume[]    PROGMEM = "ГРОМКОСТЬ";
const char    const_DlgLost[]    PROGMEM = "ОТКЛЮЧЕНО";
const char  const_DlgUpdate[]    PROGMEM = "ОБНОВЛЕНИЕ";
const char const_DlgNextion[]    PROGMEM = "NEXTION";
const char const_getWeather[]    PROGMEM = "";
const char  const_waitForSD[]    PROGMEM = "ИНДЕКС SD";

const char        apNameTxt[]    PROGMEM = "ТОЧКА ДОСТУПА";
const char        apPassTxt[]    PROGMEM = "ПАРОЛЬ";
const char       bootstrFmt[]    PROGMEM = "Соединяюсь с %s";
const char        apSettFmt[]    PROGMEM = "НАСТРОЙКИ: HTTP://%s/";

#if (DSP_MODEL == DSP_ILI9341) || (DSP_MODEL == DSP_ST7735) || (DSP_MODEL == DSP_ST7789) || (DSP_MODEL == DSP_ST7789_76)
const char weatherFmtShort[] PROGMEM =
  "%d гПа · %d%% RH · %.1f км/год";
#elif (DSP_MODEL == DSP_ST7789_240) || (DSP_MODEL==DSP_GC9A01) || (DSP_MODEL==DSP_GC9A01A) || (DSP_MODEL==DSP_GC9A01_I80)
  "%d гПа · %d%% RH";
#else
const char weatherFmtShort[] PROGMEM =
  "%d гПа · %d%% RH · %.1f км/год [%s]";
#endif

#if EXT_WEATHER
const char weatherFmtLong[] PROGMEM =
  "%s, %.1f°C · ощущается: %.1f\011C · давление: %d гПа · влажность: %d%% · ветер: %.1f км/год [%s]";
#else
const char weatherFmtLong[] PROGMEM =
  "%s, %.1f°C · давление: %d гПа · влажность: %d%%";
#endif

static inline const char* getWeatherFmt() {
  return config.store.shortWeather ? weatherFmtShort : weatherFmtLong;
}

const char     weatherUnits[]    PROGMEM = "metric";   /* standard, metric, imperial */
const char      weatherLang[]    PROGMEM = "ru";       /* https://openweathermap.org/current#multi */

#endif
