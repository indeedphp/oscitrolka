// Версия от Watashi 
// 30.12.23 V1.0 ))))
// 
// Глубокая модернизация проекта версии 6 от @Avtosega телега "Автоэлектрика онлайн." https://t.me/+kgjzf6Xn2m0yOWIy 
// Еще не доделаный, сырой скетч. Что сделано:
// Изменена работа с меню, в обработке кнопок устранен дребезг + добавлена возможность короткого и длинного нажатия
// В вольтметре добавлено усреднение паказаний 
// Изменена библиотека работы с генератором частоты с PWM, на моем МК старая не работала
// Включение выключение подсветки 
// Не менялось пока, изменения только чтобы запускалось:
// осцилограф, кан лин тестер, частотомер

#define USE_SSD1306 // Use I2C OLED screen on SSD1306 chipset
#include "U8g2lib.h"
#include <Cl_do_btn_long.h>                        //библиотека одиночных кнопок
// настройка на мой МК ATmega32U4 дисплей и подключение кнопок
//#define WatashiON          // если у вас другой МК например Ардуино NANO надо закомментировать эту строку   
//
#ifndef WatashiON
  #include <PWM.h>
  U8G2_SH1106_128X32_VISIONOX_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);  //модель дисплея
  #define Batt1 12     // 12
  #define Batt2 10     // 10
  #define Batt3 8     // 
  #define PinVolt A7   // пин а7 вольтметр и осцилограф
  #define PinChast 4  // пин 4 ввод данных частотомер
  #define Gena 13     // пин 13 генератор для проверки осцила и частотомера
  #define LedP 7      // пин подсветки
  int frequency = 50;          // частота  на генераторе начальная
  byte dutyCycle = 25;         // частота ШИМ (0-255)
#else
  #include <AVR_PWM.h>
  U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);  // UNIVISION
  AVR_PWM* PWM_Instance;        //creates pwm instance
  #define Batt1 4     // 12
  #define Batt2 6     // 10
  #define Batt3 8     // 
  #define PinVolt 9  // пин а7 вольтметр и осцилограф
  #define PinChast 4  // пин 4 ввод данных частотомер
  #define Gena 5     // пин 13 генератор для проверки осцила и частотомера
  #define LedP 7      // пин подсветки
  #define _PWM_LOGLEVEL_  4
  float frequency = 250;
  int dutyCycle = 20;
#endif 

#define Voltmetr_TIME   250
Cl_do_btn_long Btn1(Batt1);                                 //передаём данные одиночных кнопок в библиотеку Cl_do_btn_long.h
Cl_do_btn_long Btn2(Batt2);                             //
Cl_do_btn_long Btn3(Batt3);                             //

//AVR_PWM* PWM_Instance;        //creates pwm instance

// float frequency = 250;
// int dutyCycle = 20;
int Oscbuf[128];             // буфер для осцилографа
unsigned long timer, nextRead1;
byte q = 0;     // номер состояния
byte u = 0;
byte e = 0;
byte r = 0;
byte k = 15;     //  на генераторе частоту отображает
byte m = 5;      //  инфа на осцилографе
byte p = 1;      // счетчик кнопки 2
int w = 100;
int Htime = 0;   // переменная целого типа для хранения длительности высокого уровня
int Ltime = 0;   // переменная целого типа для хранения длительности низкого уровня
float Ttime;     // переменная для хранения длительности периода
float rtime;     // переменная для хранения длительности периода
float CenaV = 52 / 1023.0;  // цена в вольтах одной единицы измерения
int freq;        // переменная для хранения значения частоты
int pwm;         // переменная для хранения значения частоты
float proc;      // переменная для хранения значения частоты
bool updateScreen, Ksel;
//
typedef enum REFLOW_STATE : byte {                //---структура статусов событий---// 
  STATE_IDLE,                        //экран в режиме выборa 
  STATE_voltmetr,                    //режим вольтметра 
  STATE_oscilograf,                  //режим осцилографа
  STATE_shastotomer,                 //режим частотометра
  STATE_generator,                   //режим генератора
  STATE_can_lin,                     //режим определения кан шины
  STATE_timer                        //режим таймера 
}
reflowState_t;
reflowState_t StateGl;                        //переменная статуса 
//
const char Mes0[] PROGMEM = "VOLTMETR";    // Каждое имя в своей строке, символ переноса не нужен
const char Mes1[] PROGMEM = "OSCILOGRAF";
const char Mes2[] PROGMEM = "CHASTOTOMER";
const char Mes3[] PROGMEM = "GENERATOR";
const char Mes4[] PROGMEM = "CAN LIN TEST";
const char Mes5[] PROGMEM = "TIMER";
const char* const names[] PROGMEM =     {      // Создаем массив ссылок, обращение к названию пунктов по их номеру
  Mes0, Mes1, Mes2, Mes3, Mes4, Mes5
  };
//-----------------------------
void Push1() {
  //Serial.println("Batt1");
  timer = millis();
  if (StateGl == STATE_IDLE) {
    q++;
    if (q >= 6) q = 0;
    updateScreen = true;
  } 
   else ChangeState(STATE_IDLE,1);
    /*  switch (StateGl) {
      case STATE_IDLE: { 
        q++;
        if (q == 6) q = 0;
        updateScreen = true;
      } break;
      case STATE_voltmetr: { 
        ChangeState(STATE_IDLE,1);
      } break;
      case STATE_oscilograf: { 
        ChangeState(STATE_IDLE,1);
      } break;

      case STATE_generator: { 
        ChangeState(STATE_IDLE,1);
      } break;
    } */
}
void Push2() {
  if (StateGl == STATE_generator) {
    if (Ksel) {
      if (dutyCycle < 90) dutyCycle = dutyCycle + 10;
        else dutyCycle = 100;
      }
     else {
      if (frequency < 2000) frequency = frequency + 50;
        else frequency = 2000;
      }
    
  }
}
void Push3() {
  if (StateGl == STATE_generator) {
    if (Ksel) { 
      if (dutyCycle >= 10) dutyCycle = dutyCycle - 10;
        else dutyCycle = 0;
    }
      else {
      if (frequency >= 250) frequency = frequency - 50;
        else frequency = 250;
      }
      
  }
}
void Push1Long() {
  //Serial.println("Batt1 Long");
  if (StateGl == STATE_IDLE && q == 0) digitalWrite(LedP, !digitalRead(LedP));
    else {
      q = 0;
      timer = millis();
      updateScreen = true;
    }
}
void Push2Long() {
  if (StateGl == STATE_generator) {
    Ksel = !Ksel;
    //updateScreen = true;
  }  
}
void Push3Long() {
}
void ChangeState(reflowState_t ChSt, boolean UpScr) {           //для обновления LCD в разных состояниях программы
  StateGl = ChSt;
  updateScreen = UpScr;
}
void printMenuItem(uint8_t num) {                 // Функция для печати строки из prm
  char buffer[14];                                // Буфер на полную строку

  uint16_t ptr = pgm_read_word(&(names[num]));    // Получаем указатель на первый символ строки
  uint8_t i = 0;                                  // Переменная - счетчик
  do {                                            // Начало цикла
    buffer[i] = (char)(pgm_read_byte(ptr++));     // Прочитать в буфер один символ из PGM и подвинуть указатель на 1
  } while (buffer[i++] != NULL);                  // Если это не конец строки - вернуться в начало цикла
  if (i>2) {
    for (i--; i<13; i++) buffer[i] = char(0x20);
    buffer[i] = NULL;
    }
  u8g2.print(buffer);                           // Вывод готовой строки с переносом на следующую
}
//--------------------------
void setup() {
  Serial.begin(9600);
  pinMode(PinVolt, INPUT);       // пин а7 вольтметр и осцилограф
  pinMode(PinChast, INPUT);      // пин 4 ввод данных частотомер
  pinMode(LedP, OUTPUT);            // пин 7 выход на подсветку
  digitalWrite(LedP, HIGH);
  pinMode(Gena, OUTPUT);           // пин 13 генератор для проверки осцила и частотомера
  u8g2.begin();                      // Инициализируем дисплей
  
  updateScreen = true;
  Ksel = true;
  StateGl = STATE_IDLE;
  #ifndef WatashiON
    InitTimersSafe();           // запускаем таймер
  #else
    PWM_Instance = new AVR_PWM(Gena, 20000, 0);
  #endif 
}
void loop() {
  //Serial.print("dutyCycle= ");  Serial.println(dutyCycle);
  Btn1.run(&Push1, &Push1Long);             //указываем обработчики для кнопки Btn1
  Btn2.run(&Push2, &Push2Long);             //указываем обработчики для кнопки Btn2
  Btn3.run(&Push3, &Push3Long);             //указываем обработчики для кнопки Btn3
 //-------------------------
  switch (StateGl) {
    case STATE_IDLE: {                         //главное меню
      if (updateScreen) {                           //Настройка экрана в режиме ожидания
        u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_10x20_mr);
        u8g2.setCursor(0, 13);
        printMenuItem(q);
        u8g2.setCursor(0, 32);
        for (uint8_t i=0; i<6; i++) {
          if (i == q) u8g2.print(" *");
            else u8g2.print(" .");
        }
        u8g2.sendBuffer();
        updateScreen = false;
        }
       //Serial.print("q= ");  Serial.println(q);
      if (millis() - timer > 2000)  {
        updateScreen = true;
        if (q == 0) StateGl = STATE_voltmetr;
        if (q == 1) StateGl = STATE_oscilograf;
        if (q == 2) StateGl = STATE_shastotomer;
        if (q == 3) StateGl = STATE_generator;
        if (q == 4) StateGl = STATE_can_lin;
      }
        
      } break;
    case STATE_voltmetr: {
        if (updateScreen) {                           //Настройка экрана в режиме ожидания
          updateScreen = false;
        }
        if (millis() > nextRead1) {                   //чтение и вывод на LCD температур, подготовка данных для ПК
          nextRead1 = millis() + Voltmetr_TIME; //устраняем колебания датчиков температуры
          voltmetr();
          u8g2.clearBuffer();
          u8g2.setFont(u8g2_font_10x20_mr);
          u8g2.setCursor(0, 13);
          u8g2.print("V");
          u8g2.setFont(u8g2_font_fub30_t_symbol);
          u8g2.setCursor(15, 32);
          u8g2.print(Ttime);
          u8g2.sendBuffer();
        }
      } break;
    case STATE_oscilograf: {
      if (updateScreen) {                           //Настройка экрана в режиме ожидания
        u8g2.clearBuffer();
        u8g2.sendBuffer();
        updateScreen = false;
      }
      if (millis() > nextRead1) {                   
        nextRead1 = millis() + Voltmetr_TIME; 
        oscilograf();
      }
      } break;   
    case STATE_shastotomer: {
      if (updateScreen) {                           //Настройка экрана в режиме ожидания
        u8g2.clearBuffer();
        u8g2.sendBuffer();
        updateScreen = false;
      }
      shastotomer();
      } break;   
    case STATE_generator: {
      if (updateScreen) {                           //Настройка экрана в режиме ожидания
        u8g2.clearBuffer();
        u8g2.sendBuffer();
        updateScreen = false;
      }
      u8g2.firstPage();
      u8g2.setCursor(0, 13);
      if (Ksel) u8g2.print(">");
        else  u8g2.print(" ");
      u8g2.print(dutyCycle);
      u8g2.print(F(" % PWM"));
      u8g2.setCursor(0, 32);
      if (!Ksel) u8g2.print(">");
        else  u8g2.print(" ");     
      u8g2.print(frequency);
      u8g2.print(F("   Hz"));
      u8g2.sendBuffer();
      //PWM_Instance->setPWM(Gena, frequency, dutyCycle);
      #ifndef WatashiON
        pwmWrite(Gena, dutyCycle);
        SetPinFrequency(Gena, frequency);
      #else      
        PWM_Instance->setPWM(Gena, frequency, dutyCycle);
      #endif 
      } break;
    case STATE_can_lin: {
      if (updateScreen) {                           //Настройка экрана в режиме ожидания
        u8g2.clearBuffer();
        u8g2.sendBuffer();
        updateScreen = false;
      }
      can_lin_test();
     } break;  
  } // end switch
} // end loop
//---------------------
void shastotomer() {  // функция частотомер
  Ttime = Htime + Ltime;
  rtime = Ttime - Ltime;
  proc =  Ttime / 100;
  pwm = rtime / proc;
  freq = 1000000 / Ttime; // вычисляем значение частоты учитывая что время у нас в микросекундах
  u8g2.firstPage();
  u8g2.setCursor(0, 13);
  u8g2.print("PWM  Hz     ");
  u8g2.setCursor(0, 32);
  u8g2.print(pwm);
  u8g2.print("   ");
  u8g2.print(freq);
  u8g2.sendBuffer();
  if (millis() - timer > 1000) {
    Htime = pulseIn(PinChast, HIGH);   // считываем длительность высокого уровня
    Ltime = pulseIn(PinChast, LOW);     // считываем длительность низкого уровня
    //timer = millis();
  }
}
//
void voltmetr() {                 // функция вольтметр
  Htime = analogRead(PinVolt);
  //Serial.print("PinVolt "); Serial.println(Htime);
  //if (Htime >= Ltime + 4 || Htime <= Ltime - 4) {  Ltime = Htime;  }
  if (Ttime == 0) Ttime = CenaV * Htime;
    else Ttime = Ttime*0.8 + 0.2*CenaV * Htime;
}
//
void can_lin_test() {  // кан лин тестер
  Htime = analogRead(PinVolt);
  //Serial.println(Htime);
  u8g2.firstPage();
  u8g2.setCursor(0, 13);
  u8g2.print("CAN LIN TEST");
  u8g2.setCursor(0, 32);
  if (Htime >= 20 && Htime <= 35  ) {
    u8g2.print("  < CAN L >     ");
    //delay(1000);
    u8g2.sendBuffer();
  }
  if (Htime >= 90 && Htime <= 105 ) {
    u8g2.print("  < CAN H >      ");
    u8g2.sendBuffer();
    //delay(1000);
  }
  if (Htime >= 150 && Htime <= 600 ) {
    u8g2.print("  < LIN >     ");
    u8g2.sendBuffer();
    //delay(1000);
  }
  if (Htime >= 0 && Htime <= 10 ) {
    u8g2.print("  < TEST >      ");
    u8g2.sendBuffer();
  }

}
//------------------------------
void oscilograf() {  // функция осцилограф
  uint32_t start = micros();
  for (uint8_t x = 0; x < 128; ++x) {  // 128 размер буфера
    Oscbuf[x] = map(analogRead(PinVolt), 0, w, u8g2.getDisplayHeight() - 1, 0); // а7 пин ацп
  }
  u8g2.firstPage();
  do {
    drawScreen(start);
  } while (u8g2.nextPage());
  r = 1;
  e = 3;
}
void drawScreen(uint32_t duration) {  // функция для работы осцилографа
  for (int8_t v = 5; v >= 0; --v) {
    uint8_t tickY = u8g2.getDisplayHeight() - u8g2.getDisplayHeight() * v / 5;
    for (uint8_t x = 0; x < u8g2.getDisplayWidth(); x += 8)
      u8g2.drawPixel(x, tickY);
  }
  for (uint8_t x = 1; x < 128; ++x) {  // 128 размер буфера
    u8g2.drawLine(x - 1, Oscbuf[x - 1], x, Oscbuf[x]);
  }
  u8g2.setCursor(0, 13);
  u8g2.print(m);
}