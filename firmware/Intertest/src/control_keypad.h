#pragma once
#include <Arduino.h>
#include "configuration.h"
#include <Arduino.h>

#define KEY_DELAY_PERIOD           400 // Supress of key jitter with ms_delay

#define PIN_KEYPAD_KEY1             16 
#define PIN_KEYPAD_KEY2             17 
#define PIN_KEYPAD_KEY3             19
#define PIN_KEYPAD_KEY4             18
#define PIN_KEYPAD_KEYSHUNT         18

#define PIN_MC_A7                   13

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

extern void control_init();

extern void control_loop();
