/* **********************************************
Author: JackFromBB - jack@boringbar.ru /
Placement from: https://github.com/jackfrombb/
The library for ESP32 under Arduino Environment
************************************************ */
#pragma once

#ifndef BUFFER_LENNGH
#define BUFFER_LENGTH 168
#endif

#include <Arduino.h>
#include "hard_timer.h"

/// @brief Делегирование функции считывания, для облегчения совместимости между платами
typedef uint32_t (*value_read_func)();

class Oscilloscope
{
private:
public:
    bool _bufferReady = false;
    // static bool IRAM_ATTR timerInterrupt(void *args);
    const int bufferLength = BUFFER_LENGTH;
    int32_t _buffer[BUFFER_LENGTH];
    int _measureTime;
    int _lastPos = 0;
    int32_t _lastValue = -1;
    value_read_func _readValue;
    ulong _interruptTime = 0;
    HardTimer oscilTimer;

    // bool IRAM_ATTR oscillTimerInterrupt(void *args);
    Oscilloscope() {} // Для неинициализированных объектов
    Oscilloscope(value_read_func readValue, int measureTime)
    {
        _readValue = readValue;
        _measureTime = measureTime;
    }
    // oscil(int32_t &buffer, int bufferLength, uint32_t (readValue)(void), int *measureTime);
    ~Oscilloscope()
    {
    }

    ulong prevInterTime = 0; // Предыдущее время тика
    int missTick = 0;        // Подсчитываем пропущеные тики
    int synchTick = 0;       // Пропускаем для синхронизауии записи в буффер

    bool triggerSynk = true;
    int triggerSynchTrys = BUFFER_LENGTH / 2;
    static bool IRAM_ATTR timerInterrupt(void *args)
    {
        Oscilloscope *oscil = (Oscilloscope *)args;

        // Если буфер готов то пропускаем заполнение
        // if (oscil->_bufferReady)
        // {
        //     oscil->missTick += 1;
        //     return false;
        // }

        // if (oscil->missTick > 0)
        // {
        //     oscil->synchTick = oscil->missTick % oscil->bufferLength;
        //     oscil->missTick = 0;
        // }

        // if (oscil->synchTick > 0)
        // {
        //     oscil->synchTick -= 1;
        //     return false;
        // }

        oscil->_interruptTime = micros() - oscil->prevInterTime;

        // Измерение
        uint32_t reading = oscil->_readValue(); // adc1_get_raw(ADC1_CHANNEL_2);

        oscil->_buffer[oscil->_lastPos] = reading;

        if (oscil->_lastPos == BUFFER_LENGTH - 1)
        {
            oscil->_lastPos = 0;
            oscil->_bufferReady = true;
        }
        else
        {
            oscil->_lastPos += 1;
        }

        oscil->prevInterTime = micros();

        return false;
    }

    ulong getInterruptTime()
    {
        return _interruptTime;
    }

    int32_t *getBuffer()
    {
        return _buffer;
    }

    bool isBufferReady()
    {
        return _bufferReady;
    }

    void readNext()
    {
        _bufferReady = false;
    }

    void init()
    {
        oscilTimer = HardTimer(timerInterrupt, TIMER_GROUP_0, TIMER_1, 4500, 2);
        oscilTimer.setArgs(this);
        oscilTimer.init();
    }

    HardTimer getTimer()
    {
        return oscilTimer;
    }
};