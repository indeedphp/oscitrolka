#include <Arduino.h>
#include <U8g2lib.h>
#include "menu.h"

extern U8G2 *u8g2;
extern int displayHeight;
extern int displayWidth;
extern int settingsVal; // 0 - Частота опроса, 1 - частота кадров, 2 - частота шима
int showVal = 0; //0 - средний вольтаж, 1 - пик ту пик, 
extern const float maxMeasureValue;
extern ulong framesForMenuTitleTimer; //Кол-во кадров с надписью, которое осталось показать

int sectionsCountH = 3;
int sectionHeight = displayHeight / sectionsCountH;

/// @brief Отрисовать ориентиры и надписи
void drawBack()
{
  // Смена управления вращение инкодера
  if (millis() - framesForMenuTitleTimer < 1000)
  {
    String title = String(c_menu_err);
    switch (settingsVal)
    {
    case 0:
      title = String(c_menu_interview);
      break;

    case 1:
      title = String( c_menu_summ );
      break;

    case 2:
      title = String( c_menu_pwm );
      break;

    default:
      title = String(c_menu_err);
      break;
    }

    u8g2->setFont(u8g2_font_8x13_t_cyrillic);
    point_t pos = getDisplayCener(title, u8g2->getMaxCharWidth(), u8g2->getMaxCharHeight());
    u8g2->setCursor(pos.x, pos.y); // На середину
    u8g2->print(title);
  }

  // Отображение значения регулируемого энкодером
  u8g2->setCursor(20, displayHeight);
  u8g2->setFont(u8g2_font_ncenB08_tr);

  
  switch ( settingsVal )
  {
  case 0:
    uint64_t val;
    timer_get_alarm_value(TIMER_GROUP_0, TIMER_1, &val);
    u8g2->print(String(val));
    break;

  case 1:
    break;

  case 2:
    u8g2->print(String(pwmF));
    break;

  default:
    break;
  }

  // Отрисовка точек деления шкалы
  u8g2->setFont(u8g2_font_4x6_tr);
  for (int8_t v = 1; v <= sectionsCountH; ++v)
  {
    int tickY = displayHeight - ((displayHeight * v) / sectionsCountH);

    for (uint8_t x = 0; x < displayWidth - 1; x += 8)
    {
      int titlePos = displayWidth - 8;

      if (x >= titlePos)
      {
        u8g2->setCursor(titlePos, tickY + 10);
        u8g2->print(v);
      }

      u8g2->drawPixel(x, tickY);
    }
  }
}

/// @brief Отрисовать график
/// @param duration время старта
void drawValues(int32_t buf[])
{
  // Преобразованный предел
  static const int maxMeasureValNormalized = maxMeasureValue * 1000;

  // Оцилограмма
  for (uint8_t x = 0; x <= displayWidth; x++)
  {
    int realVolt = esp_adc_cal_raw_to_voltage(buf[x], adc_chars);
    int next = x == displayWidth ? 0 : buf[x + 1];

    // // Высчитывание среднего значения
    // mV = midArifm2(realVolt / 1000, displayWidth); // expRunningAverage(realVolt / 1000);
    byte val = map(realVolt, 0, maxMeasureValNormalized, displayHeight - 1, 0);

    if (x == displayWidth - 1)
    {
      u8g2->drawPixel(x, val);
    }
    else
    {
      byte val2 = map(esp_adc_cal_raw_to_voltage(next, adc_chars), 0, maxMeasureValNormalized , displayHeight - 1, 0);
      u8g2->drawLine(x, val, x + 1, val2);
    }
  }

  u8g2->setCursor(0, 12);
  u8g2->setFont(u8g2_font_ncenB08_tr);
  u8g2->print(oscil.getInterruptTime() / 1000.0);
}

// Отрисовка в режиме осцилографа
void drawOscilograf(int32_t buf[])
{
  u8g2->firstPage();
  drawBack();
  drawValues(buf);
  u8g2->nextPage();
}
