#ifndef BREAKOUT_GAME_H
#define BREAKOUT_GAME_H

#include "..\include\util\controller_input.h"
#include "..\include\util\draw.h"

namespace Breakout {

constexpr uint8_t TOTAL_NUM_BLOCKS = 27;
constexpr uint8_t GAME_OVER_Y = 24;
constexpr uint8_t WIDTH_RIGHT = 28;
constexpr uint8_t WIDTH_LEFT = 1;
constexpr uint8_t BLOCK_WIDTH = 3;
constexpr uint8_t PADDLE_WIDTH = 4;

struct BreakoutBlock {
    uint8_t block_x[2 * BLOCK_WIDTH];
    uint8_t block_y[2 * BLOCK_WIDTH];
    int8_t color;

    BreakoutBlock() = default;

    BreakoutBlock(const uint8_t &init_x, const uint8_t &init_y, const int8_t &color) {
        setBlockXY(init_x, init_y, color);
    }

    void setBlockXY(const uint8_t &init_x, const uint8_t &init_y, const int8_t &color) {
        this->color = color;
        for (uint8_t i = 0; i < BLOCK_WIDTH; ++i) {
            block_x[i] = init_x + i;
            block_y[i] = init_y;
            block_x[i + BLOCK_WIDTH] = init_x + i;
            block_y[i + BLOCK_WIDTH] = init_y + 1;
        }
    }
};

class BreakoutGame {
public:
    BreakoutGame();
    void update(const ControllerInput &input);

private:
    void initGame();
    void checkController(const ControllerInput &input);
    void movePaddle();
    void drawPaddle();
    void drawWalls();
    void resetBall();
    void moveBall();
    void checkBallCollision(const ControllerInput &input);
    void saveBlocks();
    void loadBlocks();
    int checkBlock(int8_t offset_x, int8_t offset_y);
    bool checkWinCondition();
    void hitBlock(int blockIndex);

    uint8_t score = 0;
    bool new_game = true;

    BreakoutBlock blocks[TOTAL_NUM_BLOCKS];

    uint8_t ball_x, ball_y;
    int8_t ball_dx, ball_dy;

    uint8_t padd_x[PADDLE_WIDTH];
    uint8_t padd_y;
    int8_t padd_dir = 0;

    uint8_t prev_x1, prev_y1, prev_x2, prev_y2;
    uint32_t frame = 0;
    uint32_t last_delay = 0;
    uint32_t game_delay = 100;
};

} // namespace Breakout

#endif // BREAKOUT_GAME_H
