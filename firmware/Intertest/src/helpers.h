/* **********************************************
Author: JackFromBB - jack@boringbar.ru / 
Placement from: https://github.com/jackfrombb/
The library for ESP32 under Arduino Environment
************************************************ */
#pragma once
#include <stdio.h>
#include <Arduino.h>

extern int range(int input, int min, int max, bool infin = false)
;

extern void copy(int *src, int *dst, int len);

extern float mK;// = 0.4; // коэффициент фильтрации, 0.0-1.0
/// бегущее среднее
extern float expRunningAverage(float newVal)
;

// Обычное среднее
extern float midArifm2(float newVal, float measureSize)
;