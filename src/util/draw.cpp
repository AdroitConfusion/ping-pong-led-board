#include "../include/util/draw.h"

CRGB leds_plus_safety_pixel[NUM_LEDS + 1];
CRGB *const leds(leds_plus_safety_pixel + 1);

int convertXY(const int &x, const int &y) {
    if (y >= HEIGHT || x >= WIDTH)
        return -1;
    return y % 2 == 0 ? (y * WIDTH) + x : ((y + 1) * WIDTH) - (x + 1);
}

void fill(const int &hue, const int &satur, const int &value) {
    for (int i = 0; i < NUM_LEDS; i++)
        leds[i] = CHSV(hue, satur, value);
}

void drawArray(const int arr[][2], const int &x, const int &y, const int &arrSize, const int &color) {
    int saturation = 255;
    if (color == -1)
        saturation = 0;
    for (int i = 0; i < arrSize; ++i) {
        if (arr[i][0] + x > -1 && arr[i][0] + x < WIDTH && arr[i][1] + y > -1 && arr[i][1] + y < HEIGHT)
            leds[convertXY(arr[i][0] + x, arr[i][1] + y)] = CHSV(color, saturation, 255);
    }
}
