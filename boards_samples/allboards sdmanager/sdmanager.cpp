#include "options.h"
#if SDC_CS!=255

#ifndef SD_OWNS_SPI   //CYD extras
  #define SD_OWNS_SPI 0
#endif

#ifndef SD_NEEDS_MUTEX   //Guition extras
  #define SD_NEEDS_MUTEX 0
#endif

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include "vfs_api.h"
#include "sd_diskio.h"
#include "config.h"
#include "sdmanager.h"
#include "display.h"
#include "player.h"

#if SD_NEEDS_MUTEX
  #include "freertos/FreeRTOS.h"
  #include "freertos/semphr.h"
  extern SemaphoreHandle_t spiMutex;
  #include "spidog.h"
#endif

#if SD_OWNS_SPI
  #if defined(SD_SPIPINS)
    SPIClass  SDSPI(VSPI);
   #define SDREALSPI SDSPI
  #elif SD_HSPI
    SPIClass  SDSPI(HSPI);
    #define SDREALSPI SDSPI
  #else
    #define SDREALSPI SPI
  #endif

#else
  #if defined(SD_SPIPINS) || SD_HSPI
  SPIClass  SDSPI(HSPI);
  #define SDREALSPI SDSPI
  #else
    #define SDREALSPI SPI
  #endif
#endif

#ifndef SDSPISPEED
  #define SDSPISPEED 20000000
#endif

SDManager sdman(FSImplPtr(new VFSImpl()));

bool SDManager::start(){
#if SD_NEEDS_MUTEX
  sdog.begin();
  bool ok = false;

  if (xSemaphoreTake(spiMutex, portMAX_DELAY)) {
    ready = begin(SDC_CS, SDREALSPI, SDSPISPEED);
    vTaskDelay(10);
    if (!ready) ready = begin(SDC_CS, SDREALSPI, SDSPISPEED);
    vTaskDelay(20);
    if (!ready) ready = begin(SDC_CS, SDREALSPI, SDSPISPEED);
    vTaskDelay(50);
    if (!ready) ready = begin(SDC_CS, SDREALSPI, SDSPISPEED);
    ok = ready;
    xSemaphoreGive(spiMutex);
  }
  return ok;
#else
#if SD_OWNS_SPI
  #if defined(SD_SPIPINS)
    SDREALSPI.begin(SD_SPIPINS, SDC_CS);
  #elif SD_HSPI
    SDREALSPI.begin();
  #endif
#endif
  ready = begin(SDC_CS, SDREALSPI, SDSPISPEED);
  vTaskDelay(10);
  if(!ready) ready = begin(SDC_CS, SDREALSPI, SDSPISPEED);
  vTaskDelay(20);
  if(!ready) ready = begin(SDC_CS, SDREALSPI, SDSPISPEED);
  vTaskDelay(50);
  if(!ready) ready = begin(SDC_CS, SDREALSPI, SDSPISPEED);
  return ready;
#endif
}

void SDManager::stop(){
#if SD_NEEDS_MUTEX
  if (xSemaphoreTake(spiMutex, portMAX_DELAY)) {
    end();
    ready = false;
    xSemaphoreGive(spiMutex);
  }
#else
  end();
  ready = false;
#endif
}
#include "diskio_impl.h"
bool SDManager::cardPresent() {
#if SD_NEEDS_MUTEX
  if (!ready) return false;
  bool result = false;
  if (xSemaphoreTake(spiMutex, portMAX_DELAY)) {
    if (sectorSize() < 1) result = false;
    else {
      uint8_t buff[sectorSize()] = {0};
      bool bread = readRAW(buff, 1);
      result = (sectorSize() > 0 && bread);
    }
    xSemaphoreGive(spiMutex);
  }
  return result;
#else
  if(!ready) return false;
  if(sectorSize()<1) {
    return false;
  }
  uint8_t buff[sectorSize()] = { 0 };
  bool bread = readRAW(buff, 1);
  if(sectorSize()>0 && !bread) return false;
  return bread;
#endif
}

bool SDManager::_checkNoMedia(const char* path){
  if (path[strlen(path) - 1] == '/')
    snprintf(config.tmpBuf, sizeof(config.tmpBuf), "%s%s", path, ".nomedia");
  else
    snprintf(config.tmpBuf, sizeof(config.tmpBuf), "%s/%s", path, ".nomedia");
  bool nm = exists(config.tmpBuf);
  return nm;
}

bool SDManager::_endsWith (const char* base, const char* str) {
  int slen = strlen(str) - 1;
  const char *p = base + strlen(base) - 1;
  while(p > base && isspace(*p)) p--;
  p -= slen;
  if (p < base) return false;
  return (strncmp(p, str, slen) == 0);
}

void SDManager::listSD(File &plSDfile, File &plSDindex, const char* dirname, uint8_t levels) {
    File root = sdman.open(dirname);
    if (!root) {
        Serial.println("##[ERROR]#\tFailed to open directory");
        return;
    }
    if (!root.isDirectory()) {
        Serial.println("##[ERROR]#\tNot a directory");
        return;
    }

    uint32_t pos = 0;
    char* filePath;
    while (true) {
        vTaskDelay(2);
        player.loop();
        bool isDir;
        String fileName = root.getNextFileName(&isDir);
        if (fileName.isEmpty()) break;
        filePath = (char*)malloc(fileName.length() + 1);
        if (filePath == NULL) {
            Serial.println("Memory allocation failed");
            break;
        }
        strcpy(filePath, fileName.c_str());
        const char* fn = strrchr(filePath, '/') + 1;
        if (isDir) {
            if (levels && !_checkNoMedia(filePath)) {
                listSD(plSDfile, plSDindex, filePath, levels - 1);
            }
        } else {
            if (_endsWith(strlwr((char*)fn), ".mp3") || _endsWith(fn, ".m4a") || _endsWith(fn, ".aac") ||
                _endsWith(fn, ".wav") || _endsWith(fn, ".flac")) {
                pos = plSDfile.position();
                plSDfile.printf("%s\t%s\t0\n", fn, filePath);
                plSDindex.write((uint8_t*)&pos, 4);
                Serial.print(".");
#if SD_NEEDS_MUTEX
                if (display.mode() == SDCHANGE && !_sdFCount) {
                    display.putRequest(SDFILEINDEX, 0); 
                }
#else
                if(display.mode()==SDCHANGE) display.putRequest(SDFILEINDEX, _sdFCount+1);
#endif
                _sdFCount++;
                if (_sdFCount % 64 == 0) Serial.println();
            }
        }
        free(filePath);
    }
    root.close();
}

void SDManager::indexSDPlaylist() {
  _sdFCount = 0;
#if SD_NEEDS_MUTEX
  if (xSemaphoreTake(spiMutex, portMAX_DELAY)) {
    if (exists(PLAYLIST_SD_PATH)) remove(PLAYLIST_SD_PATH);
    if (exists(INDEX_SD_PATH)) remove(INDEX_SD_PATH);
    File playlist = open(PLAYLIST_SD_PATH, "w", true);
    if (!playlist) { xSemaphoreGive(spiMutex); return; }


    File index = open(INDEX_SD_PATH, "w", true);
    xSemaphoreGive(spiMutex);

    listSD(playlist, index, "/", SD_MAX_LEVELS);

    if (xSemaphoreTake(spiMutex, portMAX_DELAY)) {
      index.flush();
      index.close();
      playlist.flush();
      playlist.close();
      xSemaphoreGive(spiMutex);
    }
  }

#else
  if(exists(PLAYLIST_SD_PATH)) remove(PLAYLIST_SD_PATH);
  if(exists(INDEX_SD_PATH)) remove(INDEX_SD_PATH);
  File playlist = open(PLAYLIST_SD_PATH, "w", true);
  if (!playlist) {
    return;
  }
  File index = open(INDEX_SD_PATH, "w", true);
  listSD(playlist, index, "/", SD_MAX_LEVELS);
  index.flush();
  index.close();
  playlist.flush();
  playlist.close();
#endif
  Serial.println();
  delay(50);
}
#endif


