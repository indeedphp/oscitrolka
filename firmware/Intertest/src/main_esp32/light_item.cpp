#include "light_item.h"
#include <Arduino.h>
#include "configuration.h"

bool Light_state = false;


void setup_light(){

    pinMode(PIN_MC_LIGHT_, OUTPUT);
}

void switch_light(){

    Light_state = ! Light_state;
    digitalWrite( PIN_MC_LIGHT_, Light_state);
}