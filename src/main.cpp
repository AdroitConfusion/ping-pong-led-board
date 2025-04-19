// Libraries
#include <Arduino.h>
#include <SoftwareSerial.h>

// Header files
#include "..\include\games\bird.h"
#include "..\include\games\breakout.h"
#include "..\include\games\connect_four.h"
#include "..\include\games\snake.h"
#include "..\include\games\space_invaders.h"
#include "..\include\games\tetris.h"

// Macros
#define LED_PIN D4
#define COLOR_ORDER GRB
#define CHIPSET WS2811

// Constants
constexpr int BAUD_RATE = 115200;
constexpr int BRIGHTNESS = 80;

constexpr int NUM_GAMES = 1;
enum GAMES {
    SNAKE = 0
};

int screen_num = 900;

void setup() {
    FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
    FastLED.setBrightness(BRIGHTNESS);

    Serial.begin(BAUD_RATE);

    nchuk.begin();

    if (!nchuk.connect()) {
        Serial.println(F("Nunchuk not detected!"));
        delay(1000);
    }
}

void loop() {

    bool success = nchuk.update(); // Get new data from the nunchuk
    if (!success) {
        Serial.println(F("Controller disconnected!"));
        delay(1000);
    } else {
        // Nunchuk inputs
        joy_y = nchuk.joyY();
        joy_x = nchuk.joyX();
        butt_z = nchuk.buttonZ();
        butt_c = nchuk.buttonC();

        int screen_num = screen_num % 10;

        // Switch screen and reset idle timer based on joystick movement in x direction
        if (joy_x > 240 && millis() % 200 == 0 && new_game) {
            screen_num = (++screen_num) % NUM_GAMES;
            idle = millis();
        } else if (joy_x < 10 && millis() % 200 == 0 && new_game) {
            screen_num = (--screen_num) % NUM_GAMES;
            idle = millis();
        }

        // Show screen
        switch (screen_num) {
        case GAMES::SNAKE:
            showSnakeScreen();
            break;
        default:
            showSnakeScreen();
            break;
        }

        // Select game
        if (butt_z) {
            switch (screen_num) {
            case GAMES::SNAKE:
                Snake::snake();
                break;
            default:
                break;
            }
        }

        FastLED.show();
    }
}

void showSnakeScreen() {
    fill(100);
    drawArray(S, 0, 11, 20, -1);
    drawArray(N, 6, 11, 20, -1);
    drawArray(A, 12, 11, 20, -1);
    drawArray(K, 18, 11, 20, -1);
    drawArray(E, 24, 11, 20, -1);
}
