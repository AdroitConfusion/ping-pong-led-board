#include "draw.h"

namespace Breakout {
struct BreakoutBlock {
    int block_x[6];
    int block_y[6];
    int color;

    BreakoutBlock(){};

    BreakoutBlock(int init_x, int init_y, int color) {
        setBlockXY(init_x, init_y, color);
    }

    void setBlockXY(int init_x, int init_y, int color) {
        this->color = color;
        block_x[0] = init_x;
        block_x[1] = init_x + 1;
        block_x[2] = init_x + 2;
        block_x[3] = init_x;
        block_x[4] = init_x + 1;
        block_x[5] = init_x + 2;

        block_y[0] = init_y;
        block_y[1] = init_y;
        block_y[2] = init_y;
        block_y[3] = init_y + 1;
        block_y[4] = init_y + 1;
        block_y[5] = init_y + 1;
    }
};

void breakout();
void initBreakout();

void breakoutCheckController();
void movePaddle();
void moveBreakoutBall();

void drawPaddle();
void drawBreakoutWalls();

void resetBreakoutBall();
void checkBreakoutBall();

void saveBreakoutBlocks();
void loadBreakoutBlocks();
int checkBreakoutBlock(const int &offset_x, const int &offset_y);
bool numberOfBreakoutBlocksDestroyed();
void changeScoreAndBlockColor(const int &check);
} // namespace Breakout
