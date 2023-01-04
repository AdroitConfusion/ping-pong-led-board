#include "draw.h"

namespace ConnectFour {
void connectFour();
void initConnectFour();
void checkController();
void moveCurrBlock();
void clearCurrBlock();
void drawCurrBlock();
void drawWalls();
void dropCurrBlock();
void saveCurrBlock();
void resetBlock();
void drawSavedBlocks();
int checkWinCondition(int init_x, int init_y);
} // namespace ConnectFour