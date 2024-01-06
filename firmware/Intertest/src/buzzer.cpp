/* **********************************************
Author: JackFromBB - jack@boringbar.ru / 
Placement from: https://github.com/jackfrombb/
The library for ESP32 under Arduino Environment

In cooperatiion with svdpfaf (svddevelop@gmail.com)

Communication with encoder. Depended with the library from Alex Giver.

************************************************ */

#include <Arduino.h>
#include "buzzer.h"
#include "configuration.h"

#ifdef BUZZ_
void setup_buzzer(){
    pinMode(PIN_MK_BUZZ, OUTPUT);
    digitalWrite( PIN_MK_BUZZ, MK_BUZZ_INACTIVE );
    Serial.println(F("setup Buzz"));
}

void do_buzz( int a_period ){
    digitalWrite( PIN_MK_BUZZ, MK_BUZZ_ACTIVE );
    Serial.printf_P(PSTR("buzz:%d\r\n"), a_period );
    delay( a_period );
    digitalWrite( PIN_MK_BUZZ, MK_BUZZ_INACTIVE );
}
#endif