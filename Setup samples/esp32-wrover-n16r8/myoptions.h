/* Experimental ESP32-WROVER-E N16R8 example for FusionEdge. */
/* Review every GPIO assignment before connecting hardware. */

#pragma once

#define L10N_LANGUAGE HU // HU NL PL RU EN EL
#define NAMEDAYS_FILE HU
#define CLOCK_TTS_LANGUAGE "hu"
// #define IMPERIALUNIT // Requires LANGUAGE EN

//#define USE_DLNA
//#define dlnaHost "192.168.1.10"
//#define dlnaIDX 21

//#define LEDSTRIP_PIN 4
#define RSSI_DIGIT true

//#define DSP_MODEL DSP_ILI9486
#define DSP_MODEL DSP_ILI9488
//#define DSP_MODEL DSP_ILI9341
//#define DSP_MODEL DSP_ST7796

#define TFT_DC   4
#define TFT_CS   5
#define TFT_RST -1
#define TFT_SCK 18
#define TFT_MOSI 23
//#define TFT_MISO 19

#define BRIGHTNESS_PIN 14

/* Touch panel - disabled in this tested example. */
//#define TS_MODEL TS_MODEL_XPT2046
//#define TS_CS 15
//#define TS_MODEL TS_MODEL_FT6X36
//#define TS_MODEL TS_MODEL_GT911
//#define TS_SDA 21
//#define TS_SCL 22
//#define TS_RST 255
//#define TS_INT 255

/* SD card - disabled in this tested example. */
//#define SDC_CS 15
//#define SD_SPIPINS 18, 19, 23, SDC_CS
//#define SDSPISPEED 4000000

/* PCM5102A or compatible I2S DAC. */
#define I2S_DOUT 27
#define I2S_BCLK 26
#define I2S_LRC  25

/* Encoder 1. GPIO34-39 are input-only and require external pull resistors. */
#define ENC_BTNR 34
#define ENC_BTNL 36
#define ENC_BTNB 39
#define ENC_INTERNALPULLUP false

/* Encoder 2. GPIO35 is input-only and requires an external pull resistor. */
#define ENC2_BTNR 33
#define ENC2_BTNL 35
#define ENC2_BTNB 32
#define ENC2_INTERNALPULLUP false

//#define RTC_SCL 22
//#define RTC_SDA 21
//#define RTC_MODULE DS3231

#define IR_PIN 13
//#define WAKE_PIN1 32
//#define WAKE_PIN2 33

//#define EXT_WEATHER true
//#define MUTE_PIN 2
//#define MUTE_VAL LOW
//#define PLAYER_FORCE_MONO false
//#define I2S_INTERNAL false
//#define TFT_ROTATE 0
//#define HIDE_VOLPAGE
