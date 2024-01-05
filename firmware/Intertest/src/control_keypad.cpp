#include "control_keypad.h"
#include <Arduino.h>
#include "helpers.h"
#include "buzzer.h"

#define BUZZ_DELAY_         100
extern int settingsVal;


void IRAM_ATTR isr_key1fall() {
    digitalWrite( PIN_MK_BUZZ, MK_BUZZ_ACTIVE );
    //LastestKeypad = Keypad;
	Keypad.as_keys.key1 = ! digitalRead(PIN_KEYPAD_KEY1);
    //Serial.printf_P(PSTR("key1 %d\r\n"), Keypad.as_keys.key1);

    digitalWrite( PIN_MK_BUZZ, MK_BUZZ_INACTIVE );
}

void IRAM_ATTR isr_key2fall() {
    digitalWrite( PIN_MK_BUZZ, MK_BUZZ_ACTIVE );
    //LastestKeypad = Keypad;
	Keypad.as_keys.key2 = ! digitalRead(PIN_KEYPAD_KEY2);
    //Serial.printf_P(PSTR("key2 %d\r\n"), Keypad.as_keys.key2);
    digitalWrite( PIN_MK_BUZZ, MK_BUZZ_INACTIVE );
}


void IRAM_ATTR isr_key3fall() {
    digitalWrite( PIN_MK_BUZZ, MK_BUZZ_ACTIVE );
     //LastestKeypad = Keypad;
	Keypad.as_keys.key3 = ! digitalRead(PIN_KEYPAD_KEY3);
    //Serial.printf_P(PSTR("key3 %d\r\n"), Keypad.as_keys.key3);
    digitalWrite( PIN_MK_BUZZ, MK_BUZZ_INACTIVE );
}



void control_init(){
    LastestKeypad.as_int = 0x7;
    Keypad.as_int = 0x7;
    Serial.println(F("Keypad init"));
    pinMode(PIN_KEYPAD_KEY1, INPUT_PULLUP);
    attachInterrupt(PIN_KEYPAD_KEY1, isr_key1fall, CHANGE);
    Serial.println(F("kp k1 r"));

    pinMode(PIN_KEYPAD_KEY2, INPUT_PULLUP);
    attachInterrupt(PIN_KEYPAD_KEY2, isr_key2fall, CHANGE);
    Serial.println(F("kp k2 r"));

    pinMode(PIN_KEYPAD_KEY3, INPUT_PULLUP);
    attachInterrupt(PIN_KEYPAD_KEY3, isr_key3fall, CHANGE);
    Serial.println(F("kp k3 r"));

    pinMode(PIN_KEYPAD_KEYSHUNT, INPUT_PULLUP);
}

bool isKey2pressed(){
    return (Keypad.as_keys.key2) & (! LastestKeypad.as_keys.key2);
}

bool isKey2freed(){
    return ( Keypad.as_keys.key2) & ( LastestKeypad.as_keys.key2 );
}
bool isKey2PressedKey1freed(){
    //кнопка К2 нажата, а К1 совобождается
    return isKey2pressed() & (  ! Keypad.as_keys.key1  ) & ( LastestKeypad.as_keys.key1 );
}

bool isKey2PressedKey3freed(){
    //кнопка К2 нажата, а К3 совобождается
    return isKey2pressed() & (  ! Keypad.as_keys.key3  ) & ( LastestKeypad.as_keys.key3 );
}

bool isKey1freedWithoutKey2(){
    //кнопка К2 ненажата, а К1 совобождается
    return (! isKey2pressed() ) & (  ! Keypad.as_keys.key1  ) & ( LastestKeypad.as_keys.key1 );
}

bool isKey3freedWithoutKey2(){
    //кнопка К2 ненажата, а К3 совобождается
    return (! isKey2pressed() ) & (  ! Keypad.as_keys.key3  ) & ( LastestKeypad.as_keys.key3 );
}

inline void keypad_invoke_menu_item(const int a_menuItem){

}


void control_loop(){
    
    if ( Keypad.as_int != LastestKeypad.as_int){

        Serial.printf_P(PSTR("Keypad:%x>:"), LastestKeypad.as_int );
        Serial.printf_P(PSTR("%x\r\n"), Keypad.as_int );

        LastestKeypad.as_int = Keypad.as_int;

    }
}