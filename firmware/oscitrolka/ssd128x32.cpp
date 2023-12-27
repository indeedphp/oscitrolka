#include "ssd128x32.h"


U8G2_SH1106_128X32_VISIONOX_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);  //модель дисплея

void setup_display(){

  u8g2.begin();                      // Инициализируем дисплей
  u8g2.setFont(u8g2_font_10x20_mr);  // Выставляем шрифт (шрифты жрут прорву памяти так что аккуратнее если меняете)   
  u8g2.sendBuffer();                 // Отсылаем данные на дисплей
  
}

void loop_display_draw( const char * a_caption, const char * a_slider ){
  
    u8g2.clearBuffer();
    u8g2.setCursor(0, 13);
    u8g2.print(a_caption);
    u8g2.setCursor(0, 32);
    u8g2.print(a_slider);
    u8g2.sendBuffer();

  
}
