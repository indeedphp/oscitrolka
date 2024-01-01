/*Cl_do_btn_long_3mode.cpp
*/
#include "Arduino.h"
#include "Cl_do_btn_long.h"
Cl_do_btn_long::Cl_do_btn_long(byte _pin) {
  
  //Setup the input pins
  pinMode(pin, INPUT_PULLUP);
  digitalWrite(pin, HIGH);   // turn on pullup resistor
 	
  pin = _pin;
}

void Cl_do_btn_long::run(void (* press)(), void (* longPress)()) {
  if (! bounce && digitalRead(pin) != btn) { // если прошел фронт изм на выводe
    bounce = 1;                                 // выставить флаг
    time_Pressed = millis();                        // сделать временую засветку
  }
  if (bounce && millis() - time_Pressed >= 40) { // если прошло антидребезговое время
    bounce = 0;      // то снять флаг
    oldBtn = btn ;
    btn = digitalRead(pin) ; // прочитать реальное значение на выводе
    if (!btn && oldBtn) { // если нажата кнопка
      button_state = 1;
      last_state = millis();
    }
    if (!oldBtn && btn && button_state && millis() - last_state < 500  ) {
      button_state = 0;
      press();// короткое нажатие
    }
  }
  if (button_state && millis() - last_state >= 500) {
    button_state = 0;
    longPress();//длиное нажатие
  }
}
//-- создание исполняемых модулей
