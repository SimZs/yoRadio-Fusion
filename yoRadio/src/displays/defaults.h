#pragma once
#include "dspcore.h" 

// --------------------------------------------------------------------------------------240x240
#if DSP_MODEL==DSP_GC9A01 || DSP_MODEL==DSP_GC9A01A || DSP_MODEL==DSP_GC9A01_I80 || DSP_MODEL==DSP_ST7789_240
  #define VU_DEF_HGT_DEF  35
  #define VU_DEF_BARS_DEF 10
  #define VU_DEF_GAP_DEF  2
  #define VU_DEF_FADE_DEF 5
  
  #define VU_DEF_HGT_STR  20
  #define VU_DEF_BARS_STR 10
  #define VU_DEF_GAP_STR  2
  #define VU_DEF_FADE_STR 5
  
  #define VU_DEF_HGT_BBX  20
  #define VU_DEF_BARS_BBX 10
  #define VU_DEF_GAP_BBX  2
  #define VU_DEF_FADE_BBX 5
  
  #define VU_DEF_HGT_STD  8
  #define VU_DEF_BARS_STD 14
  #define VU_DEF_GAP_STD  2
  #define VU_DEF_FADE_STD 2
  

// Smoothing (percent)
  #define VU_DEF_AUP_DEF  30  // 0.40
  #define VU_DEF_ADN_DEF  6  // 0.15
  #define VU_DEF_AUP_STR  30  // 0.25
  #define VU_DEF_ADN_STR  6  // 0.07
  #define VU_DEF_AUP_BBX  30  // 0.25
  #define VU_DEF_ADN_BBX  6  // 0.07
  #define VU_DEF_AUP_STD  24  // 0.10
  #define VU_DEF_ADN_STD  5  // 0.05

// Peak p_up / p_down (percent)
  #define VU_DEF_PUP_DEF  90  // 0.90
  #define VU_DEF_PDN_DEF   1  // 0.01
  #define VU_DEF_PUP_STR  90
  #define VU_DEF_PDN_STR   1
  #define VU_DEF_PUP_BBX  90
  #define VU_DEF_PDN_BBX   1
  #define VU_DEF_PUP_STD  90
  #define VU_DEF_PDN_STD   1

// Dymanics
  #define VU_DEF_EXPO_DEF  120  // 0.40
  #define VU_DEF_EXPO_STR  140  // 0.25
  #define VU_DEF_EXPO_BBX  140  // 0.25
  #define VU_DEF_EXPO_STD  130  // 0.10
  #define VU_DEF_FLOOR_DEF  35  // 0.40
  #define VU_DEF_FLOOR_STR  82  // 0.25
  #define VU_DEF_FLOOR_BBX  82  // 0.25
  #define VU_DEF_FLOOR_STD  20  // 0.10
  #define VU_DEF_CEIL_DEF  100  // 0.40
  #define VU_DEF_CEIL_STR  100  // 0.25
  #define VU_DEF_CEIL_BBX  100  // 0.25
  #define VU_DEF_CEIL_STD  100  // 0.10
  #define VU_DEF_GAIN_DEF  120  // 0.40
  #define VU_DEF_GAIN_STR  195  // 0.25
  #define VU_DEF_GAIN_BBX  195  // 0.25
  #define VU_DEF_GAIN_STD  90  // 0.10
  #define VU_DEF_KNEE_DEF  0  // 0.40
  #define VU_DEF_KNEE_STR  5  // 0.25
  #define VU_DEF_KNEE_BBX  5  // 0.25
  #define VU_DEF_KNEE_STD  3  // 0.10

// Band indexes
  #define VU_DEF_MID_PCT_DEF 60
  #define VU_DEF_MID_PCT_STR 60
  #define VU_DEF_MID_PCT_BBX 60
  #define VU_DEF_MID_PCT_STD 60

  #define VU_DEF_HIGH_PCT_DEF 80
  #define VU_DEF_HIGH_PCT_STR 80
  #define VU_DEF_HIGH_PCT_BBX 80
  #define VU_DEF_HIGH_PCT_STD 80

// Label heights
  #define VU_DEF_LHGT_DEF 18
  #define VU_DEF_LHGT_STR 22
  #define VU_DEF_LHGT_BBX 22
  #define VU_DEF_LHGT_STD 30
// --------------------------------------------------------------------------------------320x240
#elif DSP_MODEL==DSP_ST7789 || DSP_MODEL==DSP_ILI9341
  #define VU_DEF_HGT_DEF  35
  #define VU_DEF_BARS_DEF 10
  #define VU_DEF_GAP_DEF  2
  #define VU_DEF_FADE_DEF 2
  
  #define VU_DEF_HGT_STR  20
  #define VU_DEF_BARS_STR 16
  #define VU_DEF_GAP_STR  2
  #define VU_DEF_FADE_STR 5
  
  #define VU_DEF_HGT_BBX  20
  #define VU_DEF_BARS_BBX 16
  #define VU_DEF_GAP_BBX  2
  #define VU_DEF_FADE_BBX 2
  
  #define VU_DEF_HGT_STD  7
  #define VU_DEF_BARS_STD 16
  #define VU_DEF_GAP_STD  2
  #define VU_DEF_FADE_STD 2

// Smoothing (percent)
  #define VU_DEF_AUP_DEF  30  // 0.40
  #define VU_DEF_ADN_DEF  6  // 0.15
  #define VU_DEF_AUP_STR  30  // 0.25
  #define VU_DEF_ADN_STR  6  // 0.07
  #define VU_DEF_AUP_BBX  30  // 0.25
  #define VU_DEF_ADN_BBX  6  // 0.07
  #define VU_DEF_AUP_STD  24  // 0.10
  #define VU_DEF_ADN_STD  5  // 0.05

// Peak p_up / p_down (percent)
  #define VU_DEF_PUP_DEF  90  // 0.90
  #define VU_DEF_PDN_DEF   1  // 0.01
  #define VU_DEF_PUP_STR  90
  #define VU_DEF_PDN_STR   1
  #define VU_DEF_PUP_BBX  90
  #define VU_DEF_PDN_BBX   1
  #define VU_DEF_PUP_STD  90
  #define VU_DEF_PDN_STD   1

// Dymanics
  #define VU_DEF_EXPO_DEF   50  // 0.40
  #define VU_DEF_EXPO_STR  130  // 0.25
  #define VU_DEF_EXPO_BBX  130  // 0.25
  #define VU_DEF_EXPO_STD  130  // 0.10
  #define VU_DEF_FLOOR_DEF  70  // 0.40
  #define VU_DEF_FLOOR_STR  90  // 0.25
  #define VU_DEF_FLOOR_BBX  90  // 0.25
  #define VU_DEF_FLOOR_STD  20  // 0.10
  #define VU_DEF_CEIL_DEF  100  // 0.40
  #define VU_DEF_CEIL_STR   95  // 0.25
  #define VU_DEF_CEIL_BBX   95  // 0.25
  #define VU_DEF_CEIL_STD  100  // 0.10
  #define VU_DEF_GAIN_DEF   90  // 0.40
  #define VU_DEF_GAIN_STR  110  // 0.25
  #define VU_DEF_GAIN_BBX  110  // 0.25
  #define VU_DEF_GAIN_STD   90  // 0.10
  #define VU_DEF_KNEE_DEF   10  // 0.40
  #define VU_DEF_KNEE_STR    5  // 0.25
  #define VU_DEF_KNEE_BBX    5  // 0.25
  #define VU_DEF_KNEE_STD    3  // 0.10

// Band indexes
  #define VU_DEF_MID_PCT_DEF 60
  #define VU_DEF_MID_PCT_STR 60
  #define VU_DEF_MID_PCT_BBX 60
  #define VU_DEF_MID_PCT_STD 60

  #define VU_DEF_HIGH_PCT_DEF 80
  #define VU_DEF_HIGH_PCT_STR 80
  #define VU_DEF_HIGH_PCT_BBX 80
  #define VU_DEF_HIGH_PCT_STD 80

// Label heights
  #define VU_DEF_LHGT_DEF 15
  #define VU_DEF_LHGT_STR 20
  #define VU_DEF_LHGT_BBX 20
  #define VU_DEF_LHGT_STD 30
// --------------------------------------------------------------------------------------480x320
#elif DSP_MODEL==DSP_ILI9488 || DSP_MODEL==DSP_ILI9486 || DSP_MODEL==DSP_ST7796
  #define VU_DEF_HGT_DEF  37
  #define VU_DEF_BARS_DEF 10
  #define VU_DEF_GAP_DEF  2
  #define VU_DEF_FADE_DEF 3
  
  #define VU_DEF_HGT_STR  23
  #define VU_DEF_BARS_STR 20
  #define VU_DEF_GAP_STR  3
  #define VU_DEF_FADE_STR 5
  
  #define VU_DEF_HGT_BBX  23
  #define VU_DEF_BARS_BBX 20
  #define VU_DEF_GAP_BBX  3
  #define VU_DEF_FADE_BBX 5
  
  #define VU_DEF_HGT_STD  10
  #define VU_DEF_BARS_STD 20
  #define VU_DEF_GAP_STD  3
  #define VU_DEF_FADE_STD 7

// Smoothing (percent)
  #define VU_DEF_AUP_DEF  30  // 0.40
  #define VU_DEF_ADN_DEF  6  // 0.15
  #define VU_DEF_AUP_STR  60  // 0.25
  #define VU_DEF_ADN_STR  6  // 0.07
  #define VU_DEF_AUP_BBX  60  // 0.25
  #define VU_DEF_ADN_BBX  6  // 0.07
  #define VU_DEF_AUP_STD  20  // 0.10
  #define VU_DEF_ADN_STD  8  // 0.05

// Peak p_up / p_down (percent)
  #define VU_DEF_PUP_DEF  90  // 0.90
  #define VU_DEF_PDN_DEF   1  // 0.01
  #define VU_DEF_PUP_STR  90
  #define VU_DEF_PDN_STR   1
  #define VU_DEF_PUP_BBX  90
  #define VU_DEF_PDN_BBX   1
  #define VU_DEF_PUP_STD  90
  #define VU_DEF_PDN_STD   1

// Dymanics
  #define VU_DEF_EXPO_DEF   75  // 0.40
  #define VU_DEF_EXPO_STR  190  // 0.25
  #define VU_DEF_EXPO_BBX  190  // 0.25
  #define VU_DEF_EXPO_STD  150  // 0.10
  #define VU_DEF_FLOOR_DEF  85  // 0.40
  #define VU_DEF_FLOOR_STR  82  // 0.25
  #define VU_DEF_FLOOR_BBX  82  // 0.25
  #define VU_DEF_FLOOR_STD  10  // 0.10
  #define VU_DEF_CEIL_DEF   95  // 0.40
  #define VU_DEF_CEIL_STR   95  // 0.25
  #define VU_DEF_CEIL_BBX   95  // 0.25
  #define VU_DEF_CEIL_STD  100  // 0.10
  #define VU_DEF_GAIN_DEF  100  // 0.40
  #define VU_DEF_GAIN_STR  160  // 0.25
  #define VU_DEF_GAIN_BBX  160  // 0.25
  #define VU_DEF_GAIN_STD   95  // 0.10
  #define VU_DEF_KNEE_DEF    7  // 0.40
  #define VU_DEF_KNEE_STR   15  // 0.25
  #define VU_DEF_KNEE_BBX   15  // 0.25
  #define VU_DEF_KNEE_STD   10  // 0.10

// Band indexes
  #define VU_DEF_MID_PCT_DEF 60
  #define VU_DEF_MID_PCT_STR 60
  #define VU_DEF_MID_PCT_BBX 60
  #define VU_DEF_MID_PCT_STD 60

  #define VU_DEF_HIGH_PCT_DEF 80
  #define VU_DEF_HIGH_PCT_STR 80
  #define VU_DEF_HIGH_PCT_BBX 80
  #define VU_DEF_HIGH_PCT_STD 80

// Label heights
  #define VU_DEF_LHGT_DEF 18
  #define VU_DEF_LHGT_STR 22
  #define VU_DEF_LHGT_BBX 22
  #define VU_DEF_LHGT_STD 30
// --------------------------------------------------------------------------------------480x272
#elif DSP_MODEL==DSP_NV3041A
  #define VU_DEF_HGT_DEF  37
  #define VU_DEF_BARS_DEF 8
  #define VU_DEF_GAP_DEF  2
  #define VU_DEF_FADE_DEF 3
  
  #define VU_DEF_HGT_STR  25
  #define VU_DEF_BARS_STR 20
  #define VU_DEF_GAP_STR  3
  #define VU_DEF_FADE_STR 5
  
  #define VU_DEF_HGT_BBX  25
  #define VU_DEF_BARS_BBX 20
  #define VU_DEF_GAP_BBX  3
  #define VU_DEF_FADE_BBX 3
  
  #define VU_DEF_HGT_STD  12
  #define VU_DEF_BARS_STD 20
  #define VU_DEF_GAP_STD  3
  #define VU_DEF_FADE_STD 5

// Smoothing (percent)
  #define VU_DEF_AUP_DEF  30  // 0.40
  #define VU_DEF_ADN_DEF  6  // 0.15
  #define VU_DEF_AUP_STR  30  // 0.25
  #define VU_DEF_ADN_STR  6  // 0.07
  #define VU_DEF_AUP_BBX  30  // 0.25
  #define VU_DEF_ADN_BBX  6  // 0.07
  #define VU_DEF_AUP_STD  24  // 0.10
  #define VU_DEF_ADN_STD  5  // 0.05

// Peak p_up / p_down (percent)
  #define VU_DEF_PUP_DEF  90  // 0.90
  #define VU_DEF_PDN_DEF   1  // 0.01
  #define VU_DEF_PUP_STR  90
  #define VU_DEF_PDN_STR   1
  #define VU_DEF_PUP_BBX  90
  #define VU_DEF_PDN_BBX   1
  #define VU_DEF_PUP_STD  90
  #define VU_DEF_PDN_STD   1

// Dymanics
  #define VU_DEF_EXPO_DEF  120  // 0.40
  #define VU_DEF_EXPO_STR  140  // 0.25
  #define VU_DEF_EXPO_BBX  140  // 0.25
  #define VU_DEF_EXPO_STD  130  // 0.10
  #define VU_DEF_FLOOR_DEF  80  // 0.40
  #define VU_DEF_FLOOR_STR  82  // 0.25
  #define VU_DEF_FLOOR_BBX  82  // 0.25
  #define VU_DEF_FLOOR_STD  20  // 0.10
  #define VU_DEF_CEIL_DEF  100  // 0.40
  #define VU_DEF_CEIL_STR  100  // 0.25
  #define VU_DEF_CEIL_BBX  100  // 0.25
  #define VU_DEF_CEIL_STD  100  // 0.10
  #define VU_DEF_GAIN_DEF  100  // 0.40
  #define VU_DEF_GAIN_STR  195  // 0.25
  #define VU_DEF_GAIN_BBX  195  // 0.25
  #define VU_DEF_GAIN_STD  90  // 0.10
  #define VU_DEF_KNEE_DEF  5  // 0.40
  #define VU_DEF_KNEE_STR  5  // 0.25
  #define VU_DEF_KNEE_BBX  5  // 0.25
  #define VU_DEF_KNEE_STD  3  // 0.10

// Band indexes
  #define VU_DEF_MID_PCT_DEF 60
  #define VU_DEF_MID_PCT_STR 60
  #define VU_DEF_MID_PCT_BBX 60
  #define VU_DEF_MID_PCT_STD 60

  #define VU_DEF_HIGH_PCT_DEF 80
  #define VU_DEF_HIGH_PCT_STR 80
  #define VU_DEF_HIGH_PCT_BBX 80
  #define VU_DEF_HIGH_PCT_STD 80

// Label heights
  #define VU_DEF_LHGT_DEF 18
  #define VU_DEF_LHGT_STR 22
  #define VU_DEF_LHGT_BBX 22
  #define VU_DEF_LHGT_STD 30
// --------------------------------------------------------------------------------------320x170
#elif DSP_MODEL==DSP_ST7789_170
  #define VU_DEF_HGT_DEF  35
  #define VU_DEF_BARS_DEF 10
  #define VU_DEF_GAP_DEF  2
  #define VU_DEF_FADE_DEF 2
  
  #define VU_DEF_HGT_STR  15
  #define VU_DEF_BARS_STR 16
  #define VU_DEF_GAP_STR  2
  #define VU_DEF_FADE_STR 5
  
  #define VU_DEF_HGT_BBX  15
  #define VU_DEF_BARS_BBX 16
  #define VU_DEF_GAP_BBX  2
  #define VU_DEF_FADE_BBX 2
  
  #define VU_DEF_HGT_STD  6
  #define VU_DEF_BARS_STD 18
  #define VU_DEF_GAP_STD  2
  #define VU_DEF_FADE_STD 2

// Smoothing (percent)
  #define VU_DEF_AUP_DEF  30  // 0.40
  #define VU_DEF_ADN_DEF  6  // 0.15
  #define VU_DEF_AUP_STR  30  // 0.25
  #define VU_DEF_ADN_STR  6  // 0.07
  #define VU_DEF_AUP_BBX  30  // 0.25
  #define VU_DEF_ADN_BBX  6  // 0.07
  #define VU_DEF_AUP_STD  24  // 0.10
  #define VU_DEF_ADN_STD  5  // 0.05

// Peak p_up / p_down (percent)
  #define VU_DEF_PUP_DEF  90  // 0.90
  #define VU_DEF_PDN_DEF   1  // 0.01
  #define VU_DEF_PUP_STR  90
  #define VU_DEF_PDN_STR   1
  #define VU_DEF_PUP_BBX  90
  #define VU_DEF_PDN_BBX   1
  #define VU_DEF_PUP_STD  90
  #define VU_DEF_PDN_STD   1

// Dymanics
  #define VU_DEF_EXPO_DEF   50  // 0.40
  #define VU_DEF_EXPO_STR  140  // 0.25
  #define VU_DEF_EXPO_BBX  140  // 0.25
  #define VU_DEF_EXPO_STD  130  // 0.10
  #define VU_DEF_FLOOR_DEF  95  // 0.40
  #define VU_DEF_FLOOR_STR  82  // 0.25
  #define VU_DEF_FLOOR_BBX  82  // 0.25
  #define VU_DEF_FLOOR_STD  20  // 0.10
  #define VU_DEF_CEIL_DEF  100  // 0.40
  #define VU_DEF_CEIL_STR  100  // 0.25
  #define VU_DEF_CEIL_BBX  100  // 0.25
  #define VU_DEF_CEIL_STD  100  // 0.10
  #define VU_DEF_GAIN_DEF   86  // 0.40
  #define VU_DEF_GAIN_STR  195  // 0.25
  #define VU_DEF_GAIN_BBX  195  // 0.25
  #define VU_DEF_GAIN_STD   90  // 0.10
  #define VU_DEF_KNEE_DEF   10  // 0.40
  #define VU_DEF_KNEE_STR    5  // 0.25
  #define VU_DEF_KNEE_BBX    5  // 0.25
  #define VU_DEF_KNEE_STD    3  // 0.10

// Band indexes
  #define VU_DEF_MID_PCT_DEF 60
  #define VU_DEF_MID_PCT_STR 60
  #define VU_DEF_MID_PCT_BBX 60
  #define VU_DEF_MID_PCT_STD 60

  #define VU_DEF_HIGH_PCT_DEF 80
  #define VU_DEF_HIGH_PCT_STR 80
  #define VU_DEF_HIGH_PCT_BBX 80
  #define VU_DEF_HIGH_PCT_STD 80

// Label heights
  #define VU_DEF_LHGT_DEF 10
  #define VU_DEF_LHGT_STR 15
  #define VU_DEF_LHGT_BBX 15
  #define VU_DEF_LHGT_STD 25
// --------------------------------------------------------------------------------------428x142
#elif DSP_MODEL==DSP_NV3007_142
  #define VU_DEF_HGT_DEF  35
  #define VU_DEF_BARS_DEF 10
  #define VU_DEF_GAP_DEF  2
  #define VU_DEF_FADE_DEF 2
  
  #define VU_DEF_HGT_STR  9
  #define VU_DEF_BARS_STR 24
  #define VU_DEF_GAP_STR  2
  #define VU_DEF_FADE_STR 5
  
  #define VU_DEF_HGT_BBX  9
  #define VU_DEF_BARS_BBX 24
  #define VU_DEF_GAP_BBX  2
  #define VU_DEF_FADE_BBX 5
  
  #define VU_DEF_HGT_STD  4
  #define VU_DEF_BARS_STD 36
  #define VU_DEF_GAP_STD  2
  #define VU_DEF_FADE_STD 3

// Smoothing (percent)
  #define VU_DEF_AUP_DEF  30  // 0.40
  #define VU_DEF_ADN_DEF  6  // 0.15
  #define VU_DEF_AUP_STR  30  // 0.25
  #define VU_DEF_ADN_STR  6  // 0.07
  #define VU_DEF_AUP_BBX  30  // 0.25
  #define VU_DEF_ADN_BBX  6  // 0.07
  #define VU_DEF_AUP_STD  24  // 0.10
  #define VU_DEF_ADN_STD  5  // 0.05

// Peak p_up / p_down (percent)
  #define VU_DEF_PUP_DEF  90  // 0.90
  #define VU_DEF_PDN_DEF   1  // 0.01
  #define VU_DEF_PUP_STR  90
  #define VU_DEF_PDN_STR   1
  #define VU_DEF_PUP_BBX  90
  #define VU_DEF_PDN_BBX   1
  #define VU_DEF_PUP_STD  90
  #define VU_DEF_PDN_STD   1

// Dymanics
  #define VU_DEF_EXPO_DEF   50  // 0.40
  #define VU_DEF_EXPO_STR  140  // 0.25
  #define VU_DEF_EXPO_BBX  100  // 0.25
  #define VU_DEF_EXPO_STD  100  // 0.10
  #define VU_DEF_FLOOR_DEF  95  // 0.40
  #define VU_DEF_FLOOR_STR  50  // 0.25
  #define VU_DEF_FLOOR_BBX  50  // 0.25
  #define VU_DEF_FLOOR_STD  30  // 0.10
  #define VU_DEF_CEIL_DEF  100  // 0.40
  #define VU_DEF_CEIL_STR   95  // 0.25
  #define VU_DEF_CEIL_BBX   95  // 0.25
  #define VU_DEF_CEIL_STD   95  // 0.10
  #define VU_DEF_GAIN_DEF   86  // 0.40
  #define VU_DEF_GAIN_STR  120  // 0.25
  #define VU_DEF_GAIN_BBX  110  // 0.25
  #define VU_DEF_GAIN_STD  100  // 0.10
  #define VU_DEF_KNEE_DEF   10  // 0.40
  #define VU_DEF_KNEE_STR   10  // 0.25
  #define VU_DEF_KNEE_BBX    5  // 0.25
  #define VU_DEF_KNEE_STD    3  // 0.10

// Band indexes
  #define VU_DEF_MID_PCT_DEF 60
  #define VU_DEF_MID_PCT_STR 60
  #define VU_DEF_MID_PCT_BBX 60
  #define VU_DEF_MID_PCT_STD 60

  #define VU_DEF_HIGH_PCT_DEF 80
  #define VU_DEF_HIGH_PCT_STR 80
  #define VU_DEF_HIGH_PCT_BBX 80
  #define VU_DEF_HIGH_PCT_STD 80

// Label heights
  #define VU_DEF_LHGT_DEF 10
  #define VU_DEF_LHGT_STR 20
  #define VU_DEF_LHGT_BBX 20
  #define VU_DEF_LHGT_STD 25
// --------------------------------------------------------------------------------------284x76
#elif DSP_MODEL==DSP_ST7789_76
  #define VU_DEF_HGT_DEF  7
  #define VU_DEF_BARS_DEF 17
  #define VU_DEF_GAP_DEF  1
  #define VU_DEF_FADE_DEF 2
  
  #define VU_DEF_HGT_STR  7
  #define VU_DEF_BARS_STR 17
  #define VU_DEF_GAP_STR  1
  #define VU_DEF_FADE_STR 2
  
  #define VU_DEF_HGT_BBX  7
  #define VU_DEF_BARS_BBX 17
  #define VU_DEF_GAP_BBX  1
  #define VU_DEF_FADE_BBX 2
  
  #define VU_DEF_HGT_STD  3
  #define VU_DEF_BARS_STD 34
  #define VU_DEF_GAP_STD  1
  #define VU_DEF_FADE_STD 2

// Smoothing (percent)
  #define VU_DEF_AUP_DEF  30  // 0.40
  #define VU_DEF_ADN_DEF  6  // 0.15
  #define VU_DEF_AUP_STR  30  // 0.25
  #define VU_DEF_ADN_STR  6  // 0.07
  #define VU_DEF_AUP_BBX  30  // 0.25
  #define VU_DEF_ADN_BBX  6  // 0.07
  #define VU_DEF_AUP_STD  24  // 0.10
  #define VU_DEF_ADN_STD  5  // 0.05

// Peak p_up / p_down (percent)
  #define VU_DEF_PUP_DEF  90  // 0.90
  #define VU_DEF_PDN_DEF   1  // 0.01
  #define VU_DEF_PUP_STR  90
  #define VU_DEF_PDN_STR   1
  #define VU_DEF_PUP_BBX  90
  #define VU_DEF_PDN_BBX   1
  #define VU_DEF_PUP_STD  90
  #define VU_DEF_PDN_STD   1

// Dymanics
  #define VU_DEF_EXPO_DEF  120  // 0.40
  #define VU_DEF_EXPO_STR  140  // 0.25
  #define VU_DEF_EXPO_BBX  140  // 0.25
  #define VU_DEF_EXPO_STD  130  // 0.10
  #define VU_DEF_FLOOR_DEF  35  // 0.40
  #define VU_DEF_FLOOR_STR  82  // 0.25
  #define VU_DEF_FLOOR_BBX  82  // 0.25
  #define VU_DEF_FLOOR_STD  20  // 0.10
  #define VU_DEF_CEIL_DEF  100  // 0.40
  #define VU_DEF_CEIL_STR  100  // 0.25
  #define VU_DEF_CEIL_BBX  100  // 0.25
  #define VU_DEF_CEIL_STD  100  // 0.10
  #define VU_DEF_GAIN_DEF  120  // 0.40
  #define VU_DEF_GAIN_STR  195  // 0.25
  #define VU_DEF_GAIN_BBX  195  // 0.25
  #define VU_DEF_GAIN_STD  90  // 0.10
  #define VU_DEF_KNEE_DEF  0  // 0.40
  #define VU_DEF_KNEE_STR  5  // 0.25
  #define VU_DEF_KNEE_BBX  5  // 0.25
  #define VU_DEF_KNEE_STD  3  // 0.10

// Band indexes
  #define VU_DEF_MID_PCT_DEF 60
  #define VU_DEF_MID_PCT_STR 60
  #define VU_DEF_MID_PCT_BBX 60
  #define VU_DEF_MID_PCT_STD 60

  #define VU_DEF_HIGH_PCT_DEF 80
  #define VU_DEF_HIGH_PCT_STR 80
  #define VU_DEF_HIGH_PCT_BBX 80
  #define VU_DEF_HIGH_PCT_STD 80

// Label heights
  #define VU_DEF_LHGT_DEF 18
  #define VU_DEF_LHGT_STR 20
  #define VU_DEF_LHGT_BBX 20
  #define VU_DEF_LHGT_STD 15
// --------------------------------------------------------------------------------------160x128
#elif DSP_MODEL==DSP_ST7735
  #define VU_DEF_HGT_DEF  20
  #define VU_DEF_BARS_DEF 8
  #define VU_DEF_GAP_DEF  1
  #define VU_DEF_FADE_DEF 2
  
  #define VU_DEF_HGT_STR  10
  #define VU_DEF_BARS_STR 13
  #define VU_DEF_GAP_STR  1
  #define VU_DEF_FADE_STR 2
  
  #define VU_DEF_HGT_BBX  10
  #define VU_DEF_BARS_BBX 13
  #define VU_DEF_GAP_BBX  1
  #define VU_DEF_FADE_BBX 2
  
  #define VU_DEF_HGT_STD  4
  #define VU_DEF_BARS_STD 15
  #define VU_DEF_GAP_STD  1
  #define VU_DEF_FADE_STD 2

// Smoothing (percent)
  #define VU_DEF_AUP_DEF  30  // 0.40
  #define VU_DEF_ADN_DEF  6  // 0.15
  #define VU_DEF_AUP_STR  30  // 0.25
  #define VU_DEF_ADN_STR  6  // 0.07
  #define VU_DEF_AUP_BBX  30  // 0.25
  #define VU_DEF_ADN_BBX  6  // 0.07
  #define VU_DEF_AUP_STD  24  // 0.10
  #define VU_DEF_ADN_STD  5  // 0.05

// Peak p_up / p_down (percent)
  #define VU_DEF_PUP_DEF  90  // 0.90
  #define VU_DEF_PDN_DEF   1  // 0.01
  #define VU_DEF_PUP_STR  90
  #define VU_DEF_PDN_STR   1
  #define VU_DEF_PUP_BBX  90
  #define VU_DEF_PDN_BBX   1
  #define VU_DEF_PUP_STD  90
  #define VU_DEF_PDN_STD   1

// Dymanics
  #define VU_DEF_EXPO_DEF  77  // 0.40
  #define VU_DEF_EXPO_STR  200  // 0.25
  #define VU_DEF_EXPO_BBX  200  // 0.25
  #define VU_DEF_EXPO_STD  135  // 0.10
  #define VU_DEF_FLOOR_DEF  88  // 0.40
  #define VU_DEF_FLOOR_STR  82  // 0.25
  #define VU_DEF_FLOOR_BBX  82  // 0.25
  #define VU_DEF_FLOOR_STD  55  // 0.10
  #define VU_DEF_CEIL_DEF  100  // 0.40
  #define VU_DEF_CEIL_STR  94  // 0.25
  #define VU_DEF_CEIL_BBX  94  // 0.25
  #define VU_DEF_CEIL_STD  95  // 0.10
  #define VU_DEF_GAIN_DEF  83  // 0.40
  #define VU_DEF_GAIN_STR  80  // 0.25
  #define VU_DEF_GAIN_BBX  80  // 0.25
  #define VU_DEF_GAIN_STD  80  // 0.10
  #define VU_DEF_KNEE_DEF  3  // 0.40
  #define VU_DEF_KNEE_STR  8  // 0.25
  #define VU_DEF_KNEE_BBX  3  // 0.25
  #define VU_DEF_KNEE_STD  4  // 0.10

// Band indexes
  #define VU_DEF_MID_PCT_DEF 60
  #define VU_DEF_MID_PCT_STR 60
  #define VU_DEF_MID_PCT_BBX 60
  #define VU_DEF_MID_PCT_STD 60

  #define VU_DEF_HIGH_PCT_DEF 80
  #define VU_DEF_HIGH_PCT_STR 80
  #define VU_DEF_HIGH_PCT_BBX 80
  #define VU_DEF_HIGH_PCT_STD 80

// Label heights
  #define VU_DEF_LHGT_DEF 10
  #define VU_DEF_LHGT_STR 11
  #define VU_DEF_LHGT_BBX 11
  #define VU_DEF_LHGT_STD 12
// Fallback
#else
  #define VU_DEF_BARS_DEF 32
  #define VU_DEF_GAP_DEF  2
  #define VU_DEF_BARS_STR 32
  #define VU_DEF_GAP_STR  2
  #define VU_DEF_BARS_BBX 24
  #define VU_DEF_GAP_BBX  2
  #define VU_DEF_BARS_STD 36
  #define VU_DEF_GAP_STD  2
#endif

#ifndef VU_DEF_MIDCOLOR
  #define VU_DEF_MIDCOLOR 0xFFE0; // yellow
#endif

#ifndef VU_DEF_PEAK_COLOR
  #define VU_DEF_PEAK_COLOR 0xFFFF  // white
#endif

