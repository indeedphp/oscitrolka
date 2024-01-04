#pragma once
#include <Arduino.h>
#include "display_helper.h"

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#include <U8g2lib.h>

#ifdef S2MINI
// Nokia PCD8544 display
#define RST 2                       // Pin1 (RST)  GPIO2
#define CE 15                       // Pin2 (CE)  GPIO15
#define DC 4                        // Pin3 (DC)  GPIO4
#define DIN 17                      // Pin4 (Din)  GPIO17
#define CLK 18                      // Pin5 (Clk)  GPIO18
                                    // Pin6 (Vcc)  3.3V
#define DISPLAY_LED_PIN GPIO_NUM_10 // Pin7 (BL)
                                    // Pin8 (GND)  GND
#endif

#ifdef WROOM32
// Nokia PCD8544 display
#define RST GPIO_NUM_16 // Pin1 (RST)  GPIO2
#define CE GPIO_NUM_17  // Pin2 (CE)  GPIO15
#define DC GPIO_NUM_5   // Pin3 (DC)  GPIO4
#define DIN GPIO_NUM_18 // Pin4 (Din)  GPIO17
#define CLK GPIO_NUM_19 // Pin5 (Clk)  GPIO18
// #define DISPLAY_VCC GPIO_NUM_21                         // Pin6 (Vcc)  3.3V
#define DISPLAY_LED_PIN GPIO_NUM_21 
// Pin7 (BL)
                                    // Pin8 (GND)  GND
#endif

//U8G2_PCD8544_84X48_F_4W_SW_SPI u8g2 = U8G2_PCD8544_84X48_F_4W_SW_SPI(U8G2_R0, /* clock=*/GPIO_NUM_19, /* data=*/GPIO_NUM_18,
//                                    /* cs=*/GPIO_NUM_17, /* dc=*/GPIO_NUM_5, /* reset=*/GPIO_NUM_16);
U8G2 *u8g2;

void display_init()
{
    u8g2 = new  U8G2_PCD8544_84X48_F_4W_SW_SPI(U8G2_R0, /* clock=*/GPIO_NUM_19, /* data=*/GPIO_NUM_18,
                                    /* cs=*/GPIO_NUM_17, /* dc=*/GPIO_NUM_5, /* reset=*/GPIO_NUM_16);
    Serial.println("1");

    Serial.println("Width2: " + String(u8g2->getWidth()));

    //  Подсветка дисплея
    ledcSetup(3, 100, 8);
    ledcAttachPin(DISPLAY_LED_PIN, 3);
    ledcWrite(3, 150);

    u8g2->begin();
    u8g2->enableUTF8Print();
}