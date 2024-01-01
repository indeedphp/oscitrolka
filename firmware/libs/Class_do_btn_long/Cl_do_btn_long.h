/*Cl_do_btn_long.h
*/
#ifndef Cl_do_btn_long_h
#define Cl_do_btn_long_h

#include "Arduino.h"
class Cl_do_btn_long {
  public:
    Cl_do_btn_long(byte _pin);
    void run(void (* press)(), void (* longPress)());
  protected:
    byte pin ;
    bool btn = 1, oldBtn;
    bool bounce = 0; // антидребезговый флаг
    uint32_t time_Pressed = 0 ;
    bool button_state = 0;
    uint32_t last_state = 0 ;
};
//-- создание исполняемых модулей
#endif //Cl_do_btn_long_h
