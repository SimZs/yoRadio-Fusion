#ifndef myoptions_h
#define myoptions_h

#define L10N_LANGUAGE HU // HU NL PL RU EN EL Enter your own language here!!
#define NAMEDAYS_FILE HU // HU, PL, NL
#define CLOCK_TTS_LANGUAGE "hu" //Default language TTS e.g. pl,en,de,ru,fr,hu
//#define AM_PM_STYLE
//#define USDATE
#define DIRECT_CHANNEL_CHANGE 

#define DSP_MODEL            DSP_ST7789
#define DSP_HSPI             true

/******************************************/

#define LED_INVERT           true
#define TFT_CS               15
#define TFT_DC               2
#define TFT_RST              -1

/******************************************/

#define BRIGHTNESS_PIN      21
#define LIGHT_SENSOR        34
#define AUTOBACKLIGHT_MAX   1024

#define RSSI_DIGIT           false

/*  TOUCHSCREEN  */
#define TS_MODEL             TS_MODEL_XPT2046
#define TS_SPIPINS           25, 39, 32    /* SCK, MISO, MOSI */
#define TS_CS                33

/******************************************/

#define PLAYER_FORCE_MONO false             /*  mono option on boot - false stereo, true mono  */
#define I2S_INTERNAL      false             /*  If true - use esp32 internal DAC  */

/* SD Card reader */
#define SD_SPIPINS    18, 19, 23      /* SCK, MISO, MOSI */
#define SDC_CS        5              /* Chip Select */

/* Remote control sensor pin */
//#define IR_PIN               34
//#define IR_TIMEOUT           50              /*  see kTimeout description in IRremoteESP8266 example */

/* I2S DAC */

#define I2S_DOUT             27 
#define I2S_BCLK              4 
#define I2S_LRC              22 

/* Encoder1 */
#define ENC_BTNR             26
#define ENC_BTNL             35
#define ENC_BTNB             0
#define ENC_INTERNALPULLUP   false

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
