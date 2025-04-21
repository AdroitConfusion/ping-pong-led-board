// main.cpp
#include "../include/util/controller_input.h"
#include "../include/util/draw.h"

#include "../include/games/breakout.h"
#include "../include/games/snake.h"
// #include "../include/games/bird.h"
// #include "../include/games/space_invaders.h"
// #include "../include/games/tetris.h"
#include <Arduino.h>

#define LED_PIN D4
#define COLOR_ORDER GRB
#define CHIPSET WS2811

constexpr int BAUD_RATE = 115200;
constexpr int BRIGHTNESS = 80;
constexpr int JOY_X_HIGH = 200;
constexpr int JOY_X_LOW = 50;
constexpr unsigned long SCREEN_UPDATE_DELAY = 200;

Nunchuk nchuk;
int idle;
bool on_main_menu = true;
unsigned long last_delay = millis();

enum GameType {
    NONE = -1,
    SNAKE,
    BREAKOUT,
    NUM_GAMES
};

GameType current_game = NONE;
SnakeGame *snakeGame = nullptr;
Breakout::BreakoutGame *breakoutGame = nullptr;

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

void handleMainMenu(const ControllerInput &input);
void handleGameLoop(const ControllerInput &input);
void launchGame(GameType game);
void cleanupGame(GameType game);
void showSnakeScreen();
void showBreakoutScreen();
void showErrorScreen();

void loop() {
    bool success = nchuk.update();
    if (!success) {
        Serial.println(F("Controller disconnected!"));
        delay(1000);
        return;
    }

    ControllerInput input = {nchuk.joyX(), nchuk.joyY(), nchuk.buttonZ(), nchuk.buttonC()};

    if (on_main_menu) {
        handleMainMenu(input);
    } else {
        handleGameLoop(input);
    }

    FastLED.show();
}

void handleMainMenu(const ControllerInput &input) {
    static int menu_index = 0;

    if (input.joy_x > JOY_X_HIGH && millis() - last_delay >= SCREEN_UPDATE_DELAY) {
        Serial.println(F("RIGHT"));
        menu_index = (menu_index + 1) % NUM_GAMES;
        last_delay = millis();
    } else if (input.joy_x < JOY_X_LOW && millis() - last_delay >= SCREEN_UPDATE_DELAY) {
        Serial.println(F("LEFT"));
        menu_index = (menu_index - 1 + NUM_GAMES) % NUM_GAMES;
        last_delay = millis();
    }

    Serial.printf("menu_index: %d\n", menu_index);

    current_game = static_cast<GameType>(menu_index);

    switch (current_game) {
    case SNAKE:
        showSnakeScreen();
        break;
    case BREAKOUT:
        showBreakoutScreen();
        break;
    default:
        showErrorScreen();
        break;
    }

    if (input.butt_z) {
        on_main_menu = false;
        launchGame(current_game);
    }
}

void handleGameLoop(const ControllerInput &input) {

    switch (current_game) {
    case SNAKE:
        if (snakeGame)
            snakeGame->update(input);
        break;
    case BREAKOUT:
        if (breakoutGame)
            breakoutGame->update(input);
        break;
    default:
        break;
    }

    if (input.butt_c) {
        FastLED.clear(true);
        cleanupGame(current_game);
        on_main_menu = true;
    }
}

void launchGame(GameType game) {
    switch (game) {
    case SNAKE:
        snakeGame = new SnakeGame();
        break;
    case BREAKOUT:
        breakoutGame = new Breakout::BreakoutGame();
        break;
    default:
        break;
    }
}

void cleanupGame(GameType game) {
    switch (game) {
    case SNAKE:
        delete snakeGame;
        snakeGame = nullptr;
        break;
    case BREAKOUT:
        delete breakoutGame;
        breakoutGame = nullptr;
        break;
    default:
        break;
    }
}

void showSnakeScreen() {
    fill(96, 255, 50);
    drawArray(S, 0, 11, 20, -1);
    drawArray(N, 6, 11, 20, -1);
    drawArray(A, 12, 11, 20, -1);
    drawArray(K, 18, 11, 20, -1);
    drawArray(E, 24, 11, 20, -1);
}

void showBreakoutScreen() {
    fill(60, 255, 50);
    drawArray(B, 0, 11, 20, -1);
    drawArray(R, 6, 11, 20, -1);
    drawArray(E, 12, 11, 20, -1);
    drawArray(A, 18, 11, 20, -1);
    drawArray(K, 24, 11, 20, -1);
}

void showErrorScreen() {
    fill(0, 255, 50);
    drawArray(E, 0, 11, 20, -1);
    drawArray(R, 6, 11, 20, -1);
    drawArray(R, 12, 11, 20, -1);
    drawArray(O, 18, 11, 20, -1);
    drawArray(R, 24, 11, 20, -1);
}