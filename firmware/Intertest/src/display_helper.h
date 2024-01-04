#pragma once
#include <Arduino.h>
#include "display_structs.h"
#include <U8g2lib.h>

extern int displayHeight;
extern int displayWidth;

/// @brief Координаты для отображения надписи по центру дисплея
/// @param title Надпись
/// @return point с координатами
static point_t getDisplayCener(String title, int8_t maxCharWidth, int8_t maxCharHeight)
{
  int x = displayWidth / 2 - ((title.length() * maxCharWidth) / 4);
  int y = displayHeight / 2 + maxCharHeight / 4;

  return point_t{
      .x = x,
      .y = y,
  };
}

