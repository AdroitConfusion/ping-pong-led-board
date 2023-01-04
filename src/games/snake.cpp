#include "..\include\games\snake.h"

namespace Snake {
int snake_x[NUM_LEDS];
int snake_y[NUM_LEDS];
int snake_len;
int apple_x;
int apple_y;
int snake_dir; // snake direction has integer values of degrees in circle
int last_dir;
int snake_color;

unsigned long last_delay;
unsigned long game_delay;

void snake() {
    if (new_game)
        initSnake();

    checkController();

    if (millis() - last_delay >= game_delay) {
        if (!new_game) {
            if (butt_c)
                new_game = true;

            last_delay = millis();

            moveSnake();
            checkSnake();
            changeDelayAndColor();

            FastLED.clear(false);
            // Draw apple
            leds[convertXY(apple_x, apple_y)] = CHSV(255, 255, 255);
            // Draw snake
            for (int i = 1; i < snake_len; i++)
                leds[convertXY(snake_x[i], snake_y[i])] = CHSV(snake_color, 255, 255);
            // Color snake head
            leds[convertXY(snake_x[0], snake_y[0])] = CRGB(255, 255, 255);

        } else {
            idle = millis();
        }
    }
}

// Define variables for new game of snake
void initSnake() {
    FastLED.clear(true);

    srand(millis());

    memset(&snake_x, -1, sizeof(snake_x[0]) * NUM_LEDS);
    memset(&snake_y, -1, sizeof(snake_y[0]) * NUM_LEDS);

    snake_x[0] = 14;
    snake_y[0] = 14;
    snake_len = 5;

    snake_dir = 0;
    last_dir = 0;
    snake_color = 100;

    apple_x = 0;
    apple_y = 14;

    game_delay = 300;
    last_delay = millis();

    new_game = false;
}

// Determine direction of snake head based on nunchuk joystick
void checkController() {
    if (joy_x < 50 && last_dir != 0)
        snake_dir = 180;
    else if (joy_x > 200 && last_dir != 180)
        snake_dir = 0;
    else if (joy_y > 200 && last_dir != 90)
        snake_dir = 270;
    else if (joy_y < 50 && last_dir != 270)
        snake_dir = 90;
}

void moveSnake() {
    // Assign new snake segments
    for (int i = snake_len - 1; i > 0; i--) {
        snake_x[i] = snake_x[i - 1];
        snake_y[i] = snake_y[i - 1];
    }

    // Move snake head in the given direction
    switch (snake_dir) {
    case 180:
        snake_x[0]--;
        break;
    case 0:
        snake_x[0]++;
        break;
    case 90:
        snake_y[0]++;
        break;
    case 270:
        snake_y[0]--;
        break;
    default:
        break;
    }
    last_dir = snake_dir;

    // If touching a wall, teleport to opposite wall
    if (snake_x[0] < 0)
        snake_x[0] = WIDTH - 1;
    else if (snake_x[0] > WIDTH - 1)
        snake_x[0] = 0;

    if (snake_y[0] < 0)
        snake_y[0] = HEIGHT - 1;
    else if (snake_y[0] > HEIGHT - 1)
        snake_y[0] = 0;
}

void checkSnake() {
    // Determine if snake head has eaten the apple
    if (snake_x[0] == apple_x && snake_y[0] == apple_y) {
        snake_len++;
        bool apple_respawned = false;
        while (!apple_respawned) {
            apple_x = rand() % WIDTH;
            apple_y = rand() % HEIGHT;
            apple_respawned = true;
            for (int i = 0; i < snake_len; i++) {
                if (snake_x[i] == apple_x && snake_y[i] == apple_y)
                    apple_respawned = false;
            }
        }
    }

    // Detect if the snake head is biting itself
    for (int i = 1; i < snake_len; i++) {
        if (snake_x[i] == snake_x[0] && snake_y[i] == snake_y[0])
            new_game = true;
    }
}

// Change snake color and speed as the game goes on
void changeDelayAndColor() {
    if (snake_len < 8) {
        game_delay = 300;
        snake_color = 100;
    } else if (snake_len < 11) {
        game_delay = 200;
        snake_color = 120;
    } else if (snake_len < 14) {
        game_delay = 150;
        snake_color = 140;
    } else if (snake_len < 17) {
        game_delay = 100;
        snake_color = 160;
    } else if (snake_len < 20) {
        game_delay = 50;
        snake_color = 180;
    } else {
        game_delay = 25;
        snake_color += 20;
    }
}
}