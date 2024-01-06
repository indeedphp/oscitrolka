#include "control_keypad.h"
#include <Arduino.h>
#include "helpers.h"
#include "buzzer.h"
#include "interface_wide.h"
#include "menu.h"
#include "pwm_item.h"
#include "light_item.h"


#define BUZZ_DELAY_         100
extern int settingsVal;

#define DBGPRINTF_(x,y)   Serial.printf_P(PSTR( x ), y);


void IRAM_ATTR isr_key1change() {
    //digitalWrite( PIN_MK_BUZZ, MK_BUZZ_ACTIVE );
	Keypad.keys.as_keys.key1 = ! digitalRead(PIN_KEYPAD_KEY1);
    Keypad.time = millis();
    //digitalWrite( PIN_MK_BUZZ, MK_BUZZ_INACTIVE );
}

void IRAM_ATTR isr_key2change() {
    //digitalWrite( PIN_MK_BUZZ, MK_BUZZ_ACTIVE );
	Keypad.keys.as_keys.key2 = ! digitalRead(PIN_KEYPAD_KEY2);
    Keypad.time = millis();
    //digitalWrite( PIN_MK_BUZZ, MK_BUZZ_INACTIVE );
}


void IRAM_ATTR isr_key3change() {
    //digitalWrite( PIN_MK_BUZZ, MK_BUZZ_ACTIVE );
	Keypad.keys.as_keys.key3 = ! digitalRead(PIN_KEYPAD_KEY3);
    Keypad.time = millis();
    //digitalWrite( PIN_MK_BUZZ, MK_BUZZ_INACTIVE );
}



void control_init(){
    LastestKeypad.keys.as_int = 0x0;
    Keypad.keys.as_int = 0x0;
    //Serial.println(F("Keypad init"));
    pinMode(PIN_KEYPAD_KEY1, INPUT_PULLUP);
    attachInterrupt(PIN_KEYPAD_KEY1, isr_key1change, CHANGE);
    //Serial.println(F("kp k1 r"));

    pinMode(PIN_KEYPAD_KEY2, INPUT_PULLUP);
    attachInterrupt(PIN_KEYPAD_KEY2, isr_key2change, CHANGE);
    //Serial.println(F("kp k2 r"));

    pinMode(PIN_KEYPAD_KEY3, INPUT_PULLUP);
    attachInterrupt(PIN_KEYPAD_KEY3, isr_key3change, CHANGE);
    //Serial.println(F("kp k3 r"));

    pinMode(PIN_KEYPAD_KEYSHUNT, INPUT_PULLUP);
}


void control_loop(){
    
    if ( Keypad.keys.as_int != LastestKeypad.keys.as_int)
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
                (Keypad.keys.as_int == KEYS1ONLY_)
              & (LastestKeypad.keys.as_int == KEYSNONE_)
           ){
            menu_current_positon++;
            is_menu_selected = false;
            //DBGPRINTF_("+menu_current_positon\r\n",menu_current_positon);
        }


        if ( 
                (Keypad.keys.as_int == KEYS3ONLY_)
              & (LastestKeypad.keys.as_int == KEYSNONE_)
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
            stop_pwm();
            display_currentmenu();
        }

        //*****************************************************************

        if ( //check if Key2 pressed
                (Keypad.keys.as_int == KEYSNONE_)
              & (LastestKeypad.keys.as_int == KEYS2ONLY_)
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

                    case MENU_IDX_PWM_:
                        //Serial.println(F("select PWM"));
                        start_pwm( pwm_freq, pwm_duty_cycle );
                        break;

                    case MENU_IDX_SHUNT_:
                        //Serial.println(F("select SHUNT"));
                        is_menu_selected = false;
                        pwm_once_start_stop( SHUNT_PWM_PULSE_DELAY );
                        break;    

                    case MENU_IDX_LIGHT_:
                        //Serial.println(F("select LIGHT"));
                        switch_light();
                        is_menu_selected = false;
                        break;    
                } //switch( menu_current_positon )
            } //if ( ! is_menu_selected)
            else {
                //Serial.println(F("not is_menu_selected"));
                display_currentmenu();
            }
        }//if Key2 pressed


        LastestKeypad.keys.as_int = Keypad.keys.as_int;

        
        
        //Serial.printf_P(PSTR("2_menu_current_positon:%d "), menu_current_positon);
        //Serial.printf_P(PSTR("is_menu_selected:%d\r\n"), is_menu_selected);
        

    
    }
    
}