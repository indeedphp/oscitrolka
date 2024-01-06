#pragma once
#include <Arduino.h>
#include "configuration.h"
#include <Arduino.h>
#include "voltmeter.h"

#define KEY_DELAY_PERIOD           50 // Supress of key jitter with ms_delay

#define PIN_KEYPAD_KEY1             16 
#define PIN_KEYPAD_KEY2             17 
#define PIN_KEYPAD_KEY3             19
#define PIN_KEYPAD_KEY4             18
#define PIN_KEYPAD_KEYSHUNT         18

#define PIN_MC_A7                   13


#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  ((byte) & 0x80 ? '1' : '0'), \
  ((byte) & 0x40 ? '1' : '0'), \
  ((byte) & 0x20 ? '1' : '0'), \
  ((byte) & 0x10 ? '1' : '0'), \
  ((byte) & 0x08 ? '1' : '0'), \
  ((byte) & 0x04 ? '1' : '0'), \
  ((byte) & 0x02 ? '1' : '0'), \
  ((byte) & 0x01 ? '1' : '0') 
#define CRLF        "\r\n"

#define KEYSNONE_                           (0)
#define KEYS1ONLY_                          (1)
#define KEYS2ONLY_                          (2)
#define KEYS3ONLY_                          (4)
#define KEYS12_                             (KEYS1ONLY_ | KEYS2ONLY_)
#define KEYS23_                             (KEYS3ONLY_ | KEYS2ONLY_)

typedef struct {
    uint8_t key1:1;
    uint8_t key2:1;
    uint8_t key3:1;
    //uint8_t key4:1;
    uint8_t keyshunt:1;
} keys_t;

typedef union {   
    uint8_t as_int;
    keys_t as_keys;
} keypad_t;

static keypad_t Keypad, LastestKeypad;

extern Voltmetr voltmetr;

extern void control_init();

extern void control_loop();
