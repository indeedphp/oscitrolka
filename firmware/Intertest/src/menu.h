#pragma once

#include<Arduino.h>

typedef  const char* const_char_pointer_t; 
#define MENU_LASTLIDX                   ( 6 -1)

extern const char* c_menu_err         ;
extern const char* c_menu_interview   ;
extern const char* c_menu_summ        ;
extern const char* c_menu_pwm         ;



extern const_char_pointer_t*  menu[MENU_LASTLIDX+1]; 

