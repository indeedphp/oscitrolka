#pragma once

#include <Arduino.h>
#include "configuration.h"


#define PWM_RESOLUTION_         8
#define PWM_CH_                 4

extern uint16_t  pwm_freq;
extern uint8_t pwm_duty_cycle; 


extern void start_pwm(  uint16_t a_freq, uint8_t a_dirty_cycle  );
extern void stop_pwm( );
extern void pwm_once_start_stop( uint16_t a_period );
