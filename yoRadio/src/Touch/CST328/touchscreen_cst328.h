#pragma once
#include <Arduino.h>

struct TP_Point {
  uint16_t x = 0;
  uint16_t y = 0;
};

class CST328_Adapter {
public:
  bool begin(uint16_t w, uint16_t h, bool flip);
  void setRotation(uint8_t r);
  void read();
  bool touched() const;

public:
  bool     isTouched = false;
  TP_Point points[1];

private:
  // dimensiuni ecran
  uint16_t _w = 0, _h = 0;

  // rotație
  uint8_t _rot = 0;
  bool    _flip = false;

  // coordonate și touch
  TP_Point _last = {0,0};
  bool     _hadTouch = false;

  // variabile pentru rotație și mapare
  bool _swapXY  = false;
  bool _invertX = true;
  bool _invertY = false;

  // funcție privată pentru mapare coordonate
  void mapXY(uint16_t &x, uint16_t &y);
};

extern CST328_Adapter ts;