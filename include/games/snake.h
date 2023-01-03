#ifndef SNAKE_H
#define SNAKE_H

#include "draw.h"

//Methods
void snake(const int& joy_x, const int& joy_y, const bool& butt_z, const bool& butt_c, bool& new_game);
void initSnake(bool& newGame);
void checkController(const int& joy_x, const int& joy_y);
void moveSnake();
void checkSnake();
void changeDelayAndColor();

#endif