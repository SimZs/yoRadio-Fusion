#ifndef myoptions_h
#define myoptions_h

#define L10N_LANGUAGE HU // HU NL PL RU EN Enter your own language here!!
#define NAMEDAYS_FILE HU // HU, PL, NL
#define CLOCK_TTS_LANGUAGE "hu" //Default language TTS e.g. pl,en,de,ru,fr,hu
//#define USDATE   //US Date format
//#define IMPERIALUNIT

// --- DLNA / Synology ---
#define USE_DLNA
#define dlnaHost "192.168.180.122" // DLNA server IP address
#define dlnaIDX  21  //MUSIC container index, on Synology NAS is usually 21

#define SAVER_Y_MIN  160

#define DSP_MODEL         DSP_NV3041A

/******************************************/

#define TFT_CS  45                /*  SPI CS pin  */    
#define TFT_RST -1                /*  SPI RST pin.  set to -1 and connect to Esp EN pin */
#define TFT_SCK 47                /*  SPI DC/RS pin  */
#define TFT_D0 21
#define TFT_D1 48
#define TFT_D2 40
#define TFT_D3 39
#define GFX_BL 1 
/******************************************/

#define BRIGHTNESS_PIN		  1

#define RSSI_STEPS       -50,-60,-70,-80
#define RSSI_DIGIT       false

/*  TOUCHSCREEN  */
#define  TS_MODEL   TS_MODEL_GT911

/*  Capacitive I2C touch screen  */
#define TS_SDA      8    
#define TS_SCL      4       
#define TS_INT      3    
#define TS_RST      38 

/******************************************/

#define PLAYER_FORCE_MONO false             /*  mono option on boot - false stereo, true mono  */
#define I2S_INTERNAL      false             /*  If true - use esp32 internal DAC  */

/* SD Card reader */
#define SD_SPIPINS     12, 13, 11      /* SCK, MISO, MOSI */
#define SDC_CS         10              /* Chip Select */
#define SDSPISPEED     4000000 /* 4MHz - Slower speed to prevent display flicker on shared SPI bus */

/* Remote control sensor pin */
#define IR_PIN        17
#define IR_TIMEOUT    50              /*  see kTimeout description in IRremoteESP8266 example */

/* I2S DAC */

//#define VS1053_CS 255

#define I2S_DOUT 9
#define I2S_BCLK 14
#define I2S_LRC 5

/* Encoder1 */
#define ENC_BTNR             15
#define ENC_BTNL             16
#define ENC_BTNB             7
#define ENC_INTERNALPULLUP   false

/* Encoder2 */
//#define ENC2_BTNR             46
//#define ENC2_BTNL             6
//#define ENC2_BTNB             18
//#define ENC2_INTERNALPULLUP   false

/*  BUTTONS  */
//#define BTN_LEFT              46          /*  VolDown, Prev */
#define BTN_CENTER            46          /*  Play, Stop, Show playlist */
//#define BTN_RIGHT             18          /*  VolUp, Next */
//#define BTN_UP                18           /*  Prev, Move Up */
//#define BTN_DOWN              255           /*  Next, Move Down */
//#define BTN_INTERNALPULLUP    true          /*  Enable the weak pull up resistors */
//#define BTN_LONGPRESS_LOOP_DELAY    200     /*  Delay between calling DuringLongPress event */
//#define BTN_CLICK_TICKS    300              /*  Event Timing https://github.com/mathertel/OneButton#event-timing */
//#define BTN_PRESS_TICKS    500              /*  Event Timing https://github.com/mathertel/OneButton#event-timing */

/*  Other settings.  */
#define MUTE_PIN    6           /*  MUTE Pin */
#define MUTE_VAL    LOW     /*  Write this to MUTE_PIN when player is stopped. Default is "HIGH" */
//#define PLAYER_FORCE_MONO false   /*  mono option on boot - false stereo, true mono. Default is "false" */
//#define ROTATE_90 false     /*  Optional 90 degree rotation for square displays. Default is "false"*/
//#define HIDE_VOLPAGE       /* Hide the "Volume" page, using the progress bar as a guide. (MOD nva_lw and Maleksm)  */
//#define WAKE_PIN              255
//#define NAME_STRIM                /* Show station name from stream. (MOD Maleksm)  */

/*  Indicator TM1637           MOD Maleksm (from version v0.9.435(m)-3.3.2a) */
//#define TM_CLK       3    /* GPIO for CLK. For TM1637 required parameter. */
//#define TM_DIO       8    /* GPIO for DIO. For TM1637 required parameter. */
//#define TM_BRIGHT   5    /* TM1637 indicator brightness. Optional parameter. Valid values are 0 - 7. Default is 4. */

/*  RTC control  */
//#define RTC_MODULE    DS3231  /* or DS1307  */
//#define RTC_SDA       255    /* 47 */
//#define RTC_SCL       255    /* 48 */
#endif
