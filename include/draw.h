#ifndef DRAW_H
#define DRAW_H

#include <FastLED.h>
#include <NintendoExtensionCtrl.h>

#include "letters.h"

constexpr int WIDTH = 30;
constexpr int HEIGHT = 30;
constexpr int NUM_LEDS = WIDTH * HEIGHT;

extern CRGB leds_plus_safety_pixel[NUM_LEDS + 1];
extern CRGB *const leds;

int convertXY(const int &x, const int &y);
void fill(const int &hue, const int &satur = 255, const int &value = 100);
void drawArray(const int arr[][2], int x, int y, int arrSize, int color);

#endif