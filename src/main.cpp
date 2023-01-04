// Libraries
#include <Arduino.h>
#include <SoftwareSerial.h>

// Header files
#include "..\include\games\bird.h"
#include "..\include\games\breakout.h"
#include "..\include\games\connect_four.h"
#include "..\include\games\snake.h"
#include "..\include\games\tetris.h"

// Macros
#define LED_PIN D4
#define COLOR_ORDER GRB
#define CHIPSET WS2811

// Constants
const int BAUD_RATE = 115200;
const int BRIGHTNESS = 80;

int screen_num = 900;

bool new_game = true;

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
        int joy_y = nchuk.joyY();
        int joy_x = nchuk.joyX();
        bool butt_z = nchuk.buttonZ();
        bool butt_c = nchuk.buttonC();

        int curr_screen = screen_num % 10;

        // Switch screen and reset idle timer based on joystick movement in x direction
        if (joy_x > 240 && millis() % 200 == 0 && new_game) {
            screen_num++;
            idle = millis();
        } else if (joy_x < 10 && millis() % 200 == 0 && new_game) {
            screen_num--;
            idle = millis();
        }

        ConnectFour::connectFour(joy_x, joy_y, butt_z, butt_c);

        // if (butt_z || new_game == false) {
        //   switch (curr_screen) {
        //     case 0:
        //       snake(joy_x, joy_y, butt_z, butt_c, new_game);
        //       break;
        //     case 1:
        //       tetris(joy_x, joy_y, butt_z, butt_c, new_game);
        //       break;
        //     default:
        //       break;
        //   }
        // }

        // snakeScreen();
        FastLED.show();
    }
}
