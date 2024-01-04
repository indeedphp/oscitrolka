#include "pwm_item.h"

void start_pwm(  uint16_t a_freq, uint8_t a_dirty_cycle ){

    Serial.printf_P( PSTR("PWM setup: freq=%d; "), a_freq);
    Serial.printf_P( PSTR("%%=%d;"), a_dirty_cycle);
    //PWM_RESOLUTION_ - диапазон изменения Duty Cycle - от 0 до 100%
    
    uint32_t duty = ( 1 << PWM_RESOLUTION_)*a_dirty_cycle/100;
    Serial.printf_P( PSTR(" dc=%d\r\n"), duty);
    ledcSetup(PWM_CH_, a_freq, PWM_RESOLUTION_);
    ledcAttachPin(PIN_PWM_MC_D9_, PWM_CH_);
    ledcWrite(PWM_CH_, a_dirty_cycle);
}

void stop_pwm( ){

    Serial.println( F("PWM stop"));
    ledcWrite( PWM_CH_, 0 );
    ledcDetachPin( PIN_PWM_MC_D9_ );

}
