#ifndef myoptions_h
#define myoptions_h

#define L10N_LANGUAGE HU // Supported languages: HU NL PL RU EN GR SK by Tamás Várai
#define NAMEDAYS_FILE HU // Supported languages: HU, PL, NL, GR, (UA Local/namedays/namedays_UA.h is not filled in.) by Tamás Várai
#define CLOCK_TTS_LANGUAGE "hu" //Default language TTS e.g. pl,en,de,ru,fr,hu
//#define USDATE
//#define IMPERIALUNIT

// --- DLNA / Synology ---
//#define USE_DLNA
//#define dlnaHost "192.168.180.122" // DLNA server IP address
//#define dlnaIDX  21  //MUSIC container index, on Synology NAS is usually 21

#define DSP_MODEL            DSP_GC9A01
#define DSP_HSPI             false
#define TFT_CS               2
#define TFT_DC               18
#define TFT_RST              21
#define TFT_SCLK             3
#define TFT_MOSI             10
#define TFT_MISO             -1     
#define TFT_BL               42  
#define LED_INVERT           false  

/******************************************/

/*  TOUCHSCREEN  */
/*  Capacitive I2C touch screen  */

#define TS_MODEL             TS_MODEL_CST816
#define TS_SDA               8
#define TS_SCL               9
#define TS_INT               11
#define TS_RST               0
#define TS_TCA6408_PRESENT   0

/******************************************/

#define BRIGHTNESS_PIN       42

#define PLAYER_FORCE_MONO false             /*  mono option on boot - false stereo, true mono  */
#define I2S_INTERNAL      false             /*  If true - use esp32 internal DAC  */

/* SD Card reader */
#define SD_HSPI       true
#define SD_SPIPINS    41, 48, 47      /* SCK, MISO, MOSI */
#define SDC_CS        40              /* Chip Select */
#define SDSPISPEED    4000000 /* 4MHz - Slower speed to prevent display flicker on shared SPI bus */

/* Remote control sensor pin */
//#define IR_PIN               43
//#define IR_TIMEOUT           50              /*  see kTimeout description in IRremoteESP8266 example */

/* I2S DAC */

#define I2S_DOUT             7
#define I2S_BCLK             5
#define I2S_LRC              4

/* Encoder1 */
//#define ENC_BTNL             38
//#define ENC_BTNB             39
//#define ENC_BTNR             44
//#define ENC_INTERNALPULLUP   false

/* Encoder2 */
//#define ENC2_BTNR             46
//#define ENC2_BTNL             6
//#define ENC2_BTNB             18
//#define ENC2_INTERNALPULLUP   true


/* Light sensor */
//#define LIGHT_SENSOR         34
//#define AUTOBACKLIGHT_MAX    1024

/*  BUTTONS  */
//#define BTN_LEFT              46          /*  VolDown, Prev */
//#define BTN_CENTER            6           /*  Play, Stop, Show playlist */
//#define BTN_RIGHT             18          /*  VolUp, Next */
//#define BTN_UP                18           /*  Prev, Move Up */
//#define BTN_DOWN              255           /*  Next, Move Down */
//#define BTN_INTERNALPULLUP    true          /*  Enable the weak pull up resistors */
//#define BTN_LONGPRESS_LOOP_DELAY    200     /*  Delay between calling DuringLongPress event */
//#define BTN_CLICK_TICKS    300              /*  Event Timing https://github.com/mathertel/OneButton#event-timing */
//#define BTN_PRESS_TICKS    500              /*  Event Timing https://github.com/mathertel/OneButton#event-timing */

/*  Other settings.  */
//#define MUTE_PIN    38           /*  MUTE Pin */
//#define MUTE_VAL    LOW     /*  Write this to MUTE_PIN when player is stopped. Default is "HIGH" */
//#define PLAYER_FORCE_MONO false   /*  mono option on boot - false stereo, true mono. Default is "false" */
//#define ROTATE_90 false     /*  Optional 90 degree rotation for square displays. Default is "false"*/
//#define HIDE_VOLPAGE       /* Hide the "Volume" page, using the progress bar as a guide. (MOD nva_lw and Maleksm)  */
//#define HIDE_DATE            /* Hide date. (MOD nva_lw and Maleksm)  */
//#define WAKE_PIN              255
//#define NAME_STRIM                /* Show station name from stream. (MOD Maleksm)  */

//#define DOWN_LEVEL           2      /* lowest level brightness (from 0 to 255, default "2"). (MOD Maleksm) */
//#define DOWN_INTERVAL     60     /* interval for BacklightDown in sec (60 sec = 1 min, default "60"). (MOD Maleksm) */

/*  Indicator TM1637           MOD Maleksm (from version v0.9.435(m)-3.3.2a) */
//#define TM_CLK       3    /* GPIO for CLK. For TM1637 required parameter. */
//#define TM_DIO       8    /* GPIO for DIO. For TM1637 required parameter. */
//#define TM_BRIGHT   5    /* TM1637 indicator brightness. Optional parameter. Valid values are 0 - 7. Default is 4. */

/*  RTC control  */
//#define RTC_MODULE    DS3231  /* or DS1307  */
//#define RTC_SDA       255    /* 47 */
//#define RTC_SCL       255    /* 48 */
#endif
