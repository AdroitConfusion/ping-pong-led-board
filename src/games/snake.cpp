#include "..\include\games\snake.h"

namespace Snake {

enum Direction {
    LEFT = 180,
    RIGHT = 0,
    DOWN = 90,
    UP = 270
};

int snake_x[NUM_LEDS];
int snake_y[NUM_LEDS];
int snake_len;
int apple_x;
int apple_y;
int snake_dir;
int last_dir;
int snake_color;
int snake_level = 1;
bool is_snake_rainbow = false;

bool grid[WIDTH][HEIGHT] = {};
std::pair<int, int> free_positions[WIDTH * HEIGHT];

unsigned long last_delay;
unsigned long game_delay;

constexpr int SNAKE_STARTING_X = 14;
constexpr int SNAKE_STARTING_Y = 14;
constexpr int SNAKE_STARTING_DIR = Direction::RIGHT;
constexpr int SNAKE_RAINBOW_SPEED = 10;
constexpr std::array<int, 6> SNAKE_COLOR_ARR = {90, 120, 140, 160, 180, 200};
constexpr std::array<int, 6> SNAKE_LEN_ARR = {5, 10, 15, 20, 25, 30};

constexpr int APPLE_STARTING_X = 0;
constexpr int APPLE_STARTING_Y = 14;

constexpr int JOY_X_LOW = 50;
constexpr int JOY_X_HIGH = 200;
constexpr int JOY_Y_LOW = 50;
constexpr int JOY_Y_HIGH = 200;

constexpr std::array<int, 6> DELAY_ARR = {300, 200, 150, 100, 50, 25};

void snake() {
    if (new_game)
        initSnake();

    checkController();

    if (millis() - last_delay >= game_delay) {
        if (!new_game) {
            if (butt_c)
                on_main_menu = true;

            last_delay = millis();

            moveSnake();
            checkSnake();

            FastLED.clear(false);
            // Draw apple
            leds[convertXY(apple_x, apple_y)] = CHSV(255, 255, 255);
            // Draw snake body
            for (int i = 1; i < snake_len; ++i) {
                if (snake_x[i] != -1 && snake_y[i] != -1) {
                    leds[convertXY(snake_x[i], snake_y[i])] = CHSV(snake_color, 255, 255);
                }
            }
            // Draw snake head
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

    snake_x[0] = SNAKE_STARTING_X;
    snake_y[0] = SNAKE_STARTING_Y;
    snake_len = SNAKE_LEN_ARR[0];

    snake_dir = last_dir = SNAKE_STARTING_DIR;
    snake_color = SNAKE_COLOR_ARR[0];
    is_snake_rainbow = false;

    // apple_x = APPLE_STARTING_X;
    // apple_y = APPLE_STARTING_Y;
    respawnApple();

    game_delay = DELAY_ARR[0];
    last_delay = millis();

    new_game = false;
}

// Determine direction of snake head based on nunchuk joystick
void checkController() {
    if (joy_x < JOY_X_LOW && last_dir != Direction::RIGHT)
        snake_dir = Direction::LEFT;
    else if (joy_x > JOY_X_HIGH && last_dir != Direction::LEFT)
        snake_dir = Direction::RIGHT;
    else if (joy_y > JOY_Y_HIGH && last_dir != Direction::DOWN)
        snake_dir = Direction::UP;
    else if (joy_y < JOY_Y_LOW && last_dir != Direction::UP)
        snake_dir = Direction::DOWN;
}

void moveSnake() {
    // Assign new snake segments
    for (int i = snake_len - 1; i > 0; i--) {
        snake_x[i] = snake_x[i - 1];
        snake_y[i] = snake_y[i - 1];
    }

    // Move snake head in the given direction
    switch (snake_dir) {
    case Direction::LEFT:
        --snake_x[0];
        break;
    case Direction::RIGHT:
        ++snake_x[0];
        break;
    case Direction::DOWN:
        ++snake_y[0];
        break;
    case Direction::UP:
        --snake_y[0];
        break;
    default:
        break;
    }
    last_dir = snake_dir;

    // If the snake is touching a wall, teleport it to opposite wall
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
        ++snake_len;

        changeDelayAndColor();
        respawnApple();
    }

    if (is_snake_rainbow) {
        changeSnakeRainbowColor();
    }

    checkSnakeSelfCollision();
}

// Change snake color and speed as the game progresses
void changeDelayAndColor() {
    if (snake_len >= SNAKE_LEN_ARR[SNAKE_LEN_ARR.size() - 1]) {
        // Activate rainbow mode
        game_delay = DELAY_ARR[DELAY_ARR.size() - 1];
        is_snake_rainbow = true;
    } else if (snake_len == SNAKE_LEN_ARR[snake_level]) {
        ++snake_level;
        game_delay = DELAY_ARR[snake_level];
        snake_color = SNAKE_COLOR_ARR[snake_level];
    }
}

void respawnApple() {
    // Find all spots that occupied by the snake
    memset(grid, 0, sizeof(grid));
    for (int i = 0; i < snake_len; ++i) {
        grid[snake_x[i]][snake_y[i]] = true;
    }

    // Find all unoccupied spots
    int free_count = 0;
    for (int x = 0; x < WIDTH; ++x) {
        for (int y = 0; y < HEIGHT; ++y) {
            if (!grid[x][y]) {
                free_positions[free_count++] = {x, y};
            }
        }
    }

    // Pick random unoccupied spot to respawn apple
    int index = rand() % free_count;
    apple_x = free_positions[index].first;
    apple_y = free_positions[index].second;
}

void changeSnakeRainbowColor() {
    snake_color = (snake_color + SNAKE_RAINBOW_SPEED) % 256;
}

// Detect if the snake head is biting itself
void checkSnakeSelfCollision() {
    for (int i = 1; i < snake_len; ++i) {
        if (snake_x[i] == snake_x[0] && snake_y[i] == snake_y[0]) {
            new_game = true;
            return;
        }
    }
}

} // namespace Snake