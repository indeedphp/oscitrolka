#include "ssd128x32.h"
#ifdef __AVR__
  #include <avr/pgmspace.h>
  
  #include <PWM.h>
    //https://forum.arduino.cc/t/pwm-frequency-library/114988
    //InitTimersSafe()  Same as InitTimers() except timer 0 is not initialized in order to preserve time keeping functions
    //pwmWrite(uint8_t pin, uint8_t val)  Same as 'analogWrite()', but it only works with initialized timers. Continue to use analogWrite() on uninitialized timers
    //SetPinFrequency(int8_t pin, int32_t frequency)  Sets the pin's frequency (in Hz) and returns a bool for success
#endif

#ifdef ESP32
#include <Esp.h>
#endif

#include "common.h"

U8G2_SH1106_128X32_VISIONOX_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);  //модель дисплея


byte led = 9;                // пин генератора
int frequency = 50;          // частота  на генераторе начальная
byte brightness = 25;        // частота ШИМ (0-255)
byte buffer[128];
unsigned long timer;
byte q = 1;
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
int freq;        // переменная для хранения значения частоты
int pwm;         // переменная для хранения значения частоты
float proc;      // переменная для хранения значения частоты




const PROGMEM char caption_voltmeter[]      = "VOLTMETER";
const PROGMEM char caption_oscilloscope[]   = "OSCILLOSCOPE";
const PROGMEM char caption_freq[]           = "FREQUENCE";
const PROGMEM char caption_generator[]      = "GENERATOR";
const PROGMEM char caption_canlintst[]      = "CAN LIN TEST";
const PROGMEM char caption_timer[]          = "TIMER";

const PROGMEM char slider_voltmeter[]       = "| . . . . .";
const PROGMEM char slider_oscilloscope[]    = ". | . . . .";
const PROGMEM char slider_freq[]            = ". . | . . .";
const PROGMEM char slider_generator[]       = ". . . | . .";
const PROGMEM char slider_canlintst[]       = ". . . . | .";
const PROGMEM char slider_timer[]           = ". . . . . |";

const PROGMEM char caption_clt_canl[]       = "  < CAN L >     ";
const PROGMEM char caption_clt_canh[]       = "  < CAN H >     ";
const PROGMEM char caption_clt_lin[]        = "  < LIN >       ";
const PROGMEM char caption_clt_test[]       = "  < TEST >      ";
const PROGMEM char caption_freq_pwnhz[]     = "PWM  Hz     ";
const PROGMEM char caption_freq_space[]     = "   ";
const PROGMEM char caption_gen_pwm[]        = "  PWM";
const PROGMEM char caption_gen_hz[]         = "  Hz";
const PROGMEM char caption_volt_v[]         = "V";
const PROGMEM char caption_volt_n[]         = "N";


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

void drawScreen(uint32_t duration) {  // функция для работы осцилографа
  for (int8_t v = 5; v >= 0; --v) {
    uint8_t tickY = u8g2.getDisplayHeight() - u8g2.getDisplayHeight() * v / 5;
    for (uint8_t x = 0; x < u8g2.getDisplayWidth(); x += 8)
      u8g2.drawPixel(x, tickY);
  }
  for (uint8_t x = 1; x < 128; ++x) {  // 128 размер буфера
    u8g2.drawLine(x - 1, buffer[x - 1], x, buffer[x]);
  }
  u8g2.setCursor(0, 13);
  u8g2.print(m);
}

void oscilograf() {  // функция осцилограф
  uint32_t start = micros();
  for (uint8_t x = 0; x < 128; ++x) {  // 128 размер буфера
    buffer[x] = map(analogRead(A7), 0, w, u8g2.getDisplayHeight() - 1, 0); // а7 пин ацп
  }
  u8g2.firstPage();
  do {
    drawScreen(start);
  } while (u8g2.nextPage());
  r = 1;
  e = 3;
}

void can_lin_test() {  // кан лин тестер
  Htime = analogRead(A7);
  Serial.println(Htime);
  u8g2.firstPage();
  u8g2.setCursor(0, 13);
  u8g2.print( caption_canlintst );
  u8g2.setCursor(0, 32);
  if (Htime >= 20 && Htime <= 35  ) {
    u8g2.print(caption_clt_canl);
    delay(1000);
    u8g2.sendBuffer();
  }
  if (Htime >= 90 && Htime <= 105 ) {
    u8g2.print( caption_clt_canh );
    u8g2.sendBuffer();
    delay(1000);
  }
  if (Htime >= 150 && Htime <= 600 ) {
    u8g2.print( caption_clt_lin );
    u8g2.sendBuffer();
    delay(1000);
  }
  if (Htime >= 0 && Htime <= 10 ) {
    u8g2.print( caption_clt_test );
    u8g2.sendBuffer();
  }
  r = 1;
  e = 6;
}

void voltmetr() { // функция вольтметр
  Htime = analogRead(A7);
  if (Htime >= Ltime + 4 || Htime <= Ltime - 4) {
    Ltime = Htime;
  }
  Ttime = (52 / 1023.0) * Ltime;
  u8g2.firstPage();
  u8g2.setFont(u8g2_font_10x20_mr);
  u8g2.setCursor(0, 13);
  if (digitalRead(8) == 0 && e == 2){
    u8g2.print( caption_volt_v );
    u8g2.setCursor(0, 32);
    u8g2.print( caption_volt_n );
  }
  else u8g2.print( caption_volt_v );
  u8g2.setFont(u8g2_font_fub30_t_symbol);
  u8g2.setCursor(15, 32);
  u8g2.print(Ttime);
  u8g2.sendBuffer();
  if (Ltime <= 5)Ltime = 0;
  r = 1;
  e = 2;
}

void shastotomer() {  // функция частотомер
  Ttime = Htime + Ltime;
  rtime = Ttime - Ltime;
  proc =  Ttime / 100;
  pwm = rtime / proc;
  freq = 1000000 / Ttime; // вычисляем значение частоты учитывая что время у нас в микросекундах
  u8g2.firstPage();
  u8g2.setCursor(0, 13);
  u8g2.print( caption_freq_pwnhz );
  u8g2.setCursor(0, 32);
  u8g2.print(pwm);
  u8g2.print( caption_freq_space );
  u8g2.print(freq);
  u8g2.sendBuffer();
  if (millis() - timer > 1000) {
    Htime = pulseIn(4, HIGH);   // считываем длительность высокого уровня
    Ltime = pulseIn(4, LOW);     // считываем длительность низкого уровня
    timer = millis();
  }
  r = 1;
  e = 4;
}
///////////////////////////////
void generator() {  // функция генератор
  pwmWrite(led, brightness);     //0-255   используйте эту функцию вместо analogWrite
  u8g2.firstPage();
  u8g2.setCursor(0, 13);
  u8g2.print(k);
  u8g2.print( caption_gen_pwm );
  u8g2.setCursor(0, 32);
  u8g2.print(frequency);
  u8g2.print( caption_gen_hz );
  u8g2.sendBuffer();
  pwmWrite(led, brightness);
  SetPinFrequency(led, frequency);
  r = 1;
  e = 5;
}

void loop_display(){
  tone(13, 40, 50000); // выводим тестовый сгнал частоты на 13 пин 100 герц , шим 50%, длительность 50секунд
  if (digitalRead(12) == 0) { // 1 кнопка от юсб разьема обработка нажатия
    u8g2.setFont(u8g2_font_10x20_mr);
    pwmWrite(led, 0);
    SetPinFrequency(led, 0);
    q++;
    u = 0;
    r = 0;
    e = 0;
    timer = millis();
    delay(500);
  }
  if (digitalRead(10) == 0) { // 2 кнопка от юсб разьема обработка нажатия
    p++;
    tone(13, 100, 500);
    if (p >= 6) {
      p = 1;
    }
    if (p == 1) {
      w = 100;
      m = 5;
      brightness = 25;  // 10% PWM
      k = 10;
    }
    if (p == 2  && e == 3) {  // пропускаем шаг настройки для осцилографа
      p = 3;
    }
    if (p == 2) {
      m = 5;
      w = 100;
      brightness = 89; // 35% PWM
      k = 35;
    }
    if (p == 3) {
      m = 15;
      w = 300;
      brightness = 127;  // 50% PWM
      k = 50;
    }
    if (p == 4) {
      m = 25;
      w = 500;
      brightness = 191;  // 75% PWM
      k = 75;
    }
    if (p == 5) {
      m = 35;
      w = 700;
      brightness = 229;  // 90% PWM
      k = 90;
    }
    delay(500);
  }
  if (e == 5 && digitalRead(8) == 0) { // 3 кнопка от юсб разьема обработка нажатия
    frequency = frequency + 50;
    if (frequency >= 1050)frequency = 50;
    delay(500);
  }
  if (e == 2 && digitalRead(8) == 0) { // 3 кнопка от юсб разьема включаем нагрузочный резистор
    digitalWrite(9, HIGH);
    delay(500);
  }
  if (e == 2 && digitalRead(8) == 1) { // 3 кнопка от юсб разьема выключаем нагрузочный резистор
    digitalWrite(9, LOW);
  }

  if (u == 0 && q == 1 ) {
    loop_display_draw(caption_voltmeter, slider_voltmeter);
    timer = millis();
    u = 1;
    r = 0;
  }
  if (u == 0 && q == 2 ) {
    loop_display_draw(caption_freq, slider_freq);
    timer = millis();
    u = 1;
    r = 0;
  }
  if ( u == 0 && q == 3  ) {
    loop_display_draw(caption_freq, slider_freq);

    timer = millis();
    u = 1;
    r = 0;
  }
  if ( u == 0 && q == 4  ) {
    loop_display_draw(caption_generator, slider_generator);
    timer = millis();
    u = 1;
    r = 0;
  }
  if ( u == 0 && q == 5  ) {
    loop_display_draw(caption_canlintst, slider_canlintst);
    timer = millis();
    u = 1;
    r = 0;
  }
  if ( u == 0 && q == 6  ) {
    loop_display_draw(caption_timer, slider_timer);
    timer = millis();
    u = 1;
  }
  if (q >= 7) {
    q = 1;
  }
  ///////////////////////////////////////////////////////////////////////////
  if (r == 1 && e == 2)  {  // короткий путь до вольтметра
    voltmetr();
  }
  if (millis() - timer > 3000 && q == 1 && r == 0)  { // если выбран вольтметр
    u8g2.clearBuffer();
    u8g2.sendBuffer();
    timer = millis();
    voltmetr();
  }
  ////////////////
  if (r == 1 && e == 3)  {  // короткий путь до осцилограф
    oscilograf();
  }
  if (millis() - timer > 3000 && q == 2 && r == 0)  { // если выбран осцилограф
    u8g2.clearBuffer();
    u8g2.sendBuffer();
    timer = millis();
    oscilograf();
  }
  ////////////////
  if (r == 1 && e == 4)  {  // короткий путь до частотомер
    shastotomer();
  }
  if (millis() - timer > 3000 && q == 3 && r == 0)  { // если выбран частотомер
    u8g2.clearBuffer();
    u8g2.sendBuffer();
    timer = millis();
    shastotomer();
  }
  ////////////////
  if (r == 1 && e == 5)  {  // короткий путь до генератор
    generator();
  }
  if (millis() - timer > 3000 && q == 4 && r == 0)  { // если выбран генератор
    u8g2.clearBuffer();
    u8g2.sendBuffer();
    timer = millis();
    generator();
  }
  ////////////////
  if (r == 1 && e == 6)  {  // короткий путь до кан лин тест
    can_lin_test();
  }
  if (millis() - timer > 3000 && q == 5 && r == 0)  { // если выбран кан лин тест
    u8g2.clearBuffer();
    u8g2.sendBuffer();
    timer = millis();
    can_lin_test();
  }  
}
