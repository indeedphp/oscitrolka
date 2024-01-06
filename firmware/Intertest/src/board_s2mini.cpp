#include "board_s2mini.h"
#include <Arduino.h>

//esp32 библиотеки для работы ADC
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include <hal/adc_types.h>

#ifndef ADC_WIDTH_BIT_13
//    #define ADC_WIDTH_BIT_13    4
#endif

#ifndef ADC1_CHANNEL_8
//    #define ADC1_CHANNEL_8  8
#endif

#if defined( S2MINI )
uint32_t board_readAnalogVal()
{
  return adc1_get_raw(ADC1_CHANNEL_0);
}

void board_init(esp_adc_cal_characteristics_t *adc_chars){
  // Сохраняем характеристики АЦП для последующих преобразований
  adc_chars = (esp_adc_cal_characteristics_t *)calloc(1, sizeof(esp_adc_cal_characteristics_t));

  adc1_config_width(ADC_WIDTH_BIT_13);
  adc1_config_channel_atten(ADC1_CHANNEL_8, ADC_ATTEN_DB_11);
  esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_11db, ADC_WIDTH_BIT_13, 1.1, adc_chars);
}

#endif