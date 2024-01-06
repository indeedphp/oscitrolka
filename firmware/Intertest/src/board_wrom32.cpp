#include "board_wrom32.h"

#include <Arduino.h>

//esp32 библиотеки для работы ADC
#include "driver/adc.h"
#include "esp_adc_cal.h"



  #define BRD_ADC_CH_       ADC1_CHANNEL_0


uint32_t board_readAnalogVal()
{

  return adc1_get_raw(BRD_ADC_CH_);
}

void board_init(esp_adc_cal_characteristics_t *adc_chars){

  // Конфигурация и настройка АЦП(ADC)
  // Указываем разрядность, канал и аттенюацию (ADC_ATTEN_DB_11 должен уменьшать макс напряжение до 2.5v)

  adc1_config_width(ADC_WIDTH_12Bit);
  adc1_config_channel_atten(BRD_ADC_CH_, ADC_ATTEN_DB_11);
  esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_11db, ADC_WIDTH_12Bit, 1.1, adc_chars);
 if ( val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
    Serial.println(F("EFuse VRef"));
  } else
    if ( val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
      Serial.println(F("EFuse Two point"));
    } else 
      Serial.println(F("EFuse default"));


}