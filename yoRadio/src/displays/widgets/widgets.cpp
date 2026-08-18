#include "../../core/options.h"
#undef HIDE_DATE
#define HIDE_DATE
#if DSP_MODEL!=DSP_DUMMY
#ifdef NAMEDAYS_FILE
#include "../../core/namedays.h"
#endif
#include "../dspcore.h"
#include "../fonts/clockfont_api.h"
#include "../fonts/iconsweather.h"
#include "../fonts/iconsweather_mono.h"
#include "Arduino.h"
#include "widgets.h"
#include "../../core/player.h"    //  for VU widget
#include "../../core/network.h"   //  for Clock widget
#include "../fonts/yo_freefonts.h"   //  FreeSans proporcionális fontok
#include "../../core/config.h"
#include "../tools/l10n.h"
#include "../tools/psframebuffer.h"
#include <math.h> 

namespace {

enum WeatherIconId {
  WI_01D, WI_01N, WI_02D, WI_02N, WI_03, WI_04D, WI_04N,
  WI_09D, WI_09N, WI_10D, WI_10N, WI_11, WI_13D, WI_13N, WI_50D, WI_50N,
  WI_UNKNOWN
};

static WeatherIconId iconIdFromCode(const char* code) {
  if (!code || !*code) return WI_UNKNOWN;

  if      (strstr(code, "01d")) return WI_01D;
  else if (strstr(code, "01n")) return WI_01N;
  else if (strstr(code, "02d")) return WI_02D;
  else if (strstr(code, "02n")) return WI_02N;
  else if (strstr(code, "03d") || strstr(code, "03n")) return WI_03;
  else if (strstr(code, "04d")) return WI_04D;
  else if (strstr(code, "04n")) return WI_04N;
  else if (strstr(code, "09d")) return WI_09D;
  else if (strstr(code, "09n")) return WI_09N;
  else if (strstr(code, "10d")) return WI_10D;
  else if (strstr(code, "10n")) return WI_10N;
  else if (strstr(code, "11d") || strstr(code, "11n")) return WI_11;
  else if (strstr(code, "13d")) return WI_13D;
  else if (strstr(code, "13n")) return WI_13N;
  else if (strstr(code, "50d")) return WI_50D;
  else if (strstr(code, "50n")) return WI_50N;

  return WI_UNKNOWN;
}

struct IconSet {
  const uint16_t* icons[WI_UNKNOWN];  // WI_UNKNOWN-ig (tehát 16 db)
  const uint16_t* fallback;
  uint16_t w;
  uint16_t h;
};

static const IconSet ICONSET_CLASSIC = {
  { img_01d, img_01n, img_02d, img_02n, img_03, img_04d, img_04n,
    img_09d, img_09n, img_10d, img_10n, img_11, img_13d, img_13n, img_50d, img_50n },
  eradio,
  64, 64
};

static const IconSet ICONSET_MONO = {
  { mono__01d, mono__01n, mono__02d, mono__02n, mono__03, mono__04d, mono__04n,
    mono__09d, mono__09n, mono__10d, mono__10n, mono__11, mono__13d, mono__13n, mono__50d, mono__50n },
  mono_eradio,
  64, 64
};

static const IconSet& currentIconSet() {
  return (config.store.weatherIconSet == 1) ? ICONSET_MONO : ICONSET_CLASSIC;
}

} // namespace

static inline void _pad2(char* out, int v) {
  out[0] = '0' + (v / 10);
  out[1] = '0' + (v % 10);
  out[2] = '\0';
}

static void formatDateCustom(char* out, size_t outlen, const tm& t, uint8_t fmt) {
  const char* month = LANG::mnths[t.tm_mon];
  const char* dow   = LANG::dowf[t.tm_wday];

  char d2[3], m2[3];
  _pad2(d2, t.tm_mday);
  _pad2(m2, t.tm_mon + 1);
  int y = t.tm_year + 1900;

#if L10N_LANGUAGE == HU
  // Hungarian
  switch (fmt) {
    case 1: // 2025. szeptember 16.  
      snprintf(out, outlen, "%d. %s %d.", y, month, t.tm_mday);
      break;

    case 2: // 2025. 09. 16. - kedd 
      snprintf(out, outlen, "%d. %s. %s. - %s", y, m2, d2, dow);
      break;

    case 3: // 2025 szeptember 16. kedd  
      snprintf(out, outlen, "%d %s %d. %s", y, month, t.tm_mday, dow);
      break;

    case 4: // 09. 16. - kedd 
      snprintf(out, outlen, "%s. %s. - %s", m2, d2, dow);
      break;

    default: // 2025. 09. 16. 
      snprintf(out, outlen, "%d. %s. %s.", y, m2, d2);
      break;
  }

#else 

  #ifdef USDATE
    // US format MM/DD/YYYY)
    switch (fmt) {
      case 1: // September 16, 2025
        snprintf(out, outlen, "%s %d, %d", month, t.tm_mday, y);
        break;

      case 2: // Tue - 09.16.2025
        snprintf(out, outlen, "%s - %s.%s.%d", dow, m2, d2, y);
        break;

      case 3: // Tue, September 16, 2025
        snprintf(out, outlen, "%s, %s %d, %d", dow, month, t.tm_mday, y);
        break;

      case 4: // Tue - 09.16.
        snprintf(out, outlen, "%s - %s.%s.", dow, m2, d2);
        break;

      default: // 09.16.2025
        snprintf(out, outlen, "%s.%s.%d", m2, d2, y);
        break;
    }

  #else
    // National format (DD/MM/YYYY)
    switch (fmt) {
      case 1: // 16 September 2025   
        snprintf(out, outlen, "%d. %s %d", t.tm_mday, month, y);
        break;

      case 2: // Tue - 16.09.2025 
        snprintf(out, outlen, "%s - %s. %s. %d", dow, d2, m2, y);
        break;

      case 3: // Tue, 16 September 2025  
        snprintf(out, outlen, "%s, %d. %s %d", dow, t.tm_mday, month, y);
        break;

      case 4: // Tue - 16.09. 
        snprintf(out, outlen, "%s - %s. %s.", dow, d2, m2);
        break;

      default: // 16.09.2025 
        snprintf(out, outlen, "%s. %s. %d", d2, m2, y);
        break;
    }
  #endif

#endif

}

static inline size_t utf8len(const char* s) {
  size_t len = 0;
  for (const unsigned char* p = (const unsigned char*)s; *p; ++p) {
    if ( (*p & 0xC0) != 0x80 ) ++len; 
  }
  return len;
}

//static uint32_t vuLastDecay = 0;

static inline float _applyVuCurve(float x, uint8_t ly) {
    // 0..1 normalizált bemenet
    float floor = REF_BY_LAYOUT(config.store, vuFloor, ly) / 100.0f;
    float ceil  = REF_BY_LAYOUT(config.store, vuCeil,  ly) / 100.0f;
    float expo  = REF_BY_LAYOUT(config.store, vuExpo,  ly) / 100.0f;  if (expo < 0.01f) expo = 0.01f;
    float gain  = REF_BY_LAYOUT(config.store, vuGain,  ly) / 100.0f;
    float knee  = REF_BY_LAYOUT(config.store, vuKnee,  ly) / 100.0f;

    // normalizálás floor..ceil tartományra
    float denom = (ceil - floor);
    if (denom < 0.001f) denom = 0.001f;
    x = (x - floor) / denom;
    // puha küszöb (smoothstep a 0..knee sávban)
    if (knee > 0.0f) {
      float t = fminf(fmaxf(x / knee, 0.0f), 1.0f);
      float smooth = t * t * (3.0f - 2.0f * t);   // smoothstep(0..1)
      x = (x < knee) ? (smooth * knee) : x;
    }
    x = fminf(fmaxf(x, 0.0f), 1.0f);
    // exponenciális alakítás + gain
    x = powf(x, expo) * gain;
    return fminf(fmaxf(x, 0.0f), 1.0f);
  }

/************************
      FILL WIDGET
 ************************/
void FillWidget::init(FillConfig conf, uint16_t bgcolor){
  Widget::init(conf.widget, bgcolor, bgcolor);
  _width = conf.width;
  _height = conf.height;
  
}

void FillWidget::_draw(){
  if(!_active) return;
  dsp.fillRect(_config.left, _config.top, _width, _height, _bgcolor);
}

void FillWidget::setHeight(uint16_t newHeight){
  _height = newHeight;
  //_draw();
}
/************************
      TEXT WIDGET
 ************************/
TextWidget::~TextWidget() {
  free(_text);
  free(_oldtext);
}

void TextWidget::_charSize(uint8_t textsize, uint8_t& width, uint16_t& height){
#ifndef DSP_LCD
  uint8_t ts = (textsize > 0) ? textsize : 1;  // 0 → 5x7 natív = 1x méret
  width = ts * CHARWIDTH;
  height = ts * CHARHEIGHT;
#else
  width = 1;
  height = 1;
#endif
}

/* ── FreeSans helper: string pixel-szélessége ─────────────────────────────── */
uint16_t TextWidget::_measureText(const char* txt) {
#ifndef DSP_LCD
  if (_gfxFont) return yoStringWidth(_gfxFont, txt);
  // textsize=0 esetén _charWidth=0 lenne → 1x mérettel számolunk
  uint8_t cw = (_charWidth > 0) ? _charWidth : CHARWIDTH;
  return utf8len(txt) * cw;
#endif
  return utf8len(txt) * _charWidth;
}

/* ── FreeSans helper: top → baseline Y konverzió ─────────────────────────── */
int16_t TextWidget::_baselineY() {
#ifndef DSP_LCD
  if (_gfxFont) return _config.top + yoBaselineOffset(_gfxFont);
#endif
  return _config.top;
}

void TextWidget::init(WidgetConfig wconf, uint16_t buffsize, bool uppercase, uint16_t fgcolor, uint16_t bgcolor) {
  Widget::init(wconf, fgcolor, bgcolor);
  _buffsize = buffsize;
  _text = (char *) malloc(sizeof(char) * _buffsize);
  memset(_text, 0, _buffsize);
  _oldtext = (char *) malloc(sizeof(char) * _buffsize);
  memset(_oldtext, 0, _buffsize);
  _charSize(_config.textsize, _charWidth, _textheight);
  _textwidth = _oldtextwidth = _oldleft = 0;
  _uppercase = uppercase;
  /* Font hozzárendelés a textsize alapján:
     0 = 5x7 natív (nullptr), 1+ = yoScrollFont (DejaVuSans8, Sans13, Bold12...) */
#ifndef DSP_LCD
  _gfxFont = (_config.textsize > 0) ? yoScrollFont(_config.textsize) : nullptr;
  if (_gfxFont) _textheight = yoFontHeight(_gfxFont);
#endif
}

void TextWidget::setText(const char* txt) {
  /* FreeSans/DejaVu font esetén az utf8To() megkerülése:
     a GFXfont write(uint16_t) az eredeti UTF-8 kódpontokat várja,
     az utf8To() által produkált glcdfont-specifikus kódok nem értelmezhetők. */
  if (_gfxFont) {
    if (_uppercase)
      yoUtf8ToUpper(_text, txt, _buffsize);
    else
      strlcpy(_text, txt, _buffsize);
  } else
    strlcpy(_text, utf8To(txt, _uppercase), _buffsize);
  _textwidth = _measureText(_text);
  if (strcmp(_oldtext, _text) == 0) return;
  if (_active) dsp.fillRect(_oldleft == 0 ? _realLeft() : min(_oldleft, _realLeft()),  _config.top, max(_oldtextwidth, _textwidth), _textheight, _bgcolor);
  _oldtextwidth = _textwidth;
  _oldleft = _realLeft();
  if (_active) _draw();
}

void TextWidget::setText(int val, const char *format){
  char buf[_buffsize];
  snprintf(buf, _buffsize, format, val);
  setText(buf);
}

void TextWidget::setText(const char* txt, const char *format){
  char buf[_buffsize];
  snprintf(buf, _buffsize, format, txt);
  setText(buf);
}

/*uint16_t TextWidget::_realLeft(bool w_fb) {
  uint16_t realwidth = (_width>0 && w_fb)?_width:dsp.width();
  switch (_config.align) {
    case WA_CENTER: return (realwidth - _textwidth) / 2; break;
    case WA_RIGHT: return (realwidth - _textwidth - (!w_fb?_config.left:0)); break;
    default: return !w_fb?_config.left:0; break;
  }
}*/
uint16_t TextWidget::_realLeft(bool w_fb) {
  uint16_t realwidth = (_width>0 && w_fb) ? _width : dsp.width();
  int32_t calc = 0;
  switch (_config.align) {
    case WA_CENTER:
      calc = (int32_t)realwidth - (int32_t)_textwidth;
      calc /= 2;
      break;
    case WA_RIGHT:
      calc = (int32_t)realwidth - (int32_t)_textwidth - (!w_fb ? _config.left : 0);
      break;
    case WA_LEFT:
    default:
      calc = !w_fb ? _config.left : 0;
      break;
  }
  if (calc < 0) calc = 0;               // ← fontos!
  return (uint16_t)calc;
}

void TextWidget::_draw() {
  if(!_active) return;
  dsp.setTextColor(_fgcolor, _bgcolor);
#ifndef DSP_LCD
  if (_gfxFont) {
    yoApplyFont(dsp, _gfxFont);
    dsp.setCursor(_realLeft(), _baselineY());
  } else {
    dsp.setFont();
    dsp.setTextSize(_config.textsize > 0 ? _config.textsize : 1);
    dsp.setCursor(_realLeft(), _config.top);
  }
#else
  dsp.setFont();
  dsp.setTextSize(_config.textsize);
  dsp.setCursor(_realLeft(), _config.top);
#endif
  if (_gfxFont) yoPrintUtf8(dsp, _text, _fgcolor, _bgcolor, _gfxFont); else dsp.print(_text);
  strlcpy(_oldtext, _text, _buffsize);
}

/************************
      SCROLL WIDGET
 ************************/
ScrollWidget::ScrollWidget(const char* separator, ScrollConfig conf, uint16_t fgcolor, uint16_t bgcolor) {
  init(separator, conf, fgcolor, bgcolor);
}

ScrollWidget::~ScrollWidget() {
  free(_fb);
  free(_sep);
  free(_window);
}

uint16_t ScrollWidget::_winLeft(bool fb) const {
  if (fb) return 0;

  switch (_config.align) {
    case WA_CENTER:
      return (uint16_t)((dsp.width() - _width) / 2);
    case WA_RIGHT:
      // WA_RIGHT 
      return (uint16_t)(dsp.width() - _width - _config.left);
    case WA_LEFT:
    default:
      return _config.left;
  }
}

/* ── ScrollWidget pixel-szélesség mérés ──────────────────────────────────── */
uint16_t ScrollWidget::_pixelTextWidth(const char* txt) {
#ifndef DSP_LCD
  if (_gfxFont) return yoStringWidth(_gfxFont, txt);
#endif
  return strlen(txt) * _charWidth;
}

/* ── Font alkalmazása dsp-re vagy fb-re ─────────────────────────────────── */
void ScrollWidget::_applyFont(Adafruit_GFX& gfx) {
#ifndef DSP_LCD
  if (_gfxFont) {
    yoApplyFont(gfx, _gfxFont);
  } else {
    gfx.setFont();
    gfx.setTextSize(_config.textsize);
  }
#else
  gfx.setTextSize(_config.textsize);
#endif
}

void ScrollWidget::init(const char* separator, ScrollConfig conf, uint16_t fgcolor, uint16_t bgcolor) {
  TextWidget::init(conf.widget, conf.buffsize, conf.uppercase, fgcolor, bgcolor);
  _sep = (char *) malloc(sizeof(char) * 4);
  memset(_sep, 0, 4);
  snprintf(_sep, 4, " %.*s ", 1, separator);
  _startscrolldelay = conf.startscrolldelay;
  _scrolldelta      = conf.scrolldelta;
  _scrolltime       = conf.scrolltime;
  _charSize(_config.textsize, _charWidth, _textheight);

  /* FreeSans: ScrollWidget-nél a textsize 2 és 3 → közepes/nagy bold font */
#ifndef DSP_LCD
  _gfxFont = yoScrollFont(_config.textsize);
  if (_gfxFont) _textheight = yoFontHeight(_gfxFont);
#endif

  _sepwidth = _pixelTextWidth(_sep);
  _width = conf.width;
  _backMove.width = _width;
  /* _window mérete: a konfig-szélességnél biztosan elég, +4 biztonsági */
  _window = (char *) malloc(sizeof(char) * (conf.buffsize + 4));
  memset(_window, 0, conf.buffsize + 4);
  _doscroll = false;
  const uint16_t wl  = _winLeft(false);
  const uint16_t fbl = _fb && _fb->ready() ? 0 : wl;
  _x = fbl;

#ifdef PSFBUFFER
  _fb = new psFrameBuffer(dsp.width(), dsp.height());
  _fb->begin(&dsp, wl, _config.top, _width, _textheight, _bgcolor);
#endif
}


void ScrollWidget::_setTextParams() {
  // textsize 0 selects DejaVuSans8 on graphical displays. Treat it as
  // disabled only on targets where no graphical font is available.
  if (_config.textsize == 0 && !_gfxFont) return;
  if(_fb->ready()){
  #ifdef PSFBUFFER
    _applyFont(*_fb);
    _fb->setTextColor(_fgcolor, _bgcolor);
  #endif
  }else{
    _applyFont(dsp);
    dsp.setTextColor(_fgcolor, _bgcolor);
  }
}

bool ScrollWidget::_checkIsScrollNeeded() {
  return _textwidth > _width;
}

void ScrollWidget::setText(const char* txt) {
  if (_gfxFont) {
    if (_uppercase)
      yoUtf8ToUpper(_text, txt, _buffsize - 1);
    else
      strlcpy(_text, txt, _buffsize - 1);
  } else
    strlcpy(_text, utf8To(txt, _uppercase), _buffsize - 1);
  if (strcmp(_oldtext, _text) == 0) return;

  _textwidth = _pixelTextWidth(_text);

  const uint16_t wl  = _winLeft(false);
  const uint16_t fbl = _fb->ready() ? 0 : wl;
  _x = fbl;

  _doscroll = _checkIsScrollNeeded();
  if (dsp.getScrollId() == this) dsp.setScrollId(NULL);
  _scrolldelay = millis();

  if (_active) {
    _setTextParams();

    if (_doscroll) {
#ifdef PSFBUFFER
      if (_fb->ready()) {
        _fb->fillRect(0, 0, _width, _textheight, _bgcolor);
        _fb->setCursor(0, _gfxFont ? yoBaselineOffset(_gfxFont) : 0);
        if (_gfxFont) yoPrintUtf8(*_fb, _text, _fgcolor, _bgcolor, _gfxFont); else _fb->print(_text);
        _fb->display();
      } else
#endif
      {
        dsp.fillRect(wl, _config.top, _width, _textheight, _bgcolor);
        dsp.setClipping({wl, _config.top, _width, _textheight});
        dsp.setCursor(wl, _config.top + (_gfxFont ? yoBaselineOffset(_gfxFont) : 0));
        if (_gfxFont) yoPrintUtf8(dsp, _text, _fgcolor, _bgcolor, _gfxFont); else dsp.print(_text);
        dsp.clearClipping();
      }
    } else {
#ifdef PSFBUFFER
      if (_fb->ready()) {
        _fb->fillRect(0, 0, _width, _textheight, _bgcolor);
        _fb->setCursor(_realLeft(true), _gfxFont ? yoBaselineOffset(_gfxFont) : 0);
        if (_gfxFont) yoPrintUtf8(*_fb, _text, _fgcolor, _bgcolor, _gfxFont); else _fb->print(_text);
        _fb->display();
      } else
#endif
      {
        dsp.fillRect(wl, _config.top, _width, _textheight, _bgcolor);
        dsp.setCursor(_realLeft(), _config.top + (_gfxFont ? yoBaselineOffset(_gfxFont) : 0));
        dsp.setClipping({wl, _config.top, _width, _textheight});
        if (_gfxFont) yoPrintUtf8(dsp, _text, _fgcolor, _bgcolor, _gfxFont); else dsp.print(_text);
        dsp.clearClipping();
      }
    }

    strlcpy(_oldtext, _text, _buffsize);
  }
}

void ScrollWidget::setText(const char* txt, const char *format){
  char buf[_buffsize];
  snprintf(buf, _buffsize, format, txt);
  setText(buf);
}

void ScrollWidget::loop() {
  if (_locked) return;
  if (!_doscroll || (_config.textsize == 0 && !_gfxFont) ||
      (dsp.getScrollId() != NULL && dsp.getScrollId() != this)) return;

  const uint16_t wl  = _winLeft(false);
  const uint16_t fbl = _fb->ready() ? 0 : wl;

  if (_checkDelay(_x == fbl ? _startscrolldelay : _scrolltime, _scrolldelay)) {
    _calcX();
    if (_active) _draw();
  }
}


void ScrollWidget::_clear(){
  if(_fb->ready()){
    #ifdef PSFBUFFER
    _fb->fillRect(0, 0, _width, _textheight, _bgcolor);
    _fb->display();
    #endif
  } else {
    const uint16_t wl = _winLeft(false);
    //dsp.fillRect(_config.left, _config.top, _width, _textheight, _bgcolor);
    dsp.fillRect(wl, _config.top, _width, _textheight, _bgcolor);
  }
}

void ScrollWidget::_draw() {
  if (!_active || _locked) return;
  _setTextParams();

  const uint16_t wl  = _winLeft(false);
  const uint16_t fbl = _fb->ready() ? 0 : wl;
  /* GFXfont módban a baseline offset kell a setCursor Y-hoz */
  const int16_t baseOff = _gfxFont ? yoBaselineOffset(_gfxFont) : 0;

  if (_doscroll) {
    /* Pixel-alapú scroll: ha a psFrameBuffer épp nem ready (realloc alatt),
       skip – töredékes dsp-re rajzolás helyett inkább kihagyunk egy ticket */
    if (!_fb->ready()) return;
    if (_fb->ready()) {
#ifdef PSFBUFFER
      _fb->fillRect(0, 0, _width, _textheight, _bgcolor);
      _fb->setCursor(_x, baseOff);
      if (_gfxFont) { yoPrintUtf8(*_fb, _text, _fgcolor, _bgcolor, _gfxFont); yoPrintUtf8(*_fb, _sep, _fgcolor, _bgcolor, _gfxFont); yoPrintUtf8(*_fb, _text, _fgcolor, _bgcolor, _gfxFont); }
      else          { _fb->print(_text); _fb->print(_sep); _fb->print(_text); }
      _fb->display();
#endif
    } else {
      dsp.setClipping({wl, _config.top, _width, _textheight});
      dsp.setCursor(wl + (_x - fbl), _config.top + baseOff);
      if (_gfxFont) { yoPrintUtf8(dsp, _text, _fgcolor, _bgcolor, _gfxFont); yoPrintUtf8(dsp, _sep, _fgcolor, _bgcolor, _gfxFont); yoPrintUtf8(dsp, _text, _fgcolor, _bgcolor, _gfxFont); }
      else          { dsp.print(_text); dsp.print(_sep); dsp.print(_text); }
      dsp.clearClipping();
    }
  } else {
    if (_fb->ready()) {
#ifdef PSFBUFFER
      _fb->fillRect(0, 0, _width, _textheight, _bgcolor);
      _fb->setCursor(_realLeft(true), baseOff);
      if (_gfxFont) yoPrintUtf8(*_fb, _text, _fgcolor, _bgcolor, _gfxFont); else _fb->print(_text);
      _fb->display();
#endif
    } else {
      dsp.fillRect(wl, _config.top, _width, _textheight, _bgcolor);
      dsp.setCursor(_realLeft(), _config.top + baseOff);
      dsp.setClipping({wl, _config.top, _width, _textheight});
      if (_gfxFont) yoPrintUtf8(dsp, _text, _fgcolor, _bgcolor, _gfxFont); else dsp.print(_text);
      dsp.clearClipping();
    }
  }
}

void ScrollWidget::_calcX() {
  if (!_doscroll || (_config.textsize == 0 && !_gfxFont)) return;

  _x -= _scrolldelta;

  const uint16_t wl  = _winLeft(false);
  const uint16_t fbl = _fb->ready() ? 0 : wl;

  /* _textwidth és _sepwidth már pixelben van → az összehasonlítás helyes */
  if (-_x > (int16_t)(_textwidth + _sepwidth) - (int16_t)fbl) {
    _x = fbl;
    dsp.setScrollId(NULL);
  } else {
    dsp.setScrollId(this);
  }
}


bool ScrollWidget::_checkDelay(int m, uint32_t &tstamp) {
  if (millis() - tstamp > m) {
    tstamp = millis();
    return true;
  } else {
    return false;
  }
}

void ScrollWidget::setWindowWidth(uint16_t w) {
  if (w == 0 || w == _width) return;

  _width = w;
  _backMove.width = _width;
  /* pixel-alapú újraszámítás */
  _sepwidth = _pixelTextWidth(_sep);
  _textwidth = _pixelTextWidth(_text);

#ifdef PSFBUFFER
  if (_fb) {
    const uint16_t wl = _winLeft(false);
    if (_fb->ready() && _fb->width() == (int16_t)_width && _fb->height() == (int16_t)_textheight) {
      // Méret nem változott: csak pozíció frissítés, vibrálásmentes
      _fb->reposition(wl, _config.top);
    } else {
      // Méret változott: újraallokálás szükséges
      _fb->freeBuffer();
      _fb->begin(&dsp, wl, _config.top, _width, _textheight, _bgcolor);
    }
  }
#endif

  const uint16_t fbl = _fb->ready() ? 0 : _winLeft(false);
  _x = fbl;

  _clear();
  _doscroll = _checkIsScrollNeeded();
}

void ScrollWidget::_reset(){
  dsp.setScrollId(NULL);

  /* pixel-alapú frissítés */
  _textwidth = _pixelTextWidth(_text);
  _sepwidth  = _pixelTextWidth(_sep);

  const uint16_t wl  = _winLeft(false);
  const uint16_t fbl = _fb->ready() ? 0 : wl;
  _x = fbl;

  _scrolldelay = millis();
  _doscroll = _checkIsScrollNeeded();

#ifdef PSFBUFFER
  if (_fb->ready() && _fb->width() == (int16_t)_width && _fb->height() == (int16_t)_textheight) {
    // Méret nem változott: csak pozíció frissítés, vibrálásmentes
    _fb->reposition(wl, _config.top);
  } else {
    _fb->freeBuffer();
    _fb->begin(&dsp, wl, _config.top, _width, _textheight, _bgcolor);
  }
#endif
}


/************************
      SLIDER WIDGET
 ************************/
void SliderWidget::init(FillConfig conf, uint16_t fgcolor, uint16_t bgcolor, uint32_t maxval, uint16_t oucolor) {
  Widget::init(conf.widget, fgcolor, bgcolor);
  _width = conf.width; _height = conf.height; _outlined = conf.outlined; _oucolor = oucolor, _max = maxval;
  _oldvalwidth = _value = 0;
}

void SliderWidget::setValue(uint32_t val) {
  _value = val;
  if (_active && !_locked) _drawslider();

}

void SliderWidget::_drawslider() {
  uint16_t valwidth = map(_value, 0, _max, 0, _width - _outlined * 2);
  if (_oldvalwidth == valwidth) return;
  dsp.fillRect(_config.left + _outlined + min(valwidth, _oldvalwidth), _config.top + _outlined, abs(_oldvalwidth - valwidth), _height - _outlined * 2, _oldvalwidth > valwidth ? _bgcolor : _fgcolor);
  _oldvalwidth = valwidth;
}

void SliderWidget::_draw() {
  if(_locked) return;
  _clear();
  if(!_active) return;
  if (_outlined) dsp.drawRect(_config.left, _config.top, _width, _height, _oucolor);
  uint16_t valwidth = map(_value, 0, _max, 0, _width - _outlined * 2);
  dsp.fillRect(_config.left + _outlined, _config.top + _outlined, valwidth, _height - _outlined * 2, _fgcolor);
}

void SliderWidget::_clear() {
//  _oldvalwidth = 0;
  dsp.fillRect(_config.left, _config.top, _width, _height, _bgcolor);
}
void SliderWidget::_reset() {
  _oldvalwidth = 0;
}

/************************
      VU WIDGET
 ************************/
#if !defined(DSP_LCD) && !defined(DSP_OLED)

VuWidget::~VuWidget() {
  if (_canvas) {
    delete _canvas;
    _canvas = nullptr;
  }
}

void VuWidget::init(WidgetConfig wconf, VUBandsConfig bands,
                    uint16_t vumaxcolor, uint16_t vumincolor, uint16_t bgcolor) {
  Widget::init(wconf, bgcolor, bgcolor);
  _vumaxcolor = vumaxcolor;
  _vumincolor = vumincolor;
  _bands      = bands;

  _maxDimension = _config.align ? _bands.width : _bands.height;

  _peakL = 0;
  _peakR = 0;
  _peakFallDelayCounter = 0;

  // Canvas méret: NEM változtatjuk!
  switch (config.store.vuLayout) {
    case 3: // Studio
      _canvas = new Canvas(_maxDimension, _bands.height * 2 + _bands.space);
      break;
    case 2: // Boombox
    case 1: // Streamline
      _canvas = new Canvas(_maxDimension * 2 + _bands.space, _bands.height);
      break;
    case 0:
    default: // Default
      _canvas = new Canvas(_bands.width * 2 + _bands.space, _maxDimension);
      break;
  }
}

static inline void _labelTextStyle(Canvas* c, uint16_t textColor) {
  c->setFont();
  c->setTextSize(1);
  c->setTextColor(textColor);
  c->setTextWrap(false);
}

static inline void _drawCenteredCharInBox(Canvas* c,
                                         int boxX, int boxY, int boxW, int boxH,
                                         char ch)
{
  char s[2] = { ch, 0 };

  int16_t tbx, tby;
  uint16_t tbw, tbh;
  c->getTextBounds(s, 0, 0, &tbx, &tby, &tbw, &tbh);

  int cx = boxX + (boxW - (int)tbw) / 2 - tbx;
  int cy = boxY + (boxH - (int)tbh) / 2 - tby;

  c->setCursor(cx, cy);
  c->print(s);
}

// Studio: bal oldali oszlop, két sor
void VuWidget::_drawLabelsVertical(int x, int y, int h, int labelSize) {
  uint16_t bg  = config.store.vuLabelBgColor;
  uint16_t txt = config.store.vuLabelTextColor;
  _labelTextStyle(_canvas, txt);

  const int pad = 1;
  int boxW = labelSize - 2 * pad;
  if (boxW < 1) return;

  int boxX = x + pad;

  // L
  int boxY = y;
  _canvas->fillRect(boxX, boxY, boxW, h, bg);
  #if (DSP_MODEL != DSP_ST7735) && (DSP_MODEL != DSP_ST7789_170)
  _drawCenteredCharInBox(_canvas, boxX, boxY, boxW, h, 'L');
  #endif

  // R
  boxY = y + h + _bands.space;
  _canvas->fillRect(boxX, boxY, boxW, h, bg);
  #if (DSP_MODEL != DSP_ST7735) && (DSP_MODEL != DSP_ST7789_170)
  _drawCenteredCharInBox(_canvas, boxX, boxY, boxW, h, 'R');
  #endif
}

// Default: alul egy csík, L/R cellával
void VuWidget::_drawLabelsHorizontal(int x, int y, int w, int labelSize) {
  uint16_t bg  = config.store.vuLabelBgColor;
  uint16_t txt = config.store.vuLabelTextColor;
  _labelTextStyle(_canvas, txt);

  const int pad = 1;
  int boxH = labelSize - 2 * pad;
  if (boxH < 1) return;

  _canvas->fillRect(x, y + pad, w, boxH, bg);

  int cellW = w / 2;
  int boxY  = y + pad;

  _drawCenteredCharInBox(_canvas, x,         boxY, cellW,     boxH, 'L');
  _drawCenteredCharInBox(_canvas, x + cellW, boxY, w-cellW,   boxH, 'R');
}

// Egy darab label doboz (szélessége = labelSize)
void VuWidget::_drawSingleLabel(int x, int y, int h, char ch, int labelSize)
{
  uint16_t bg  = config.store.vuLabelBgColor;
  uint16_t txt = config.store.vuLabelTextColor;
  _labelTextStyle(_canvas, txt);

  const int pad = 1;
  int boxW = labelSize - 2 * pad;
  if (boxW < 1) return;

  int boxX = x + pad;

  _canvas->fillRect(boxX, y, boxW, h, bg);
  _drawCenteredCharInBox(_canvas, boxX, y, boxW, h, ch);
}

void VuWidget::_draw() {
  if (!_active || _locked) return;

  // ------------------------------------------------------------
  // GEO: egyetlen közös geometria (a label MINDIG a VU-ból vesz el!)
  // ------------------------------------------------------------
  struct Geo {
    uint8_t ly;

    int canvasW, canvasH;

    // label méret + flag
    int labelSize;
    bool showLabels;

    // alap gap (a két csatorna közti fix rés)
    int gap;

    // Default (függőleges) VU terület (két oszlop)
    int def_vuX, def_vuY, def_vuW, def_vuH; // def_vuW == canvasW, def_vuH == canvasH-labelSize

    // Studio (két sor) VU terület
    int std_vuX, std_vuY, std_vuW, std_vuH; // std_vuW == canvasW-labelSize, std_vuH == _bands.height

    // Streamline: két csatorna egymás mellett, mindkettőből levágunk labelSize-t balról
    int str_LX, str_LW; // teljes csatorna sáv (névleges: _maxDimension)
    int str_RX, str_RW;
    int str_vuLX, str_vuLW; // tényleges VU rész (label levonva)
    int str_vuRX, str_vuRW;

    // Boombox: középről kifelé, a label a közép környékén a csatornákból vesz el
    int bbx_center;
    int bbx_leftEdge;   // bal csatorna "belső" széle (közép felőli)
    int bbx_rightEdge;  // jobb csatorna "belső" széle (közép felőli)
    int bbx_lblLX, bbx_lblRX; // label dobozok X kezdete (L a gap bal oldalán, R a gap jobb oldalán)
    int bbx_vuLW;       // bal csatorna max hossza (a label levonása után)
    int bbx_vuRW;       // jobb csatorna max hossza
    int bbx_vuY, bbx_vuH;

    int scaleDim; // 0..scaleDim (a VU “hossza”)
  } g;

  g.ly = config.store.vuLayout;

  // --- layoutfüggő label méret ---
  switch (g.ly) {
    case 3: g.labelSize = config.store.vuLabelHeightStd; break; // Studio
    case 2: g.labelSize = config.store.vuLabelHeightBbx; break; // Boombox
    case 1: g.labelSize = config.store.vuLabelHeightStr; break; // Streamline
    case 0:
    default: g.labelSize = config.store.vuLabelHeightDef; break; // Default
  }
  if (g.labelSize < 0) g.labelSize = 0;
  g.showLabels = (g.labelSize > 2);

  // --- canvas méretek (a meglévő init logikával egyezően) ---
  g.canvasW = (g.ly == 3) ? (int)_maxDimension
            : (g.ly == 0) ? (int)(_bands.width * 2 + _bands.space)
                          : (int)(_maxDimension * 2 + _bands.space);

  g.canvasH = (g.ly == 3) ? (int)(_bands.height * 2 + _bands.space)
            : (g.ly == 0) ? (int)_maxDimension
                          : (int)_bands.height;

  g.gap = (int)_bands.space;
  if (g.gap < 0) g.gap = 0;

  // --- Default geometry (függőleges) ---
  g.def_vuX = 0;
  g.def_vuY = 0;
  g.def_vuW = g.canvasW;
  g.def_vuH = g.canvasH - g.labelSize;
  if (g.def_vuH < 1) g.def_vuH = 1;

  // --- Studio geometry (két sor, label bal oldalt) ---
  g.std_vuX = g.labelSize;
  g.std_vuY = 0;
  g.std_vuW = g.canvasW - g.labelSize;
  if (g.std_vuW < 1) g.std_vuW = 1;
  g.std_vuH = (int)_bands.height;

  // --- Streamline geometry ---
  // két csatorna: [0.._maxDimension] és [(_maxDimension+gap)..]
  g.str_LX = 0;
  g.str_LW = (int)_maxDimension;
  g.str_RX = (int)_maxDimension + g.gap;
  g.str_RW = (int)_maxDimension;

  // label mindkét csatornából levág
  int cut = g.showLabels ? g.labelSize : 0;
  if (cut > g.str_LW - 1) cut = g.str_LW - 1;
  if (cut < 0) cut = 0;

  g.str_vuLX = g.str_LX + cut;
  g.str_vuLW = g.str_LW - cut;
  g.str_vuRX = g.str_RX + cut;
  g.str_vuRW = g.str_RW - cut;
  if (g.str_vuLW < 1) g.str_vuLW = 1;
  if (g.str_vuRW < 1) g.str_vuRW = 1;

  // --- Boombox geometry (középről kifelé) ---
  g.bbx_center    = g.canvasW / 2;
  g.bbx_leftEdge  = g.bbx_center - (g.gap / 2); // bal csatorna belső széle
  g.bbx_rightEdge = g.bbx_center + (g.gap / 2); // jobb csatorna belső széle

  // label a gap két oldalán, és a csatornákból vesz el
  int bbxLabel = g.showLabels ? g.labelSize : 0;
  // ha túl nagy, legalább 1px maradjon a csatornának
  int maxLeft  = g.bbx_leftEdge;
  int maxRight = g.canvasW - g.bbx_rightEdge;
  if (bbxLabel > maxLeft - 1)  bbxLabel = maxLeft - 1;
  if (bbxLabel > maxRight - 1) bbxLabel = maxRight - 1;
  if (bbxLabel < 0) bbxLabel = 0;

  g.bbx_lblLX = g.bbx_leftEdge - bbxLabel;
  g.bbx_lblRX = g.bbx_rightEdge;

  g.bbx_vuLW = g.bbx_leftEdge - bbxLabel;          // bal csatorna max hossza (0..bbx_vuLW)
  g.bbx_vuRW = g.canvasW - (g.bbx_rightEdge + bbxLabel); // jobb csatorna max hossza
  if (g.bbx_vuLW < 1) g.bbx_vuLW = 1;
  if (g.bbx_vuRW < 1) g.bbx_vuRW = 1;

  g.bbx_vuY = 0;
  g.bbx_vuH = g.canvasH;

  // --- scaleDim (VU hossza) layout szerint ---
  switch (g.ly) {
    case 0: g.scaleDim = g.def_vuH; break;                 // függőleges
    case 3: g.scaleDim = g.std_vuW; break;                 // Studio: vízszintes hossza
    case 1: g.scaleDim = min(g.str_vuLW, g.str_vuRW); break; // Streamline: csatorna hossza
    case 2: g.scaleDim = min(g.bbx_vuLW, g.bbx_vuRW); break; // Boombox: csatorna hossza
    default: g.scaleDim = g.def_vuH; break;
  }
  if (g.scaleDim < 1) return;

  // ------------------------------------------------------------
  // INPUT + SCALING
  // ------------------------------------------------------------
  uint16_t vulevel = player.getVUlevel();

  uint16_t L_raw = (vulevel >> 8) & 0xFF;
  uint16_t R_raw = (vulevel) & 0xFF;


  // AUTOGAIN
  uint16_t currentMax = max(L_raw, R_raw);

  uint32_t now = millis();

  if (currentMax > config.vuRefLevel)
      config.vuRefLevel = currentMax;

  static uint32_t vuLastDecay = 0;

  if (now - vuLastDecay > 120) //original value 200
  {
      vuLastDecay = now;

      if (config.vuRefLevel > 1)
          config.vuRefLevel--;
  }

  if (config.vuRefLevel < 50)
      config.vuRefLevel = 50;


  // SCALE
  uint16_t vuLpx = map(L_raw, 0, config.vuRefLevel, 0, g.scaleDim);
  uint16_t vuRpx = map(R_raw, 0, config.vuRefLevel, 0, g.scaleDim);


  // CURVE
  float vuNorm_L = (float)vuLpx / g.scaleDim;
  float vuNorm_R = (float)vuRpx / g.scaleDim;

  float vuAdj_L = _applyVuCurve(vuNorm_L, g.ly);
  float vuAdj_R = _applyVuCurve(vuNorm_R, g.ly);

  uint16_t L_scaled = (uint16_t)roundf(vuAdj_L * g.scaleDim);
  uint16_t R_scaled = (uint16_t)roundf(vuAdj_R * g.scaleDim);

  // ------------------------------------------------------------
  // BANDS + COLORS + SMOOTH
  // ------------------------------------------------------------
  const uint16_t vumidcolor = config.store.vuMidColor;
  const uint8_t  bandsCount = _bands.perheight ? _bands.perheight : 1;
  const uint16_t h = (uint16_t)max(1, g.scaleDim / (int)bandsCount);

  const float alpha_up   = REF_BY_LAYOUT(config.store, vuAlphaUp, g.ly) / 100.0f;
  const float alpha_down = REF_BY_LAYOUT(config.store, vuAlphaDn, g.ly) / 100.0f;

  uint8_t midPct  = REF_BY_LAYOUT(config.store, vuMidPct,  g.ly);
  uint8_t highPct = REF_BY_LAYOUT(config.store, vuHighPct, g.ly);

  const uint16_t peakColor = config.store.vuPeakColor;

  int cfgBars = (int)REF_BY_LAYOUT(config.store, vuBarCount, g.ly);
  int effectiveBars = (int)bandsCount;
  if (effectiveBars <= 0) effectiveBars = cfgBars;
  if (effectiveBars <= 0) effectiveBars = 1;

  int midBandIndex  = (effectiveBars * midPct  + 99) / 100;
  int highBandIndex = (effectiveBars * highPct + 99) / 100;
  if (midBandIndex  < 0) midBandIndex = 0;
  if (highBandIndex < 0) highBandIndex = 0;
  if (midBandIndex  > effectiveBars) midBandIndex = effectiveBars;
  if (highBandIndex > effectiveBars) highBandIndex = effectiveBars;
  if (highBandIndex < midBandIndex) highBandIndex = midBandIndex;

  static float smoothL = 0, smoothR = 0;
  static float peakSmoothL = 0, peakSmoothR = 0;
  static uint16_t measL = 0, measR = 0;

  smoothL = (L_scaled > smoothL) ? (alpha_up   * L_scaled + (1.0f - alpha_up)   * smoothL)
                                 : (alpha_down * L_scaled + (1.0f - alpha_down) * smoothL);
  smoothR = (R_scaled > smoothR) ? (alpha_up   * R_scaled + (1.0f - alpha_up)   * smoothR)
                                 : (alpha_down * R_scaled + (1.0f - alpha_down) * smoothR);

  bool played = player.isRunning();
  if (played) {
    measL = (L_raw >= measL) ? (measL + _bands.fadespeed) : L_raw;
    measR = (R_raw >= measR) ? (measR + _bands.fadespeed) : R_raw;
  } else {
    if (measL < (uint16_t)g.scaleDim) measL += _bands.fadespeed;
    if (measR < (uint16_t)g.scaleDim) measR += _bands.fadespeed;
    peakSmoothL = 0; peakSmoothR = 0;
  }
  if (measL > (uint16_t)g.scaleDim) measL = (uint16_t)g.scaleDim;
  if (measR > (uint16_t)g.scaleDim) measR = (uint16_t)g.scaleDim;

#if DSP_MODEL == DSP_ST7735 || DSP_MODEL == DSP_ST7789_170
  const int peakWidth = 2;
#else
  const int peakWidth = 4;
#endif

  // ------------------------------------------------------------
  // CLEAR CANVAS
  // ------------------------------------------------------------
  _canvas->fillRect(0, 0, g.canvasW, g.canvasH, _bgcolor);

  // ------------------------------------------------------------
  // DRAW LABELS (mindenhol a VU-ból levonva számolt geo szerint!)
  // ------------------------------------------------------------
  switch (g.ly) {
    case 0: // Default: alul
      if (g.showLabels) _drawLabelsHorizontal(0, g.def_vuH, g.canvasW, g.labelSize);
      break;

    case 3: // Studio: bal oszlop, két sor
      if (g.showLabels) _drawLabelsVertical(0, 0, _bands.height, g.labelSize);
      break;

    case 1: // Streamline: label mindkét csatornában baloldalt
      if (g.showLabels) {
        _drawSingleLabel(g.str_LX, 0, _bands.height, 'L', g.labelSize);
        _drawSingleLabel(g.str_RX, 0, _bands.height, 'R', g.labelSize);
      }
      break;

    case 2: // Boombox: label a gap két oldalán (L bal oldal, R jobb oldal), a csatornákból levonva
      if (g.showLabels) {
        _drawSingleLabel(g.bbx_lblLX, 0, _bands.height, 'L', g.labelSize);
        _drawSingleLabel(g.bbx_lblRX, 0, _bands.height, 'R', g.labelSize);
      }
      break;
  }

  // ------------------------------------------------------------
  // PEAK SMOOTH + SNAP
  // ------------------------------------------------------------
  uint16_t drawL = (uint16_t)smoothL;
  uint16_t drawR = (uint16_t)smoothR;

  const float p_up   = REF_BY_LAYOUT(config.store, vuPeakUp, g.ly) / 100.0f;
  const float p_down = REF_BY_LAYOUT(config.store, vuPeakDn, g.ly) / 100.0f;

  peakSmoothL = (drawL > peakSmoothL) ? (p_up * drawL + (1 - p_up) * peakSmoothL)
                                      : (p_down * drawL + (1 - p_down) * peakSmoothL);
  peakSmoothR = (drawR > peakSmoothR) ? (p_up * drawR + (1 - p_up) * peakSmoothR)
                                      : (p_down * drawR + (1 - p_down) * peakSmoothR);

  uint16_t snappedL = ((drawL + h - 1) / h) * h;
  uint16_t snappedR = ((drawR + h - 1) / h) * h;
  if (snappedL > (uint16_t)g.scaleDim) snappedL = (uint16_t)g.scaleDim;
  if (snappedR > (uint16_t)g.scaleDim) snappedR = (uint16_t)g.scaleDim;

  // ------------------------------------------------------------
  // DRAW BARS (switch-case, átlátható)
  // ------------------------------------------------------------
  switch (g.ly) {

    case 3: { // ===================== Studio =====================
      const int peakOfs = 10;
      int peakX_L = constrain((int)max((int)snappedL, (int)peakSmoothL + peakOfs), 0, g.scaleDim - peakWidth);
      int peakX_R = constrain((int)max((int)snappedR, (int)peakSmoothR + peakOfs), 0, g.scaleDim - peakWidth);

      for (int band = 0; band < bandsCount; band++) {
        int i = band * h;
        int bandW = h - _bands.space;
        if (band == bandsCount - 1) bandW = h;

        uint16_t col = (band < midBandIndex) ? _vumincolor : (band < highBandIndex ? vumidcolor : _vumaxcolor);

        if (i + bandW <= snappedL)
          _canvas->fillRect(g.std_vuX + i, 0, bandW, _bands.height, col);

        if (i + bandW <= snappedR)
          _canvas->fillRect(g.std_vuX + i, _bands.height + _bands.space, bandW, _bands.height, col);
      }

      _canvas->fillRect(g.std_vuX + peakX_L, 0, peakWidth, _bands.height, peakColor);
      _canvas->fillRect(g.std_vuX + peakX_R, _bands.height + _bands.space, peakWidth, _bands.height, peakColor);
    } break;

    case 1: { // ===================== Streamline =====================
      const int peakOfs = 4;

      // bal csatorna VU: g.str_vuLX.. + g.scaleDim
      int peakX_L = constrain((int)max((int)snappedL, (int)peakSmoothL + peakOfs), 0, g.scaleDim - peakWidth);
      int peakX_R = constrain((int)max((int)snappedR, (int)peakSmoothR + peakOfs), 0, g.scaleDim - peakWidth);

      for (int band = 0; band < bandsCount; band++) {
        int i = band * h;
        int bandW = h - _bands.vspace;
        if (band == bandsCount - 1) bandW = h;

        uint16_t col = (band < midBandIndex) ? _vumincolor : (band < highBandIndex ? vumidcolor : _vumaxcolor);

        if (i + bandW <= snappedL)
          _canvas->fillRect(g.str_vuLX + i, 0, bandW, _bands.height, col);

        if (i + bandW <= snappedR)
          _canvas->fillRect(g.str_vuRX + i, 0, bandW, _bands.height, col);
      }

      _canvas->fillRect(g.str_vuLX + peakX_L, 0, peakWidth, _bands.height, peakColor);
      _canvas->fillRect(g.str_vuRX + peakX_R, 0, peakWidth, _bands.height, peakColor);
    } break;

    case 2: { // ===================== Boombox =====================
      const int peakOfs = 10;

      // bal: a belső széltől (bbx_lblLX) BALRA rajzolunk, max hossza g.scaleDim
      // jobb: a belső széltől (bbx_lblRX + labelSize) JOBBRA
      int leftInner  = g.bbx_lblLX;                 // L label bal széle (itt kezdődik a "közép előtti" blokk)
      int rightInner = g.bbx_lblRX + g.labelSize;   // R label utáni rész a jobb csatornának

      // csatorna végek
      int leftMinX   = 0;
      int leftMaxX   = leftInner;                   // ideig rajzolhatunk (bal csatorna jobboldali széle)
      int rightMinX  = rightInner;
      int rightMaxX  = g.canvasW;

      // peak hely
      int peakLenL = (int)max((int)snappedL, (int)peakSmoothL);
      int peakLenR = (int)max((int)snappedR, (int)peakSmoothR);

      int peakX_L = leftMaxX - peakLenL - peakOfs - peakWidth;
      int peakX_R = rightMinX + peakLenR + peakOfs;

      if (peakX_L < leftMinX) peakX_L = leftMinX;
      if (peakX_L > leftMaxX - peakWidth) peakX_L = leftMaxX - peakWidth;

      if (peakX_R < rightMinX) peakX_R = rightMinX;
      if (peakX_R > rightMaxX - peakWidth) peakX_R = rightMaxX - peakWidth;

      for (int band = 0; band < bandsCount; band++) {
        int i = band * h;
        int bandW = h - _bands.vspace;
        if (band == bandsCount - 1) bandW = h;

        uint16_t col = (band < midBandIndex) ? _vumincolor : (band < highBandIndex ? vumidcolor : _vumaxcolor);

        // bal: belső széltől BALRA
        if (i + bandW <= snappedL) {
          int x = leftMaxX - i - bandW;
          if (x < leftMinX) x = leftMinX;
          _canvas->fillRect(x, 0, bandW, _bands.height, col);
        }

        // jobb: belső széltől JOBBRA
        if (i + bandW <= snappedR) {
          int x = rightMinX + i;
          if (x + bandW > rightMaxX) bandW = max(1, rightMaxX - x);
          _canvas->fillRect(x, 0, bandW, _bands.height, col);
        }
      }

      _canvas->fillRect(peakX_L, 0, peakWidth, _bands.height, peakColor);
      _canvas->fillRect(peakX_R, 0, peakWidth, _bands.height, peakColor);
    } break;

    case 0:
    default: { // ===================== Default (vertical) =====================
      const int peakOfs = 4;

      int peakY_L = constrain((int)peakSmoothL + peakOfs, 0, g.scaleDim - peakWidth);
      int peakY_R = constrain((int)peakSmoothR + peakOfs, 0, g.scaleDim - peakWidth);

      int baseY = g.def_vuY + g.def_vuH;

      for (int band = 0; band < bandsCount; band++) {
        int i = band * h;
        int bandH = h - _bands.vspace;
        if (band == bandsCount - 1) bandH = h;

        uint16_t col = (band < midBandIndex) ? _vumincolor : (band < highBandIndex ? vumidcolor : _vumaxcolor);

        if (i + bandH <= snappedL)
          _canvas->fillRect(0, baseY - i - bandH, _bands.width, bandH, col);

        if (i + bandH <= snappedR)
          _canvas->fillRect(_bands.width + _bands.space, baseY - i - bandH, _bands.width, bandH, col);
      }

      _canvas->fillRect(0, baseY - peakY_L - peakWidth, _bands.width, peakWidth, peakColor);
      _canvas->fillRect(_bands.width + _bands.space, baseY - peakY_R - peakWidth, _bands.width, peakWidth, peakColor);
    } break;
  }

  // ------------------------------------------------------------
  // BLIT TO DISPLAY
  // ------------------------------------------------------------
  dsp.drawRGBBitmap(_config.left, _config.top, _canvas->getBuffer(), g.canvasW, g.canvasH);
}

void VuWidget::loop() {
  if (_active && !_locked) _draw();
}

void VuWidget::_clear() {
  switch (config.store.vuLayout) {
    case 3:
      dsp.fillRect(_config.left, _config.top, _maxDimension, _bands.height * 2 + _bands.space, _bgcolor);
      break;
    case 2:
    case 1:
      dsp.fillRect(_config.left, _config.top, _maxDimension * 2 + _bands.space, _bands.height, _bgcolor);
      break;
    case 0:
    default:
      dsp.fillRect(_config.left, _config.top, _bands.width * 2 + _bands.space, _maxDimension, _bgcolor);
      break;
  }
}

#else // DSP_LCD

VuWidget::~VuWidget() {}
void VuWidget::init(WidgetConfig wconf, VUBandsConfig bands, uint16_t vumaxcolor, uint16_t vumincolor, uint16_t bgcolor) {
  Widget::init(wconf, bgcolor, bgcolor);
}
void VuWidget::_draw() {}
void VuWidget::loop() {}
void VuWidget::_clear() {}

#endif

/************************
      NUM & CLOCK
 ************************/
#if !defined(DSP_LCD)
  #if TIME_SIZE<19 //19->NOKIA
    const GFXfont* Clock_GFXfontPtr = nullptr;
    #define CLOCKFONT5x7
  #else
    const GFXfont* Clock_GFXfontPtr = nullptr;
  #endif
#endif //!defined(DSP_LCD)

static inline void applyClockFontFromConfig() {
  #if !defined(DSP_LCD)
    const uint8_t id = clockfont_clamp_id(config.store.clockFontId); // 0..4
    Clock_GFXfontPtr = clockfont_get(id);
  #endif
}

#if !defined(CLOCKFONT5x7) && !defined(DSP_LCD)
// --- GFXfont metrika-alapú számolás (nincs ':' kivétel, nincs API-advance) ---
inline const GFXglyph* glyphPtr(const GFXfont* f, unsigned char c){
  if (!f) return nullptr;
  if (c < f->first || c > f->last) return nullptr;
  return f->glyph + (c - f->first);
}

uint8_t _charWidth(unsigned char c){
  if (!Clock_GFXfontPtr) return TIME_SIZE;        // 5x7 fallback biztosíték
  const GFXglyph* g = glyphPtr(Clock_GFXfontPtr, c);
  return g ? pgm_read_byte(&g->xAdvance) : 0;
}

uint16_t _textHeight(){
  if (!Clock_GFXfontPtr) return TIME_SIZE;        // 5x7 fallback biztosíték
  const GFXglyph* g = glyphPtr(Clock_GFXfontPtr, '8'); // reprezentatív magas szám
  return g ? pgm_read_byte(&g->height) : TIME_SIZE;
}

#else // CLOCKFONT5x7 || DSP_LCD
// --- 5x7 / LCD fallback: marad a régi skálázás ---
uint8_t _charWidth(unsigned char){
#ifndef DSP_LCD
  return CHARWIDTH * TIME_SIZE;
#else
  return 1;
#endif
}
uint16_t _textHeight(){
  return CHARHEIGHT * TIME_SIZE;
}
#endif

uint16_t _textWidth(const char *txt){
  uint16_t w = 0, l=strlen(txt);
  for(uint16_t c=0;c<l;c++) w+=_charWidth(txt[c]);
  //#if DSP_MODEL==DSP_ILI9225
  //return w+l;
  //#else
  return w;
  //#endif
}

/************************
      NUM WIDGET
 ************************/
void NumWidget::init(WidgetConfig wconf, uint16_t buffsize, bool uppercase, uint16_t fgcolor, uint16_t bgcolor) {
  Widget::init(wconf, fgcolor, bgcolor);
  _buffsize = buffsize;
  _text = (char *) malloc(sizeof(char) * _buffsize);
  memset(_text, 0, _buffsize);
  _oldtext = (char *) malloc(sizeof(char) * _buffsize);
  memset(_oldtext, 0, _buffsize);
  _textwidth = _oldtextwidth = _oldleft = 0;
  _uppercase = uppercase;
  _textheight = TIME_SIZE/*wconf.textsize*/;
}

void NumWidget::setText(const char* txt) {
  strlcpy(_text, txt, _buffsize);
  _getBounds();
  if (strcmp(_oldtext, _text) == 0) return;
  uint16_t realth = _textheight;
#if defined(DSP_OLED) && DSP_MODEL!=DSP_SSD1322
  if(Clock_GFXfontPtr==nullptr) realth = _textheight * 8; //CHARHEIGHT
#endif
  if (_active)
  #ifndef CLOCKFONT5x7
    dsp.fillRect(_oldleft == 0 ? _realLeft() : min(_oldleft, _realLeft()),  _config.top-_textheight+1, max(_oldtextwidth, _textwidth), realth, _bgcolor);
  #else
    dsp.fillRect(_oldleft == 0 ? _realLeft() : min(_oldleft, _realLeft()),  _config.top, max(_oldtextwidth, _textwidth), realth, _bgcolor);
  #endif

  _oldtextwidth = _textwidth;
  _oldleft = _realLeft();
  if (_active) _draw();
}

void NumWidget::setText(int val, const char *format){
  char buf[_buffsize];
  snprintf(buf, _buffsize, format, val);
  setText(buf);
}

void NumWidget::_getBounds() {
  _textwidth= _textWidth(_text);
}

void NumWidget::_draw() {
#ifndef DSP_LCD
  if(!_active || TIME_SIZE<2) return;
  applyClockFontFromConfig();
  dsp.setTextSize(Clock_GFXfontPtr==nullptr?TIME_SIZE:1);
  dsp.setFont(Clock_GFXfontPtr);
  dsp.setTextColor(_fgcolor, _bgcolor);
#endif
  if(!_active) return;
  dsp.setCursor(_realLeft(), _config.top);
  dsp.print(_text);
  strlcpy(_oldtext, _text, _buffsize);
  dsp.setFont();
}

/**************************
      PROGRESS WIDGET
 **************************/
void ProgressWidget::_progress() {
  char buf[_width + 1];
  snprintf(buf, _width, "%*s%.*s%*s", _pg <= _barwidth ? 0 : _pg - _barwidth, "", _pg <= _barwidth ? _pg : 5, ".....", _width - _pg, "");
  _pg++; if (_pg >= _width + _barwidth) _pg = 0;
  setText(buf);
}

bool ProgressWidget::_checkDelay(int m, uint32_t &tstamp) {
  if (millis() - tstamp > m) {
    tstamp = millis();
    return true;
  } else {
    return false;
  }
}

void ProgressWidget::loop() {
  if (_checkDelay(_speed, _scrolldelay)) {
    _progress();
  }
}

/**************************
      CLOCK WIDGET
 **************************/
void ClockWidget::init(WidgetConfig wconf, uint16_t fgcolor, uint16_t bgcolor){
  Widget::init(wconf, fgcolor, bgcolor);
  _timeheight = _textHeight();
  _fullclock = TIME_SIZE>35 || DSP_MODEL==DSP_ILI9225 || DSP_MODEL==DSP_ST7789_170 || DSP_MODEL==DSP_ST7789_76 || DSP_MODEL==DSP_ST7735 || DSP_MODEL == DSP_ST7789 || DSP_MODEL==DSP_ILI9341;
/*#if DSP_MODEL == DSP_ST7789 || DSP_MODEL==DSP_ILI9341
    if (config.store.vuLayout != 0) {
        _fullclock = false;
    }
#endif*/
  // _superfont: csak kompatibilitáshoz marad, fullclock esetén mindig 1
  if(_fullclock) _superfont = 1;
  else if(TIME_SIZE==19 || TIME_SIZE==2) _superfont=1;
  else _superfont=0;
  _space = _fullclock ? 4 : (5*_superfont)/2;
  #ifndef HIDE_DATE
  if(_fullclock){
    _dateheight = 1;  // dátum sor: DejaVuSans8 = 1 sor
    _clockheight = _timeheight + _space + CHARHEIGHT * _dateheight;
  } else {
    _clockheight = _timeheight;
    _dateheight = 0;
  }
  #else
    _clockheight = _timeheight;
  #endif
  applyClockFontFromConfig();

  auto& gfx = getRealDsp();
  gfx.setTextSize(Clock_GFXfontPtr==nullptr?TIME_SIZE:1);
  gfx.setFont(Clock_GFXfontPtr);
  
  _getTimeBounds();
#ifdef PSFBUFFER
  _fb = new psFrameBuffer(dsp.width(), dsp.height());
  _begin();
#endif
}

void ClockWidget::_begin(){
#ifdef PSFBUFFER
  _fb->begin(&dsp, _clockleft, _config.top-_timeheight, _clockwidth, _clockheight+1, config.theme.background);
#endif
}

bool ClockWidget::_getTime(){
  if (config.store.hours12) {
  strftime(_timebuffer, sizeof(_timebuffer), "%I:%M", &network.timeinfo);
  } else {
  strftime(_timebuffer, sizeof(_timebuffer), "%H:%M", &network.timeinfo);
  }
  bool ret = network.timeinfo.tm_sec==0 || _forceflag!=network.timeinfo.tm_year;
  _forceflag = network.timeinfo.tm_year;
  return ret;
}


uint16_t ClockWidget::_left(){
  if(_fb->ready()) return 0; else return _clockleft;
}
uint16_t ClockWidget::_top(){
  if(_fb->ready()) return _timeheight; else return _config.top;
}

void ClockWidget::_getTimeBounds() {
  _timewidth = _textWidth(_timebuffer);
  if(_fullclock){
    // Jobb oldal: kis font szélessége "88"-ra + space
    const uint8_t fid2 = clockfont_clamp_id(config.store.clockFontId);
    const GFXfont* sf2 = clockfont_get_small(fid2);
    const GFXfont* sf2eff = (sf2 != nullptr) ? sf2 : yoScrollFont(0);
    int16_t tbx2; int16_t tby2; uint16_t tbw2; uint16_t tbh2;
    dsp.setFont(sf2eff); dsp.setTextSize(1);
    dsp.getTextBounds("88", 0, 0, &tbx2, &tby2, &tbw2, &tbh2);
    dsp.setFont();
    uint16_t rightside = (uint16_t)tbw2 + _space * 2;
    // 12h módban az AM/PM is helyet foglal – ha szélesebb mint "88", azt vesszük
    if (config.store.hours12) {
      const GFXfont* amf = yoScrollFont(0);
      uint16_t ampmW = yoStringWidth(amf, "AM");  // "AM"≈"PM" szélesség
      if (ampmW > (uint16_t)tbw2) rightside = ampmW + _space * 2;
    }
    _clockwidth = _timewidth + rightside;
  } else {
    uint8_t fs = _superfont>0?_superfont:TIME_SIZE;
    uint16_t rightside = CHARWIDTH * fs * 2;
    if(_superfont==0)
      _clockwidth = _timewidth;
    else
      _clockwidth = _timewidth + rightside;
  }
  switch(_config.align){
    case WA_LEFT: _clockleft = _config.left; break;
    case WA_RIGHT: _clockleft = dsp.width()-_clockwidth-_config.left; break;
    default:
      _clockleft = (dsp.width()/2 - _clockwidth/2)+_config.left;
      break;
  }
  char buf[4];
  strftime(buf, 4, "%H", &network.timeinfo);
  _dotsleft=_textWidth(buf);
}

#ifndef DSP_LCD

  Adafruit_GFX& ClockWidget::getRealDsp(){
  #ifdef PSFBUFFER
    if (_fb && _fb->ready()) return *_fb;
  #endif
    return dsp;
  }

void ClockWidget::_printClock(bool force){
  time_t now = time(nullptr);
  struct tm ti;
  localtime_r(&now, &ti);
  
  auto& gfx = getRealDsp();
  applyClockFontFromConfig();
  gfx.setTextSize(Clock_GFXfontPtr==nullptr?TIME_SIZE:1);
  gfx.setFont(Clock_GFXfontPtr);
  const int16_t timeLeft        = _left();
  //const int16_t timeTop         = _top() - _timeheight;
  const int16_t timeWidthLive   = _textWidth(_timebuffer);
  //const int16_t vlineX          = timeLeft + timeWidthLive + _space;
  // secLeftFull: az óra jobb széle + space, ide kerül a másodperc
  const int16_t secLeftFull = timeLeft + timeWidthLive + _space;
  
  static const GFXfont* s_lastPrintFont = nullptr;
  if (Clock_GFXfontPtr != s_lastPrintFont) {
    _getTimeBounds();
    s_lastPrintFont = Clock_GFXfontPtr;
  }

  bool clockInTitle=!config.isScreensaver && _config.top<_timeheight; //DSP_SSD1306x32
  if(force){
    _clearClock();
    _getTimeBounds();
    #ifndef DSP_OLED
    if (config.store.clockFontMono) {
      gfx.setTextColor(config.theme.clockbg, config.theme.background);
      gfx.setCursor(_left(), _top());
      gfx.print("88:88");
    }
    #endif
    if (clockInTitle) {
      gfx.setTextColor(config.theme.meta, config.theme.metabg);
    } else {
      gfx.setTextColor(config.theme.clock, config.theme.background);
    }

    gfx.setCursor(_left(), _top());

  if (config.store.hours12) {
    if (_timebuffer[0] == '0') {
      int16_t leadX = _left();
      int16_t leadY = _top() - _timeheight + 1; 
      uint16_t leadW = _charWidth('0'); 
      uint16_t leadH = _timeheight + 2;

     gfx.fillRect(leadX, leadY, leadW, leadH, config.theme.background);

      if (clockInTitle) {
        gfx.setTextColor(config.theme.meta, config.theme.metabg);
      } else {
        gfx.setTextColor(config.theme.clock, config.theme.background);
      }

      gfx.setCursor(leadX + leadW, _top());
      gfx.print(_timebuffer + 1);
    } else {
      gfx.print(_timebuffer);
    }
  } else {
    gfx.print(_timebuffer);
  }

  } // if(force)

// ------------------------------------------------------------
// FULLCLOCK: AM/PM (12h esetén) + dátum  [vonalak eltávolítva]
// ------------------------------------------------------------
if (_fullclock) {
  bool fullClockOnScreensaver = (!config.isScreensaver || (_fb->ready() && FULL_SCR_CLOCK));
  _linesleft = _left() + _timewidth + _space;

  if (fullClockOnScreensaver) {

    // 12 órás mód: AM/PM felirat a másodperc FÖLÉ, DejaVuSans8 fix fonttal
    if (config.store.hours12) {
      #if TIME_SIZE==19
      const GFXfont* ampmFont = yoScrollFont(0); // DejaVuSans8 – fix
      #elif TIME_SIZE==35
      const GFXfont* ampmFont = yoScrollFont(0); // DejaVuSans8 – fix
      #elif TIME_SIZE==52
      const GFXfont* ampmFont = yoScrollFont(5); // DejaVuSans8 – fix
      #elif TIME_SIZE==70
      const GFXfont* ampmFont = yoScrollFont(1); // DejaVuSans8 – fix
      #endif
      
      uint8_t ampmH   = yoFontHeight(ampmFont);
//      int16_t ampmBaseOff = yoBaselineOffset(ampmFont);

      // AM/PM: a buffer/képernyő tetejére igazítva, X = _linesleft (space nélkül)
      #if TIME_SIZE==19
      int16_t ampmY = _top()-11; // az óra baseline-jával azonos szint
      #elif TIME_SIZE==35
      int16_t ampmY = _top()-21; // az óra baseline-jával azonos szint
      #elif TIME_SIZE==52
      int16_t ampmY = _top()-40; // az óra baseline-jával azonos szint
      #elif TIME_SIZE==70
      int16_t ampmY = _top()-55; // az óra baseline-jával azonos szint
      #endif
      int16_t ampmX = _linesleft;

      gfx.setTextColor(config.theme.dow, config.theme.background);
      char buf[3];
      strftime(buf, sizeof(buf), "%p", &network.timeinfo);
      gfx.fillRect(ampmX, 0, yoStringWidth(ampmFont, "AM") + 2, ampmH + 1, config.theme.background);
      gfx.setFont(ampmFont); gfx.setTextSize(1);
      gfx.setCursor(ampmX, ampmY);
      yoPrintUtf8(gfx, buf, config.theme.dow, config.theme.background, ampmFont);
    }

    // Dátum (ha nincs HIDE_DATE)
    formatDateCustom(_tmp, sizeof(_tmp), ti, config.store.dateFormat);
#ifndef HIDE_DATE
    strlcpy(_datebuf, _tmp, sizeof(_datebuf));
    uint16_t _datewidth = yoStringWidth(&DejaVuSans8_HU, _datebuf);
    dsp.setFont(&DejaVuSans8_HU);
    dsp.setTextSize(1);
    int16_t _dateY2 = _top() + _space + yoBaselineOffset(&DejaVuSans8_HU);
#if DSP_MODEL==DSP_GC9A01A || DSP_MODEL==DSP_GC9A01 || DSP_MODEL==DSP_GC9A01_I80
    dsp.setCursor((dsp.width() - _datewidth) / 2, _dateY2);
#else
    dsp.setCursor(_left() + _clockwidth - _datewidth, _dateY2);
#endif
    dsp.setTextColor(config.theme.date, config.theme.background);
    yoPrintUtf8(dsp, _datebuf, config.theme.date, config.theme.background, &DejaVuSans8_HU);
    dsp.setFont();
#endif
  }
}

// ------------------------------------------------------------
// SECONDS: kis órafont, az óra baseline-jával vízszintesen igazítva
// ------------------------------------------------------------
if (_fullclock || _superfont > 0) {

  // Másodperc értéke (kis simítással visszaugrás ellen)
  int sec = ti.tm_sec;
  if (_lastSec >= 0) {
    int diff = sec - _lastSec;
    if (diff < 0 && diff > -3) sec = _lastSec;
  }
  _lastSec = sec;
  sprintf(_tmp, "%02d", sec);

  const uint8_t fid = clockfont_clamp_id(config.store.clockFontId);
  const GFXfont* secFont    = clockfont_get_small(fid);
  const GFXfont* secFontFinal = (secFont != nullptr) ? secFont : yoScrollFont(0);
  const int8_t   secBl      = clockfont_baseline_small(fid);

  // "88" bounding box a pontos szélességhez és x-igazításhoz
  int16_t tbx, tby; uint16_t tbw, tbh;
  gfx.setFont(secFontFinal);
  gfx.setTextSize(1);
  gfx.getTextBounds("88", 0, 0, &tbx, &tby, &tbw, &tbh);

  // X: secLeftFull-tól középre igazítva a "88" szélességéhez
  int16_t x = secLeftFull - tbx;

  // Y: az óra baseline-jával azonos vonal
  // _top() az óra baseline koordinátája a GFX rendszerben
  int16_t y = _top() + secBl;

  // Törlési terület: a kis font teljes magassága felett és alatt
  int16_t clearTop = _top() - (int16_t)tbh - 2;
  int16_t clearH   = (int16_t)tbh + 4;
  if (clearTop < 0) { clearH += clearTop; clearTop = 0; }

  gfx.fillRect(secLeftFull - 2, clearTop, (int16_t)tbw + tbx + 4, clearH, config.theme.background);

  // Mono maszk "88"
  if (config.store.clockFontMono) {
    gfx.setTextColor(config.theme.clockbg, config.theme.background);
    gfx.setFont(secFontFinal);
    gfx.setTextSize(1);
    gfx.setCursor(x, y);
    gfx.print("88");
  }

  // Valódi másodperc
  gfx.setTextColor(
    config.theme.seconds,
    config.store.clockFontMono ? config.theme.clockbg : config.theme.background
  );
  gfx.setFont(secFontFinal);
  gfx.setTextSize(1);
  gfx.setCursor(x, y);
  gfx.print(_tmp);
}

  applyClockFontFromConfig();
  gfx.setTextSize(Clock_GFXfontPtr==nullptr?TIME_SIZE:1);
  gfx.setFont(Clock_GFXfontPtr);
  bool even = (ti.tm_sec % 2) == 0;
#ifndef DSP_OLED
  uint16_t bg = config.store.clockFontMono ? config.theme.clockbg : config.theme.background;
  uint16_t fg = even ? config.theme.clock : bg;
  gfx.setTextColor(fg, bg);
#else
  if(clockInTitle)
    gfx.setTextColor(even ? config.theme.meta : config.theme.metabg, config.theme.metabg);
  else
    gfx.setTextColor(even ? config.theme.clock : config.theme.background, config.theme.background);
#endif

  gfx.setCursor(_left()+_dotsleft, _top());
  gfx.print(":");
  gfx.setFont();
  if(_fb->ready()) _fb->display();
}

void ClockWidget::_clearClock(){
#ifdef PSFBUFFER
  if(_fb->ready()) _fb->clear();
  else
#endif
#ifndef CLOCKFONT5x7
  dsp.fillRect(_left(), _top()-_timeheight, _clockwidth, _clockheight+1, config.theme.background);
#else
  dsp.fillRect(_left(), _top(), _clockwidth+1, _clockheight+1, config.theme.background);
#endif
}

void ClockWidget::draw(bool force){
  if(!_active) return;

  applyClockFontFromConfig();

  static const GFXfont* s_lastFontPtr = nullptr;
  if (Clock_GFXfontPtr != s_lastFontPtr) {
    _getTimeBounds(); 
    s_lastFontPtr = Clock_GFXfontPtr;
    _printClock(true);
  } else {
    _printClock(_getTime());
  }
}

void ClockWidget::_draw(){
  if(!_active) return;
  _printClock(true);
}

uint16_t ClockWidget::clockWidth() const {
  return _clockwidth;
}

uint16_t ClockWidget::clockHeight() const {
  return _clockheight;
}

uint16_t ClockWidget::dateSize() const {
#ifdef HIDE_DATE
  return 0; 
#else
  return _fullclock ? (CHARHEIGHT * _dateheight) : 0;
#endif
}

void ClockWidget::_reset(){
#ifdef PSFBUFFER
  if(_fb->ready()) {
    _fb->freeBuffer();
    _getTimeBounds();
    _begin();
  }
#endif
}

void ClockWidget::_clear(){
  _clearClock();
}
#else //#ifndef DSP_LCD

void ClockWidget::_printClock(bool force){
  strftime(_timebuffer, sizeof(_timebuffer), "%H:%M", &ti);
  if(force){
    dsp.setCursor(dsp.width()-5, 0);
    dsp.print(_timebuffer);
  }
  dsp.setCursor(dsp.width()-5+2, 0);
  dsp.print((network.timeinfo.tm_sec % 2 == 0)?":":" ");
}

void ClockWidget::_clearClock(){}

void ClockWidget::draw(bool force){
  if(!_active) return;
  _printClock(true);
}
void ClockWidget::_draw(){
  if(!_active) return;
  _printClock(true);
}
void ClockWidget::_reset(){}
void ClockWidget::_clear(){}
#endif //#ifndef DSP_LCD

/**************************
      BITRATE WIDGET
 **************************/
void BitrateWidget::init(BitrateConfig bconf, uint16_t fgcolor, uint16_t bgcolor) {
    Widget::init(bconf.widget, fgcolor, bgcolor);
    _dimension = bconf.dimension;      // ez lesz a magasság
    _bitrate   = 0;
    _format    = BF_UNKNOWN;

    _charSize(bconf.widget.textsize, _charWidth, _textheight);
    memset(_buf, 0, sizeof(_buf));
}

void BitrateWidget::setBitrate(uint16_t bitrate) {
    _bitrate = bitrate;

    // Ha nagyobb mint 20000 → kbps konverzió
    if (_bitrate > 20000)
        _bitrate /= 1000;

    _draw();
}

void BitrateWidget::setFormat(BitrateFormat format) {
    _format = format;
    _draw();
}

void BitrateWidget::_charSize(uint8_t textsize, uint8_t &width, uint16_t &height) {
#ifndef DSP_LCD
    width  = textsize * CHARWIDTH;
    height = textsize * CHARHEIGHT;
#else
    width  = 1;
    height = 1;
#endif
}

void BitrateWidget::_draw() {
    _clear();

    if (!_active || _format == BF_UNKNOWN || _bitrate == 0)
        return;

    uint16_t boxW = _dimension * 2;      // teljes szélesség
    uint16_t boxH = _dimension / 2;      // magasság

    // --- Keret és kitöltés ---
    dsp.drawRect(_config.left, _config.top, boxW, boxH, _fgcolor);
    dsp.fillRect(_config.left + _dimension, _config.top, _dimension, boxH, _fgcolor);

    const GFXfont* _bfont = &DejaVuSans8_HU;
    uint8_t _bfontH = yoFontHeight(_bfont);
    int16_t _bbaseOff = yoBaselineOffset(_bfont);
    dsp.setFont(_bfont);
    dsp.setTextSize(1);
    dsp.setTextColor(_fgcolor, _bgcolor);

    // --- Bitrate string ---
    if (_bitrate < 1000)
        snprintf(_buf, sizeof(_buf), "%d", _bitrate);
    else
        snprintf(_buf, sizeof(_buf), "%.1f", _bitrate / 1000.0);

    // Bitrate középre → BAL blokk
    uint16_t leftX = _config.left;
    uint16_t centerX = leftX + (_dimension / 2);
    uint16_t textW = yoStringWidth(_bfont, _buf);

    dsp.setCursor(centerX - textW/2,
                  _config.top + boxH/2 - _bfontH/2 + _bbaseOff);
    yoPrintUtf8(dsp, _buf, _fgcolor, _bgcolor, _bfont);

    // --- Formátum → JOBB blokk ---
    dsp.setTextColor(_bgcolor, _fgcolor);

    char fmt[4] = "";
    switch(_format){
        case BF_MP3: strcpy(fmt,"MP3"); break;
        case BF_AAC: strcpy(fmt,"AAC"); break;
        case BF_FLAC:strcpy(fmt,"FLC"); break;
        case BF_OGG: strcpy(fmt,"OGG"); break;
        case BF_WAV: strcpy(fmt,"WAV"); break;
        case BF_VOR: strcpy(fmt,"VOR"); break;
        case BF_OPU: strcpy(fmt,"OPU"); break;
        default: break;
    }

    uint16_t rightCenterX = _config.left + _dimension + (_dimension/2);
    uint16_t fmtW = yoStringWidth(_bfont, fmt);

    dsp.setCursor(rightCenterX - fmtW/2,
                  _config.top + boxH/2 - _bfontH/2 + _bbaseOff);
    yoPrintUtf8(dsp, fmt, _bgcolor, _fgcolor, _bfont);
    dsp.setFont();
}

void BitrateWidget::_clear() {
    dsp.fillRect(_config.left, _config.top,
                 _dimension * 2, _dimension / 2,
                 _bgcolor);
}

/**********************************************************************************
                                          PLAYLIST WIDGET
    **********************************************************************************/
void PlayListWidget::init(ScrollWidget* current) {
  Widget::init({0, 0, 0, WA_LEFT}, 0, 0);
  _current = current;

#ifndef DSP_LCD
  {
    /* Playlist font: playlistConf.widget.textsize alapján (0=Sans8, 1=Sans13, 2=Bold12 ...) */
    const GFXfont* _plf = yoScrollFont(playlistConf.widget.textsize);
    if (_plf)
      _plItemHeight = yoFontHeight(_plf) + 4;
    else
      _plItemHeight = playlistConf.widget.textsize * (CHARHEIGHT - 1)
                    + playlistConf.widget.textsize * 4;
  }

  if (_plItemHeight < 10) _plItemHeight = 10;

  _plTtemsCount = (dsp.height() - 2) / _plItemHeight;
  if (_plTtemsCount < 1) _plTtemsCount = 1;
  if (_plTtemsCount > MAX_PL_PAGE_ITEMS)
      _plTtemsCount = MAX_PL_PAGE_ITEMS;

  // center alignment
  uint16_t contentHeight = _plTtemsCount * _plItemHeight;
  _plYStart = (dsp.height() - contentHeight) / 2;

  // center-scroll módhoz legyen páratlan elemszám
  if ((_plTtemsCount % 2) == 0 && _plTtemsCount > 1)
      _plTtemsCount--;

  _plCurrentPos = _plTtemsCount / 2;

  // moving cache reset
  for (int i = 0; i < MAX_PL_PAGE_ITEMS; i++)
      _plCache[i] = "";

  _plLoadedPage = -1;
  _plLastGlobalPos = -1;
  _plLastDrawTime = 0;

#else
  _plTtemsCount = PLMITEMS;
  _plCurrentPos = 1;
#endif
}

void PlayListWidget::_loadPlaylistPage(int pageIndex, int itemsPerPage) {

  for (int i = 0; i < MAX_PL_PAGE_ITEMS; i++)
      _plCache[i] = "";

  if (config.playlistLength() == 0) return;

  File playlist = config.SDPLFS()->open(REAL_PLAYL, "r");
  File index    = config.SDPLFS()->open(REAL_INDEX, "r");
  if (!playlist || !index) return;

  int startIdx = pageIndex * itemsPerPage;

  for (int i = 0; i < itemsPerPage; i++) {

    int globalIdx = startIdx + i;
    if (globalIdx >= config.playlistLength()) break;

    index.seek(globalIdx * 4, SeekSet);

    uint32_t posAddr;
    if (index.readBytes((char*)&posAddr, 4) != 4) break;

    playlist.seek(posAddr, SeekSet);

    String line = playlist.readStringUntil('\n');
    int tabIdx = line.indexOf('\t');
    if (tabIdx > 0) line = line.substring(0, tabIdx);
    line.trim();

    if (config.store.numplaylist && line.length() > 0)
      _plCache[i] = String(globalIdx + 1) + " " + line;
    else
      _plCache[i] = line;
  }

  playlist.close();
  index.close();
}

void PlayListWidget::drawPlaylist(uint16_t currentItem) {

#ifndef DSP_LCD
  if (config.store.playlistMode == 1) {
    _drawMovingCursor(currentItem);
  } else {
    _drawScrollCenter(currentItem);
  }
#else
  dsp.clear();
  _fillPlMenu(currentItem - _plCurrentPos, _plTtemsCount);
  dsp.setCursor(0, 1);
  dsp.write(uint8_t(126));
#endif
}

void PlayListWidget::_drawMovingCursor(uint16_t currentItem) {

  /* A _plcurrent ScrollWidget loop()-ja automatikusan újrarajzolna ha _doscroll=true.
     Üres szöveggel biztosítjuk hogy _doscroll=false legyen → nincs auto-rajzolás.
     Moving cursor módban a _printMoving kezeli a teljes vizuális megjelenítést. */
  if (_current) _current->setText("");

  bool isLongPause = (millis() - _plLastDrawTime > 2000);
  _plLastDrawTime = millis();

  int activeIdx   = (currentItem > 0) ? (currentItem - 1) : 0;
  int itemsPerPage = _plTtemsCount;

  int newPage     = activeIdx / itemsPerPage;
  int newLocalPos = activeIdx % itemsPerPage;

  _plCurrentPos = newLocalPos;

  bool pageChanged = (newPage != _plLoadedPage);

  if (pageChanged || isLongPause || _plCache[0].length() == 0) {

    if (pageChanged || _plCache[0].length() == 0) {
      _loadPlaylistPage(newPage, itemsPerPage);
      _plLoadedPage = newPage;
    }

    dsp.fillRect(0, _plYStart,
                 dsp.width(),
                 itemsPerPage * _plItemHeight,
                 config.theme.background);

    for (int i = 0; i < itemsPerPage; i++)
      _printMoving(i, _plCache[i].c_str());

  } else {

    int oldLocalPos = -1;
    if (_plLastGlobalPos > 0) {
       int oldIdx = _plLastGlobalPos - 1;
       oldLocalPos = oldIdx % itemsPerPage;
    }

    if (oldLocalPos >= 0 &&
        oldLocalPos < itemsPerPage &&
        oldLocalPos != newLocalPos) {

        _printMoving(oldLocalPos, _plCache[oldLocalPos].c_str());
    }

    _printMoving(newLocalPos, _plCache[newLocalPos].c_str());
  }

  _plLastGlobalPos = currentItem;
}

void PlayListWidget::_printMoving(uint8_t pos, const char* item) {

  if (pos >= _plTtemsCount) return;

  int16_t yPos = _plYStart + pos * _plItemHeight;
  bool isSelected = (pos == _plCurrentPos);

  uint16_t fgColor = isSelected
        ? config.theme.plcurrent
//        ? 0xFFE0   // sárga - teszt
        : config.theme.playlist[0];

  uint16_t bgColor = config.theme.background;

  dsp.fillRect(0, yPos, dsp.width(),
               _plItemHeight - 1,
               bgColor);

  if (item && item[0] != '\0') {
    const GFXfont* _plfont = yoScrollFont(playlistConf.widget.textsize);
    uint8_t _plH = yoFontHeight(_plfont);
    int16_t _plBase = yoBaselineOffset(_plfont);
    dsp.setFont(_plfont);
    dsp.setTextSize(1);
    dsp.setTextColor(fgColor, bgColor);
    int16_t _plY = yPos + (_plItemHeight - _plH) / 2 + _plBase;
    dsp.setCursor(TFT_FRAMEWDT, _plY);
    yoPrintUtf8(dsp, item, fgColor, bgColor, _plfont);
    dsp.setFont();
  }
}

void PlayListWidget::_drawScrollCenter(uint16_t currentItem) {

  uint8_t lastPos =
    _fillPlMenu((int)currentItem - _plCurrentPos,
                _plTtemsCount);

  if (lastPos < _plTtemsCount) {
    dsp.fillRect(0,
                 lastPos * _plItemHeight + _plYStart,
                 dsp.width(),
                 dsp.height() / 2,
                 config.theme.background);
  }
}

void PlayListWidget::_printScroll(uint8_t pos,
                                  const char* item) {

  dsp.setTextSize(playlistConf.widget.textsize);

  {
    /* Minden sort ugyanolyan fonttal rajzolunk – aktuális = plcurrent szín */
    uint16_t _psColor;
    uint16_t _psBg;
    if (pos == _plCurrentPos) {
      _psColor = config.theme.plcurrent;
      _psBg    = config.theme.plcurrentbg;
    } else {
      uint8_t dist   = abs((int)pos - (int)_plCurrentPos);
      uint8_t plColor = (dist > 5) ? 4 : (dist - 1);
      _psColor = config.theme.playlist[plColor];
      _psBg    = config.theme.background;
    }

    dsp.fillRect(0,
                 _plYStart + pos * _plItemHeight - 1,
                 dsp.width(),
                 _plItemHeight - 2,
                 _psBg);

    const GFXfont* _plfont2 = yoScrollFont(playlistConf.widget.textsize);
    int16_t _plBase2 = yoBaselineOffset(_plfont2);
    uint8_t _plH2    = yoFontHeight(_plfont2);
    dsp.setFont(_plfont2);
    dsp.setTextSize(1);
    int16_t _plY2 = _plYStart + pos * _plItemHeight + (_plItemHeight - _plH2) / 2 + _plBase2;
    dsp.setCursor(TFT_FRAMEWDT, _plY2);
    yoPrintUtf8(dsp, item, _psColor, _psBg, _plfont2);
    dsp.setFont();
  }
}

uint8_t PlayListWidget::_fillPlMenu(int from, uint8_t count) {
    int     ls = from;
    uint8_t c = 0;
    bool    finded = false;
    if (config.playlistLength() == 0) { return 0; }
    File playlist = config.SDPLFS()->open(REAL_PLAYL, "r");
    File index = config.SDPLFS()->open(REAL_INDEX, "r");
    while (true) {
        if (ls < 1) {
            ls++;
            _printScroll(c, "");
            c++;
            continue;
        }
        if (!finded) {
            index.seek((ls - 1) * 4, SeekSet);
            uint32_t pos;
            index.readBytes((char*)&pos, 4);
            finded = true;
            index.close();
            playlist.seek(pos, SeekSet);
        }
        bool pla = true;
        while (pla) {
            pla = playlist.available();
            String stationName = playlist.readStringUntil('\n');
            stationName = stationName.substring(0, stationName.indexOf('\t'));
            if (config.store.numplaylist && stationName.length() > 0) { stationName = String(from + c) + " " + stationName; }
            _printScroll(c, stationName.c_str());
            c++;
            if (c >= count) { break; }
        }
        break;
    }
    playlist.close();
    return c;
}
/**************************
      DATE WIDGET
 **************************/
void DateWidget::update() {
  if (!_active) return;

  time_t now = time(nullptr);
  struct tm ti;
  localtime_r(&now, &ti);

  char dateBuf[64];
  uint8_t fmt = config.store.dateFormat;
  #if (DSP_MODEL==DSP_ST7789 || DSP_MODEL==DSP_ILI9341)
    if (fmt > 1) {
      fmt = 0; 
    }
  #endif

  formatDateCustom(dateBuf, sizeof(dateBuf), ti, fmt);

  uint16_t desired;
  #ifndef DSP_LCD
  if (_gfxFont)
    desired = yoStringWidth(_gfxFont, dateBuf) + yoGlyphWidth(_gfxFont, ' ') * 2;
  else {
    uint8_t _cw = _config.textsize * CHARWIDTH;
    auto utf8len = [](const char* s) -> size_t {
      size_t len = 0;
      for (const unsigned char* p = (const unsigned char*)s; *p; ++p)
        if ( (*p & 0xC0) != 0x80 ) ++len;
      return len;
    };
    desired = (uint16_t)(utf8len(dateBuf) * _cw) + _cw * 2;
  }
  #else
  desired = 1;
  #endif

  uint16_t minW = (_gfxFont ? yoGlyphWidth(_gfxFont, 'W') : (uint8_t)CHARWIDTH) * 6;
  uint16_t maxW = dsp.width() > 10 ? dsp.width() - 10 : dsp.width();
  if (desired < minW) desired = minW;
  if (desired > maxW) desired = maxW;

#if DSP_MODEL == DSP_ST7789_76
  // This compact layout shares the row with weather. Keep the configured
  // width so both widgets draw and clear exactly the same area.
  setWindowWidth(getDateConf().width);
#else
  setWindowWidth(desired);  // vibrálásmentes: setWindowWidth csak reposition()-t hív ha méret nem változott
#endif

  char line[192];
  strlcpy(line, dateBuf, sizeof(line));

#ifdef NAMEDAYS_FILE
  if (config.store.showNameday) {
    char nd[160] = {0};
    if (namedays_get_str((uint8_t)ti.tm_mon + 1, (uint8_t)ti.tm_mday, nd, sizeof(nd)) && nd[0]) {
      strlcat(line, " ¦ ", sizeof(line));
      strlcat(line, nd,    sizeof(line));
    }
  }
#endif

  /* _gfxFont esetén utf8To() kihagyása - a DejaVu font UTF-8-at vár */
  if (_gfxFont)
    setText(line);
  else
    setText(utf8To(line, false));
}

/************************
   WEATHER ICON WIDGET
 ************************/

void WeatherIconWidget::init(WidgetConfig wconf, uint16_t bgcolor){
  // fgcolor itt nem kell, az ikon egy bitmap
  Widget::init(wconf, 0, bgcolor);
  _img = nullptr;
  _iw  = 0;
  _ih  = 0;
}

void WeatherIconWidget::_clear(){
  if (!_active) return;
  uint16_t w = _width  ? _width  : (_iw ? _iw : 64);
  uint16_t h = _ih ? _ih : 64;
  dsp.fillRect(_config.left, _config.top, w, h, _bgcolor);
}

void WeatherIconWidget::_draw(){
  if(!_active) return;

  uint16_t w = _iw ? _iw : 64;
  uint16_t h = _ih ? _ih : 64;
  uint16_t areaW = _width ? _width : w;
  
  int16_t iconX = _config.left;
  int16_t iconY = _config.top;

  // ikon kirajzolás
  if (_img) dsp.drawRGBBitmap(iconX, iconY, _img, w, h);

  // ha nincs hőmérséklet → nincs további rajz
  if (_temp[0] == '\0') return;

  const GFXfont* _wifont = &DejaVuSans11_HU;
  uint16_t txtW = yoStringWidth(_wifont, _temp);
  uint8_t  txtH = yoFontHeight(_wifont);
  int16_t  base = yoBaselineOffset(_wifont);
  dsp.setFont(_wifont);
  dsp.setTextSize(1);
  dsp.setTextColor(config.theme.dow, _bgcolor);

  bool vertical = (config.store.vuLayout == 0);

  if (vertical) {
      int16_t tx = _config.left + (areaW - txtW) / 2;
      int16_t ty = _config.top - txtH + base;
      dsp.setCursor(tx, ty);
      yoPrintUtf8(dsp, _temp, config.theme.dow, _bgcolor, _wifont);
  } else {
      int16_t tx = iconX + w + 6;
      int16_t ty = iconY + (h - txtH) / 2 + base;
      dsp.setCursor(tx, ty);
      yoPrintUtf8(dsp, _temp, config.theme.dow, _bgcolor, _wifont);
  }
  dsp.setFont();
}

void WeatherIconWidget::setIcon(const char* code) {
  const IconSet& set = currentIconSet();

  WeatherIconId id = iconIdFromCode(code);

  if (id == WI_UNKNOWN) {
    _img = set.fallback;
  } else {
    _img = set.icons[(int)id];
  }

  _iw = set.w;
  _ih = set.h;

  if (_active && !_locked) {
    _clear();
    _draw();
  }
}

void WeatherIconWidget::setTemp(float tempC) {
#ifdef IMPERIALUNIT
  snprintf(_temp, sizeof(_temp), "%.0f°F", tempC);
#else
  snprintf(_temp, sizeof(_temp), "%.0f°C", tempC);
#endif

  _hasTemp = true;

  if (_active && !_locked) {
    _clear();
    _draw();
  }
}


/**************************
      STATIONNUM WIDGET
 **************************/
void StationNumWidget::init(StationNumConfig conf, uint16_t fgcolor, uint16_t bgcolor) {
    Widget::init(conf.widget, fgcolor, bgcolor);
    _dimension = conf.dimension;
    _num = 0;
}

void StationNumWidget::setNum(uint16_t num) {
    _num = num;
    _draw();
}

void StationNumWidget::_draw() {
    _clear();
    if (!_active || _num == 0) return;

    uint16_t boxW = _dimension;      // szélesség = dimension
    uint16_t boxH = _dimension / 2;  // magasság  = dimension/2

    // Üres keret (outline), mint a BitrateWidget bal fele
    dsp.drawRect(_config.left, _config.top, boxW, boxH, _fgcolor);

    const GFXfont* _fnt = yoScrollFont(_config.textsize);
    uint8_t  _fntH    = yoFontHeight(_fnt);
    int16_t  _baseOff = yoBaselineOffset(_fnt);
    dsp.setFont(_fnt);
    dsp.setTextSize(1);
    dsp.setTextColor(_fgcolor, _bgcolor);

    char buf[6];
    snprintf(buf, sizeof(buf), "%d", _num);
    uint16_t textW = yoStringWidth(_fnt, buf);
    uint16_t cx    = _config.left + boxW / 2;

    dsp.setCursor(cx - textW / 2,
                  _config.top + boxH / 2 - _fntH / 2 + _baseOff);
    yoPrintUtf8(dsp, buf, _fgcolor, _bgcolor, _fnt);
    dsp.setFont();
}

void StationNumWidget::_clear() {
    dsp.fillRect(_config.left, _config.top,
                 _dimension, _dimension / 2,
                 _bgcolor);
}

/**************************
      PLAYMODE WIDGET
 **************************/
void PlayModeWidget::init(PlayModeConfig conf, uint16_t fgcolor, uint16_t bgcolor) {
    Widget::init(conf.widget, fgcolor, bgcolor);
    _dimension = conf.dimension;
    _mode = 255;  // ismeretlen
}

void PlayModeWidget::setMode(uint8_t mode) {
    _mode = mode;
    _draw();
}

void PlayModeWidget::_draw() {
    _clear();
    if (!_active) return;

    uint16_t boxW = _dimension;      // szélesség = dimension
    uint16_t boxH = _dimension / 2;  // magasság  = dimension/2

    // Teli háttér (filled), mint a BitrateWidget jobb (codec) fele
    dsp.fillRect(_config.left, _config.top, boxW, boxH, _fgcolor);

    const GFXfont* _fnt = yoScrollFont(_config.textsize);
    uint8_t  _fntH    = yoFontHeight(_fnt);
    int16_t  _baseOff = yoBaselineOffset(_fnt);
    dsp.setFont(_fnt);
    dsp.setTextSize(1);

    const char* label = "WEB";
    if (_mode == 1)      label = "SD";
    else if (_mode == 2) label = "DLNA";

    uint16_t textW = yoStringWidth(_fnt, label);
    uint16_t cx    = _config.left + boxW / 2;

    dsp.setCursor(cx - textW / 2,
                  _config.top + boxH / 2 - _fntH / 2 + _baseOff);
    yoPrintUtf8(dsp, label, _bgcolor, _fgcolor, _fnt);
    dsp.setFont();
}

void PlayModeWidget::_clear() {
    dsp.fillRect(_config.left, _config.top,
                 _dimension, _dimension / 2,
                 _bgcolor);
}

// ─── StatusWidget ───────────────────────────────────────────────────────────
void StatusWidget::init(StatusWidgetConfig conf, const char* label,
                        uint16_t activecolor, uint16_t inactivecolor) {
    Widget::init(conf.widget, activecolor, inactivecolor);
    _boxW  = conf.width;
    _boxH  = conf.height;
    _label = label;
}

void StatusWidget::_draw() {
    uint16_t boxColor  = _active ? _fgcolor : _bgcolor;
    uint16_t textColor = _active ? _bgcolor : _fgcolor;
    dsp.fillRect(_config.left, _config.top, _boxW, _boxH, boxColor);

    const GFXfont* fnt     = yoScrollFont(_config.textsize);
    uint8_t        fntH    = yoFontHeight(fnt);
    int16_t        baseOff = yoBaselineOffset(fnt);
    dsp.setFont(fnt);
    dsp.setTextSize(1);

    uint16_t textW = yoStringWidth(fnt, _label);
    uint16_t cx    = _config.left + _boxW / 2;
    dsp.setCursor(cx - textW / 2,
                  _config.top + _boxH / 2 - fntH / 2 + baseOff);
    yoPrintUtf8(dsp, _label, textColor, boxColor, fnt);
    dsp.setFont();
}

void StatusWidget::_clear() {
    dsp.fillRect(_config.left, _config.top, _boxW, _boxH, _bgcolor);
}

#endif // #if DSP_MODEL!=DSP_DUMMY
