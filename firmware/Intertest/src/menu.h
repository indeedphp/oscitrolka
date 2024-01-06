#pragma once

#include<Arduino.h>

typedef  const char* const_char_pointer_t; 
#define MENU_LASTLIDX_                   ( 7 -1)

#define MENU_IDX_VOLT_                   0
#define MENU_IDX_OSC_                    1
#define MENU_IDX_PWM_                    2
#define MENU_IDX_SHUNT_                  3
#define MENU_IDX_LIGHT_                  4
#define MENU_IDX_INFO_                   5
#define MENU_IDX_UPD_                    6

extern const char* c_menu_err         ;
extern const char* c_menu_interview   ;
extern const char* c_menu_summ        ;
extern const char* c_menu_pwm         ;

//extern int sub_menu;
//extern bool is_menu_selected;
extern int sub_menu;
extern bool is_menu_selected;



extern const_char_pointer_t*  menu[MENU_LASTLIDX_+1]; 

