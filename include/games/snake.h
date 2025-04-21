// snake_game.h
#ifndef SNAKE_H
#define SNAKE_H

#include "draw.h"
#include <array>
#include <utility>

// Struct for passing controller input cleanly
struct ControllerInput {
    int joy_x;
    int joy_y;
    bool butt_z;
    bool butt_c;
};

class SnakeGame {
public:
    SnakeGame();
    ~SnakeGame();

    void update(const ControllerInput &input);

private:
    enum Direction {
        LEFT = 180,
        RIGHT = 0,
        DOWN = 90,
        UP = 270
    };

    // Game state
    short int snake_x[NUM_LEDS];
    short int snake_y[NUM_LEDS];
    uint8_t snake_len;
    uint8_t apple_x;
    uint8_t apple_y;
    int snake_dir;
    uint8_t last_dir;
    short int snake_color;
    uint8_t snake_level;
    bool is_snake_rainbow;

    bool grid[WIDTH][HEIGHT];
    std::pair<uint8_t, uint8_t> free_positions[WIDTH * HEIGHT];

    unsigned long last_delay;
    unsigned long game_delay;

    // Core methods
    void init();
    void checkController(const ControllerInput &input);
    void moveSnake();
    void checkSnake();
    void progressSnakeLevel();
    void respawnApple();
    void changeSnakeRainbowColor();
    void checkSnakeSelfCollision();
};

#endif // SNAKE_H
