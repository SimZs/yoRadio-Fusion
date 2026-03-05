#ifndef myoptions_h
#define myoptions_h

#define L10N_LANGUAGE HU // HU NL PL RU EN EL Enter your own language here!!
#define NAMEDAYS_FILE HU // HU, PL, NL
#define CLOCK_TTS_LANGUAGE "hu" //Default language TTS e.g. pl,en,de,ru,fr,hu
//#define AM_PM_STYLE
//#define USDATE   //US Date format
//#define DIRECT_CHANNEL_CHANGE
//#define IMPERIALUNIT 

// --- DLNA / Synology ---
//#define USE_DLNA
//#define dlnaHost "192.168.180.122"
//#define dlnaIDX  21

#define DSP_MODEL            DSP_GC9A01_I80
// --- GC9A01 I80 (8080) ---
#define TFT_CS               2
#define TFT_DC               18
#define TFT_RST              21
#define TFT_BL               42
#define LED_INVERT           false

// 8080 Databus (D0..D7)
#define TFT_D0               10
#define TFT_D1               11
#define TFT_D2               12
#define TFT_D3               13
#define TFT_D4               14
#define TFT_D5               15
#define TFT_D6               16
#define TFT_D7               17

// 8080
#define TFT_WR               3
#define TFT_RD               -1 

/******************************************/

/*  TOUCHSCREEN  */
/*  Capacitive I2C touch screen  */

//#define TS_MODEL             TS_MODEL_CST816
//#define TS_SDA               8
//#define TS_SCL               9
//#define TS_INT               45
//#define TS_RST               0
//#define TS_TCA6408_PRESENT   1
//#define TCA6408_ADDR         0x20
//#define TCA6408_TS_BIT       0
//#define TS_INT               45

/******************************************/

#define BRIGHTNESS_PIN       42

#define PLAYER_FORCE_MONO true             /*  mono option on boot - false stereo, true mono  */
#define I2S_INTERNAL      false             /*  If true - use esp32 internal DAC  */

/* SD Card reader */
//#define SD_SPIPINS    41, 48, 47      /* SCK, MISO, MOSI */
//#define SDC_CS        40              /* Chip Select */

/* Remote control sensor pin */
#define IR_PIN               43
#define IR_TIMEOUT           50              /*  see kTimeout description in IRremoteESP8266 example */

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
#define WEATHER_FMT_SHORT
//#define MUTE_PIN    38           /*  MUTE Pin */
//#define MUTE_VAL    LOW     /*  Write this to MUTE_PIN when player is stopped. Default is "HIGH" */
//#define PLAYER_FORCE_MONO true   /*  mono option on boot - false stereo, true mono. Default is "false" */
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
