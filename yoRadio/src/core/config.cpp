#include "options.h"
#include "config.h"
#include <SPIFFS.h> /* ------------------myTheme webUI upload needed---------------*/ 
#include "display.h"
#include "player.h"
#include "network.h"
#include "netserver.h"
#include "controls.h"
#include "timekeeper.h"
#include "telnet.h"
#include "rtcsupport.h"
#include "../displays/defaults.h" 
#include "../displays/tools/l10n.h"
#ifdef USE_SD
#include "sdmanager.h"
#endif
#ifdef USE_NEXTION
#include "../displays/nextion.h"
#endif
#include <cstddef>
#include "driver/rtc_io.h"

#if DSP_MODEL==DSP_DUMMY
#define DUMMYDISPLAY
#endif

#ifdef USE_DLNA
extern volatile bool g_dlnaPlaylistDirty;
#endif

Config config;
#if IR_PIN != 255
QueueHandle_t irQueue = nullptr;
#endif

void u8fix(char *src){
  char last = src[strlen(src)-1]; 
  if ((uint8_t)last >= 0xC2) src[strlen(src)-1]='\0';
}

bool Config::_isFSempty() {
  const char* reqiredFiles[] = {"dragpl.js.gz","ir.css.gz","irrecord.html.gz","ir.js.gz","logo.svg.gz","options.html.gz","player.html.gz","script.js.gz",
                                "style.css.gz","updform.html.gz","theme.css"};
  const uint8_t reqiredFilesSize = 11;
  char fullpath[28];
  if(SPIFFS.exists("/www/settings.html")) SPIFFS.remove("/www/settings.html");
  if(SPIFFS.exists("/www/update.html")) SPIFFS.remove("/www/update.html");
  if(SPIFFS.exists("/www/index.html")) SPIFFS.remove("/www/index.html");
  if(SPIFFS.exists("/www/ir.html")) SPIFFS.remove("/www/ir.html");
  if(SPIFFS.exists("/www/elogo.png")) SPIFFS.remove("/www/elogo.png");
  if(SPIFFS.exists("/www/elogo84.png")) SPIFFS.remove("/www/elogo84.png");
  for (uint8_t i=0; i<reqiredFilesSize; i++){
    sprintf(fullpath, "/www/%s", reqiredFiles[i]);
    if(!SPIFFS.exists(fullpath)) {
      Serial.println(fullpath);
      return true;
    }
  }
  return false;
}

void Config::init() {
  EEPROM.begin(EEPROM_SIZE);
  sdResumePos = 0;
    /*----- I2C init -----*/
#if (RTC_MODULE == DS3231) || (TS_MODEL == TS_MODEL_FT6X36) || (TS_MODEL == TS_MODEL_GT911)
    Wire.begin(TS_SDA, TS_SCL);
    Wire.setClock(400000);
    Serial.println("Scanning I2S...");
    for (uint8_t addr = 1; addr < 127; addr++) {
        Wire.beginTransmission(addr);
        if (Wire.endTransmission() == 0) { Serial.printf("Found: 0x%02X\n", addr); }
    }
#endif
//DLNA modplus
#ifdef USE_DLNA 
  isBooting = true; 
  resumeAfterModeChange = false; 
#endif 
//DLNA modplus
  screensaverTicks = 0;
  screensaverPlayingTicks = 0;
  newConfigMode = 0;
  isScreensaver = false;
  memset(tmpBuf, 0, BUFLEN);
  //bootInfo();
#if RTCSUPPORTED
  _rtcFound = false;
  BOOTLOG("RTC begin(SDA=%d,SCL=%d)", RTC_SDA, RTC_SCL);
  if(rtc.init()){
    BOOTLOG("done");
    _rtcFound = true;
  }else{
    BOOTLOG("[ERROR] - Couldn't find RTC");
  }
#endif
  emptyFS = true;
#if IR_PIN!=255
    irBtnId = -1;
#endif
#if defined(SD_SPIPINS) || SD_HSPI
  #if !defined(SD_SPIPINS)
    SDSPI.begin();
  #else
    SDSPI.begin(SD_SPIPINS); // SCK, MISO, MOSI
  #endif
#endif
  eepromRead(EEPROM_START, store);
// ---- Playlist source normalization ----
if (store.playlistSource > PL_SRC_DLNA)
    store.playlistSource = PL_SRC_WEB;

if (store.lastPlayedSource > PL_SRC_DLNA)
    store.lastPlayedSource = PL_SRC_WEB;

#ifdef USE_DLNA
    // Restore the playlist that actually played last. playlistSource may be
    // changed temporarily while browsing DLNA and is therefore not authoritative.
    store.playlistSource = store.lastPlayedSource;
#else
    store.playlistSource   = PL_SRC_WEB;
    store.lastPlayedSource = PL_SRC_WEB;
#endif
  bootInfo(); // https://github.com/e2002/yoradio/pull/149
  if (store.config_set != 4262) {
    setDefaults();
  }
  if (store.version > CONFIG_VERSION) {
    saveValue(&store.version, (uint16_t)CONFIG_VERSION, true, true);
  } else {
    while (store.version != CONFIG_VERSION) _setupVersion();
  }
#if (DSP_MODEL == DSP_GC9A01) || (DSP_MODEL == DSP_GC9A01A) || (DSP_MODEL == DSP_GC9A01_I80) || (DSP_MODEL == DSP_ST7789_76) || (DSP_MODEL == DSP_ST7789_240) || (DSP_MODEL == DSP_NV3007_142)
  // These displays do not have enough vertical space for the Default VU layout.
  if (store.vuLayout == 0 || store.vuLayout > 3) {
    saveValue(&store.vuLayout, (uint8_t)2);
  }
#endif
  BOOTLOG("CONFIG_VERSION\t%d", store.version);

  store.play_mode = store.play_mode & 0b11;
//DLNA modplus
#ifdef USE_DLNA 
#else
  if(store.play_mode>1) store.play_mode=PM_WEB;
#endif
//DLNA modplus
  _initHW();
  if (!SPIFFS.begin(true)) {
    Serial.println("##[ERROR]#\tSPIFFS Mount Failed");
    return;
  }
  BOOTLOG("SPIFFS mounted");
  emptyFS = _isFSempty();
  if(emptyFS) BOOTLOG("SPIFFS is empty!");
  #if IR_PIN!=255
  // Ha van ircodes.csv a SPIFFS-en (pl. SPIFFS feltöltés után), beolvassa és törli
  if(SPIFFS.exists(IR_CSV_PATH)) {
    if(importIR()) Serial.println("IR codes loaded from SPIFFS ircodes.csv");
    SPIFFS.remove(IR_CSV_PATH);
  }
  #endif
  loadTheme();  /* ------------------myTheme webUI upload: reloading from SPIFFS--------------*/
  ssidsCount = 0;
  #ifdef USE_SD
  _SDplaylistFS = getMode()==PM_SDCARD?&sdman:(true?&SPIFFS:_SDplaylistFS);
  #else
  _SDplaylistFS = &SPIFFS;
  #endif
  _bootDone=false;
  setTimeConf();
//DLNA modplus
#ifdef USE_DLNA 
  isBooting = false;
#endif
//DLNA modplus
}

void Config::_setupVersion(){
  uint16_t currentVersion = store.version;
  switch(currentVersion){
    case 1:
      if (store.screensaverTimeout < 5 || store.screensaverTimeout > 65520) {
        saveValue(&store.screensaverTimeout, (uint16_t)20);
      }
      break;
    case 2:
      snprintf(tmpBuf, MDNS_LENGTH, "yoradio-%x", (unsigned int)getChipId());
      saveValue(store.mdnsname, tmpBuf, MDNS_LENGTH);
      saveValue(&store.skipPlaylistUpDown, false);
      break;
    case 3:
      if (store.screensaverPlayingTimeout < 1 || store.screensaverPlayingTimeout > 1080) {
        saveValue(&store.screensaverPlayingTimeout, (uint16_t)5);
      }
      break;
    case 4:
      saveValue(&store.abuff, (uint16_t)(VS1053_CS==255?7:10));
      saveValue(&store.telnet, true);
      saveValue(&store.watchdog, true);
      saveValue(&store.timeSyncInterval, (uint16_t)60);    //min
      saveValue(&store.timeSyncIntervalRTC, (uint16_t)24); //hours
      saveValue(&store.weatherSyncInterval, (uint16_t)30); // min
    default:
      break;
    case 12:
      // ledstrip plugin settings – _future_reserved[2] helyén + 2 új byte
      saveValue(&store.lsEnabled,    (uint8_t)1);
      saveValue(&store.lsSsEnabled,  (uint8_t)1);
      saveValue(&store.lsModel,      (uint8_t)0);
      saveValue(&store.lsBrightness, (uint8_t)30);
      break;
  }
  currentVersion++;
  saveValue(&store.version, currentVersion);
}

void Config::toggleMode() {

#ifdef USE_DLNA

    // --- WEB módban vagyunk ---
    if (getMode() == PM_WEB) {

        if (store.playlistSource == PL_SRC_WEB) {

            // WEB → DLNA
            uint8_t oldSrc = store.playlistSource;
            store.playlistSource = (uint8_t)PL_SRC_DLNA;

            if (playlistLength() == 0) {
              store.playlistSource = oldSrc;
              changeMode(PM_SDCARD);
              return;
             }

            saveValue(&store.playlistSource, (uint8_t)PL_SRC_DLNA, true, true);

            initPlaylistMode();
            display.resetQueue();
            display.putRequest(NEWMODE, PLAYER);
            display.putRequest(NEWSTATION);
            return;
        }
        else {
            // DLNA → SD
            changeMode(PM_SDCARD);
            return;
        }
    }

    // --- SD → WEB ---
    store.playlistSource = PL_SRC_WEB;
    saveValue(&store.playlistSource, (uint8_t)PL_SRC_WEB, true, true);
    changeMode(PM_WEB);

#else

    // DLNA nincs → sima toggle
    changeMode(getMode() == PM_SDCARD ? PM_WEB : PM_SDCARD);

#endif
}

void Config::changeMode(int newmode) { // DLNA mod
    // Serial.printf("Config.cpp-->changeMode() newmode: %d", newmode);
#ifdef USE_SD
    // Encoder dupla klikk (paraméter nélküli hívás)
    if (newmode == -1) {
        // DLNA nem választható encoderről
        newmode = (getMode() == PM_SDCARD) ? PM_WEB : PM_SDCARD;
    }

    // 🔒 biztonsági ellenőrzés
    if (newmode < 0 || newmode >= 2) { // 0 --> radio; 1 --> SD; 2 --> DLNA
        Serial.printf("##[ERROR]# changeMode invalid newmode: %d\n", newmode);
        return;
    }

    bool pir = player.isRunning();

    if (SDC_CS == 255 && newmode == PM_SDCARD) { return; }

    if (network.status == SOFT_AP || display.mode() == LOST) {
        saveValue(&store.play_mode, (uint8_t)PM_SDCARD);
        delay(50);
        ESP.restart();
    }

    /* === SD only when explicitly requested === */
    if (newmode == PM_SDCARD) {
        if (!sdman.ready) {
            if (!sdman.start()) {
                Serial.println("##[ERROR]# SD Not Found");
                netserver.requestOnChange(GETPLAYERMODE, 0);
                return;
            }
        }
    }

    /* === set mode === */
    store.play_mode = (playMode_e)newmode;
    saveValue(&store.play_mode, (uint8_t)store.play_mode, true, true);

    /* === filesystem binding === */
    if (getMode() == PM_SDCARD) {
        _SDplaylistFS = &sdman;
    } else {
        _SDplaylistFS = &SPIFFS; // WEB + DLNA
    }

    /* === SD specific actions === */
    if (getMode() == PM_SDCARD) {
        if (pir) { player.sendCommand({PR_STOP, 0}); }
        display.putRequest(NEWMODE, SDCHANGE);
        delay(50);
    } else {
        sdman.stop(); // WEB + DLNA → SD off
    }

    if (!_bootDone) { return; }

    initPlaylistMode();

    if (pir) {
    #ifdef USE_DLNA
        uint16_t st = (getMode() == PM_SDCARD) ? store.lastSdStation : (store.playlistSource == PL_SRC_DLNA ? store.lastDlnaStation : store.lastStation);
        player.sendCommand({PR_PLAY, st});
    #else
        uint16_t st = (getMode() == PM_SDCARD) ? store.lastSdStation : store.lastStation;
        player.sendCommand({PR_PLAY, st});
    #endif
    }

    netserver.resetQueue();
    netserver.requestOnChange(GETINDEX, 0);

    display.resetQueue();
    display.putRequest(NEWMODE, PLAYER);
    display.putRequest(NEWSTATION);
#endif
}

void Config::initSDPlaylist() {
#ifdef USE_SD
  //store.countStation = 0;
  //bool doIndex = !sdman.exists(INDEX_SD_PATH);
  //if(doIndex) sdman.indexSDPlaylist();
  sdman.indexSDPlaylist();
  if (SDPLFS()->exists(INDEX_SD_PATH)) {
    File index = SDPLFS()->open(INDEX_SD_PATH, "r");
    //store.countStation = index.size() / 4;
    //if(doIndex){
      lastStation(_randomStation());
      sdResumePos = 0;
    //}
    index.close();
    //saveValue(&store.countStation, store.countStation, true, true);
  }
#endif //#ifdef USE_SD
}

bool Config::spiffsCleanup(){
  bool ret = (SPIFFS.exists(PLAYLIST_SD_PATH)) || (SPIFFS.exists(INDEX_SD_PATH)) || (SPIFFS.exists(INDEX_PATH));
  if(SPIFFS.exists(PLAYLIST_SD_PATH)) SPIFFS.remove(PLAYLIST_SD_PATH);
  if(SPIFFS.exists(INDEX_SD_PATH)) SPIFFS.remove(INDEX_SD_PATH);
  if(SPIFFS.exists(INDEX_PATH)) SPIFFS.remove(INDEX_PATH);
  return ret;
}

void Config::waitConnection(){
#if I2S_DOUT==255
  return;
#endif
  while(!player.connproc) vTaskDelay(50);
  vTaskDelay(500);
}

char * Config::ipToStr(IPAddress ip){
  snprintf(ipBuf, 16, "%u.%u.%u.%u", ip[0], ip[1], ip[2], ip[3]);
  return ipBuf;
}
bool Config::prepareForPlaying(uint16_t stationId){
  setDspOn(1);
  vuRefLevel = 0;
  screensaverTicks=SCREENSAVERSTARTUPDELAY;
  screensaverPlayingTicks=SCREENSAVERSTARTUPDELAY;
  if(getMode()!=PM_SDCARD) {
    display.putRequest(PSTOP);
  }
  
  if(!loadStation(stationId)) return false;
  //setTitle(getMode()==PM_WEB?LANG::const_PlConnect:"[next track]");
  setTitle(LANG::const_PlConnect); //ittvan
  station.bitrate=0;
  setBitrateFormat(BF_UNKNOWN);
  display.putRequest(DBITRATE);
  display.putRequest(NEWSTATION);
  display.putRequest(NEWMODE, PLAYER);
  netserver.requestOnChange(STATION, 0);
  netserver.requestOnChange(MODE, 0);
  netserver.loop();
  netserver.loop();
  if(store.smartstart!=2)
    setSmartStart(0);
  return true;
}
void Config::configPostPlaying(uint16_t stationId){ //DLNA mod
  if (getMode() == PM_SDCARD) {
    saveValue(&store.lastSdStation, stationId);
  }
#ifdef USE_DLNA
  else if (store.playlistSource == PL_SRC_DLNA) {
    saveValue(&store.lastDlnaStation, stationId);
  }
#endif
  else {
    saveValue(&store.lastStation, stationId);
  }

  if(store.smartstart!=2) setSmartStart(1);
  netserver.requestOnChange(MODE, 0);
  display.putRequest(PSTART);
}

void Config::setSDpos(uint32_t val){
  if (getMode()==PM_SDCARD){
    sdResumePos = 0;                 // ha kézzel állítasz pozíciót, ne legyen régi resume
    if(!player.isRunning()){
      config.sdResumePos = val-player.sd_min;
    }else{
      player.setAudioFilePosition(val-player.sd_min); // futó lejátszásnál seek webről
    }
  }
}

void Config::initPlaylistMode() {
  uint16_t _lastStation = 0;

#ifdef USE_SD
  if (getMode() == PM_SDCARD) {
    if (!sdman.start()) {
      changeMode(PM_WEB);
      return;
    }
    initSDPlaylist();
    uint16_t cs = playlistLength();
    _lastStation = store.lastSdStation;
    if (_lastStation == 0 && cs > 0) _lastStation = _randomStation();
  } else
#endif
  {

#ifdef USE_DLNA
    if (store.playlistSource == PL_SRC_DLNA) {

      if (SPIFFS.exists(PLAYLIST_DLNA_PATH)) {
        initDLNAPlaylist();
      }

      uint16_t cs = playlistLength();

      // ⬇️ DLNA indexet CSAK innen vesszük
      _lastStation = store.lastDlnaStation;
      if (_lastStation == 0 && cs > 0) _lastStation = 1;

    } else
#endif
    {
      initPlaylist();
      uint16_t cs = playlistLength();
      _lastStation = store.lastStation;
      if (_lastStation == 0 && cs > 0) _lastStation = 1;
    }
  }

  // ⬇️ EGYSZER
  lastStation(_lastStation);
  loadStation(_lastStation);

  _bootDone = true;
}

void Config::_initHW(){
  loadTheme();
  #if IR_PIN!=255
  eepromRead(EEPROM_START_IR, ircodes);
/*  if(ircodes.ir_set!=4224){
    ircodes.ir_set=4224;
    memset(ircodes.irVals, 0, sizeof(ircodes.irVals));
  }*/
  #endif
  #if BRIGHTNESS_PIN!=255
    pinMode(BRIGHTNESS_PIN, OUTPUT);
    // analogWrite() creates the LEDC channel; its polarity can only be set afterwards.
    analogWrite(BRIGHTNESS_PIN, 0);
    ledcOutputInvert(BRIGHTNESS_PIN, BRIGHTNESS_INVERTED);
    setBrightness(false);
  #endif
}

uint16_t Config::color565(uint8_t r, uint8_t g, uint8_t b)
{
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

/* ------------------myTheme webUI upload---------------*/
void Config::loadThemeFromHFile() {
  File file = SPIFFS.open("/mytheme.h", "r");
  if (!file) {
    return;
  }

  while (file.available()) {
    String line = file.readStringUntil('\n');
    line.trim();

    if (line.startsWith("#define ")) {
      line.remove(0, 8); 
      int sepIndex = line.indexOf(' ');
      if (sepIndex == -1) continue;

      String key = line.substring(0, sepIndex);
      String val = line.substring(sepIndex + 1);
      val.trim();

      // Split RGB by comma
      int r = -1, g = -1, b = -1;
      int i1 = val.indexOf(',');
      int i2 = val.indexOf(',', i1 + 1);

      if (i1 > 0 && i2 > i1) {
        r = val.substring(0, i1).toInt();
        g = val.substring(i1 + 1, i2).toInt();
        b = val.substring(i2 + 1).toInt();

        if (r >= 0 && g >= 0 && b >= 0) {
          uint16_t color = color565(r, g, b);
          applyThemeColor(key, color);
        }
      }
    }
  }

  file.close();
}

void Config::applyThemeColor(const String& name, uint16_t color) {
  if      (name == "COLOR_BACKGROUND")           theme.background = color;
  else if (name == "COLOR_STATION_NAME")         theme.meta = color;
  else if (name == "COLOR_STATION_BG")           theme.metabg = color;
  else if (name == "COLOR_STATION_FILL")         theme.metafill = color;
  else if (name == "COLOR_SNG_TITLE_1")          theme.title1 = color;
  else if (name == "COLOR_SNG_TITLE_2")          theme.title2 = color;
  else if (name == "COLOR_DIGITS")               theme.digit = color;
  else if (name == "COLOR_DIVIDER")              theme.div = color;
  else if (name == "COLOR_CLOCK")                theme.clock = color;
  else if (name == "COLOR_CLOCK_BG")             theme.clockbg = color;
  else if (name == "COLOR_SECONDS")              theme.seconds = color;
  else if (name == "COLOR_DAY_OF_W")             theme.dow = color;
  else if (name == "COLOR_DATE")                 theme.date = color;
  else if (name == "COLOR_HEAP")                 theme.heap = color;
  else if (name == "COLOR_BUFFER")               theme.buffer = color;
  else if (name == "COLOR_IP")                   theme.ip = color;
  else if (name == "COLOR_VOLUME_VALUE")         theme.vol = color;
  else if (name == "COLOR_RSSI")                 theme.rssi = color;
  else if (name == "COLOR_BITRATE")              theme.bitrate = color;
  else if (name == "COLOR_VOLBAR_OUT")           theme.volbarout = color;
  else if (name == "COLOR_VOLBAR_IN")            theme.volbarin = color;
  else if (name == "COLOR_PL_CURRENT")           theme.plcurrent = color;
  else if (name == "COLOR_PL_CURRENT_BG")        theme.plcurrentbg = color;
  else if (name == "COLOR_PL_CURRENT_FILL")      theme.plcurrentfill = color;
  else if (name == "COLOR_PLAYLIST_0")           theme.playlist[0] = color;
  else if (name == "COLOR_PLAYLIST_1")           theme.playlist[1] = color;
  else if (name == "COLOR_PLAYLIST_2")           theme.playlist[2] = color;
  else if (name == "COLOR_PLAYLIST_3")           theme.playlist[3] = color;
  else if (name == "COLOR_PLAYLIST_4")           theme.playlist[4] = color;
  else if (name == "COLOR_WEATHER")              theme.weather = color;
  else if (name == "COLOR_VU_MAX")               theme.vumax = color;
  else if (name == "COLOR_VU_MIN")               theme.vumin = color;
}

void Config::loadTheme(){
 if (config.store.monoTheme) {
  theme.background = color565(0,0,0);
  theme.meta       = color565(255,255,255);
  theme.metabg     = color565(0,0,0);
  theme.metafill   = color565(180,180,180);
  theme.title1     = color565(240,240,240);
  theme.title2     = color565(140,140,140);
  theme.digit      = color565(240,240,240);
  theme.div        = color565(162,162,162);
  theme.weather    = color565(130,130,130);
  theme.vumax      = color565(240,240,240);
  theme.vumin      = color565(130,130,130);
  theme.clock      = color565(240,240,240);
  theme.clockbg    = color565(30,30,30);
  theme.seconds    = color565(170,170,170);
  theme.dow        = color565(255,255,255);
  theme.date       = color565(150,150,150);
  theme.heap       = color565(255,168,162);
  theme.buffer     = color565(150,150,150);
  theme.ip         = color565(150,150,150);
  theme.vol        = color565(240,240,240);
  theme.rssi       = color565(140,140,140);
  theme.bitrate    = color565(255,255,255);
  theme.volbarout  = color565(162,162,162);
  theme.volbarin   = color565(162,162,162);
  if (config.store.playlistMode) {
  theme.plcurrent     = color565(255,255,255);
  theme.plcurrentbg   = color565(10,10,10);
  theme.plcurrentfill = color565(10,10,10);
  theme.playlist[0]   = color565(130,130,130);
  theme.playlist[1]   = color565(130,130,130);
  theme.playlist[2]   = color565(130,130,130);
  theme.playlist[3]   = color565(130,130,130);
  theme.playlist[4]   = color565(130,130,130);
  } else {
  theme.plcurrent     = color565(0,0,0);
  theme.plcurrentbg   = color565(255,255,255);
  theme.plcurrentfill = color565(255,255,255);
  theme.playlist[0]   = color565(255,255,255);
  theme.playlist[1]   = color565(130,130,130);
  theme.playlist[2]   = color565(130,130,130);
  theme.playlist[3]   = color565(130,130,130);
  theme.playlist[4]   = color565(130,130,130);
  }
 } else {
  theme.background    = color565(COLOR_BACKGROUND);
  theme.meta          = color565(COLOR_STATION_NAME);
  theme.metabg        = color565(COLOR_STATION_BG);
  theme.metafill      = color565(COLOR_STATION_FILL);
  theme.title1        = color565(COLOR_SNG_TITLE_1);
  theme.title2        = color565(COLOR_SNG_TITLE_2);
  theme.digit         = color565(COLOR_DIGITS);
  theme.div           = color565(COLOR_DIVIDER);
  theme.weather       = color565(COLOR_WEATHER);
  theme.vumax         = color565(COLOR_VU_MAX);
  theme.vumin         = color565(COLOR_VU_MIN);
  theme.clock         = color565(COLOR_CLOCK);
  theme.clockbg       = color565(COLOR_CLOCK_BG);
  theme.seconds       = color565(COLOR_SECONDS);
  theme.dow           = color565(COLOR_DAY_OF_W);
  theme.date          = color565(COLOR_DATE);
  theme.heap          = color565(COLOR_HEAP);
  theme.buffer        = color565(COLOR_BUFFER);
  theme.ip            = color565(COLOR_IP);
  theme.vol           = color565(COLOR_VOLUME_VALUE);
  theme.rssi          = color565(COLOR_RSSI);
  theme.bitrate       = color565(COLOR_BITRATE);
  theme.volbarout     = color565(COLOR_VOLBAR_OUT);
  theme.volbarin      = color565(COLOR_VOLBAR_IN);
  if (config.store.playlistMode) {
  theme.plcurrent     = color565(255,255,255);
  theme.plcurrentbg   = color565(10,10,10);
  theme.plcurrentfill = color565(10,10,10);
  theme.playlist[0]   = color565(130,130,130);
  theme.playlist[1]   = color565(130,130,130);
  theme.playlist[2]   = color565(130,130,130);
  theme.playlist[3]   = color565(130,130,130);
  theme.playlist[4]   = color565(130,130,130);
  } else {
  theme.plcurrent     = color565(COLOR_PL_CURRENT);
  theme.plcurrentbg   = color565(COLOR_PL_CURRENT_BG);
  theme.plcurrentfill = color565(COLOR_PL_CURRENT_FILL);
  theme.playlist[0]   = color565(COLOR_PLAYLIST_0);
  theme.playlist[1]   = color565(COLOR_PLAYLIST_1);
  theme.playlist[2]   = color565(COLOR_PLAYLIST_2);
  theme.playlist[3]   = color565(COLOR_PLAYLIST_3);
  theme.playlist[4]   = color565(COLOR_PLAYLIST_4);
  }

  loadThemeFromHFile();
  if (config.store.playlistMode) {
    theme.plcurrent     = color565(255,255,255);
    theme.plcurrentbg   = color565(10,10,10);
    theme.plcurrentfill = color565(10,10,10);
    theme.playlist[0]   = color565(130,130,130);
    theme.playlist[1]   = color565(130,130,130);
    theme.playlist[2]   = color565(130,130,130);
    theme.playlist[3]   = color565(130,130,130);
    theme.playlist[4]   = color565(130,130,130);
  }
 }
  #include "../displays/tools/tftinverttitle.h"
}

void Config::reset(){
  setDefaults();
  delay(500);
  ESP.restart();
}
void Config::enableScreensaver(bool val){
  saveValue(&store.screensaverEnabled, val);
#ifndef DSP_LCD
  display.putRequest(NEWMODE, PLAYER);
#endif
}
void Config::setScreensaverTimeout(uint16_t val){
  val=constrain(val,5,65520);
  saveValue(&store.screensaverTimeout, val);
#ifndef DSP_LCD
  display.putRequest(NEWMODE, PLAYER);
#endif
}
void Config::setScreensaverBlank(bool val){
  saveValue(&store.screensaverBlank, val);
#ifndef DSP_LCD
  display.putRequest(NEWMODE, PLAYER);
#endif
}
void Config::setScreensaverPlayingEnabled(bool val){
  saveValue(&store.screensaverPlayingEnabled, val);
#ifndef DSP_LCD
  display.putRequest(NEWMODE, PLAYER);
#endif
}
void Config::setScreensaverPlayingTimeout(uint16_t val){
  val=constrain(val,1,1080);
  config.saveValue(&config.store.screensaverPlayingTimeout, val);
#ifndef DSP_LCD
  display.putRequest(NEWMODE, PLAYER);
#endif
}
void Config::setScreensaverPlayingBlank(bool val){
  saveValue(&store.screensaverPlayingBlank, val);
#ifndef DSP_LCD
  display.putRequest(NEWMODE, PLAYER);
#endif
}
void Config::setSntpOne(const char *val){
  bool tzdone = false;
  if (strlen(val) > 0 && strlen(store.sntp2) > 0) {
    configTime(store.tzHour * 3600 + store.tzMin * 60, getTimezoneOffset(), val, store.sntp2);
    tzdone = true;
  } else if (strlen(val) > 0) {
    configTime(store.tzHour * 3600 + store.tzMin * 60, getTimezoneOffset(), val);
    tzdone = true;
  }
  if (tzdone) {
    timekeeper.forceTimeSync = true;
    saveValue(config.store.sntp1, val, 35);
  }
}
void Config::setShowweather(bool val){
  config.saveValue(&config.store.showweather, val);
  timekeeper.forceWeather = true;
  display.putRequest(SHOWWEATHER);
}
void Config::setWeatherKey(const char *val){
  saveValue(store.weatherkey, val, WEATHERKEY_LENGTH);
  display.putRequest(NEWMODE, CLEAR);
  display.putRequest(NEWMODE, PLAYER);
}

#if IR_PIN != 255
void Config::setIrBtn(int val) {
    irBtnId = val;
    netserver.irRecordEnable = (irBtnId >= 0);
    irBankId = 0;
    netserver.irValsToWs(); // kiküldi a három mentett gombot a webszervernek
    IRCommand ircmd;
    if (val >= 0) {
        ircmd.irBtnId = val;   // a gombhoz tartozó index, -1 a mentéséshez
        ircmd.hasBtnId = true; // mentés engedélyezése
        ircmd.irBankId = 0;    // 0, 1, 2
        ircmd.hasBank = true;  // mentés engedélyezése
        xQueueSend(irQueue, &ircmd, 0);
        Serial.printf("config.cpp--> setIrBtn--> xQueueSend\n");
    } else {
        saveIR();
        Serial.println("config.cpp--> setIrBtn--> val: -1 (save)");
    }
}
#endif
void Config::resetSystem(const char *val, uint8_t clientId){
  if (strcmp(val, "system") == 0) {
    saveValue(&store.smartstart, (uint8_t)2, false);
    saveValue(&store.audioinfo, false, false);
    saveValue(&store.vumeter, false, false);
    saveValue(&store.softapdelay, (uint8_t)0, false);
    saveValue(&store.abuff, (uint16_t)(VS1053_CS==255?7:10), false);
    saveValue(&store.telnet, true);
    saveValue(&store.watchdog, true);
    snprintf(store.mdnsname, MDNS_LENGTH, "yoradio-%x", (unsigned int)getChipId());
    saveValue(store.mdnsname, store.mdnsname, MDNS_LENGTH, true, true);
    display.putRequest(NEWMODE, CLEAR); display.putRequest(NEWMODE, PLAYER);
    netserver.requestOnChange(GETSYSTEM, clientId);
    return;
  }
  if (strcmp(val, "screen") == 0) {
    saveValue(&store.flipscreen, false, false);
    display.flip();
    saveValue(&store.invertdisplay, false, false);
    display.invert();
    saveValue(&store.dspon, true, false);
    store.brightness = 100;
    setBrightness(false);
    saveValue(&store.contrast, (uint8_t)55, false);
    display.setContrast();
    saveValue(&store.numplaylist, false);
    saveValue(&store.screensaverEnabled, false);
    saveValue(&store.screensaverTimeout, (uint16_t)20);
    saveValue(&store.screensaverBlank, false);
    saveValue(&store.screensaverPlayingEnabled, false);
    saveValue(&store.screensaverPlayingTimeout, (uint16_t)5);
    saveValue(&store.screensaverPlayingBlank, false);
    display.putRequest(NEWMODE, CLEAR); display.putRequest(NEWMODE, PLAYER);
    netserver.requestOnChange(GETSCREEN, clientId);
    return;
  }
  if (strcmp(val, "timezone") == 0) {
    saveValue(&store.tzHour, (int8_t)3, false);
    saveValue(&store.tzMin, (int8_t)0, false);
    saveValue(store.sntp1, "pool.ntp.org", 35, false);
    saveValue(store.sntp2, "0.ru.pool.ntp.org", 35);
    saveValue(&store.timeSyncInterval, (uint16_t)60);
    saveValue(&store.timeSyncIntervalRTC, (uint16_t)24);
    configTime(store.tzHour * 3600 + store.tzMin * 60, getTimezoneOffset(), store.sntp1, store.sntp2);
    timekeeper.forceTimeSync = true;
    netserver.requestOnChange(GETTIMEZONE, clientId);
    return;
  }
  if (strcmp(val, "weather") == 0) {
    saveValue(&store.showweather, false, false);
    saveValue(store.weatherlat, "55.7512", 10, false);
    saveValue(store.weatherlon, "37.6184", 10, false);
    saveValue(store.weatherkey, "", WEATHERKEY_LENGTH);
    saveValue(&store.weatherSyncInterval, (uint16_t)30);
    //network.trueWeather=false;
    display.putRequest(NEWMODE, CLEAR); display.putRequest(NEWMODE, PLAYER);
    netserver.requestOnChange(GETWEATHER, clientId);
    return;
  }
  if (strcmp(val, "controls") == 0) {
    saveValue(&store.volsteps, (uint8_t)1, false);
    saveValue(&store.fliptouch, false, false);
    saveValue(&store.dbgtouch, false, false);
    saveValue(&store.skipPlaylistUpDown, false);
    setEncAcceleration(200);
    setIRTolerance(40);
    netserver.requestOnChange(GETCONTROLS, clientId);
    return;
  }
  if (strcmp(val, "1") == 0) {
    config.reset();
    return;
  }
}



void Config::setDefaults() {
  store.config_set = 4262;
  store.version = CONFIG_VERSION;
  store.volume = 12;
  store.balance = 0;
  store.trebble = 0;
  store.middle = 0;
  store.bass = 0;
  store.lastStation = 0;
  store.countStation = 0;
  store.lastSSID = 0;
  store.audioinfo = false;
  store.smartstart = 2;
  store.tzHour = 1;
  store.tzMin = 0;
  store.timezoneOffset = 0;
  store.vumeter=false;
  store.softapdelay=0;
  store.flipscreen=false;
  store.invertdisplay=false;
  store.numplaylist=false;
#if (DSP_MODEL == DSP_GC9A01) || (DSP_MODEL == DSP_GC9A01A) || (DSP_MODEL == DSP_GC9A01_I80) || (DSP_MODEL==DSP_ST7789_76) || (DSP_MODEL==DSP_ST7789_240) || (DSP_MODEL==DSP_NV3007_142)
  store.vuLayout = 2;   // ------ WEB UI STYLE: BoomBox-----
#else
  store.vuLayout = 0;   // ------ WEB UI STYLE: Default-----
#endif
  store.vuBarCountDef = VU_DEF_BARS_DEF; 
  store.vuBarGapDef = VU_DEF_GAP_DEF;
  store.vuBarCountStr = VU_DEF_BARS_STR; 
  store.vuBarGapStr = VU_DEF_GAP_STR;
  store.vuBarCountBbx = VU_DEF_BARS_BBX; 
  store.vuBarGapBbx = VU_DEF_GAP_BBX;
  store.vuBarCountStd = VU_DEF_BARS_STD; 
  store.vuBarGapStd = VU_DEF_GAP_STD;
  store.vuBarHeightDef = VU_DEF_HGT_DEF;
  store.vuBarHeightStr = VU_DEF_HGT_STR;
  store.vuBarHeightBbx = VU_DEF_HGT_BBX;
  store.vuBarHeightStd = VU_DEF_HGT_STD;
  store.vuMidColor    = VU_DEF_MIDCOLOR;
  store.vuFadeSpeedDef  = VU_DEF_FADE_DEF;
  store.vuFadeSpeedStr  = VU_DEF_FADE_STR;
  store.vuFadeSpeedBbx  = VU_DEF_FADE_BBX;
  store.vuFadeSpeedStd  = VU_DEF_FADE_STD;
  store.vuAlphaUpDef = VU_DEF_AUP_DEF;
  store.vuAlphaUpStr = VU_DEF_AUP_STR;
  store.vuAlphaUpBbx = VU_DEF_AUP_BBX;
  store.vuAlphaUpStd = VU_DEF_AUP_STD;
  store.vuAlphaDnDef = VU_DEF_ADN_DEF;
  store.vuAlphaDnStr = VU_DEF_ADN_STR;
  store.vuAlphaDnBbx = VU_DEF_ADN_BBX;
  store.vuAlphaDnStd = VU_DEF_ADN_STD;
  store.vuPeakUpDef  = VU_DEF_PUP_DEF;
  store.vuPeakUpStr  = VU_DEF_PUP_STR;
  store.vuPeakUpBbx  = VU_DEF_PUP_BBX;
  store.vuPeakUpStd  = VU_DEF_PUP_STD;
  store.vuPeakDnDef  = VU_DEF_PDN_DEF;
  store.vuPeakDnStr  = VU_DEF_PDN_STR;
  store.vuPeakDnBbx  = VU_DEF_PDN_BBX;
  store.vuPeakDnStd  = VU_DEF_PDN_STD;
  store.vuPeakColor  = VU_DEF_PEAK_COLOR;
  store.vuExpoDef    = VU_DEF_EXPO_DEF;
  store.vuExpoStr    = VU_DEF_EXPO_STR;
  store.vuExpoBbx    = VU_DEF_EXPO_BBX;
  store.vuExpoStd    = VU_DEF_EXPO_STD;
  store.vuFloorDef   = VU_DEF_FLOOR_DEF;
  store.vuFloorStr   = VU_DEF_FLOOR_STR;
  store.vuFloorBbx   = VU_DEF_FLOOR_BBX;
  store.vuFloorStd   = VU_DEF_FLOOR_STD;
  store.vuCeilDef    = VU_DEF_CEIL_DEF;
  store.vuCeilStr    = VU_DEF_CEIL_STR;
  store.vuCeilBbx    = VU_DEF_CEIL_BBX;
  store.vuCeilStd    = VU_DEF_CEIL_STD;
  store.vuGainDef    = VU_DEF_GAIN_DEF;
  store.vuGainStr    = VU_DEF_GAIN_STR;
  store.vuGainBbx    = VU_DEF_GAIN_BBX;
  store.vuGainStd    = VU_DEF_GAIN_STD;
  store.vuKneeDef    = VU_DEF_KNEE_DEF;
  store.vuKneeStr    = VU_DEF_KNEE_STR;
  store.vuKneeBbx    = VU_DEF_KNEE_BBX;
  store.vuKneeStd    = VU_DEF_KNEE_STD;
  store.vuMidPctDef     = VU_DEF_MID_PCT_DEF;
  store.vuMidPctStr     = VU_DEF_MID_PCT_STR;
  store.vuMidPctBbx     = VU_DEF_MID_PCT_BBX;
  store.vuMidPctStd     = VU_DEF_MID_PCT_STD;
  store.vuHighPctDef    = VU_DEF_HIGH_PCT_DEF;
  store.vuHighPctStr    = VU_DEF_HIGH_PCT_STR;
  store.vuHighPctBbx    = VU_DEF_HIGH_PCT_BBX;
  store.vuHighPctStd    = VU_DEF_HIGH_PCT_STD;
  store.vuMidOn  = 1;
  store.vuPeakOn = 1;
  store.monoTheme = 0;
  store.vuMidUserColor  = store.vuMidColor;
  store.vuPeakUserColor = store.vuPeakColor;
  config.store.vuLabelBgColor   = 0x7BEF; // label háttér szürke
  config.store.vuLabelTextColor = 0xFFFF; // L/R betű fehér
  store.vuLabelHeightDef = VU_DEF_LHGT_DEF;
  store.vuLabelHeightStr = VU_DEF_LHGT_STR;
  store.vuLabelHeightBbx = VU_DEF_LHGT_BBX;
  store.vuLabelHeightStd = VU_DEF_LHGT_STD;
  store.dateFormat = 1;
  store.showNameday = 0;
  store.clockFontId = 0;
  store.metaStNameSkip = 0;
  store.directChannelChange = 1;        // default ON
  store.stationsListReturnTime = 5;     // default 5 sec
  store.hours12 = 0;        // default OFF
  store.weatherIconSet = 0;
  store.playlistMode = 0;
  store.stallWatchdog = 0;
  store.blDimEnable   = 0;
  store.blDimLevel    = 5;
  store.blDimInterval = 60;
  store.ttsEnabled = 1;
  store.ttsDuringPlayback = false;
  store.ttsInterval = 60;
  store.clockFontMono = false;
  store.stationLine = true;
  store.shortWeather = false;
  store.fliptouch=false;
  store.dbgtouch=false;
  store.dspon=true;
  store.brightness=100;
  store.contrast=55;
  strlcpy(store.sntp1,"pool.ntp.org", 35);
  strlcpy(store.sntp2,"1.hu.pool.ntp.org", 35);
  store.showweather=false;
  strlcpy(store.weatherlat,"47.1109", 10);
  strlcpy(store.weatherlon,"18.5773", 10);
  strlcpy(store.weatherkey,"", WEATHERKEY_LENGTH);
  store.grndHeight = 0;
  store.pressureSlope_x1000 = 120;
  store._reserved = 0;
  store.lastSdStation = 0;
  store.lastDlnaStation = 0; //DLNA mod
  store.sdsnuffle = false;
  store.volsteps = 1;
  store.encacc = 200;
  store.play_mode = 0;
  store.irtlp = 35;
  store.btnpullup = true;
  store.btnlongpress = 200;
  store.btnclickticks = 300;
  store.btnpressticks = 500;
  store.encpullup = false;
  store.enchalf = false;
  store.enc2pullup = false;
  store.enc2half = false;
  store.forcemono = false;
  store.i2sinternal = false;
  store.rotate90 = false;
  store.screensaverEnabled = false;
  store.screensaverTimeout = 20;
  store.screensaverBlank = false;
  snprintf(store.mdnsname, MDNS_LENGTH, "yoradio-%x", (unsigned int)getChipId());
  store.skipPlaylistUpDown = false;
  store.screensaverPlayingEnabled = false;
  store.screensaverPlayingTimeout = 5;
  store.screensaverPlayingBlank = false;
  store.abuff = VS1053_CS==255?7:10;
  store.telnet = true;
  store.watchdog = true;
  store.timeSyncInterval = 60;    //min
  store.timeSyncIntervalRTC = 24; //hour
  store.weatherSyncInterval = 30; //min
  strlcpy(store.autoStartTime, "", sizeof(store.autoStartTime)); /* ----- Auto On-Off Timer --format: empty means disabled --- */
  strlcpy(store.autoStopTime, "", sizeof(store.autoStopTime)); /* ----- Auto On-Off Timer --format: empty means disabled --- */
  strlcpy(store.ttsDndStart, "", sizeof(store.ttsDndStart));
  strlcpy(store.ttsDndStop,  "", sizeof(store.ttsDndStop));
  store.playlistSource = PL_SRC_WEB;
  store.lastPlayedSource = PL_SRC_WEB;
  // ledstrip defaults
  store.lsEnabled     = 1;
  store.lsSsEnabled   = 1;
  store.lsModel       = 0;
  store.lsBrightness  = 30;
  eepromWrite(EEPROM_START, store);
}

void Config::setTimezone(int8_t tzh, int8_t tzm) {
  saveValue(&store.tzHour, tzh, false);
  saveValue(&store.tzMin, tzm);
}

void Config::setTimezoneOffset(uint16_t tzo) {
  saveValue(&store.timezoneOffset, tzo);
}

uint16_t Config::getTimezoneOffset() {
  return 0; // TODO
}

void Config::setSnuffle(bool sn){
  saveValue(&store.sdsnuffle, sn);
  //if(store.sdsnuffle) player.next();
}

#if IR_PIN!=255
void Config::saveIR(){
  eepromWrite(EEPROM_START_IR, ircodes);
}

// IR kódok exportálása CSV fájlba SPIFFS-re
// Formátum: gombNév\tval0\tval1\tval2
bool Config::exportIR() {
  static const char* irNames[] = {
    "POWER", "PLAY_STOP", "BACK", "PREV", "LIST", "NEXT",
    "VOLUME_DOWN", "MODE", "VOLUME_UP",
    "1", "2", "3", "4", "5", "6", "7", "8", "9", "0"
  };
  File file = SPIFFS.open(IR_CSV_PATH, "w");
  if (!file) return false;
  for (int i = 0; i < 19; i++) {
    file.printf("%s\t%llu\t%llu\t%llu\n",
      irNames[i],
      ircodes.irVals[i][0],
      ircodes.irVals[i][1],
      ircodes.irVals[i][2]);
  }
  file.close();
  return true;
}

// IR kódok importálása CSV fájlból (SPIFFS /data/ircodes.csv)
// Formátum: gombNév\tval0\tval1\tval2
bool Config::importIR() {
  File file = SPIFFS.open(IR_CSV_PATH, "r");
  if (!file) return false;
  static const char* irNames[] = {
    "POWER", "PLAY_STOP", "BACK", "PREV", "LIST", "NEXT",
    "VOLUME_DOWN", "MODE", "VOLUME_UP",
    "1", "2", "3", "4", "5", "6", "7", "8", "9", "0"
  };
  const int irCount = 19;
  bool anyLoaded = false;
  while (file.available()) {
    String line = file.readStringUntil('\n');
    line.trim();
    if (line.length() == 0) continue;
    // Parse: name\tval0\tval1\tval2
    int t1 = line.indexOf('\t');
    if (t1 < 0) continue;
    String name = line.substring(0, t1);
    int t2 = line.indexOf('\t', t1 + 1);
    if (t2 < 0) continue;
    int t3 = line.indexOf('\t', t2 + 1);
    if (t3 < 0) t3 = line.length();
    uint64_t v0 = strtoull(line.substring(t1 + 1, t2).c_str(), nullptr, 10);
    uint64_t v1 = strtoull(line.substring(t2 + 1, t3).c_str(), nullptr, 10);
    uint64_t v2 = (t3 < (int)line.length()) ? strtoull(line.substring(t3 + 1).c_str(), nullptr, 10) : 0;
    for (int i = 0; i < irCount; i++) {
      if (name == irNames[i]) {
        ircodes.irVals[i][0] = v0;
        ircodes.irVals[i][1] = v1;
        ircodes.irVals[i][2] = v2;
        anyLoaded = true;
        break;
      }
    }
  }
  file.close();
  if (anyLoaded) saveIR();
  return anyLoaded;
}
#endif

void Config::saveVolume(){
  saveValue(&store.volume, store.volume, true, true);
}

uint8_t Config::setVolume(uint8_t val) {
  store.volume = val;
  display.putRequest(DRAWVOL);
  netserver.requestOnChange(VOLUME, 0);
  return store.volume;
}

void Config::setTone(int8_t bass, int8_t middle, int8_t trebble) {
  saveValue(&store.bass, bass, false);
  saveValue(&store.middle, middle, false);
  saveValue(&store.trebble, trebble);
  player.setTone(store.bass, store.middle, store.trebble);
  netserver.requestOnChange(EQUALIZER, 0);
}

void Config::setSmartStart(uint8_t ss) {
  saveValue(&store.smartstart, ss);
}

void Config::setBalance(int8_t balance) {
  saveValue(&store.balance, balance);
  player.setBalance(-store.balance);
  netserver.requestOnChange(BALANCE, 0);
}

uint8_t Config::setLastStation(uint16_t val) {
  // Make "current item" persistent per mode
  if (getMode() == PM_SDCARD) {
    saveValue(&store.lastSdStation, val);
    return store.lastSdStation;
  }
#ifdef USE_DLNA
  if (store.playlistSource == PL_SRC_DLNA) {
    saveValue(&store.lastDlnaStation, val);
    return store.lastDlnaStation;
  }
#endif
  saveValue(&store.lastStation, val);
  return store.lastStation;
}

uint8_t Config::setCountStation(uint16_t val) {
  saveValue(&store.countStation, val);
  return store.countStation;
}

uint8_t Config::setLastSSID(uint8_t val) {
  saveValue(&store.lastSSID, val);
  return store.lastSSID;
}

void Config::setTitle(const char* title) {
  vuRefLevel = 0;
  memset(config.station.title, 0, BUFLEN);
  strlcpy(config.station.title, title, BUFLEN);
  u8fix(config.station.title);
  netserver.requestOnChange(TITLE, 0);
  netserver.loop();
  display.putRequest(NEWTITLE);
}

void Config::setStation(const char* station) {
  memset(config.station.name, 0, BUFLEN);
  strlcpy(config.station.name, station, BUFLEN);
  u8fix(config.station.title);
}

void Config::indexPlaylist() {
  File playlist = SPIFFS.open(PLAYLIST_PATH, "r");
  if (!playlist) {
    return;
  }
  int sOvol;
  File index = SPIFFS.open(INDEX_PATH, "w");
  while (playlist.available()) {
    uint32_t pos = playlist.position();
    if (parseCSV(playlist.readStringUntil('\n').c_str(), tmpBuf, tmpBuf2, sOvol)) {
      index.write((uint8_t *) &pos, 4);
    }
  }
  index.close();
  playlist.close();
}
//DLNA mod
#ifdef USE_DLNA
void Config::indexDLNAPlaylist() {
  File playlist = SPIFFS.open(PLAYLIST_DLNA_PATH, "r");
  if (!playlist) {
    Serial.println("[DLNA][IDX] Cannot open DLNA playlist");
    return;
  }

  File index = SPIFFS.open(INDEX_DLNA_PATH, "w");
  if (!index) {
    Serial.println("[DLNA][IDX] Cannot create DLNA index");
    playlist.close();
    return;
  }

  static char lineBuf[512]; 
  int sOvol = 0;

  uint32_t lines = 0;
  uint32_t ok = 0;

  while (playlist.available()) {
    uint32_t pos = playlist.position();

    // readBytesUntil nem allokál, stabil
    size_t n = playlist.readBytesUntil('\n', lineBuf, sizeof(lineBuf) - 1);
    lineBuf[n] = 0;

    // CRLF kezelés
    if (n > 0 && lineBuf[n - 1] == '\r') lineBuf[n - 1] = 0;

    // üres sor skip
    if (lineBuf[0] == 0) {
      lines++;
      continue;
    }

    // FONTOS: parseCSV kapjon ÍRHATÓ buffert (lineBuf), ne String.c_str()-t
    if (parseCSV(lineBuf, tmpBuf, tmpBuf2, sOvol)) {
      index.write((uint8_t*)&pos, 4);
      ok++;
    }

    lines++;

    // WDT/Task starvation ellen (DLNA/WiFi közben kellhet)
    if ((lines % 50) == 0) {
      delay(0);   // vagy yield();
    }
  }

  index.close();
  playlist.close();

  Serial.printf("[DLNA][IDX] DLNA playlist indexed: %lu/%lu\n", (unsigned long)ok, (unsigned long)lines);
}
#endif

void Config::initPlaylist() {
  //store.countStation = 0;
  if (!SPIFFS.exists(INDEX_PATH)) indexPlaylist();

  /*if (SPIFFS.exists(INDEX_PATH)) {
    File index = SPIFFS.open(INDEX_PATH, "r");
    store.countStation = index.size() / 4;
    index.close();
    saveValue(&store.countStation, store.countStation, true, true);
  }*/
}

#ifdef USE_DLNA //DLNA mod
void Config::initDLNAPlaylist() {
  indexDLNAPlaylist();

  if (SPIFFS.exists(INDEX_DLNA_PATH)) {
    File index = SPIFFS.open(INDEX_DLNA_PATH, "r");
    if (index) {
      //lastStation(_randomStation());
      index.close();
    }
  }
}
#endif

uint16_t Config::playlistLength(){
  uint16_t out = 0;
  if (SDPLFS()->exists(REAL_INDEX)) {
    File index = SDPLFS()->open(REAL_INDEX, "r");
    out = index.size() / 4;
    index.close();
  }
  return out;
}
bool Config::loadStation(uint16_t ls) {
  int sOvol;
  uint16_t cs = playlistLength();
  if (cs == 0) {
    memset(station.url, 0, BUFLEN);
    memset(station.name, 0, BUFLEN);
    strncpy(station.name, "yoRadio", BUFLEN);
    station.ovol = 0;
    return false;
  }
  if (ls > playlistLength()) {
    ls = 1;
  }
  File playlist = SDPLFS()->open(REAL_PLAYL, "r");
  File index = SDPLFS()->open(REAL_INDEX, "r");
  index.seek((ls - 1) * 4, SeekSet);
  uint32_t pos;
  index.readBytes((char *) &pos, 4);
  index.close();
  playlist.seek(pos, SeekSet);
  if (parseCSV(playlist.readStringUntil('\n').c_str(), tmpBuf, tmpBuf2, sOvol)) {
    memset(station.url, 0, BUFLEN);
    memset(station.name, 0, BUFLEN);
    strncpy(station.name, tmpBuf, BUFLEN);
    strncpy(station.url, tmpBuf2, BUFLEN);
    station.ovol = sOvol;
    setLastStation(ls);
  }
  playlist.close();
  return true;
}

char * Config::stationByNum(uint16_t num){
  File playlist = SDPLFS()->open(REAL_PLAYL, "r");
  File index = SDPLFS()->open(REAL_INDEX, "r");
  index.seek((num - 1) * 4, SeekSet);
  uint32_t pos;
  memset(_stationBuf, 0, sizeof(_stationBuf));
  index.readBytes((char *) &pos, 4);
  index.close();
  playlist.seek(pos, SeekSet);
  strncpy(_stationBuf, playlist.readStringUntil('\t').c_str(), sizeof(_stationBuf));
  playlist.close();
  return _stationBuf;
}

void Config::escapeQuotes(const char* input, char* output, size_t maxLen) {
  size_t j = 0;
  for (size_t i = 0; input[i] != '\0' && j < maxLen - 1; ++i) {
    if (input[i] == '"' && j < maxLen - 2) {
      output[j++] = '\\';
      output[j++] = '"';
    } else {
      output[j++] = input[i];
    }
  }
  output[j] = '\0';
}

bool Config::parseCSV(const char* line, char* name, char* url, int &ovol) {
  char *tmpe;
  const char* cursor = line;
  char buf[5];
  tmpe = strstr(cursor, "\t");
  if (tmpe == NULL) return false;
  strlcpy(name, cursor, tmpe - cursor + 1);
  if (strlen(name) == 0) return false;
  cursor = tmpe + 1;
  tmpe = strstr(cursor, "\t");
  if (tmpe == NULL) return false;
  strlcpy(url, cursor, tmpe - cursor + 1);
  if (strlen(url) == 0) return false;
  cursor = tmpe + 1;
  if (strlen(cursor) == 0) return false;
  strlcpy(buf, cursor, 4);
  ovol = atoi(buf);
  return true;
}

bool Config::parseJSON(const char* line, char* name, char* url, int &ovol) {
  char* tmps, *tmpe;
  const char* cursor = line;
  char port[8], host[246], file[254];
  tmps = strstr(cursor, "\":\"");
  if (tmps == NULL) return false;
  tmpe = strstr(tmps, "\",\"");
  if (tmpe == NULL) return false;
  strlcpy(name, tmps + 3, tmpe - tmps - 3 + 1);
  if (strlen(name) == 0) return false;
  cursor = tmpe + 3;
  tmps = strstr(cursor, "\":\"");
  if (tmps == NULL) return false;
  tmpe = strstr(tmps, "\",\"");
  if (tmpe == NULL) return false;
  strlcpy(host, tmps + 3, tmpe - tmps - 3 + 1);
  if (strlen(host) == 0) return false;
  if (strstr(host, "http://") == NULL && strstr(host, "https://") == NULL) {
    sprintf(file, "http://%s", host);
    strlcpy(host, file, strlen(file) + 1);
  }
  cursor = tmpe + 3;
  tmps = strstr(cursor, "\":\"");
  if (tmps == NULL) return false;
  tmpe = strstr(tmps, "\",\"");
  if (tmpe == NULL) return false;
  strlcpy(file, tmps + 3, tmpe - tmps - 3 + 1);
  cursor = tmpe + 3;
  tmps = strstr(cursor, "\":\"");
  if (tmps == NULL) return false;
  tmpe = strstr(tmps, "\",\"");
  if (tmpe == NULL) return false;
  strlcpy(port, tmps + 3, tmpe - tmps - 3 + 1);
  int p = atoi(port);
  if (p > 0) {
    sprintf(url, "%s:%d%s", host, p, file);
  } else {
    sprintf(url, "%s%s", host, file);
  }
  cursor = tmpe + 3;
  tmps = strstr(cursor, "\":\"");
  if (tmps == NULL) return false;
  tmpe = strstr(tmps, "\"}");
  if (tmpe == NULL) return false;
  strlcpy(port, tmps + 3, tmpe - tmps - 3 + 1);
  ovol = atoi(port);
  return true;
}

bool Config::parseWsCommand(const char* line, char* cmd, char* val, uint8_t cSize) {
  char *tmpe;
  tmpe = strstr(line, "=");
  if (tmpe == NULL) return false;
  memset(cmd, 0, cSize);
  strlcpy(cmd, line, tmpe - line + 1);
  //if (strlen(tmpe + 1) == 0) return false;
  memset(val, 0, cSize);
  strlcpy(val, tmpe + 1, strlen(line) - strlen(cmd) + 1);
  return true;
}

bool Config::parseSsid(const char* line, char* ssid, char* pass) {
  char *tmpe;
  tmpe = strstr(line, "\t");
  if (tmpe == NULL) return false;
  uint16_t pos = tmpe - line;
  if (pos > 29 || strlen(line) > 71) return false;
  memset(ssid, 0, 30);
  strlcpy(ssid, line, pos + 1);
  memset(pass, 0, 40);
  strlcpy(pass, line + pos + 1, strlen(line) - pos);
  return true;
}

bool Config::saveWifiFromNextion(const char* post){
  File file = SPIFFS.open(SSIDS_PATH, "w");
  if (!file) {
    return false;
  } else {
    file.print(post);
    file.close();
    ESP.restart();
    return true;
  }
}

bool Config::saveWifi() {
  if (!SPIFFS.exists(TMP_PATH)) return false;
  SPIFFS.remove(SSIDS_PATH);
  SPIFFS.rename(TMP_PATH, SSIDS_PATH);
  ESP.restart();
  return true;
}

void Config::setTimeConf(){
  if(strlen(store.sntp1)>0 && strlen(store.sntp2)>0){
    configTime(store.tzHour * 3600 + store.tzMin * 60, getTimezoneOffset(), store.sntp1, store.sntp2);
  }else if(strlen(store.sntp1)>0){
    configTime(store.tzHour * 3600 + store.tzMin * 60, getTimezoneOffset(), store.sntp1);
  }
}

bool Config::initNetwork() {
  File file = SPIFFS.open(SSIDS_PATH, "r");
  if (!file || file.isDirectory()) {
    return false;
  }
  char ssidval[30], passval[40];
  uint8_t c = 0;
  while (file.available()) {
    if (parseSsid(file.readStringUntil('\n').c_str(), ssidval, passval)) {
      strlcpy(ssids[c].ssid, ssidval, 30);
      strlcpy(ssids[c].password, passval, 40);
      ssidsCount++;
      c++;
    }
  }
  file.close();
  return true;
}

void Config::setBrightness(bool dosave){
#if BRIGHTNESS_PIN!=255
  if(!store.dspon && dosave) {
    display.wakeup();
  }
  analogWrite(BRIGHTNESS_PIN, map(store.brightness, 0, 100, 0, 255));
  if(!store.dspon) store.dspon = true;
  if(dosave){
    saveValue(&store.brightness, store.brightness, false, true);
    saveValue(&store.dspon, store.dspon, true, true);
  }
#endif
#ifdef USE_NEXTION
  nextion.wake();
  char cmd[15];
  snprintf(cmd, 15, "dims=%d", store.brightness);
  nextion.putcmd(cmd);
  if(!store.dspon) store.dspon = true;
  if(dosave){
    saveValue(&store.brightness, store.brightness, false, true);
    saveValue(&store.dspon, store.dspon, true, true);
  }
#endif
}

void Config::setBrightnessRaw(uint8_t percent) {
#if BRIGHTNESS_PIN != 255
  // clamp
  if (percent > 100) percent = 100;

  // csak a hardver: NEM store, NEM save
  analogWrite(BRIGHTNESS_PIN, map(percent, 0, 100, 0, 255));
#endif
}

void Config::setDspOn(bool dspon, bool saveval){
  if(saveval){
    store.dspon = dspon;
    saveValue(&store.dspon, store.dspon, true, true);
  }
#ifdef USE_NEXTION
  if(!dspon) nextion.sleep();
  else nextion.wake();
#endif
  if(!dspon){
#if BRIGHTNESS_PIN!=255
  analogWrite(BRIGHTNESS_PIN, 0);
#endif
    display.deepsleep();
  }else{
    display.wakeup();
#if BRIGHTNESS_PIN!=255
  analogWrite(BRIGHTNESS_PIN, map(store.brightness, 0, 100, 0, 255));
#endif
  }
}

void Config::doSleep() {
    if (BRIGHTNESS_PIN != 255) { analogWrite(BRIGHTNESS_PIN, 0); }
    display.deepsleep();
#ifdef USE_NEXTION
    nextion.sleep();
#endif
    uint64_t mask = 0;
#if WAKE_PIN1 >= 0 && WAKE_PIN1 < 64
    if (rtc_gpio_is_valid_gpio((gpio_num_t)WAKE_PIN1)) {
        rtc_gpio_pullup_en((gpio_num_t)WAKE_PIN1);
        rtc_gpio_pulldown_dis((gpio_num_t)WAKE_PIN1);
        mask |= (1ULL << WAKE_PIN1);
    }
#endif
#if WAKE_PIN2 >= 0 && WAKE_PIN2 < 64
    if (rtc_gpio_is_valid_gpio((gpio_num_t)WAKE_PIN2)) {
        rtc_gpio_pullup_en((gpio_num_t)WAKE_PIN2);
        rtc_gpio_pulldown_dis((gpio_num_t)WAKE_PIN2);
        mask |= (1ULL << WAKE_PIN2);
    }
#endif
    if (mask != 0) {
#if CONFIG_IDF_TARGET_ESP32
        // Classic ESP32 supports ALL_LOW and ANY_HIGH only. Wake inputs use
        // pull-ups here, so active-low buttons require ALL_LOW on this target.
        esp_sleep_enable_ext1_wakeup(mask, ESP_EXT1_WAKEUP_ALL_LOW);
#else
        esp_sleep_enable_ext1_wakeup(mask, ESP_EXT1_WAKEUP_ANY_LOW);
#endif
    }
    esp_sleep_enable_timer_wakeup(config.sleepfor * 60ULL * 1000000ULL);
    esp_deep_sleep_start();
}

void Config::doSleepW() {
    display.deepsleep();
#ifdef USE_NEXTION
    nextion.sleep();
#endif
    uint64_t mask = 0;
#if WAKE_PIN1 >= 0 && WAKE_PIN1 < 64
    if (rtc_gpio_is_valid_gpio((gpio_num_t)WAKE_PIN1)) {
        rtc_gpio_pullup_en((gpio_num_t)WAKE_PIN1);
        rtc_gpio_pulldown_dis((gpio_num_t)WAKE_PIN1);
        mask |= (1ULL << WAKE_PIN1);
    }
#endif
#if WAKE_PIN2 >= 0 && WAKE_PIN2 < 64
    if (rtc_gpio_is_valid_gpio((gpio_num_t)WAKE_PIN2)) {
        rtc_gpio_pullup_en((gpio_num_t)WAKE_PIN2);
        rtc_gpio_pulldown_dis((gpio_num_t)WAKE_PIN2);
        mask |= (1ULL << WAKE_PIN2);
    }
#endif
    delay(200);
    if (mask != 0) {
#if CONFIG_IDF_TARGET_ESP32
        esp_sleep_enable_ext1_wakeup(mask, ESP_EXT1_WAKEUP_ALL_LOW);
#else
        esp_sleep_enable_ext1_wakeup(mask, ESP_EXT1_WAKEUP_ANY_LOW);
#endif
    }
    esp_deep_sleep_start();
}

void Config::sleepForAfter(uint16_t sf, uint16_t sa) {
    sleepfor = sf;
    if (sa > 0) {
        timekeeper.waitAndDo(sa * 60, doSleep);
    } else {
        doSleep();
    }
}

/*----- number to formated string -----*/
const char* fmtThousands(uint32_t v) {
    static char buf[16];
    char        tmp[16];
    sprintf(tmp, "%lu", v);

    int len = strlen(tmp);
    int pos = len % 3;
    int j = 0;

    for (int i = 0; i < len; i++) {
        if (i && (i % 3) == pos) buf[j++] = ' ';
        buf[j++] = tmp[i];
    }
    buf[j] = 0;

    return buf;
}

void Config::bootInfo() {
  BOOTLOG("************************************************");
  BOOTLOG("*               ёPadio v%s                *", YOVERSION);
  BOOTLOG("************************************************");
  BOOTLOG("------------------------------------------------");
  BOOTLOG("arduino:\t%d", ARDUINO);
  BOOTLOG("compiler:\t%s", __VERSION__);
  BOOTLOG("esp32core:\t%d.%d.%d", ESP_ARDUINO_VERSION_MAJOR, ESP_ARDUINO_VERSION_MINOR, ESP_ARDUINO_VERSION_PATCH);
  uint32_t chipId = 0;
  for(int i=0; i<17; i=i+8) {
    chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }
  BOOTLOG("chip:\t\tmodel: %s | rev: %d | id: %lu | cores: %d | psram: %lu", ESP.getChipModel(), ESP.getChipRevision(), chipId, ESP.getChipCores(), ESP.getPsramSize());
  BOOTLOG("display:\t%d", DSP_MODEL);
  if(VS1053_CS==255) {
    BOOTLOG("audio:\t\t%s (%d, %d, %d)", "I2S", I2S_DOUT, I2S_BCLK, I2S_LRC);
  }else{
    BOOTLOG("audio:\t\t%s (%d, %d, %d, %d, %s)", "VS1053", VS1053_CS, VS1053_DCS, VS1053_DREQ, VS1053_RST, VS_HSPI?"true":"false");
  }
  BOOTLOG("audioinfo:\t%s", store.audioinfo?"true":"false");
  BOOTLOG("smartstart:\t%d", store.smartstart);
  BOOTLOG("vumeter:\t%s", store.vumeter?"true":"false");
  BOOTLOG("softapdelay:\t%d", store.softapdelay);
  BOOTLOG("flipscreen:\t%s", store.flipscreen?"true":"false");
  BOOTLOG("invertdisplay:\t%s", store.invertdisplay?"true":"false");
  BOOTLOG("showweather:\t%s", store.showweather?"true":"false");
  BOOTLOG("buttons:\tleft=%d, center=%d, right=%d, up=%d, down=%d, mode=%d, pullup=%s", 
          BTN_LEFT, BTN_CENTER, BTN_RIGHT, BTN_UP, BTN_DOWN, BTN_MODE, BTN_INTERNALPULLUP?"true":"false");
  BOOTLOG("encoders:\tl1=%d, b1=%d, r1=%d, pullup=%s, l2=%d, b2=%d, r2=%d, pullup=%s", 
          ENC_BTNL, ENC_BTNB, ENC_BTNR, ENC_INTERNALPULLUP?"true":"false", ENC2_BTNL, ENC2_BTNB, ENC2_BTNR, ENC2_INTERNALPULLUP?"true":"false");
  BOOTLOG("ir:\t\t%d", IR_PIN);
  if(SDC_CS!=255) BOOTLOG("SD:\t\t%d", SDC_CS);
  BOOTLOG("------------------------------------------------");
  BOOTLOG("------------------------------------------------");
  BOOTLOG(
    "CONFIG:\tsizeof(store)=%u B | EEPROM_START=%u | EEPROM_END=%u | EEPROM_SIZE=%u",
    (unsigned)sizeof(config.store),
    (unsigned)EEPROM_START,
    (unsigned)(EEPROM_START + sizeof(config.store)),
    (unsigned)EEPROM_SIZE
  );
  BOOTLOG("------------------------------------------------");
}
