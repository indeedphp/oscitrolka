#include <Arduino.h>
#include <U8g2lib.h>
#include "menu.h"
#include "display_structs.h"
#include "display_helper.h"

const char* c_menu_err                  PROGMEM = "VOLTMETER";
const char* c_menu_interview            PROGMEM = "ОПРОС";
const char* c_menu_summ                 PROGMEM = "ЗУМ";
const char* c_menu_pwm                  PROGMEM = "ШИМ";


//const char* c_menu_pwm                  PROGMEM = "ШИМ";

const_char_pointer_t c_menu_volt                 PROGMEM = "Volt"; 
const_char_pointer_t c_menu_osc                  PROGMEM = "OSC"; 
const_char_pointer_t c_menu_pwmn                 PROGMEM = "PWM"; 
const_char_pointer_t c_menu_shunt                PROGMEM = "SHUNT"; 
const_char_pointer_t c_menu_light                PROGMEM = "Light";
const_char_pointer_t c_menu_info                 PROGMEM = "Info"; 
const_char_pointer_t c_menu_upd                  PROGMEM = "Update";

const_char_pointer_t*  menu[MENU_LASTLIDX_+1] = {&c_menu_volt, &c_menu_osc, &c_menu_pwmn, &c_menu_shunt, &c_menu_light, &c_menu_info, &c_menu_upd}; 

int sub_menu = 0;
bool is_menu_selected = false;


extern U8G2 * u8g2;
extern int displayHeight;
extern int displayWidth;


