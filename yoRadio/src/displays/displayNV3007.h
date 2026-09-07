#ifndef displayNV3007_h
#define displayNV3007_h

#include "Arduino.h"
#include <Adafruit_GFX.h>
#include "../Adafruit_ST7735_and_ST7789_Library/Adafruit_ST7789.h"

#include "fonts/bootlogo_cust64.h"
#include "fonts/dsfont35.h"

typedef GFXcanvas16 Canvas;
typedef Adafruit_ST7789 yoDisplay;

#include "tools/commongfx.h"

#if __has_include("conf/displayNV3007_142conf_custom.h")
  #include "conf/displayNV3007_142conf_custom.h"
#else
  #include "conf/displayNV3007_142conf.h"
#endif

#endif
