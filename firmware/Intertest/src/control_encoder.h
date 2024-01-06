#pragma once
#include "configuration.h"
#include "esp32-hal-gpio.h"
#include "hard_timer.h"
#include <EncButton.h>
#include "helpers.h"
#include "oscil.h"

#ifdef S2MINI
  // Энкодер
  #define ENC_VCC GPIO_NUM_38
  #define ENC_CLCK GPIO_NUM_37
  #define ENC_DT GPIO_NUM_39
  #define ENC_SW GPIO_NUM_40 // Кнопка
#endif

#ifdef WROOM32
  #define ENC_VCC GPIO_NUM_12
  #define ENC_CLCK GPIO_NUM_26
  #define ENC_DT GPIO_NUM_27
  #define ENC_SW GPIO_NUM_14 // Кнопка
#endif

extern int pwmF;
extern ulong framesForMenuTitleTimer;
extern int settingsVal;
extern Oscilloscope oscil;

EncButton enc(ENC_DT, ENC_CLCK, ENC_SW);

void encEvent();
bool IRAM_ATTR encTick(void *args);
HardTimer encoderTimer = HardTimer(encTick, TIMER_GROUP_1, TIMER_0, 600, 80);

void control_init()
{
    pinMode(ENC_VCC, OUTPUT);
    digitalWrite(ENC_VCC, 1);
    enc.attach(encEvent);
    
    encoderTimer.init();
}

void control_loop()
{
    enc.tick();
}

// Пока пришлось вернуть, чуть позже переделаю
void encEvent()
{
  // EB_PRESS - нажатие на кнопку
  // EB_HOLD - кнопка удержана
  // EB_STEP - импульсное удержание
  // EB_RELEASE - кнопка отпущена
  // EB_CLICK - одиночный клик
  // EB_CLICKS - сигнал о нескольких кликах
  // EB_TURN - поворот энкодера
  // EB_REL_HOLD - кнопка отпущена после удержания
  // EB_REL_HOLD_C - кнопка отпущена после удержания с предв. кликами
  // EB_REL_STEP - кнопка отпущена после степа
  // EB_REL_STEP_C - кнопка отпущена после степа с предв. кликами
  Serial.println("Enc action: " + String(enc.action()));
  switch (enc.action())
  {
  case EB_HOLD:
    settingsVal = range(settingsVal + enc.dir(), 0, 2, true);
    framesForMenuTitleTimer = millis();
    break;

  case EB_CLICK:
    Serial.println(oscil.getTimer().playPause() ? "Pause - Oscil" : "Play - Oscil");
    break;

  case EB_TURN:
    if (settingsVal == 0)
    {
      const int steep = 1;
      // uint64_t val = timerAlarmReadMicros(measureTimer) + steep * enc.dir();
      // timerAlarmWrite(measureTimer, val, true);
      // timer_pause(TIMER_GROUP_0, TIMER_1);
      // uint64_t alarm_value;
      // timer_get_alarm_value(TIMER_GROUP_0, TIMER_1, &alarm_value);
      // timer_set_alarm_value(TIMER_GROUP_0, TIMER_1, alarm_value + steep * enc.dir());
      // timer_start(TIMER_GROUP_0, TIMER_1);
      auto alarm_value = oscil.getTimer().getTickTime() + (enc.dir() * steep);
      oscil.getTimer().setNewTickTime(alarm_value);
      Serial.println("Frq accur: " + String(alarm_value));
    }
    else if (settingsVal == 1)
    {
      const int steep = 100;
      auto alarm_value = oscil.getTimer().getTickTime() + (enc.dir() * steep);
      oscil.getTimer().setNewTickTime(alarm_value);
      Serial.println("Frq fast: " + String(alarm_value));
    }
    else if (settingsVal == 2)
    {
      pwmF = range(pwmF + enc.dir() * 100, 0, 200000);
      ledcSetup(3, pwmF, 8);
      Serial.println("PWM: " + String(pwmF));
    }
    break;
  }
}

/// @brief Прерывание для обработки пропущенных считываний энкодера
/// @param args = NULL
/// @return nothing
bool IRAM_ATTR encTick(void *args)
{
#ifndef EXCLUDE_GIVER_
    enc.tickISR();
#endif
    return false;
}