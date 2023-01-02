#include "draw.h"

CRGB leds_plus_safety_pixel[NUM_LEDS + 1];
CRGB* const leds(leds_plus_safety_pixel + 1);

Nunchuk nchuk;

int idle = millis();


int convertXY(int x, int y) {
  if (y >= HEIGHT || x >= WIDTH) 
    return -1;
  return y % 2 == 0 ? (y * WIDTH) + x : ((y + 1) * WIDTH) - (x + 1);
}

void fill(int color) {
  for (int i = 0; i < NUM_LEDS; i++)
    leds[i] = CHSV(color, 255, 120);
}

void drawArray(const int arr[][2], int x, int y, int arrSize, int color) {
  int saturation = 255;
  if (color == -1) 
    saturation = 0;
  for (int i = 0; i < arrSize; i++) {
    if (arr[i][0] + x > -1 && arr[i][0] + x < WIDTH && arr[i][1] + y > -1 && arr[i][1] + y < HEIGHT)
      leds[convertXY(arr[i][0] + x, arr[i][1] + y)] = CHSV(color, saturation, 255);
  }
}

void snakeScreen() {
  fill(100);
  drawArray(S,0,11,20,-1);
  drawArray(N,6,11,20,-1);
  drawArray(A,12,11,20,-1);
  drawArray(K,18,11,20,-1);
  drawArray(E,24,11,20,-1);
}