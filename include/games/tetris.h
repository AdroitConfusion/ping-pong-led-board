#ifndef TETRIS_H
#define TETRIS_H

#include "draw.h"

struct TetrisBlock {
  int init_x;
  int init_y;
  int x[4];
  int y[4];
  int color;
  int block_type;

  TetrisBlock () : init_x(0), init_y(0), color(0), block_type(0) {}
  
  TetrisBlock (const int& init_x, const int& init_y, const int& type) {
    block_type = type;
    
    switch (block_type) {
      case 0: // T - Block
        color = 195;
        
        x[0] = init_x;
        x[1] = x[0] - 1;
        x[2] = x[0] + 1;
        x[3] = x[0];
        
        y[0] = init_y;
        y[1] = y[0];
        y[2] = y[0];
        y[3] = y[0] - 1;
        break;
      case 1: // O - Block
        color = 55;

        x[0] = init_x;
        x[1] = x[0];
        x[2] = x[0] + 1;
        x[3] = x[0] + 1;
        
        y[0] = init_y;
        y[1] = y[0] + 1;
        y[2] = y[0];
        y[3] = y[0] + 1;
        break;
      case 2: // I - Block
        color = 130;
            
        x[0] = init_x;
        x[1] = x[0] + 1;
        x[2] = x[0] + 2;
        x[3] = x[0] - 1;
        
        y[0] = init_y;
        y[1] = y[0];
        y[2] = y[0];
        y[3] = y[0];
        break;
      case 3: // S - Block
        color = 100;
        
        x[0] = init_x;
        x[1] = x[0] - 1;
        x[2] = x[0];
        x[3] = x[0] + 1;
        
        y[0] = init_y;
        y[1] = y[0];
        y[2] = y[0] - 1;
        y[3] = y[0] - 1;
        break;
      case 4: // L - Block
        color = 275;

        x[0] = init_x;
        x[1] = x[0];
        x[2] = x[0];
        x[3] = x[0] + 1;
        
        y[0] = init_y;
        y[1] = y[0] - 1;
        y[2] = y[0] + 1;
        y[3] = y[0] + 1;
        break;
      case 5: // Z - Block
        color = 255;
        
        x[0] = init_x;
        x[1] = x[0] - 1;
        x[2] = x[0];
        x[3] = x[0] + 1;
        
        y[0] = init_y;
        y[1] = y[0];
        y[2] = y[0] + 1;
        y[3] = y[0] + 1;
        break;
      case 6: // J - Block
        color = 160;

        x[0] = init_x;
        x[1] = x[0];
        x[2] = x[0];
        x[3] = x[0] - 1;
        
        y[0] = init_y;
        y[1] = y[0] - 1;
        y[2] = y[0] + 1;
        y[3] = y[0] + 1;
        break;
      default:
        break;
    }
  }
};

void tetris(const int& joy_x, const int& joy_y, const bool& butt_z, const bool& butt_c, bool& new_game);
void initTetris(bool& new_game);
void tetrisCheckMovement(const int& joy_x, const int& joy_y, const bool& butt_z);
void moveTetronimo();
void rotateShape();
void setTetrisMaxAndMins();
void fall();
bool checkMultiBlockCollision(const int mX[], const int mY[], const int& y, const int& x);
bool checkSingleBlockCollision(const int& sX, const int& sY, const int& y, const int& x);
bool checkRotationInBounds(const int x[], const int y[]);
void deleteRow();
void drawTetrisWalls();
void drawShape();
void loadBlocks();
void saveBlock();

#endif