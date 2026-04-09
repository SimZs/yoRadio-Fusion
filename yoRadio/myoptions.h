#ifndef myoptions_h
#define myoptions_h

#define LED_BUILTIN     255               /*  Onboard LED Pin (turn OFF by default) */
#define L10N_LANGUAGE RO // HU NL PL RU EN EL Enter your own language here!!
//#define NAMEDAYS_FILE NL // HU, PL, NL
#define CLOCK_TTS_LANGUAGE "ro" //Default language TTS e.g. pl,en,de,ru,fr,hu

#define ARDUINO_ESP32S3_DEV
#define DSP_MODEL DSP_ST7789
#define DSP_HSPI true
#define SPI_INTERFACE HSPI

#define TFT_WIDTH  320
#define TFT_HEIGHT 240

#define TFT_MOSI 45
#define TFT_MISO -1   
#define TFT_CLK  40
#define TFT_CS   42
#define TFT_DC   41
#define TFT_RST  39   
#define TFT_BL   5
#define BRIGHTNESS_PIN 5
#define TFT_BL_ON HIGH 

#define TS_MODEL_CST328 5
#define TS_MODEL TS_MODEL_CST328
#define TS_SDA 1
#define TS_SCL 3
#define TS_INT 4
#define TS_RST 2
// pentru landscape corect
#define TS_SWAP_XY  1
#define TS_INVERT_X 1
#define TS_INVERT_Y 0

// AUDIO corect
#define I2S_DOUT  47
#define I2S_BCLK  48
#define I2S_LRC  38

// SD CARD
#define SDC_CS 21
#define SD_SPIPINS 40, 46, 45

/* ENCODER 1 */
#define ENC_BTNR 18 // S2
#define ENC_BTNL 15 // S1
#define ENC_BTNB 44 // KEY
#define ENC_INTERNALPULLUP    true

/* Light sensor */
//#define LIGHT_SENSOR         34
//#define AUTOBACKLIGHT_MAX    1024

// RTC (din tabel)
//#define RTC_SDA 11
//#define RTC_SCL 10
//#define RTC_INT 9

#define I2S_INTERNAL            false               /*  Dacă este true, se utilizează DAC-ul ESP32 încorporat  */
#define RSSI_DIGIT              true              /*   false - scară, true - cifre pentru nivelul WiFi  */
#define EXT_WEATHER             false                /*  true - Afișează datele meteo extinse   */
//#define DSP_INVERT_TITLE        true               /* true - Inversează culorile titlului stației  */
//#define CLOCKFONT_MONO          false                 /* true - Folosește fonturi monospațiate */
//#define BITRATE_FULL            true                /* true - Activează rata de biți "imagine */
//#define BUFLEN                  170                /* marimea buffer default -170 */
#define PLAYER_FORCE_MONO false  


#endif
