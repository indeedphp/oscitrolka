//сырой скетч с меню вольтметром и осцилом и частотометром и генератором частоты с шим, добавлен нагрузочный резистор
// включаюшайся от кнопки номер 3 в режиме вольтметра еще добавлен кан лин тестер 6 версия
// почищена память, изменен вольтметр

#include "ssd128x32.h"
#include "common.h"

void setup() {
  InitTimersSafe();                     // запускаем таймер
  pinMode(PIN_IN_KEY1, INPUT_PULLUP);   // 1 кнопка от юсб разьема
  pinMode(PIN_IN_KEY2, INPUT_PULLUP);   // 2 кнопка от юсб разьема
  pinMode(PIN_IN_KEY3, INPUT_PULLUP);   // 3 кнопка от юсб разьема
  pinMode(PIN_IN_ACP, INPUT);           // пин а7 вольтметр и осцилограф
  pinMode(PIN_IN_FREQ, INPUT);          // пин 4 ввод данных частотомер
  pinMode(PIN_OUT_GEN, OUTPUT);         // пин 9 выход на генератор
  pinMode(PIN_OUT_INTGEN, OUTPUT);      // пин 13 генератор для проверки осцила и частотомера
  
  setup_display();
}

void loop() {
  
  loop_display();
  
}
