#include <Arduino.h>
#include "options.h"
#include "commandhandler.h"
#include "player.h"
#include "display.h"
#include "netserver.h"
#include "config.h"
#include "controls.h"
#include "telnet.h"
#include "../clock/clock_tts.h"
#include "../displays/fonts/clockfont_api.h"
#include "../displays/widgets/widgetsconfig.h"
#include "../plugins/backlight/backlight.h"

#if DSP_MODEL==DSP_DUMMY
#define DUMMYDISPLAY
#endif

CommandHandler cmd;
// Mono theme backupok
static bool     s_monoBackupValid = false;
static uint8_t  s_prev_vuMidOn;
static uint16_t s_prev_vuMidColor;
static uint8_t  s_prev_weatherIconSet;
static uint16_t s_prev_vuLabelBgColor;
static uint16_t s_prev_vuLabelTextColor;
static uint8_t  s_prev_stationLine;

// VU paraméterek halasztott mentéséhez
uint32_t g_vuSaveDue = 0;

// Szín konvertáló (#RRGGBB, 0xFFFF, dec)
static uint16_t parseColor565(const String &val) {
  String s = val;
  s.trim();
  if (s.length() == 0) return 0;

  if (s[0] == '#') { // #RRGGBB
    if (s.length() != 7) return 0;
    auto h2 = [](char c)->uint8_t {
      if (c >= '0' && c <= '9') return c - '0';
      c |= 0x20; // to lower
      if (c >= 'a' && c <= 'f') return 10 + c - 'a';
      return 0;
    };
    uint8_t r = (h2(s[1]) << 4) | h2(s[2]);
    uint8_t g = (h2(s[3]) << 4) | h2(s[4]);
    uint8_t b = (h2(s[5]) << 4) | h2(s[6]);
    uint16_t r5 = (r >> 3) & 0x1F;
    uint16_t g6 = (g >> 2) & 0x3F;
    uint16_t b5 = (b >> 3) & 0x1F;
    return (r5 << 11) | (g6 << 5) | b5;
  }

  if (s.startsWith("0x") || s.startsWith("0X")) {
    return (uint16_t) strtoul(s.c_str(), nullptr, 16);
  }

  long v = s.toInt();
  if (v < 0) v = 0;
  if (v > 65535) v = 65535;
  return (uint16_t)v;
}

bool CommandHandler::exec(const char *command, const char *value, uint8_t cid) {
  if (strEquals(command, "start"))    { player.sendCommand({PR_PLAY, config.lastStation()}); return true; }
  if (strEquals(command, "stop"))     { player.sendCommand({PR_STOP, 0}); return true; }
  if (strEquals(command, "toggle"))   { player.toggle(); return true; }
  if (strEquals(command, "prev"))     { player.prev(); return true; }
  if (strEquals(command, "next"))     { player.next(); return true; }
  if (strEquals(command, "volm"))     { player.stepVol(false); return true; }
  if (strEquals(command, "volp"))     { player.stepVol(true); return true; }
#ifdef USE_SD
  if (strEquals(command, "mode"))     { config.changeMode(atoi(value)); return true; }
#endif
  if (strEquals(command, "reset") && cid==0)    { config.reset(); return true; }
  if (strEquals(command, "ballance")) { config.setBalance(atoi(value)); return true; }
  if (strEquals(command, "playstation") || strEquals(command, "play")){ 
    int id = atoi(value);
    if (id < 1) id = 1;
    uint16_t cs = config.playlistLength();
    if (id > cs) id = cs;
    player.sendCommand({PR_PLAY, id});
    return true;
  }
  if (strEquals(command, "vol")){
    int v = atoi(value);
    config.store.volume = v < 0 ? 0 : (v > 100 ? 100 : v);
    player.setVol(v);
    return true;
  }
  if (strEquals(command, "dspon"))     { config.setDspOn(atoi(value)!=0); return true; }
  if (strEquals(command, "dim"))       { int d=atoi(value); config.store.brightness = (uint8_t)(d < 0 ? 0 : (d > 100 ? 100 : d)); config.setBrightness(true); return true; }
  if (strEquals(command, "clearspiffs")){ config.spiffsCleanup(); config.saveValue(&config.store.play_mode, static_cast<uint8_t>(PM_WEB)); return true; }
  /*********************************************/
  /****************** WEBSOCKET ****************/
  /*********************************************/
  if (strEquals(command, "getindex"))  { netserver.requestOnChange(GETINDEX, cid); return true; }
  
  if (strEquals(command, "getsystem"))  { netserver.requestOnChange(GETSYSTEM, cid); return true; }
  if (strEquals(command, "getscreen"))  { netserver.requestOnChange(GETSCREEN, cid); return true; }
  if (strEquals(command, "gettimezone")){ netserver.requestOnChange(GETTIMEZONE, cid); return true; }
  if (strEquals(command, "getcontrols")){ netserver.requestOnChange(GETCONTROLS, cid); return true; }
  if (strEquals(command, "getweather")) { netserver.requestOnChange(GETWEATHER, cid); return true; }
  if (strEquals(command, "getactive"))  { netserver.requestOnChange(GETACTIVE, cid); return true; }
  if (strEquals(command, "newmode"))    { config.newConfigMode = atoi(value); netserver.requestOnChange(CHANGEMODE, cid); return true; }
  
  if (strEquals(command, "invertdisplay")){ config.saveValue(&config.store.invertdisplay, static_cast<bool>(atoi(value))); display.invert(); return true; }
  if (strEquals(command, "numplaylist"))  { config.saveValue(&config.store.numplaylist, static_cast<bool>(atoi(value))); display.putRequest(NEWMODE, CLEAR); display.putRequest(NEWMODE, PLAYER); return true; }
  if (strEquals(command, "fliptouch"))    { config.saveValue(&config.store.fliptouch, static_cast<bool>(atoi(value))); flipTS(); return true; }
  if (strEquals(command, "dbgtouch"))     { config.saveValue(&config.store.dbgtouch, static_cast<bool>(atoi(value))); return true; }
  if (strEquals(command, "flipscreen"))   { config.saveValue(&config.store.flipscreen, static_cast<bool>(atoi(value))); display.flip(); display.putRequest(NEWMODE, CLEAR); display.putRequest(NEWMODE, PLAYER); return true; }
if (strEquals(command, "brightness")) {

  if (!config.store.dspon)
    netserver.requestOnChange(DSPON, 0);

  uint8_t v = static_cast<uint8_t>(atoi(value));
  if (v > 100) v = 100;

  config.store.brightness = v;
  config.setBrightness(true);

#if (BRIGHTNESS_PIN != 255)
  // user activity → timer reset
  backlightPlugin.activity();
#endif

  return true;
}
  if (strEquals(command, "screenon"))     { config.setDspOn(static_cast<bool>(atoi(value))); return true; }
  if (strEquals(command, "contrast"))     { config.saveValue(&config.store.contrast, static_cast<uint8_t>(atoi(value))); display.setContrast(); return true; }
  if (strEquals(command, "screensaverenabled")){ config.enableScreensaver(static_cast<bool>(atoi(value))); return true; }
  if (strEquals(command, "screensavertimeout")){ config.setScreensaverTimeout(static_cast<uint16_t>(atoi(value))); return true; }
  if (strEquals(command, "screensaverblank"))  { config.setScreensaverBlank(static_cast<bool>(atoi(value))); return true; }
  if (strEquals(command, "screensaverplayingenabled")){ config.setScreensaverPlayingEnabled(static_cast<bool>(atoi(value))); return true; }
  if (strEquals(command, "screensaverplayingtimeout")){ config.setScreensaverPlayingTimeout(static_cast<uint16_t>(atoi(value))); return true; }
  if (strEquals(command, "screensaverplayingblank"))  { config.setScreensaverPlayingBlank(static_cast<bool>(atoi(value))); return true; }
  if (strEquals(command, "abuff")){ config.saveValue(&config.store.abuff, static_cast<uint16_t>(atoi(value))); return true; }
  if (strEquals(command, "telnet")){ config.saveValue(&config.store.telnet, static_cast<bool>(atoi(value))); telnet.toggle(); return true; }
  if (strEquals(command, "watchdog")){ config.saveValue(&config.store.watchdog, static_cast<bool>(atoi(value))); return true; }
  
  if (strEquals(command, "tzh"))        { config.saveValue(&config.store.tzHour, static_cast<int8_t>(atoi(value))); return true; }
  if (strEquals(command, "tzm"))        { config.saveValue(&config.store.tzMin, static_cast<int8_t>(atoi(value))); return true; }
  if (strEquals(command, "sntp2"))      { config.saveValue(config.store.sntp2, value, 35, false); return true; }
  if (strEquals(command, "sntp1"))      { config.setSntpOne(value); return true; }
  if (strEquals(command, "timeint"))    { config.saveValue(&config.store.timeSyncInterval, static_cast<uint16_t>(atoi(value))); return true; }
  if (strEquals(command, "timeintrtc")) { config.saveValue(&config.store.timeSyncIntervalRTC, static_cast<uint16_t>(atoi(value))); return true; }
  
  if (strEquals(command, "volsteps"))         { config.saveValue(&config.store.volsteps, static_cast<uint8_t>(atoi(value))); return true; }
  if (strEquals(command, "encacc"))  { setEncAcceleration(static_cast<uint16_t>(atoi(value))); return true; }
  if (strEquals(command, "irtlp"))            { setIRTolerance(static_cast<uint8_t>(atoi(value))); return true; }
  if (strEquals(command, "oneclickswitching")){ config.saveValue(&config.store.skipPlaylistUpDown, static_cast<bool>(atoi(value))); return true; }
  if (strEquals(command, "showweather"))      { config.setShowweather(static_cast<bool>(atoi(value))); return true; }
  if (strEquals(command, "lat"))              { config.saveValue(config.store.weatherlat, value, 10); return true; }
  if (strEquals(command, "lon"))              { config.saveValue(config.store.weatherlon, value, 10); return true; }
  if (strEquals(command, "key"))              { config.setWeatherKey(value); return true; }
  if (strEquals(command, "wint"))  { config.saveValue(&config.store.weatherSyncInterval, static_cast<uint16_t>(atoi(value))); return true; }
  
  if (strEquals(command, "volume"))  { player.setVol(static_cast<uint8_t>(atoi(value))); return true; }
  if (strEquals(command, "sdpos"))   { config.setSDpos(static_cast<uint32_t>(atoi(value))); return true; }
  if (strEquals(command, "snuffle")) { config.setSnuffle(strcmp(value, "true") == 0); return true; }
  if (strEquals(command, "balance")) { config.setBalance(static_cast<uint8_t>(atoi(value))); return true; }
  if (strEquals(command, "reboot"))  { ESP.restart(); return true; }
  if (strEquals(command, "boot"))    { ESP.restart(); return true; }
  if (strEquals(command, "format"))  { SPIFFS.format(); ESP.restart(); return true; }
  if (strEquals(command, "submitplaylist"))  { player.sendCommand({PR_STOP, 0}); return true; }

#if IR_PIN!=255
  if (strEquals(command, "irbtn")) { // Gombok  value: 0-18
    config.setIrBtn(atoi(value));
    return true;
  }
  if (strEquals(command, "chkid")) { // A három IR bank chkid, value: 0, 1 vagy 2
    IRCommand ircmd = {};
    ircmd.irBankId = static_cast<uint8_t>(atoi(value));
    ircmd.hasBank = true;
    ircmd.hasBtnId = false; // Nem szükséges az index átadása, mert a tanulás során már beállításra kerül a config.irBtnId változó.
    xQueueSend(irQueue, &ircmd, 0);
    Serial.printf("commandhandler.cpp--> xQueueSend chkid: %d\n", ircmd.irBankId);
    return true;
  }
  if (strEquals(command, "irclr")) { // Bank törlés. --> command: irclr
    config.ircodes.irVals[config.irBtnId][static_cast<uint8_t>(atoi(value))] = 0;
    Serial.printf("commandhandler.cpp--> Bank törlés--> config.irBtnId: %d, chkid: %d\n", config.irBtnId, static_cast<uint8_t>(atoi(value)));
    return true;
  }
#endif
  if (strEquals(command, "reset"))  { config.resetSystem(value, cid); return true; }

  if (strEquals(command, "smartstart")){ uint8_t ss = atoi(value) == 1 ? 1 : 2; if (!player.isRunning() && ss == 1) ss = 0; config.setSmartStart(ss); return true; }
  if (strEquals(command, "audioinfo")) { config.saveValue(&config.store.audioinfo, static_cast<bool>(atoi(value))); display.putRequest(AUDIOINFO); return true; }
  if (strEquals(command, "vumeter"))   { config.saveValue(&config.store.vumeter, static_cast<bool>(atoi(value))); display.putRequest(SHOWVUMETER); return true; }
  if (strEquals(command, "softap"))    { config.saveValue(&config.store.softapdelay, static_cast<uint8_t>(atoi(value))); return true; }
  if (strEquals(command, "mdnsname"))  { config.saveValue(config.store.mdnsname, value, MDNS_LENGTH); return true; }
  if (strEquals(command, "rebootmdns")){
    if(strlen(config.store.mdnsname)>0) snprintf(config.tmpBuf, sizeof(config.tmpBuf), "{\"redirect\": \"http://%s.local/settings.html\"}", config.store.mdnsname);
    else snprintf(config.tmpBuf, sizeof(config.tmpBuf), "{\"redirect\": \"http://%s/settings.html\"}", config.ipToStr(WiFi.localIP()));
    websocket.text(cid, config.tmpBuf); delay(500); ESP.restart();
    return true;
  }
  
  return false;
}

// ----- HTTP helperk -----
namespace CmdHttp {

// /set – AutoStart/Stop, TTS, grndHeight, stationLine, clockFontMono
void handleSet(AsyncWebServerRequest *request) {
  bool touched = false;
  bool changed = false;

  if (request->hasParam("autoStartTime")) {
    String val = request->getParam("autoStartTime")->value();
    strlcpy(config.store.autoStartTime, val.c_str(), sizeof(config.store.autoStartTime));
    changed = true;
  }

  if (request->hasParam("autoStopTime")) {
    String val = request->getParam("autoStopTime")->value();
    strlcpy(config.store.autoStopTime, val.c_str(), sizeof(config.store.autoStopTime));
    changed = true;
  }

  // --- Elevation / grndHeight (m) ---
  if (request->hasParam("grndHeight")) {
    touched = true;
    int gh = request->getParam("grndHeight")->value().toInt();
    gh = constrain(gh, 0, 3000);
    if (gh != (int)config.store.grndHeight) {
      config.store.grndHeight = (uint16_t)gh;
      changed = true;
    }
  }

  // --- Pressure slope (hPa/m * 1000) ---
  if (request->hasParam("pressureSlope_x1000")) {
    touched = true;

    int k = request->getParam("pressureSlope_x1000")->value().toInt();
    k = constrain(k, 35, 200);

    if (k != (int)config.store.pressureSlope_x1000) {
      config.store.pressureSlope_x1000 = (uint16_t)k;
      changed = true;
    }
  }

  // --- clockFontMono enabled ---
  if (request->hasParam("clockFontMono")) {
    touched = true;
    uint8_t v = request->getParam("clockFontMono")->value().toInt() ? 1 : 0;
    if (v != config.store.clockFontMono) {
      config.store.clockFontMono = v;
      changed = true;
      display.putRequest(DSP_RECONF, 0);
    }
  }

  // --- META Station Name Skip ---
  if (request->hasParam("metaStNameSkip")) {
    touched = true;
    uint8_t v = request->getParam("metaStNameSkip")->value().toInt() ? 1 : 0;
    if (v != config.store.metaStNameSkip) {
      config.store.metaStNameSkip   = v;
      changed = true;
     if (player.isRunning()) {
      player.sendCommand({PR_STOP, 0});
      player.sendCommand({PR_PLAY, config.lastStation()});
     }
    }
  }

  // --- TTS enabled ---
  if (request->hasParam("ttsEnabled")) {
    touched = true;
    uint8_t v = request->getParam("ttsEnabled")->value().toInt() ? 1 : 0;
    if (v != config.store.ttsEnabled) {
      config.store.ttsEnabled   = v;
      clock_tts_enabled         = (v != 0);  // runtime azonnal
      display.setSpeechActive(v != 0);
      changed = true;
    }
  }

  // --- TTS during playback ---
  if (request->hasParam("ttsDuringPlayback")) {
    touched = true;
    uint8_t v = request->getParam("ttsDuringPlayback")->value().toInt() ? 1 : 0;
    if (v != config.store.ttsDuringPlayback) {
      config.store.ttsDuringPlayback = v;
      changed = true;
    }
  }

  // --- TTS interval (perc) ---
  if (request->hasParam("ttsInterval")) {
    touched = true;
    int iv = constrain(request->getParam("ttsInterval")->value().toInt(), 1, 240);
    if (iv != (int)config.store.ttsInterval) {
      config.store.ttsInterval = (uint8_t)iv;
      clock_tts_interval       = iv;         // runtime azonnal
      changed = true;
    }
  }

  // --- TTS DND RESET ---
  if (request->hasParam("dndReset")) {
    touched = true;
    if (strlen(config.store.ttsDndStart) || strlen(config.store.ttsDndStop)) {
      config.store.ttsDndStart[0] = '\0';
      config.store.ttsDndStop[0]  = '\0';
      changed = true;
    }
  }

  // --- TTS DND APPLY (HH:MM) ---
  if (request->hasParam("dndStartTime")) {
    touched = true;
    String val = request->getParam("dndStartTime")->value();
    if (val.length() <= 5 && val.indexOf(':') == 2) {
      if (strncmp(config.store.ttsDndStart, val.c_str(),
                  sizeof(config.store.ttsDndStart) - 1) != 0) {
        strlcpy(config.store.ttsDndStart, val.c_str(), sizeof(config.store.ttsDndStart));
        changed = true;
      }
    }
  }

  if (request->hasParam("dndStopTime")) {
    touched = true;
    String val = request->getParam("dndStopTime")->value();
    if (val.length() <= 5 && val.indexOf(':') == 2) {
      if (strncmp(config.store.ttsDndStop, val.c_str(),
                  sizeof(config.store.ttsDndStop) - 1) != 0) {
        strlcpy(config.store.ttsDndStop, val.c_str(), sizeof(config.store.ttsDndStop));
        changed = true;
      }
    }
  }

  // --- Backlight dimmer enable ---
  if (request->hasParam("blDimEnable")) {
    touched = true;
    uint8_t v = request->getParam("blDimEnable")->value().toInt() ? 1 : 0;
    if (v != config.store.blDimEnable) {
      config.store.blDimEnable   = v;
      display.setBlfadeActive(v != 0);
      changed = true;
#if (BRIGHTNESS_PIN != 255)
    if (v == 0) {
      backlightPlugin.restoreNow(); 
    } else {
      backlightPlugin.wake(); 
    }
#endif
    }
  }

  // --- Backlight dimmer level (percent) ---
  if (request->hasParam("blDimLevel")) {
    touched = true;
    int dl = constrain(request->getParam("blDimLevel")->value().toInt(), 1, 100);
    if (dl != (int)config.store.blDimLevel) {
      config.store.blDimLevel = (uint8_t)dl;
      changed = true;
    }
  }

  // --- Backlight dimmer interval (sec) ---
  if (request->hasParam("blDimInterval")) {
    touched = true;
    int di = constrain(request->getParam("blDimInterval")->value().toInt(), 1, 300);
    if (di != (int)config.store.blDimInterval) {
      config.store.blDimInterval = (uint16_t)di;
      changed = true;
    }
  }

  // --- Station Line / Border switch ---
  if (request->hasParam("stationLine")) {
    touched = true;
    uint8_t v = request->getParam("stationLine")->value().toInt() ? 1 : 0;
    if (v != config.store.stationLine) {
      config.store.stationLine = v;
      changed = true;
      display.putRequest(DSP_RECONF, 0);
    }
  }

  // --- Playlist mode ---
  if (request->hasParam("playlistMode")) {
    touched = true;
    uint8_t v = request->getParam("playlistMode")->value().toInt() ? 1 : 0;
    if (v != config.store.playlistMode) {
      config.store.playlistMode = v;
      changed = true;
      display.putRequest(DSP_RECONF, 0);
    }
  }

  // --- Enable Stall Watchdog mode ---
  if (request->hasParam("stallWatchdog")) {
    touched = true;
    uint8_t v = request->getParam("stallWatchdog")->value().toInt() ? 1 : 0;
    if (v != config.store.stallWatchdog) {
      config.store.stallWatchdog = v;
      changed = true;
    }
  }

  // --- Short Weather / Long Weather ---
  if (request->hasParam("shortWeather")) {
    touched = true;
    uint8_t v = request->getParam("shortWeather")->value().toInt() ? 1 : 0;
    if (v != config.store.shortWeather) {
      config.store.shortWeather = v;
      changed = true;
      display.putRequest(DSP_RECONF, 0);
    }
  }

  // Direct channel change
  if (request->hasParam("directChannelChange")) {
    touched = true;
    uint8_t v = request->getParam("directChannelChange")->value().toInt() ? 1 : 0;
    if (v != config.store.directChannelChange) {
      config.store.directChannelChange = v;
      changed = true;
    }
  }

  // --- Return Time (sec) ---
  if (request->hasParam("stationsListReturnTime")) {
    touched = true;
    int v = constrain(request->getParam("stationsListReturnTime")->value().toInt(), 2, 30);
    if (v != (int)config.store.stationsListReturnTime) {
      config.store.stationsListReturnTime = (uint8_t)v;
      changed = true;
    }
  }

  // 12 Hours Clock
  if (request->hasParam("hours12")) {
    touched = true;
    uint8_t v = request->getParam("hours12")->value().toInt() ? 1 : 0;
    if (v != config.store.hours12) {
      config.store.hours12 = v;
      changed = true;
      display.putRequest(DSP_RECONF, 0);
    }
  }

  // --- Mono THEME ---
  if (request->hasParam("monoTheme")) {
    touched = true;
    uint8_t v = request->getParam("monoTheme")->value().toInt() ? 1 : 0;
    if (v != config.store.monoTheme) {
     if (v == 1 && !s_monoBackupValid) {
       s_prev_vuMidOn           = config.store.vuMidOn;
       s_prev_vuMidColor        = config.store.vuMidColor;
       s_prev_weatherIconSet    = config.store.weatherIconSet;
       s_prev_vuLabelBgColor    = config.store.vuLabelBgColor;
       s_prev_vuLabelTextColor  = config.store.vuLabelTextColor;
       s_prev_stationLine       = config.store.stationLine;
       s_monoBackupValid = true;
     }
      config.store.monoTheme   = v;
    if (v) {
      config.store.vuMidOn = 0;
      config.store.vuMidColor = parseColor565("#828282");
      config.store.weatherIconSet  = 1;
      config.store.vuLabelBgColor = 0x0000; 
      config.store.vuLabelTextColor = 0xFFFF;
      config.store.stationLine = 1;
    } else {
      if (s_monoBackupValid) {
        config.store.vuMidOn           = s_prev_vuMidOn;
        config.store.vuMidColor        = s_prev_vuMidColor;
        config.store.weatherIconSet    = s_prev_weatherIconSet;
        config.store.vuLabelBgColor    = s_prev_vuLabelBgColor;
        config.store.vuLabelTextColor  = s_prev_vuLabelTextColor;
        config.store.stationLine       = s_prev_stationLine;
        s_monoBackupValid = false; 
      }
    }
      changed = true;
      display.putRequest(DSP_RECONF, 0);
    }
  }

  // --- Weather icon set ---
  if (request->hasParam("weatherIconSet")) {
    touched = true;

    int v = request->getParam("weatherIconSet")->value().toInt();
    v = constrain(v, 0, 2);   // jelenleg 0=default, 1=mono (2 későbbre)

    if (v != config.store.weatherIconSet) {
      config.store.weatherIconSet = (uint8_t)v;
      changed = true;

      // új ikon kirajzolása
      display.putRequest(DSP_RECONF, 0);
    }
  }

  // --- LED strip: enabled ---
  if (request->hasParam("lsEnabled")) {
    touched = true;
    uint8_t v = request->getParam("lsEnabled")->value().toInt() ? 1 : 0;
    if (v != config.store.lsEnabled) {
      config.store.lsEnabled = v;
      display.setLstripActive(v != 0);
      changed = true;
    }
  }

  // --- LED strip: screensaver enabled ---
  if (request->hasParam("lsSsEnabled")) {
    touched = true;
    uint8_t v = request->getParam("lsSsEnabled")->value().toInt() ? 1 : 0;
    if (v != config.store.lsSsEnabled) {
      config.store.lsSsEnabled = v;
      changed = true;
    }
  }

  // --- LED strip: model/effect ---
  if (request->hasParam("lsModel")) {
    touched = true;
    int v = constrain(request->getParam("lsModel")->value().toInt(), 0, 7);
    if ((uint8_t)v != config.store.lsModel) {
      config.store.lsModel = (uint8_t)v;
      changed = true;
    }
  }

  // --- LED strip: brightness (0..100) ---
  if (request->hasParam("lsBrightness")) {
    touched = true;
    int v = constrain(request->getParam("lsBrightness")->value().toInt(), 0, 100);
    if ((uint8_t)v != config.store.lsBrightness) {
      config.store.lsBrightness = (uint8_t)v;
      changed = true;
    }
  }

  if (touched) {
    if (changed) config.eepromWrite(EEPROM_START, config.store);
    request->send(200, "text/plain", changed ? "OK" : "NOCHANGE");
  } else {
    request->send(400, "text/plain", "Missing parameters");
  }
}

// /setdate
void handleSetDate(AsyncWebServerRequest *request) {
  if (!request->hasParam("value")) {
    request->send(400, "text/plain", "Missing param");
    return;
  }
  int v = request->getParam("value")->value().toInt();
  v = constrain(v, 0, 5);

  config.store.dateFormat = (uint8_t)v;
  config.eepromWrite(EEPROM_START, config.store);

  display.putRequest(CLOCK, 0);

  request->send(200, "text/plain", "OK");
}

// /setnameday
void handleSetNameday(AsyncWebServerRequest *request) {
  if (!request->hasParam("value")) {
    request->send(400, "text/plain", "Missing param");
    return;
  }
  int v = request->getParam("value")->value().toInt();
  v = constrain(v, 0, 1);

  config.store.showNameday = (uint8_t)v;
  config.eepromWrite(EEPROM_START, config.store);

  display.putRequest(CLOCK, 0);

  request->send(200, "text/plain", "OK");
}

// /setClockFont
void handleSetClockFont(AsyncWebServerRequest *request) {
  if (!request->hasParam("id")) {
    request->send(400, "text/plain", "Missing id param");
    return;
  }
  uint8_t id = (uint8_t)request->getParam("id")->value().toInt();
  if (id > 6) id = 0;

  bool changed = (config.store.clockFontId != id);
  config.store.clockFontId = id;
  config.eepromWrite(EEPROM_START, config.store);

  display.putRequest(CLOCK, 0);

  request->send(200, "text/plain", changed ? "Clock font changed" : "Clock font unchanged");
}

// /setvuLayout
void handleSetVuLayout(AsyncWebServerRequest *request) {
  if (!request->hasParam("value")) {
    request->send(400, "text/plain", "Missing value param");
    return;
  }
  int v = request->getParam("value")->value().toInt();
  config.store.vuLayout = v;
  config.eepromWrite(EEPROM_START, config.store);
  display.putRequest(DSP_RECONF, 0);
  request->send(200, "text/plain", "VU layout changed. Applying...");
}
// /setvu – teljes VU tuning
void handleSetVu(AsyncWebServerRequest *request) {
  if (!request->hasParam("name") || !request->hasParam("value")) {
    request->send(400, "text/plain", "Missing params");
    return;
  }
  String name  = request->getParam("name")->value();
  String value = request->getParam("value")->value();

  auto &st = config.store;
  uint8_t ly = st.vuLayout;
  bool needReconf = false;

  if (request->hasParam("layout") && name != "layout") {
    ly = (uint8_t)constrain(request->getParam("layout")->value().toInt(), 0, 3);
  }

  if (name == "enabled") {
    uint8_t v = value.toInt() ? 1 : 0;
    st.vumeter = v;
    config.eepromWrite(EEPROM_START, st);

    display.putRequest(SHOWVUMETER, 0);
    display.putRequest(CLOCK, 0);
    request->send(200, "text/plain", "OK");
    return;
  } else if (name == "layout") {
    uint8_t v = (uint8_t)constrain(value.toInt(), 0, 3);
#if (DSP_MODEL==DSP_GC9A01 || DSP_MODEL==DSP_GC9A01A || DSP_MODEL==DSP_GC9A01_I80 || DSP_MODEL==DSP_ST7789_76)
    if (v == 0) v = 2;
#endif
    st.vuLayout = v;
    if (st.vumeter) needReconf = true;
  } else if (name == "bars") {
    if (!st.vumeter) { request->send(409, "text/plain", "VU disabled"); return; }
    REF_BY_LAYOUT(st, vuBarCount, ly) = (uint8_t)constrain(value.toInt(), 5, 64);
    needReconf = true;
  } else if (name == "gap") {
    if (!st.vumeter) { request->send(409, "text/plain", "VU disabled"); return; }
    REF_BY_LAYOUT(st, vuBarGap, ly) = (uint8_t)constrain(value.toInt(), 0, 6);
    needReconf = true;
  } else if (name == "height") {
    if (!st.vumeter) { request->send(409, "text/plain", "VU disabled"); return; }
    REF_BY_LAYOUT(st, vuBarHeight, ly) = (uint8_t)constrain(value.toInt(), 1, 50);
    needReconf = true;
  } else if (name == "fade") {
    if (!st.vumeter) { request->send(409, "text/plain", "VU disabled"); return; }
    REF_BY_LAYOUT(st, vuFadeSpeed, ly) = (uint8_t)constrain(value.toInt(), 0, 20);
  } else if (name == "midColor") {
    st.vuMidColor = parseColor565(value);
  } else if (name == "alphaUp") {
    if (!st.vumeter) { request->send(409, "text/plain", "VU disabled"); return; }
    REF_BY_LAYOUT(st, vuAlphaUp, ly) = (uint8_t)constrain(value.toInt(), 0, 100);
  } else if (name == "alphaDown") {
    if (!st.vumeter) { request->send(409, "text/plain", "VU disabled"); return; }
    REF_BY_LAYOUT(st, vuAlphaDn, ly) = (uint8_t)constrain(value.toInt(), 0, 100);
  } else if (name == "pUp") {
    if (!st.vumeter) { request->send(409, "text/plain", "VU disabled"); return; }
    REF_BY_LAYOUT(st, vuPeakUp, ly) = (uint8_t)constrain(value.toInt(), 0, 100);
  } else if (name == "pDown") {
    if (!st.vumeter) { request->send(409, "text/plain", "VU disabled"); return; }
    REF_BY_LAYOUT(st, vuPeakDn, ly) = (uint8_t)constrain(value.toInt(), 0, 100);
  } else if (name == "peakColor") {
    st.vuPeakColor = parseColor565(value);
  } else if (name == "labelBgColor") {
    st.vuLabelBgColor = parseColor565(value);
  } else if (name == "labelTextColor") {
    st.vuLabelTextColor = parseColor565(value);
  } else if (name == "labelHeight") {
    if (!st.vumeter) { request->send(409, "text/plain", "VU disabled"); return; }
    REF_BY_LAYOUT(st, vuLabelHeight, ly) = (uint8_t)constrain(value.toInt(), 0, 50);
    needReconf = true;
  } else if (name == "midOn") {
    uint8_t on = value.toInt() ? 1 : 0;
    config.store.vuMidOn = on;
    if (on) {
      uint16_t user = config.store.vuMidUserColor ? config.store.vuMidUserColor : config.store.vuMidColor;
      config.store.vuMidColor = user;
    } else {
      config.store.vuMidUserColor = config.store.vuMidColor;
      config.store.vuMidColor    = config.theme.vumin;
    }
  } else if (name == "peakOn") {
    uint8_t on = value.toInt() ? 1 : 0;
    config.store.vuPeakOn = on;
    if (on) {
      uint16_t user = config.store.vuPeakUserColor ? config.store.vuPeakUserColor : config.store.vuPeakColor;
      config.store.vuPeakColor = user;
    } else {
      config.store.vuPeakUserColor = config.store.vuPeakColor;
      config.store.vuPeakColor     = config.theme.background;
    }
  } else if (name == "midPct") {
    uint8_t pct = (uint8_t)constrain(value.toInt(), 0, 100);
    REF_BY_LAYOUT(st, vuMidPct, ly) = pct;

    uint8_t hp = REF_BY_LAYOUT(st, vuHighPct, ly);
    if (hp < pct) REF_BY_LAYOUT(st, vuHighPct, ly) = pct;
  } else if (name == "highPct") {
    uint8_t pct = (uint8_t)constrain(value.toInt(), 0, 100);
    REF_BY_LAYOUT(st, vuHighPct, ly) = pct;

    uint8_t mp = REF_BY_LAYOUT(st, vuMidPct, ly);
    if (pct < mp) REF_BY_LAYOUT(st, vuMidPct, ly) = pct;
  } else if (name == "expo") {
    if (!st.vumeter) { request->send(409, "text/plain", "VU disabled"); return; }
    REF_BY_LAYOUT(st, vuExpo, ly) = (uint8_t)constrain(value.toInt(), 50, 300);
  } else if (name == "floor") {
    if (!st.vumeter) { request->send(409, "text/plain", "VU disabled"); return; }
    uint8_t v = (uint8_t)constrain(value.toInt(), 0, 95);
    REF_BY_LAYOUT(st, vuFloor, ly) = v;

    uint8_t c = REF_BY_LAYOUT(st, vuCeil, ly);
    if (c <= v + 5) REF_BY_LAYOUT(st, vuCeil, ly) = (uint8_t)constrain(v + 5, 1, 100);
  } else if (name == "ceil") {
    if (!st.vumeter) { request->send(409, "text/plain", "VU disabled"); return; }
    uint8_t v = (uint8_t)constrain(value.toInt(), 5, 100);
    REF_BY_LAYOUT(st, vuCeil, ly) = v;

    uint8_t f = REF_BY_LAYOUT(st, vuFloor, ly);
    if (v <= f + 5) REF_BY_LAYOUT(st, vuFloor, ly) = (uint8_t)constrain(v - 5, 0, 95);
  } else if (name == "gain") {
    if (!st.vumeter) { request->send(409, "text/plain", "VU disabled"); return; }
    REF_BY_LAYOUT(st, vuGain, ly) = (uint8_t)constrain(value.toInt(), 50, 200);
  } else if (name == "knee") {
    if (!st.vumeter) { request->send(409, "text/plain", "VU disabled"); return; }
    REF_BY_LAYOUT(st, vuKnee, ly) = (uint8_t)constrain(value.toInt(), 0, 20);
  } else {
    request->send(400, "text/plain", "Unknown name");
    return;
  }

  const bool isHeavy =
    (name=="expo" || name=="floor" || name=="ceil" || name=="gain" || name=="knee" ||
     name=="alphaUp" || name=="alphaDown" || name=="pUp" || name=="pDown" ||
     name=="midPct" || name=="highPct" || name=="fade" ||
     name=="midColor" || name=="peakColor" || name=="labelBgColor" ||
     name=="labelTextColor" || name=="labelHeight");

  bool saveNow = !isHeavy;
  if (request->hasParam("save")) {
    saveNow = request->getParam("save")->value().toInt() != 0;
  }

  if (saveNow) {
    config.eepromWrite(EEPROM_START, config.store);
  } else {
    g_vuSaveDue = millis() + 1200;
  }

  if (needReconf) display.putRequest(DSP_RECONF, 0);

  request->send(200, "text/plain", "OK");
}

} // namespace CmdHttp
