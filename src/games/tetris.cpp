// #include "..\include\games\tetris.h"

// namespace Tetris {
// const int WALL_WIDTH = 2;
// const int GAME_WIDTH_LEFT = 13;
// const int GAME_WIDTH_RIGHT = 25;

// const int GAME_HEIGHT_TOP = 9;
// const int GAME_HEIGHT_BOTTOM = 25;

// int max_y;
// int max_x;

// int min_x;
// int min_y;

// int tetris_dir;
// bool tetris_rotate;

// bool row_completed;
// bool block_placed;

// TetrisBlock curr_block;

// TetrisBlock next_block;
// TetrisBlock next_block_two;
// TetrisBlock next_block_three;

// int saved_blocks[WIDTH][HEIGHT];

// int score;

// unsigned long frame_count;
// unsigned long tetris_game_delay;
// unsigned long tetris_last_delay;

// void tetris() {
//     if (new_game)
//         initTetris();

//     checkController();

//     if (millis() - tetris_last_delay >= tetris_game_delay) {
//         if (!new_game) {
//             if (butt_c)
//                 new_game = true;

//             tetris_last_delay = millis();

//             setTetrisMaxAndMins();
//             moveTetronimo();
//             tetris_dir = 0;
//             if (frame_count % 2 == 0 && tetris_rotate) {
//                 rotateShape();
//                 delay(20);
//                 tetris_rotate = false;
//             }

//             if (frame_count % 4 == 0 || joy_y < 120)
//                 fall();

//             FastLED.clear(false);

//             deleteRow();
//             loadBlocks();
//             drawTetrisWalls();
//             drawShape();

//             FastLED.show();

//             frame_count++;
//         } else {
//             idle = millis();
//         }
//     }
// }

// void initTetris() {
//     FastLED.clear(true);

//     srand(millis());

//     frame_count = 0;
//     score = 0;

//     max_y = -1;
//     max_x = -1;

//     min_x = WIDTH;
//     min_y = HEIGHT;

//     tetris_dir = 0;
//     tetris_rotate = false;

//     row_completed = false;
//     block_placed = false;

//     curr_block = TetrisBlock(17, 9, rand() % 7);
//     next_block = TetrisBlock(8, 23, rand() % 7);
//     next_block_two = TetrisBlock(8, 15, rand() % 7);
//     next_block_three = TetrisBlock(8, 7, rand() % 7);

//     memset(&saved_blocks, -1, sizeof(saved_blocks[0][0]) * NUM_LEDS);

//     tetris_game_delay = 110;
//     tetris_last_delay = millis();

//     new_game = false;
// }

// void checkController() {
//     if (joy_x < 50)
//         tetris_dir = -1;
//     else if (joy_x > 200)
//         tetris_dir = 1;

//     if (butt_z)
//         tetris_rotate = true;
// }

// void moveTetronimo() {
//     if (tetris_dir == -1 && min_x > GAME_WIDTH_LEFT && !checkMultiBlockCollision(curr_block.x, curr_block.y, 0, -1))
//         for (int i = 0; i < 4; i++)
//             curr_block.x[i]--;
//     else if (tetris_dir == 1 && max_x < GAME_WIDTH_RIGHT && !checkMultiBlockCollision(curr_block.x, curr_block.y, 0, 1))
//         for (int i = 0; i < 4; i++)
//             curr_block.x[i]++;
// }

// void rotateShape() {
//     int tempArrX[4];
//     int tempArrY[4];

//     for (int i = 3; i >= 0; i--) {
//         // Sets origin at initX and initY
//         int tempX = curr_block.x[i] - curr_block.x[0];
//         int tempY = curr_block.y[i] - curr_block.y[0];
//         // Makes old y coord negative
//         int temp = -tempY;
//         // Flips x and y
//         tempY = tempX;
//         tempX = temp;

//         // Adds initX and initY back
//         tempArrX[i] = tempX + curr_block.x[0];
//         tempArrY[i] = tempY + curr_block.y[0];

//         // Checks wall & block collision and shifts player block
//         if (curr_block.y[0] - min_y == 2 && (curr_block.x[0] == GAME_WIDTH_RIGHT || checkSingleBlockCollision(curr_block.x[0], curr_block.y[0], 0, 1)))
//             tempArrX[i] -= 2;
//         else if (max_y - curr_block.y[0] == 2 && (curr_block.x[0] == GAME_WIDTH_LEFT || checkSingleBlockCollision(curr_block.x[0], curr_block.y[0], 0, -1)))
//             tempArrX[i] += 2;
//         else if ((curr_block.y[0] - min_y == 1 && (curr_block.x[0] == GAME_WIDTH_RIGHT || checkSingleBlockCollision(curr_block.x[0], curr_block.y[0], 0, 1))) || (curr_block.y[0] - min_y == 2 && (curr_block.x[0] == GAME_WIDTH_RIGHT - 1 || checkSingleBlockCollision(curr_block.x[0], curr_block.y[0], 0, 2))))
//             tempArrX[i] -= 1;
//         else if ((max_y - curr_block.y[0] == 1 && (curr_block.x[0] == GAME_WIDTH_LEFT || checkSingleBlockCollision(curr_block.x[0], curr_block.y[0], 0, -1))) || (max_y - curr_block.y[0] == 2 && (curr_block.x[0] == GAME_WIDTH_LEFT + 1 || checkSingleBlockCollision(curr_block.x[0], curr_block.y[0], 0, -2))))
//             tempArrX[i] += 1;
//     }
//     // Double checks final rotated position after shifting x - positions
//     if (!checkMultiBlockCollision(tempArrX, tempArrY, 0, 0) && checkRotationInBounds(tempArrX, tempArrY)) {
//         for (int i = 0; i < 4; i++) {
//             curr_block.x[i] = tempArrX[i];
//             curr_block.y[i] = tempArrY[i];
//         }
//     }
// }

// void setTetrisMaxAndMins() {
//     max_x = -1;
//     max_y = -1;
//     min_x = 30;
//     min_y = 30;
//     for (int i = 0; i < 4; i++) {
//         max_y = std::max(max_y, curr_block.y[i]);
//         min_y = std::min(min_y, curr_block.y[i]);
//         max_x = std::max(max_x, curr_block.x[i]);
//         min_x = std::min(min_x, curr_block.x[i]);
//     }
// }

// void fall() {
//     if (max_y < GAME_HEIGHT_BOTTOM && !checkMultiBlockCollision(curr_block.x, curr_block.y, 1, 0)) {
//         for (int i = 0; i < 4; i++)
//             curr_block.y[i]++;
//     } else {
//         saveBlock();
//         drawShape();
//         loadBlocks();
//         block_placed = true;

//         curr_block = TetrisBlock(17, 9, next_block.block_type);
//         next_block = TetrisBlock(8, 23, next_block_two.block_type);
//         next_block_two = TetrisBlock(8, 15, next_block_three.block_type);
//         next_block_three = TetrisBlock(8, 7, rand() % 7);
//     }
// }

// // False if no blocks collide, true if at least one block will collide
// bool checkMultiBlockCollision(const int mX[], const int mY[], const int &y, const int &x) {
//     for (int i = 0; i < 4; i++)
//         if (checkSingleBlockCollision(mX[i], mY[i], y, x))
//             return true;
//     return false;
// }

// // False if no collision, true if there is a collision
// bool checkSingleBlockCollision(const int &sX, const int &sY, const int &y, const int &x) {
//     if (saved_blocks[sX + x][sY + y] != -1)
//         return true;
//     return false;
// }

// bool checkRotationInBounds(const int x[], const int y[]) {
//     for (int i = 0; i < 4; i++) {
//         if (x[i] < GAME_WIDTH_LEFT || x[i] > GAME_WIDTH_RIGHT || y[i] < GAME_HEIGHT_TOP || y[i] >= GAME_HEIGHT_BOTTOM)
//             return false;
//     }
//     return true;
// }

// void deleteRow() {
//     // Search each row
//     for (int j = GAME_HEIGHT_BOTTOM; j >= 0; j--) {
//         // Search each column
//         for (int i = GAME_WIDTH_LEFT; i <= GAME_WIDTH_RIGHT; i++) {
//             // Break column search loop if there is an empty dot in the row
//             if (saved_blocks[i][j] == -1)
//                 break;
//             // If i reaches the end, then that means there is a full row
//             else if (i == GAME_WIDTH_RIGHT) {
//                 // Set the target row values to -1
//                 score += 10;
//                 row_completed = true;
//                 for (int k = 0; k <= GAME_WIDTH_RIGHT; k++)
//                     saved_blocks[k][j] = -1;
//                 // Shift everything above down one
//                 for (int l = GAME_HEIGHT_BOTTOM; l > 0; l--)
//                     if (l < j)
//                         for (int m = 0; m <= GAME_WIDTH_RIGHT; m++)
//                             saved_blocks[m][l + 1] = saved_blocks[m][l];
//             }
//         }
//     }
// }

// void drawTetrisWalls() {
//     for (int i = 4; i < 27; i++) {
//         leds[convertXY(i, 4)] = CHSV(255, 0, 255);
//         leds[convertXY(i, 26)] = CHSV(255, 0, 255);
//         leds[convertXY(4, i)] = CHSV(255, 0, 255);
//         leds[convertXY(12, i)] = CHSV(255, 0, 255);
//         leds[convertXY(26, i)] = CHSV(255, 0, 255);
//     }
//     for (int i = 13; i < 26; i++)
//         leds[convertXY(i, 9)] = CHSV(255, 0, 100);
// }

// void drawShape() {
//     for (int i = 0; i < 4; i++) {
//         leds[convertXY(curr_block.x[i], curr_block.y[i])] = CHSV(curr_block.color, 255, 255);
//         leds[convertXY(next_block.x[i], next_block.y[i])] = CHSV(next_block.color, 255, 255);
//         leds[convertXY(next_block_two.x[i], next_block_two.y[i])] = CHSV(next_block_two.color, 255, 255);
//         leds[convertXY(next_block_three.x[i], next_block_three.y[i])] = CHSV(next_block_three.color, 255, 255);
//     }
// }

// void loadBlocks() {
//     for (int i = 0; i <= GAME_WIDTH_RIGHT; i++) {
//         for (int j = 0; j <= GAME_HEIGHT_BOTTOM; j++) {
//             if (saved_blocks[i][j] != -1)
//                 leds[convertXY(i, j)] = CHSV(saved_blocks[i][j], 255, 255);
//         }
//     }
// }

// void saveBlock() {
//     for (int k = 0; k < 4; k++) {
//         for (int j = 0; j <= GAME_HEIGHT_BOTTOM; j++) {
//             if (curr_block.y[k] == j) {
//                 for (int i = 0; i <= GAME_WIDTH_RIGHT; i++) {
//                     if (curr_block.x[k] == i) {
//                         saved_blocks[i][j] = curr_block.color;
//                         score++;
//                     }
//                 }
//             }
//         }
//         if (curr_block.y[k] == GAME_HEIGHT_TOP) {
//             new_game = true;
//         }
//     }
// }
// } // namespace Tetris
