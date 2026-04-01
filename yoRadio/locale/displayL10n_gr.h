// v0.9.555
#ifndef dsp_full_loc
#define dsp_full_loc

#include <pgmspace.h>
#include "../src/core/config.h"
#include "../myoptions.h"

const char mon[] PROGMEM = "Δε";
const char tue[] PROGMEM = "Τρ";
const char wed[] PROGMEM = "Τε";
const char thu[] PROGMEM = "Πε";
const char fri[] PROGMEM = "Πα";
const char sat[] PROGMEM = "Σα";
const char sun[] PROGMEM = "Κυ";

const char monf[] PROGMEM = "Δευτέρα";
const char tuef[] PROGMEM = "Τρίτη";
const char wedf[] PROGMEM = "Τετάρτη";
const char thuf[] PROGMEM = "Πέμπτη";
const char frif[] PROGMEM = "Παρασκευή";
const char satf[] PROGMEM = "Σάββατο";
const char sunf[] PROGMEM = "Κυριακή";

const char jan[] PROGMEM = "Ιανουάριος";
const char feb[] PROGMEM = "Φεβρουάριος";
const char mar[] PROGMEM = "Μάρτιος";
const char apr[] PROGMEM = "Απρίλιος";
const char may[] PROGMEM = "Μάιος";
const char jun[] PROGMEM = "Ιούνιος";
const char jul[] PROGMEM = "Ιούλιος";
const char aug[] PROGMEM = "Αύγουστος";
const char sep[] PROGMEM = "Σεπτέμβριος";
const char octt[] PROGMEM = "Οκτώβριος";
const char nov[] PROGMEM = "Νοέμβριος";
const char decc[] PROGMEM = "Δεκέμβριος";

// ============================================================
// WIND DIRECTIONS – SHORT
// ============================================================
const char wn_N_s[]      PROGMEM = "Β";
const char wn_NNE_s[]    PROGMEM = "ΒΒΑ";
const char wn_NE_s[]     PROGMEM = "ΒΑ";
const char wn_ENE_s[]    PROGMEM = "ΑΒΑ";
const char wn_E_s[]      PROGMEM = "Α";
const char wn_ESE_s[]    PROGMEM = "ΑΝΑ";
const char wn_SE_s[]     PROGMEM = "ΝΑ";
const char wn_SSE_s[]    PROGMEM = "ΝΝΑ";
const char wn_S_s[]      PROGMEM = "Ν";
const char wn_SSW_s[]    PROGMEM = "ΝΝΔ";
const char wn_SW_s[]     PROGMEM = "ΝΔ";
const char wn_WSW_s[]    PROGMEM = "ΔΝΔ";
const char wn_W_s[]      PROGMEM = "Δ";
const char wn_WNW_s[]    PROGMEM = "ΔΒΔ";
const char wn_NW_s[]     PROGMEM = "ΒΔ";
const char wn_NNW_s[]    PROGMEM = "ΒΒΔ";

const char *const wind_short[] PROGMEM = {
  wn_N_s, wn_NNE_s, wn_NE_s, wn_ENE_s,
  wn_E_s, wn_ESE_s, wn_SE_s, wn_SSE_s,
  wn_S_s, wn_SSW_s, wn_SW_s, wn_WSW_s,
  wn_W_s, wn_WNW_s, wn_NW_s, wn_NNW_s, wn_N_s
};

// ============================================================
// WIND DIRECTIONS – LONG
// ============================================================
const char wn_N_l[]   PROGMEM = "βόρειος";
const char wn_NNE_l[] PROGMEM = "βορειοβορειοανατολικός";
const char wn_NE_l[]  PROGMEM = "βορειοανατολικός";
const char wn_ENE_l[] PROGMEM = "ανατολοβορειοανατολικός";
const char wn_E_l[]   PROGMEM = "ανατολικός";
const char wn_ESE_l[] PROGMEM = "ανατολονοτιοανατολικός";
const char wn_SE_l[]  PROGMEM = "νοτιοανατολικός";
const char wn_SSE_l[] PROGMEM = "νοτιονοτιοανατολικός";
const char wn_S_l[]   PROGMEM = "νότιος";
const char wn_SSW_l[] PROGMEM = "νοτιονοτιοδυτικός";
const char wn_SW_l[]  PROGMEM = "νοτιοδυτικός";
const char wn_WSW_l[] PROGMEM = "δυτικονοτιοδυτικός";
const char wn_W_l[]   PROGMEM = "δυτικός";
const char wn_WNW_l[] PROGMEM = "δυτικοβορειοδυτικός";
const char wn_NW_l[]  PROGMEM = "βορειοδυτικός";
const char wn_NNW_l[] PROGMEM = "βορειοβορειοδυτικός";

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

const char    const_PlReady[]    PROGMEM = "[έτοιμο]";
const char  const_PlStopped[]    PROGMEM = "[σταμάτησε]";
const char  const_PlConnect[]    PROGMEM = "";
const char  const_DlgVolume[]    PROGMEM = "ΕΝΤΑΣΗ";
const char    const_DlgLost[]    PROGMEM = "* χάθηκε το σήμα *";
const char  const_DlgUpdate[]    PROGMEM = "* ΕΝΗΜΕΡΩΣΗ *";
const char const_DlgNextion[]    PROGMEM = "* NEXTION *";
const char const_getWeather[]    PROGMEM = "";
const char  const_waitForSD[]    PROGMEM = "INDEX SD";

const char        apNameTxt[]    PROGMEM = "ΟΝΟΜΑ AP";
const char        apPassTxt[]    PROGMEM = "ΚΩΔΙΚΟΣ";
const char       bootstrFmt[]    PROGMEM = "σύνδεση με %s";
const char        apSettFmt[]    PROGMEM = "ΣΕΛΙΔΑ ΡΥΘΜΙΣΕΩΝ: HTTP://%s/";

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
  "%s, %.1f°C · αίσθηση θερμοκρασίας: %.1f\011C · πίεση: %d hPa · υγρασία: %d%% · άνεμος: %.1f km/h [%s]";
#else
const char weatherFmtLong[] PROGMEM =
  "%s, %.1f°C · Πίεση: %d hPa · Υγρασία: %d%%";
#endif

static inline const char* getWeatherFmt() {
  return config.store.shortWeather ? weatherFmtShort : weatherFmtLong;
}

const char     weatherUnits[]    PROGMEM = "metric";   /* standard, metric, imperial */
const char      weatherLang[]    PROGMEM = "el";       /* https://openweathermap.org/current#multi */

#endif
