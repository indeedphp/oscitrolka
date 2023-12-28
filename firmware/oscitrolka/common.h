#ifndef COMMON_H_
#define COMMON_H_

//#ifdef ARDUINO_AVR_NANO
#ifdef __AVR__
  #warning common.h ARDUINO_AVR_NANO
  #define PIN_IN_KEY1    12
  #define PIN_IN_KEY2    10
  #define PIN_IN_KEY3    8
  #define PIN_IN_ACP     A7
  #define PIN_IN_FREQ    4
  #define PIN_OUT_GEN    9
  #define PIN_OUT_INTGEN 13

#endif

#ifdef ESP32
  #warning common.h ESP32
  #define PIN_IN_KEY1    12
  #define PIN_IN_KEY2    10
  #define PIN_IN_KEY3    8
  #define PIN_IN_ACP     A7
  #define PIN_IN_FREQ    4
  #define PIN_OUT_GEN    9
  #define PIN_OUT_INTGEN 13

#endif



#endif
