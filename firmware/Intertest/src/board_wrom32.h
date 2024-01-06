#pragma once
#ifndef ESP_BOARD_
#define ESP_BOARD_

#include <Arduino.h>

//esp32 библиотеки для работы ADC
#include "driver/adc.h"
#include "esp_adc_cal.h"

extern uint32_t board_readAnalogVal();

extern void board_init(esp_adc_cal_characteristics_t *adc_chars);

#endif