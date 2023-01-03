#ifndef DRAW_H
#define DRAW_H

#include <FastLED.h>
#include <NintendoExtensionCtrl.h>

#include "letters.h"

#define WIDTH 30
#define HEIGHT 30
#define NUM_LEDS WIDTH * HEIGHT

extern CRGB leds_plus_safety_pixel[NUM_LEDS + 1];
extern CRGB* const leds;

extern Nunchuk nchuk;

extern int idle;


int convertXY(int x, int y);
void fill(int color);
void drawArray(const int arr[][2], int x, int y, int arrSize, int color);

void snakeScreen();

#endif