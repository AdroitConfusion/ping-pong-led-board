#pragma once

#include "draw.h"

//Methods
void snake(const int& joy_x, const int& joy_y, const bool& butt_z, const bool& butt_c);
void initSnake(bool& newGame);
void checkSnakeDirection(const int& joy_x, const int& joy_y);
void moveSnake();
void checkSnake();
void changeDelayAndColor();