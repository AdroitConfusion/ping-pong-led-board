// snake_game.cpp
#include "..\include\games\snake.h"

SnakeGame::SnakeGame() {
    init();
}

SnakeGame::~SnakeGame() {}

void SnakeGame::init() {
    constexpr uint8_t SNAKE_START_X = 14;
    constexpr uint8_t SNAKE_START_Y = 14;
    constexpr uint8_t SNAKE_START_LEN = 5;
    constexpr uint8_t SNAKE_START_COLOR = 90;
    constexpr uint16 START_DELAY = 300;

    FastLED.clear(true);
    srand(millis()); // Seed random generator

    // Clear snake position and game grid
    memset(snake_x, -1, sizeof(snake_x));
    memset(snake_y, -1, sizeof(snake_y));
    memset(grid, 0, sizeof(grid));

    // Initialize snake's head position and properties
    snake_x[0] = SNAKE_START_X;
    snake_y[0] = SNAKE_START_Y;
    snake_len = SNAKE_START_LEN;
    snake_dir = last_dir = RIGHT;
    snake_color = SNAKE_START_COLOR;
    snake_level = 0;
    is_snake_rainbow = false;

    respawnApple(); // Place the first apple on the grid

    last_delay = millis();
    game_delay = START_DELAY;
}

void SnakeGame::update(const ControllerInput &input) {
    checkController(input);

    if (millis() - last_delay >= game_delay) {
        last_delay = millis();
        moveSnake();
        checkSnake();

        FastLED.clear(false); // Prepare to redraw LEDs

        // Draw apple
        leds[convertXY(apple_x, apple_y)] = CHSV(255, 255, 255);

        // Draw snake body
        for (uint16_t i = 1; i < snake_len; ++i) {
            if (snake_x[i] != -1 && snake_y[i] != -1) {
                leds[convertXY(snake_x[i], snake_y[i])] = CHSV(snake_color, 255, 255);
            }
        }

        // Draw snake head in white
        leds[convertXY(snake_x[0], snake_y[0])] = CRGB(255, 255, 255);
    }
}

void SnakeGame::checkController(const ControllerInput &input) {
    constexpr uint8_t JOY_X_LOW = 50;
    constexpr uint8_t JOY_X_HIGH = 200;
    constexpr uint8_t JOY_Y_LOW = 50;
    constexpr uint8_t JOY_Y_HIGH = 200;

    // Prevent snake from going in the reverse direction
    if (input.joy_x < JOY_X_LOW && last_dir != RIGHT)
        snake_dir = LEFT;
    else if (input.joy_x > JOY_X_HIGH && last_dir != LEFT)
        snake_dir = RIGHT;
    else if (input.joy_y > JOY_Y_HIGH && last_dir != DOWN)
        snake_dir = UP;
    else if (input.joy_y < JOY_Y_LOW && last_dir != UP)
        snake_dir = DOWN;
}

void SnakeGame::moveSnake() {
    // Move body
    for (uint16_t i = snake_len - 1; i > 0; i--) {
        snake_x[i] = snake_x[i - 1];
        snake_y[i] = snake_y[i - 1];
    }

    // Move head
    switch (snake_dir) {
    case LEFT:
        --snake_x[0];
        break;
    case RIGHT:
        ++snake_x[0];
        break;
    case DOWN:
        ++snake_y[0];
        break;
    case UP:
        --snake_y[0];
        break;
    }
    last_dir = snake_dir;

    // Wrap around screen edges
    if (snake_x[0] < 0)
        snake_x[0] = WIDTH - 1;
    else if (snake_x[0] >= WIDTH)
        snake_x[0] = 0;

    if (snake_y[0] < 0)
        snake_y[0] = HEIGHT - 1;
    else if (snake_y[0] >= HEIGHT)
        snake_y[0] = 0;
}

void SnakeGame::checkSnake() {
    // Eat apple
    if (snake_x[0] == apple_x && snake_y[0] == apple_y) {
        ++snake_len;
        if (!is_snake_rainbow)
            progressSnakeLevel();
        respawnApple();
    }

    if (is_snake_rainbow)
        changeSnakeRainbowColor();

    checkSnakeSelfCollision();
}

void SnakeGame::progressSnakeLevel() {
    constexpr std::array<uint16_t, 5> DELAY_ARR = {200, 150, 100, 50, 35};
    constexpr std::array<uint16_t, 5> COLOR_ARR = {120, 140, 160, 180, 200};
    constexpr std::array<uint16_t, 5> LEN_ARR = {10, 15, 20, 25, 30};
    constexpr uint8_t RAINBOW_SNAKE_LEN = 35;
    constexpr uint8_t RAINBOW_DELAY = 25;

    // Switch to rainbow mode
    if (snake_len >= RAINBOW_SNAKE_LEN) {
        game_delay = RAINBOW_DELAY;
        is_snake_rainbow = true;
        // Progress to next level if length matches
    } else if (snake_len == LEN_ARR[snake_level]) {
        ++snake_level;
        game_delay = DELAY_ARR[snake_level];
        snake_color = COLOR_ARR[snake_level];
    }
}

void SnakeGame::respawnApple() {

    // Mark grid cells occupied by the snake
    memset(grid, 0, sizeof(grid));
    for (uint16_t i = 0; i < snake_len; ++i) {
        if (snake_x[i] >= 0 && snake_y[i] >= 0) {
            grid[snake_x[i]][snake_y[i]] = true;
        }
    }

    // Find all free positions
    uint8_t free_count = 0;
    for (uint8_t x = 0; x < WIDTH; ++x) {
        for (uint8_t y = 0; y < HEIGHT; ++y) {
            if (!grid[x][y]) {
                free_positions[free_count++] = {x, y};
            }
        }
    }

    // Choose a random free position
    if (free_count > 0) {
        uint16_t index = rand() % free_count;
        apple_x = free_positions[index].first;
        apple_y = free_positions[index].second;
    }
}

void SnakeGame::changeSnakeRainbowColor() {
    constexpr int RAINBOW_TRANSITION_SPEED = 10;

    // Gradually change hue to cycle through rainbow colors
    snake_color = (snake_color + RAINBOW_TRANSITION_SPEED) % 256;
}

void SnakeGame::checkSnakeSelfCollision() {
    // Resets the game if snake collides with itself
    for (int i = 1; i < snake_len; ++i) {
        if (snake_x[i] == snake_x[0] && snake_y[i] == snake_y[0]) {
            init();
            return;
        }
    }
}