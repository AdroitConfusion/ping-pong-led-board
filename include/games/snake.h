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
    int snake_x[NUM_LEDS];
    int snake_y[NUM_LEDS];
    int snake_len;
    int apple_x;
    int apple_y;
    int snake_dir;
    int last_dir;
    int snake_color;
    int snake_level;
    bool is_snake_rainbow;

    bool grid[WIDTH][HEIGHT];
    std::pair<int, int> free_positions[WIDTH * HEIGHT];

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
