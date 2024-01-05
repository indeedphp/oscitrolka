#include <Arduino.h>
#include <U8g2lib.h>
#include "menu.h"
#include "board_s2mini.h"
#include "oscil.h"

extern U8G2 *u8g2;
extern int displayHeight;
extern int displayWidth;
extern int settingsVal; // 0 - Частота опроса, 1 - частота кадров, 2 - частота шима
int showVal = 0; //0 - средний вольтаж, 1 - пик ту пик, 
extern const float maxMeasureValue;
extern ulong framesForMenuTitleTimer; //Кол-во кадров с надписью, которое осталось показать
extern int pwmF;
extern esp_adc_cal_characteristics_t *adc_chars;
extern Oscilloscope oscil;

int sectionsCountH = 3;
int sectionHeight = displayHeight / sectionsCountH;

extern void drawOscilograf(int32_t buf[]);

extern void drawBack();

extern void drawValues(int32_t buf[]);



extern void display_currentmenu();

extern int menu_current_positon;

