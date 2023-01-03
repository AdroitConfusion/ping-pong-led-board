#include "..\include\games\tetris.h"

const int gameWidthLeft = 13;
const int gameWidthRight = 25;

const int gameHeightTop = 9;
const int gameHeightBottom = 25;

int tmaconvertXY = -1;
int tmaxX = -1;

int tminX = WIDTH;
int tminY = HEIGHT;

int tetris_dir = 0;
bool tetris_rotate = false;

bool rowComplete = false;
bool blockPlaced = false;

TetrisBlock curr_block;

TetrisBlock nextBlock;
TetrisBlock nextBlockTwo;
TetrisBlock nextBlockThree;

int savedBlocks[WIDTH][HEIGHT];

int tetrisFrameCount;
bool tetrisGameOver;
int tetrisScore;

unsigned long tetris_game_delay;
unsigned long tetris_last_delay;


void tetris(const int& joy_x, const int& joy_y, const bool& butt_z, const bool& butt_c, bool& new_game) {
  if (new_game)
    initTetris(new_game);
  
  tetrisCheckMovement(joy_x, joy_y, butt_z);

  if (millis() - tetris_last_delay >= tetris_game_delay) {
    if (!tetrisGameOver) {
      if (butt_c)
        tetrisGameOver = true;

      tetris_last_delay = millis();
    
      setTetrisMaxAndMins();
      moveTetronimo();
      tetris_dir = 0;
      if (tetris_rotate) {
        rotateShape();
        delay(20);
      }
      tetris_rotate = false;

      if (tetrisFrameCount % 4 == 0 || joy_y < 120)
        fall();

      FastLED.clear(false);
      
      deleteRow();
      loadBlocks();
      drawTetrisWalls();
      drawShape();

      FastLED.show();

      tetrisFrameCount++;
    } else {
      new_game = true;
      idle = millis();
    }
  }
}

void initTetris(bool& new_game) {
  FastLED.clear(false);

  srand(millis());
  
  tetrisFrameCount = 0;
  tetrisGameOver = false;
  tetrisScore = 0;

  tmaconvertXY = -1;
  tmaxX = -1;

  tminX = WIDTH;
  tminY = HEIGHT;

  rowComplete = false;
  blockPlaced = false;

  curr_block = TetrisBlock(17, 9, rand() % 7);
  nextBlock = TetrisBlock(8, 23, rand() % 7);
  nextBlockTwo = TetrisBlock(8, 15, rand() % 7);
  nextBlockThree = TetrisBlock(8, 7, rand() % 7);

  memset(&savedBlocks, -1, sizeof(savedBlocks[0][0]) * NUM_LEDS);

  tetris_game_delay = 110;
  tetris_last_delay = millis();

  new_game = false;
} 

void tetrisCheckMovement(const int& joy_x, const int& joy_y, const bool& butt_z) {
  if (joy_x < 50)
    tetris_dir = -1;
  else if (joy_x > 200)
    tetris_dir = 1;
  
  if (butt_z)
    tetris_rotate = true;
}

void moveTetronimo() {
  if (tetris_dir == -1 && tminX > gameWidthLeft  && !checkMultiBlockCollision(curr_block.x, curr_block.y, 0, -1))
    for (int i = 0; i < 4; i++) 
      curr_block.x[i]--;
  else if (tetris_dir == 1 && tmaxX < gameWidthRight  && !checkMultiBlockCollision(curr_block.x, curr_block.y, 0, 1))
    for (int i = 0; i < 4; i++) 
      curr_block.x[i]++;
}

void rotateShape() {
  int tempArrX[4];
  int tempArrY[4];

  for (int i = 3; i >= 0; i--) {
    //Sets origin at initX and initY
    int tempX = curr_block.x[i] - curr_block.x[0];
    int tempY = curr_block.y[i] - curr_block.y[0];
    //Makes old y coord negative
    int temp = -tempY;
    //Flips x and y
    tempY = tempX;
    tempX = temp;

    //Adds initX and initY back
    tempArrX[i] = tempX + curr_block.x[0];
    tempArrY[i] = tempY + curr_block.y[0];  

    //Checks wall & block collision and shifts player block
    if (curr_block.y[0] - tminY == 2 && (curr_block.x[0] == gameWidthRight || checkSingleBlockCollision(curr_block.x[0], curr_block.y[0],0, 1)))
      tempArrX[i] -= 2;
    else if (tmaconvertXY - curr_block.y[0] == 2 && (curr_block.x[0] == gameWidthLeft || checkSingleBlockCollision(curr_block.x[0], curr_block.y[0],0, -1)))
      tempArrX[i] += 2;
    else if ((curr_block.y[0] - tminY == 1 && (curr_block.x[0] == gameWidthRight || checkSingleBlockCollision(curr_block.x[0], curr_block.y[0],0, 1))) || (curr_block.y[0] - tminY == 2 && (curr_block.x[0] == gameWidthRight -1 || checkSingleBlockCollision(curr_block.x[0], curr_block.y[0],0, 2))))
      tempArrX[i] -= 1;
    else if ((tmaconvertXY - curr_block.y[0] == 1 && (curr_block.x[0] == gameWidthLeft || checkSingleBlockCollision(curr_block.x[0], curr_block.y[0],0, -1))) || (tmaconvertXY - curr_block.y[0] == 2 && (curr_block.x[0] == gameWidthLeft + 1 || checkSingleBlockCollision(curr_block.x[0], curr_block.y[0],0,  -2))))
      tempArrX[i] += 1;
  }
  //Double checks final rotated position after shifting x - positions
  if (!checkMultiBlockCollision(tempArrX, tempArrY,0, 0) && checkRotationInBounds(tempArrX, tempArrY)) {
    for (int i = 0; i < 4; i++) {
      curr_block.x[i] = tempArrX[i];
      curr_block.y[i] = tempArrY[i];
    }
  }
}

void setTetrisMaxAndMins() {
  tmaxX = -1;
  tmaconvertXY = -1;
  tminX = 30;
  tminY = 30;
  for (int i = 0; i < 4; i++) {
     tmaconvertXY = std::max(tmaconvertXY, curr_block.y[i]);
     tminY = std::min(tminY, curr_block.y[i]);
     tmaxX = std::max(tmaxX, curr_block.x[i]);
     tminX = std::min(tminX, curr_block.x[i]);
  }
}

void fall() {
  if (tmaconvertXY < gameHeightBottom && !checkMultiBlockCollision(curr_block.x, curr_block.y, 1, 0)) {
    for (int i = 0; i < 4; i++) 
      curr_block.y[i]++;
  } else {
      saveBlock();
      drawShape();
      loadBlocks();
      blockPlaced = true;

      curr_block =  TetrisBlock(17, 9, nextBlock.block_type);
      nextBlock = TetrisBlock(8, 23, nextBlockTwo.block_type);
      nextBlockTwo = TetrisBlock(8, 15, nextBlockThree.block_type);
      nextBlockThree = TetrisBlock(8, 7, rand() % 7);
  }
}

//False if no blocks collide, true if at least one block will collide
bool checkMultiBlockCollision(const int mX[], const int mY[], const int& y, const int& x) {
  for (int i = 0; i < 4; i++)
    if (checkSingleBlockCollision(mX[i], mY[i], y, x)) 
      return true;
  return false;
}

//False if no collision, true if there is a collision
bool checkSingleBlockCollision(const int& sX, const int& sY, const int& y, const int& x) {
  if (savedBlocks[sX + x][sY + y] != -1)
    return true;
  return false;
}

bool checkRotationInBounds(const int x[], const int y[]) {
  for (int i = 0; i < 4; i++) {
    if (x[i] < gameWidthLeft || x[i] > gameWidthRight || y[i] < gameHeightTop || y[i] >= gameHeightBottom)
      return false;
  }
  return true;
}

void deleteRow() {
  //Search each row
  for (int j = gameHeightBottom; j >= 0; j--) {
    //Search each column
    for (int i = gameWidthLeft; i <= gameWidthRight; i++) {
      //Break column search loop if there is an empty dot in the row
      if (savedBlocks[i][j] == -1)
        break;
      //If i reaches the end, then that means there is a full row
      else if (i == gameWidthRight) {
        //Set the target row values to -1
        tetrisScore += 10;
        rowComplete = true;
        for (int k = 0; k <= gameWidthRight; k++)
          savedBlocks[k][j] = -1;
        //Shift everything above down one
        for (int l = gameHeightBottom; l  >  0; l--)
          if (l < j) for (int m = 0; m  <= gameWidthRight; m++) savedBlocks[m][l + 1] = savedBlocks[m][l];
      }
    }
  }
}

void drawTetrisWalls() {
  for (int i = 4; i < 27; i++) {
    leds[convertXY(i, 4)] = CHSV(255,0,255);
    leds[convertXY(i, 26)] = CHSV(255,0,255);
    leds[convertXY(4, i)] = CHSV(255,0,255);
    leds[convertXY(12, i)] = CHSV(255,0,255);
    leds[convertXY(26, i)] = CHSV(255,0,255);
  }
  for (int i = 13; i < 26; i++)
    leds[convertXY(i, 9)] = CHSV(255,0,100);
}

void drawShape() {
  for (int i = 0; i < 4; i++) {
    leds[convertXY(curr_block.x[i], curr_block.y[i])] = CHSV(curr_block.color, 255, 255);
    leds[convertXY(nextBlock.x[i], nextBlock.y[i])] = CHSV(nextBlock.color, 255, 255);
    leds[convertXY(nextBlockTwo.x[i], nextBlockTwo.y[i])] = CHSV(nextBlockTwo.color, 255, 255);
    leds[convertXY(nextBlockThree.x[i], nextBlockThree.y[i])] = CHSV(nextBlockThree.color, 255, 255);
  }
}

void loadBlocks() {
  for (int i = 0; i <= gameWidthRight; i++) {
    for (int j = 0; j <= gameHeightBottom; j++) {
      if (savedBlocks[i][j] != -1)
        leds[convertXY(i, j)] = CHSV(savedBlocks[i][j], 255, 255);
    }
  }
}

void saveBlock() {
  for (int k = 0; k < 4; k++) {
    for (int j = 0; j <= gameHeightBottom; j++) {
      if (curr_block.y[k] == j) {
        for (int i = 0; i <= gameWidthRight; i++) {
          if (curr_block.x[k] == i) {
            savedBlocks[i][j] = curr_block.color;
            tetrisScore++;
          }
        }
      }
    }
    if (curr_block.y[k] == gameHeightTop) {
      tetrisGameOver = true;
    }
  }
}