#include <Arduino.h>
#include <U8g2lib.h>
#include "menu.h"
#ifdef S2MINI
    //#include "board_s2mini.h"
#endif
#ifdef WROOM32
    #include "board_wrom32.h"
#endif
#include "oscil.h"
#include "esp_adc_cal.h"

extern U8G2 *u8g2;
extern int displayHeight;
extern int displayWidth;
extern int settingsVal; // 0 - Частота опроса, 1 - частота кадров, 2 - частота шима
extern int showVal; //0 - средний вольтаж, 1 - пик ту пик, 
extern float maxMeasureValue;
extern ulong framesForMenuTitleTimer; //Кол-во кадров с надписью, которое осталось показать
extern int pwmF;
extern esp_adc_cal_characteristics_t *adc_chars;
extern Oscilloscope oscil;

extern int sectionsCountH ;
extern int sectionHeight ;

extern void drawOscilograf(int32_t buf[]);

extern void drawBack();

extern void drawValues(int32_t buf[]);



extern void display_currentmenu();

extern int menu_current_positon;

