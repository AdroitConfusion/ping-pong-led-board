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

extern Nunchuk nchuk;

extern int idle;
extern bool on_main_menu;

extern int joy_y;
extern int joy_x;
extern bool butt_z;
extern bool butt_c;

int convertXY(int x, int y);
void fill(int color);
void drawArray(const int arr[][2], int x, int y, int arrSize, int color);

#endif