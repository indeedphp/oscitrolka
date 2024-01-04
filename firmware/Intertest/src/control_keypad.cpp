#include "control_keypad.h"
#include <Arduino.h>
#include "helpers.h"

extern int settingsVal;

void control_init(){
    Keypad.as_int = 0;

    pinMode(PIN_KEYPAD_KEY1, INPUT_PULLUP);
    pinMode(PIN_KEYPAD_KEY2, INPUT_PULLUP);
    pinMode(PIN_KEYPAD_KEY3, INPUT_PULLUP);
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
    keypad_t tmp;
    tmp.as_int = 0;
    tmp.as_keys.key1 = digitalRead( PIN_KEYPAD_KEY1 );
    tmp.as_keys.key2 = digitalRead( PIN_KEYPAD_KEY2 );
    tmp.as_keys.key3 = digitalRead( PIN_KEYPAD_KEY3 );
    tmp.as_keys.keyshunt = 0;//digitalRead( PIN_KEYPAD_KEYSHUNT );

    if ( tmp.as_int != Keypad.as_int ){

        Serial.printf_P(PSTR("Keypad:%d>:"), Keypad.as_int );
        Serial.println( tmp.as_int );
        LastestKeypad.as_int = Keypad.as_int;
        Keypad.as_int = tmp.as_int;

        if ( isKey1freedWithoutKey2() )
            settingsVal = range(settingsVal + 1, 0, 2, true);

        if ( isKey1freedWithoutKey2() )
            settingsVal = range(settingsVal - 1, 0, 2, true);

        if ( isKey2freed() )
          keypad_invoke_menu_item( settingsVal );

        delay( KEY_DELAY_PERIOD );
    }
}