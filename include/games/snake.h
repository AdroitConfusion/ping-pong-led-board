#ifndef SNAKE_H
#define SNAKE_H

#include "draw.h"

namespace Snake {
// Methods
void snake();
void initSnake();
void checkController();
void moveSnake();
void checkSnake();
void changeDelayAndColor();
void respawnApple();
void changeSnakeRainbowColor();
void checkSnakeSelfCollision();
} // namespace Snake

#endif