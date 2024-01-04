/* **********************************************
Author: JackFromBB - jack@boringbar.ru / 
Placement from: https://github.com/jackfrombb/
The library for ESP32 under Arduino Environment

In cooperatiion with svdpfaf (svddevelop@gmail.com)

defines для изменения конфигурации железа

************************************************ */
#pragma once

//#define EXCLUDE_OSCIL_ 1 
//#define EXCLUDE_HARDTIMER_ 1

//Выбор платы
#define WROOM32
//#define S2MINI

//Выбор способа упрвления
//#define ENCODER
#ifndef ENCODER
   #define KEYPAD
#endif

//Доп функции
#define BUZZ_

// Nokia PCD8544 display
//#define NOKIA5110_
//#define NOKIA5110_
//дисплей 0.96 OLED I2C
#define OLED128x32_
#define OLED128x32_

#ifdef BUZZ
#define PIN_MK_BUZZ                 21
#define MK_BUZZ_ACTIVE              LOW
#define MK_BUZZ_INACTIVE            HIGH
#endif