#ifndef SSD128X32_H_
#define SSD128X32_H_


#define USE_SSD1306 // Use I2C OLED screen on SSD1306 chipset
#include "U8g2lib.h"

void setup_display();
void loop_display_draw( const char * a_caption, const char * a_slider );

extern U8G2_SH1106_128X32_VISIONOX_F_HW_I2C u8g2;


#endif
