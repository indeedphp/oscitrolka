#pragma once
#include <Arduino.h>
#include "display_helper.h"
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
#include <Wire.h>

#define SSD1306_SETPRECHARGE 0xD9
#define SSD1306_SETCONTRAST 0x81

// дисплей 0.96 OLED I2C
#define DSP_SDA_ 26
#define DSP_SCK_ 27

#define DISPU8X8_

// pinMode( DSP_SCK_, OUTPUT );
// pinMode( DSP_SDA_, OUTPUT );
// U8G2_SH1106_128X32_VISIONOX_F_HW_I2C loc_dsp(U8G2_R0, U8X8_PIN_NONE, DSP_SCK_, DSP_SDA_);
//U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, DSP_SCK_, DSP_SDA_, U8X8_PIN_NONE);
U8G2 * u8g2;

void display_init()
{
    #define I2C_FREQ    (1200000)
    u8g2 = new U8G2_SSD1306_128X64_NONAME_F_SW_I2C(U8G2_R0, DSP_SCK_, DSP_SDA_, U8X8_PIN_NONE);
    Wire.begin(DSP_SDA_, DSP_SCK_, I2C_FREQ);
    u8g2->setBusClock(I2C_FREQ);
    u8g2->begin();
    u8g2->setBusClock(I2C_FREQ);


    //u8g2->sendF("caaaaaaaaaaaaaaaa", 0xb8, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 180);
    //half u8g2->sendF("caaaaaaaaaaaaaaaa", 0xb8, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 90);
    //u8g2->sendF("caaaaaaaaaaaaaaaa", 0xb8, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 45);
    //u8g2->setContrast(255);
    //u8g2->setPowerSave(0);
    u8g2->enableUTF8Print();
}
