#include <Arduino.h>
#include <time.h>
#include "../../myoptions.h" 
#include "../core/player.h"
#include "../core/config.h"
#include "clock_tts.h"

extern Player player;

bool clock_tts_enabled            = false;
int  clock_tts_interval           = 60;
static int  clock_lastMinute      = -1;
static time_t lastTtsEpoch = 0;

static bool playerWasRunning      = false;
static int  clock_tts_prev_volume = 0;

static unsigned long ttsStepStart = 0;
static char ttsBuffer[128];

TaskHandle_t clock_tts_task = nullptr;

enum TTS_STATE {
  TTS_IDLE = 0,
  TTS_FADE_DOWN,
  TTS_PREPARE_SPEECH,
  TTS_PLAYING,
  TTS_FADE_UP
};

static TTS_STATE ttsState = TTS_IDLE;
char clock_tts_language[32] = CLOCK_TTS_LANGUAGE;

static int fade_target  = 0;
static int fade_current = 0;

void doFadeDownStart() {
  fade_current = player.getVolume();
  fade_target  = max(4, fade_current / 4);
}

bool doFadeDownStep() {
  if (fade_current > fade_target) {
    fade_current -= 1;
    player.setVolume(fade_current);
    vTaskDelay(pdMS_TO_TICKS(15));
    return false;
  }
  return true;
}

void doFadeUpStart() {
  fade_target = clock_tts_prev_volume;
}

bool doFadeUpStep() {
  if (fade_current < fade_target) {
    fade_current += 1;
    player.setVolume(fade_current);
    vTaskDelay(pdMS_TO_TICKS(8));
    return false;
  }
  return true;
}

static inline void clock_tts_announcement(char* buf, size_t buflen, int hour, int min, const char* lang) {
  if (strncmp(lang, "en", 2) == 0)
    snprintf(buf, buflen, "The time is %d:%02d.", hour, min);
  else if (strncmp(lang, "de", 2) == 0)
    snprintf(buf, buflen, "Es ist %d:%02d Uhr.", hour, min);
  else if (strncmp(lang, "ru", 2) == 0)
    snprintf(buf, buflen, "Сейчас %d:%02d.", hour, min);
  else if (strncmp(lang, "ro", 2) == 0)
    snprintf(buf, buflen, "Este ora %d:%02d.", hour, min);
  else if (strncmp(lang, "hu", 2) == 0)
    snprintf(buf, buflen, "A pontos idő %d:%02d.", hour, min);
  else if (strncmp(lang, "pl", 2) == 0)
    snprintf(buf, buflen, "Jest godzina %d:%02d.", hour, min);
  else if (strncmp(lang, "fr", 2) == 0)
    snprintf(buf, buflen, "Il est %d:%02d.", hour, min);
  else if (strncmp(lang, "gr", 2) == 0)
    snprintf(buf, buflen, "I ora einai %d:%02d.", hour, min);
  else if (strncmp(lang, "nl", 2) == 0)
    snprintf(buf, buflen, "De tijd %d:%02d.", hour, min);
  else
    snprintf(buf, buflen, "The time is %d:%02d.", hour, min);
}

void clock_tts_setup() {

  clock_tts_enabled  = (config.store.ttsEnabled != 0);
  clock_tts_interval = config.store.ttsInterval > 0 ? config.store.ttsInterval : 60;
  clock_lastMinute   = -1;

  if (clock_tts_task == nullptr) {
    xTaskCreatePinnedToCore(
      clock_tts_task_func,
      "clock_tts_task",
      12288,
      nullptr,
      1,
      &clock_tts_task,
      0
    );
    Serial.println("[TTS] Task created");
  }
}

void clock_tts_task_func(void *param) {

  for (;;) {

    if (!clock_tts_enabled) {
      vTaskDelay(pdMS_TO_TICKS(1000));
      continue;
    }

    // === TTS only in WEB mode ===
    if (config.getMode() != PM_WEB) {
      vTaskDelay(pdMS_TO_TICKS(1000));
      continue;
    }

    time_t now = time(nullptr);
    struct tm *tm_info = localtime(&now);

    if (!tm_info || tm_info->tm_year + 1900 < 2020) {
      vTaskDelay(pdMS_TO_TICKS(1000));
      continue;
    }

    auto timeToMinutes = [](const char* hhmm) -> int {
        if (!hhmm) return -1;
        if (strlen(hhmm) != 5 || hhmm[2] != ':') return -1;
        int h = (hhmm[0]-'0')*10 + (hhmm[1]-'0');
        int m = (hhmm[3]-'0')*10 + (hhmm[4]-'0');
        if (h < 0 || h > 23 || m < 0 || m > 59) return -1;
        return h*60 + m;
    };

    int cur      = tm_info->tm_hour * 60 + tm_info->tm_min;
    int dndStart = timeToMinutes(config.store.ttsDndStart);
    int dndStop  = timeToMinutes(config.store.ttsDndStop);

    if (dndStart >= 0 && dndStop >= 0) {
        bool inDnd = false;

        if (dndStart == dndStop) {
            inDnd = false;
        }
        else if (dndStart < dndStop) {
            inDnd = (cur >= dndStart && cur < dndStop);
        }
        else {
            inDnd = (cur >= dndStart || cur < dndStop);
        }

        if (inDnd) {
            vTaskDelay(pdMS_TO_TICKS(1000));
            continue;
        }
    }

    bool trigger =
     (tm_info->tm_min % clock_tts_interval == 0) &&
     (tm_info->tm_min != clock_lastMinute) &&
     (now - lastTtsEpoch >= 30);

    const bool allowDuring = config.store.ttsDuringPlayback;

    switch (ttsState) {

      case TTS_IDLE:
        if (trigger) {

          playerWasRunning       = player.isRunning();
          clock_tts_prev_volume  = player.getVolume();
          lastTtsEpoch = now;
          clock_lastMinute       = tm_info->tm_min;

          if (!allowDuring && playerWasRunning) {
              ttsState = TTS_IDLE;
              break;
          }

          if (allowDuring && playerWasRunning)
            doFadeDownStart();

          ttsState = (allowDuring && playerWasRunning)
                     ? TTS_FADE_DOWN
                     : TTS_PREPARE_SPEECH;

          ttsStepStart = millis();
        }
        break;

      // ==============================================================
      // FADE DOWN 
      // ==============================================================
      case TTS_FADE_DOWN:
        if (doFadeDownStep()) {
          ttsState = TTS_PREPARE_SPEECH;
          ttsStepStart = millis();
        }
        break;

      // ==============================================================
      // TTS start
      // ==============================================================
      case TTS_PREPARE_SPEECH:

        if (millis() - ttsStepStart >= 150) {

          player.lockOutput = true;
          vTaskDelay(pdMS_TO_TICKS(120));
          clock_tts_announcement(ttsBuffer, sizeof(ttsBuffer),
                                 tm_info->tm_hour,
                                 tm_info->tm_min,
                                 clock_tts_language);
          player.setVolume(clock_tts_prev_volume);
          player.connecttospeech(ttsBuffer, clock_tts_language);
          player.setOutputPins(true);  
          vTaskDelay(pdMS_TO_TICKS(80));
          ttsState = TTS_PLAYING;
          ttsStepStart = millis();
        }
        break;

      // ==============================================================
      // PLAYING – TTS 
      // ==============================================================
      case TTS_PLAYING:

        if (millis() - ttsStepStart < 1500)
          break;

        if (!player.isRunning() || millis() - ttsStepStart > 8000) {

          if (allowDuring && playerWasRunning) {
            ttsState = TTS_FADE_UP;
          } else {
            player.lockOutput = false;
            player.setOutputPins(false);
            ttsState = TTS_IDLE;
          }

          ttsStepStart = millis();
        }
        break;

      // ==============================================================
      // FADE UP 
      // ==============================================================
      case TTS_FADE_UP: {

        static bool fadeStart      = false;
        static bool streamRestart  = false;

        if (!fadeStart) {
          fade_current = player.getVolume();
          doFadeUpStart();
          fadeStart = true;
        }

        if (!streamRestart && !player.isRunning()) {
          vTaskDelay(pdMS_TO_TICKS(250));   // watchdog / I2S safety
          player.lockOutput = false;
          player.sendCommand({PR_PLAY, config.lastStation()});
          streamRestart = true;
        }

        if (doFadeUpStep()) {
          player.setVolume(clock_tts_prev_volume);
          fadeStart = false;
          streamRestart = false;
          ttsState = TTS_IDLE;
        }

        if (millis() - ttsStepStart > 12000) {
          player.lockOutput = false;
          player.setVolume(clock_tts_prev_volume);
          fadeStart = false;
          streamRestart = false;
          ttsState = TTS_IDLE;
        }
      }
      break;
    }

    vTaskDelay(pdMS_TO_TICKS(60));
  }
}
