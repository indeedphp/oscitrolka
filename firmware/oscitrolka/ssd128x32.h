#ifndef SSD128X32_H_
#define SSD128X32_H_


#define USE_SSD1306 // Use I2C OLED screen on SSD1306 chipset
#include "U8g2lib.h"
#include <PWM.h>


void setup_display();
void loop_display_draw( const char * a_caption, const char * a_slider );
void drawScreen(uint32_t duration);
void oscilograf();
void can_lin_test();
void voltmetr();
void shastotomer();
void loop_display();

extern U8G2_SH1106_128X32_VISIONOX_F_HW_I2C u8g2;

/*
extern byte led = 9;                 // пин генератора
extern int frequency = 50;          // частота  на генераторе начальная
extern byte brightness = 25;         // частота ШИМ (0-255)
extern byte buffer[128];
extern unsigned long timer;
extern byte q = 1;
extern byte u = 0;
extern byte e = 0;
extern byte r = 0;
extern byte k = 15;     //  на генераторе частоту отображает
extern byte m = 5;      //  инфа на осцилографе
extern byte p = 1;      // счетчик кнопки 2
extern int w = 100;
extern int Htime = 0;   // переменная целого типа для хранения длительности высокого уровня
extern int Ltime = 0;   // переменная целого типа для хранения длительности низкого уровня
extern float Ttime;     // переменная для хранения длительности периода
extern float rtime;     // переменная для хранения длительности периода
extern int freq;        // переменная для хранения значения частоты
extern int pwm;         // переменная для хранения значения частоты
extern float proc;      // переменная для хранения значения частоты
*/

#endif
