/*
Author: JackFromBB - jack@boringbar.ru / 
Placement from: https://github.com/jackfrombb/
The library for ESP32 under Arduino Environment
*/
#pragma once

#include <Arduino.h>
#include "driver/timer.h"

struct hard_timer_info
{
    timer_group_t group;
    timer_idx_t num;
};

typedef struct {
    void* args;
} timer_args;

class HardTimer
{
private:
    hard_timer_info _timer_info;
    int _tickTime;
    uint32_t _divider;
    timer_isr_t _handlerFunc;
    void* _args;

    static hard_timer_info initTimer(timer_group_t group, timer_idx_t num,
                              int tickTime, uint32_t divider, timer_isr_t isr_handler, void* args)
    {
        const timer_config_t config = {
            .alarm_en = TIMER_ALARM_EN,
            .counter_en = TIMER_PAUSE,
            .intr_type = TIMER_INTR_LEVEL,
            .counter_dir = TIMER_COUNT_UP,
            .auto_reload = TIMER_AUTORELOAD_EN,
            .divider = divider,
        };

        timer_init(group, num, &config);
        timer_set_counter_value(group, num, 0);
        timer_set_alarm_value(group, num, tickTime);
        timer_isr_callback_add(group, num, isr_handler, args, 0);
        timer_enable_intr(group, num);
        timer_start(group, num);

            return hard_timer_info {
                .group = group,
                .num = num,
            };
    }

    bool _isOnPause;

public:
    HardTimer(){

    }
     
    HardTimer(bool(handlerFunc)(void* args) , timer_group_t group,
               timer_idx_t num, int tickTime, uint32_t divider)
    {
        _isOnPause = true;
        _timer_info = {.group = group, .num = num};
        _tickTime = tickTime;
        _divider = divider;
        _handlerFunc = handlerFunc;
    }

    // HardTimer(timer_isr_t handlerFunc, timer_group_t group,
    //            timer_idx_t num, int tickTime, uint32_t divider)
    // {
    //     _isOnPause = true;
    //     _timer_info = {.group = group, .num = num};
    //     _tickTime = tickTime;
    //     _divider = divider;
    //     _handlerFunc = handlerFunc;
        
    // }

    ~HardTimer() {

    }

    hard_timer_info init(){
        _timer_info = initTimer(_timer_info.group, _timer_info.num, _tickTime, _divider, _handlerFunc, _args);
        _isOnPause = false;
        return _timer_info;
    }

    void setArgs(void* args) {
        _args = args;
    }

    bool playPause()
    {
        _isOnPause = !_isOnPause;
        if (_isOnPause)
        {
            timer_pause(_timer_info.group, _timer_info.num);
        }
        else
            timer_start(_timer_info.group, _timer_info.num);

            return _isOnPause;
    }

    void start(){
        timer_start(_timer_info.group, _timer_info.num);
    }

    void pause(){
        timer_pause(_timer_info.group, _timer_info.num);
    }

    /// @brief Получить время срабатывания
    /// @param  Время срабатывания (не преобразованное)
    u_int64_t getTickTime(){
        u_int64_t rValue;
         timer_get_alarm_value(_timer_info.group, _timer_info.num, &rValue);
         return rValue;
    }

    void setNewTickTime(u_int64_t t_time)
    {
        timer_pause(_timer_info.group, _timer_info.num);
        timer_set_alarm_value(_timer_info.group, _timer_info.num, t_time);
        timer_start(_timer_info.group, _timer_info.num);
    }

    hard_timer_info getInfo()
    {
        return _timer_info;
    }
};
