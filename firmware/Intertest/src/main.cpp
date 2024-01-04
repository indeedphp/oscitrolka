// ver: 0_0_j (номер релиза_любой номер комита_автор)

#include <Arduino.h>

// Все настройки железа здесь
#include "configuration.h"

// Основной буфер.
#define BUFFER_LENGTH 168

// Вспомогательные методы
#include "helpers.h"
// Вспомогательные структуры дисплея
#include "display_structs.h"

#ifndef EXCLUDE_OSCIL_
// Логика осцилографа
#include "oscil.h"
#endif
#include "voltmeter.h"
#ifndef EXCLUDE_HARDTIMER_
// Логика тамера прерываний
#include "hard_timer.h"
#endif

// esp32 библиотеки для работы ADC
#include "driver/adc.h"
#include "esp_adc_cal.h"

// Пищалка
#ifdef BUZZ
#include "buzzer.h"
#endif

// Определение контроллера
#ifdef S2MINI
#include "board_s2mini.h"
#endif

#ifdef WROOM32
#include "board_wrom32.h"
#endif

// Определение дисплея
// Nokia PCD8544 display
#ifdef NOKIA5110_
#include "display_nokia_5110.h"
// дисплей 0.96 OLED I2C
#elif defined(OLED128x32_)
#include "display_128x32.h"
#endif

// Сохраняем параметры дисплея
int displayHeight = 0;
int displayWidth = 0;
bool interfaceDrawInProcess = false; // Флаг начала прорисовки интерфейса

// Хранение характеристик ADC
esp_adc_cal_characteristics_t *adc_chars;

Oscilloscope oscil = Oscilloscope(&board_readAnalogVal, 450); // board_readAnalogVal - определяется в файле board_***.h
Voltmetr voltmetr = Voltmetr();

int settingsVal = 0;               // 0 - Частота опроса, 1 - частота кадров, 2 - частота шима
const float maxMeasureValue = 3.2; // Потолок по напряжению, если ниже 3.0 то ломается. Больше можно
ulong framesForMenuTitleTimer = 0; // Счетчик кадров для отображения названия меню, его увеличивает control, а отслеживает interface

// Частота генерации
int pwmF = 1000;

#ifdef ENCODER
#include "control_encoder.h"
#elif defined( KEYPAD )
#include "control_keypad.h"
#endif

#include "display_helper.h"

// Nokia PCD8544 display
#ifdef NOKIA5110_
#include "interface_wide.h"
// дисплей 0.96 OLED I2C
#elif defined(OLED128x32_)
#include "interface_wide.h"
#endif

void setup()
{

  delay(100);
  const float vRef = 1.1; // Опрное напряжение. Для esp32 всегда 1.1. Вынес для удобства

  Serial.begin(9600);
  delay(1000);

  display_init();
  displayHeight = u8g2->getHeight();
  displayWidth = u8g2->getWidth();

  u8g2->setFont(u8g2_font_10x20_t_cyrillic); // Выставляем шрифт (шрифты жрут прорву памяти так что аккуратнее если меняете)
  String hello = "Привет";
  point_t pHello = getDisplayCener(hello, u8g2->getMaxCharWidth(), u8g2->getBufferTileHeight());
  u8g2->setCursor(pHello.x, pHello.y);
  u8g2->print(hello);
  u8g2->sendBuffer();

  // Сохраняем характеристики АЦП для последующих преобразований
  adc_chars = (esp_adc_cal_characteristics_t *)calloc(1, sizeof(esp_adc_cal_characteristics_t));
  board_init(adc_chars);

  // Конец настройки АЦП
  delay(300);
  
  #if defined( KEYPAD )
    control_init();
  #endif

#ifdef BUZZ
  setup_buzzer();
#endif

  // Настройка шим - временный костыль для проверки АЦП, позже вынесем в отдельный класс генератора
  ledcSetup(2, pwmF, 8);
  ledcAttachPin(GPIO_NUM_4, 2);
  ledcWrite(2, 254 / 2);

  delay(1000);

  oscil.init();
  voltmetr.setAdcChars(adc_chars);
}

void loop()
{
   #if defined( KEYPAD )
     control_loop();
  #endif
  
  // Если буфер готов то начинаем прорисовку
  if (oscil.isBufferReady())
  {
    drawOscilograf(oscil.getBuffer());
    oscil.readNext();
  }
}