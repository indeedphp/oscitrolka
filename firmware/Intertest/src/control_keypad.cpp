#include "control_keypad.h"
#include <Arduino.h>
#include "helpers.h"
#include "buzzer.h"
#include "interface_wide.h"
#include "menu.h"

#define BUZZ_DELAY_         100
extern int settingsVal;

#define DBGPRINTF_(x,y)   Serial.printf_P(PSTR( x ), y);


void IRAM_ATTR isr_key1fall() {
    digitalWrite( PIN_MK_BUZZ, MK_BUZZ_ACTIVE );
	Keypad.as_keys.key1 = ! digitalRead(PIN_KEYPAD_KEY1);
    digitalWrite( PIN_MK_BUZZ, MK_BUZZ_INACTIVE );
}

void IRAM_ATTR isr_key2fall() {
    digitalWrite( PIN_MK_BUZZ, MK_BUZZ_ACTIVE );
	Keypad.as_keys.key2 = ! digitalRead(PIN_KEYPAD_KEY2);
    digitalWrite( PIN_MK_BUZZ, MK_BUZZ_INACTIVE );
}


void IRAM_ATTR isr_key3fall() {
    digitalWrite( PIN_MK_BUZZ, MK_BUZZ_ACTIVE );
	Keypad.as_keys.key3 = ! digitalRead(PIN_KEYPAD_KEY3);
    digitalWrite( PIN_MK_BUZZ, MK_BUZZ_INACTIVE );
}



void control_init(){
    LastestKeypad.as_int = 0x0;
    Keypad.as_int = 0x0;
    //Serial.println(F("Keypad init"));
    pinMode(PIN_KEYPAD_KEY1, INPUT_PULLUP);
    attachInterrupt(PIN_KEYPAD_KEY1, isr_key1fall, CHANGE);
    //Serial.println(F("kp k1 r"));

    pinMode(PIN_KEYPAD_KEY2, INPUT_PULLUP);
    attachInterrupt(PIN_KEYPAD_KEY2, isr_key2fall, CHANGE);
    //Serial.println(F("kp k2 r"));

    pinMode(PIN_KEYPAD_KEY3, INPUT_PULLUP);
    attachInterrupt(PIN_KEYPAD_KEY3, isr_key3fall, CHANGE);
    //Serial.println(F("kp k3 r"));

    pinMode(PIN_KEYPAD_KEYSHUNT, INPUT_PULLUP);
}


void control_loop(){
    
    if ( Keypad.as_int != LastestKeypad.as_int)
    {

        //Serial.printf_P(PSTR("Keypad:L" BYTE_TO_BINARY_PATTERN ">:K"), BYTE_TO_BINARY(LastestKeypad.as_int) );
        //Serial.printf_P(PSTR(BYTE_TO_BINARY_PATTERN "\r\n"), BYTE_TO_BINARY(Keypad.as_int) );

        
        //Serial.printf_P(PSTR("isKey2pressed():%d\r\n"), isKey2pressed());
        /*
        Serial.printf_P(PSTR("isKey2PressedKey1freed():%d\r\n"), isKey2PressedKey1freed());
        Serial.printf_P(PSTR("isKey2PressedKey3freed():%d\r\n"), isKey2PressedKey3freed());

        Serial.printf_P(PSTR("isKey1freedWithoutKey2():%d\r\n"), isKey1freedWithoutKey2());
        Serial.printf_P(PSTR("isKey3freedWithoutKey2():%d\r\n"), isKey3freedWithoutKey2());


        Serial.printf_P(PSTR("1_menu_current_positon:%d\r\n"), menu_current_positon);
        */

        int mcp = menu_current_positon;

        if ( 
                (Keypad.as_int == KEYS1ONLY_)
              & (LastestKeypad.as_int == KEYSNONE_)
           ){
            menu_current_positon++;
            is_menu_selected = false;
            //DBGPRINTF_("+menu_current_positon\r\n",menu_current_positon);
        }


        if ( 
                (Keypad.as_int == KEYS3ONLY_)
              & (LastestKeypad.as_int == KEYSNONE_)
           ){
            menu_current_positon--;
            is_menu_selected = false;
            //DBGPRINTF_("-menu_current_positon\r\n",menu_current_positon);
        }

        if ( menu_current_positon > MENU_LASTLIDX_ )
            menu_current_positon = 0;
        if ( menu_current_positon < 0 )
            menu_current_positon = MENU_LASTLIDX_;

        //Serial.printf_P(PSTR("menu_current_positon:%d "), menu_current_positon);
        //Serial.printf_P(PSTR("mcp:%d\r\n"), mcp);
        if ( menu_current_positon != mcp ){ //the position was changed

            //Serial.println(F("menu_current_positon != mcp"));
            is_menu_selected = false;
            display_currentmenu();
        }

        //*****************************************************************

        if ( //check if Key2 pressed
                (Keypad.as_int == KEYSNONE_)
              & (LastestKeypad.as_int == KEYS2ONLY_)
           )
        {
            is_menu_selected = ! is_menu_selected;

            if ( ! is_menu_selected ) {

                Serial.print(F("sel:"));
                switch( menu_current_positon ){
                    case MENU_IDX_VOLT_:
                        //Serial.println(F("select VOLT"));
                        voltmetr.setAdcChars(adc_chars);
                        break;

                    case MENU_IDX_OSC_:
                        //Serial.println(F("select OSC"));
                        voltmetr.setAdcChars(adc_chars);
                        break;
                } //switch( menu_current_positon )
            } //if ( ! is_menu_selected)
            else {
                //Serial.println(F("not is_menu_selected"));
                display_currentmenu();
            }
        }//if Key2 pressed


        LastestKeypad.as_int = Keypad.as_int;

        
        
        //Serial.printf_P(PSTR("2_menu_current_positon:%d "), menu_current_positon);
        //Serial.printf_P(PSTR("is_menu_selected:%d\r\n"), is_menu_selected);
        

    
    }
    
}