//сырой скетч с меню вольтметром и осцилом и частотометром и генератором частоты с шим, добавлен нагрузочный резистор
// включаюшайся от кнопки номер 3 в режиме вольтметра еще добавлен кан лин тестер 6 версия
// почищена память, изменен вольтметр

#include <avr/pgmspace.h>

//#define USE_SSD1306 // Use I2C OLED screen on SSD1306 chipset
//#include "U8g2lib.h"


//#include <PWM.h>

#include "ssd128x32.h"


//U8G2_SH1106_128X32_VISIONOX_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);  //модель дисплея

/*
byte led = 9;                 // пин генератора
int frequency = 50;          // частота  на генераторе начальная
byte brightness = 25;         // частота ШИМ (0-255)
byte buffer[128];
unsigned long timer;
byte q = 1;
byte u = 0;
byte e = 0;
byte r = 0;
byte k = 15;     //  на генераторе частоту отображает
byte m = 5;      //  инфа на осцилографе
byte p = 1;      // счетчик кнопки 2
int w = 100;
int Htime = 0;   // переменная целого типа для хранения длительности высокого уровня
int Ltime = 0;   // переменная целого типа для хранения длительности низкого уровня
float Ttime;     // переменная для хранения длительности периода
float rtime;     // переменная для хранения длительности периода
int freq;        // переменная для хранения значения частоты
int pwm;         // переменная для хранения значения частоты
float proc;      // переменная для хранения значения частоты
*/

void setup() {
  InitTimersSafe();           // запускаем таймер
  pinMode(12, INPUT_PULLUP);  // 1 кнопка от юсб разьема
  pinMode(10, INPUT_PULLUP);  // 2 кнопка от юсб разьема
  pinMode(8, INPUT_PULLUP);   // 3 кнопка от юсб разьема
  pinMode(A7, INPUT);         // пин а7 вольтметр и осцилограф
  pinMode(4, INPUT);          // пин 4 ввод данных частотомер
  pinMode(9, OUTPUT);         // пин 9 выход на генератор
  pinMode(13, OUTPUT);        // пин 13 генератор для проверки осцила и частотомера
  
  //u8g2.begin();                      // Инициализируем дисплей
  //u8g2.setFont(u8g2_font_10x20_mr);  // Выставляем шрифт (шрифты жрут прорву памяти так что аккуратнее если меняете)   
  //u8g2.sendBuffer();                 // Отсылаем данные на дисплей
  setup_display();
}
/*
const PROGMEM char caption_voltmeter[]      = "VOLTMETER";
const PROGMEM char caption_oscilloscope[]   = "OSCILLOSCOPE";
const PROGMEM char caption_freq[]           = "FREQUENCE";
const PROGMEM char caption_generator[]      = "GENERATOR";
const PROGMEM char caption_canlintst[]      = "CAN LIN TEST";
const PROGMEM char caption_timer[]          = "TIMER";

const PROGMEM char slider_voltmeter[]       = "| . . . . .";
const PROGMEM char slider_oscilloscope[]    = ". | . . . .";
const PROGMEM char slider_freq[]            = ". . | . . .";
const PROGMEM char slider_generator[]       = ". . . | . .";
const PROGMEM char slider_canlintst[]       = ". . . . | .";
const PROGMEM char slider_timer[]           = ". . . . . |";

const PROGMEM char caption_clt_canl[]       = "  < CAN L >     ";
const PROGMEM char caption_clt_canh[]       = "  < CAN H >     ";
const PROGMEM char caption_clt_lin[]        = "  < LIN >       ";
const PROGMEM char caption_clt_test[]       = "  < TEST >      ";
const PROGMEM char caption_freq_pwnhz[]     = "PWM  Hz     ";
const PROGMEM char caption_freq_space[]     = "   ";
const PROGMEM char caption_gen_pwm[]        = "  PWM";
const PROGMEM char caption_gen_hz[]         = "  Hz";
const PROGMEM char caption_volt_v[]         = "V";
const PROGMEM char caption_volt_n[]         = "N";
*/
void loop() {
  
  loop_display();
  
}
