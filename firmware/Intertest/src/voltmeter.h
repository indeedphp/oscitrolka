#pragma once

#include <Arduino.h>
#include "helpers.h"
// esp32 библиотеки для работы ADC
#include "driver/adc.h"
#include "esp_adc_cal.h"

class Voltmetr {
    private:
    esp_adc_cal_characteristics_t* _adc_chars;

    public:
    Voltmetr(){
    }
    ~Voltmetr(){
    }

    void setAdcChars(esp_adc_cal_characteristics_t*  adc_chars){
        _adc_chars = adc_chars;
    }

    float measureMax(int32_t* buffer){
        int32_t returnVal;
        for(int i=0 ; i<BUFFER_LENGTH; i++){
            int32_t rawV = esp_adc_cal_raw_to_voltage(buffer[i], _adc_chars);
            returnVal = returnVal > rawV ? returnVal : rawV;
        }

        return returnVal/1000.0;
    }

    float measureMidVolt(int32_t* buffer){
        int size = sizeof(buffer);
        int32_t returnVal;
        for(int i; i<size; i++){
            float rawV = esp_adc_cal_raw_to_voltage(buffer[i], _adc_chars);
            returnVal = midArifm2(buffer[i], size);
        }
        return returnVal;
    }

};