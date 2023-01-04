#include "..\include\games\breakout.h"

#define TOTAL_NUM_BLOCKS 27

const int GAME_OVER_Y = 24;
const int BREAKOUT_WIDTH_RIGHT = 28;
const int BREAKOUT_WIDTH_LEFT = 1;

int breakout_score;
int breakout_frame;

BreakoutBlock block_arr[TOTAL_NUM_BLOCKS];

int ball_x;
int ball_y;
int ball_dx;
int ball_dy;

int padd_x[4];
int padd_y;
int padd_dir;
int ball_frame;

int prev_ball_x_one;
int prev_ball_y_one;
int prev_ball_x_two;
int prev_ball_y_two;

unsigned long breakout_last_delay;
unsigned long breakout_game_delay;

void breakout(const int &joy_x, const int &joy_y, const bool &butt_idx, const bool &butt_c, bool &new_game) {

    if (new_game)
        initBreakout(new_game);

    breakoutCheckController(joy_x, joy_y);

    while (millis() - breakout_last_delay >= breakout_game_delay) {
        if (!new_game) {
            if (ball_y == GAME_OVER_Y || butt_c)
                new_game = true;

            breakout_last_delay = millis();

            movePaddle(joy_x);
            

            // must be even and positive
            int breakout_frame_rate = 2;
            int breakout_frame_mod = breakout_frame % breakout_frame_rate;
            int fade = breakout_frame_mod * 255 / breakout_frame_rate;

            // if (breakout_frame % 2 == 0)

            // Set previous ball positions, check area around ball, change direction if need be, and move ball
            if (breakout_frame_mod == 0) {
                prev_ball_x_one = ball_x;
                prev_ball_y_one = ball_y;
                prev_ball_x_two = prev_ball_x_one - ball_dx;
                prev_ball_y_two = prev_ball_y_one + ball_dy;

                checkBreakoutBall(joy_x);
                moveBreakoutBall();
            }

            FastLED.clear(false);

            // Draw ball
            leds[convertXY(prev_ball_x_two, prev_ball_y_two)] = CHSV(235, 255, 255 - fade);
            leds[convertXY(prev_ball_x_one, prev_ball_y_one)] = CHSV(235, 255, 255);
            leds[convertXY(ball_x, ball_y)] = CHSV(235, 255, fade);

            drawPaddle();
            drawBreakoutWalls();

            loadBreakoutBlocks();
            if (numberOfBreakoutBlocksDestroyed())
                breakout_score += 5;

            FastLED.show();

            breakout_frame++;
        } else {
            idle = millis();
        }
    }
}

void initBreakout(bool &new_game) {
    FastLED.clear(true);

    srand(millis());

    breakout_score = 0;
    breakout_frame = 0;
    for (int i = 0; i < 4; i++)
        padd_x[i] = i + 7;
    padd_y = 20;
    padd_dir = 0;

    saveBreakoutBlocks();
    resetBreakoutBall();

    breakout_last_delay = millis();
    breakout_game_delay = 100;

    new_game = false;
}

void breakoutCheckController(const int &joy_x, const int &joy_y) {
    if (joy_x < 50)
        padd_dir = -1;
    else if (joy_x > 200)
        padd_dir = 1;
}

void movePaddle(const int &joy_x) {
    if (padd_dir == -1 && padd_x[0] > BREAKOUT_WIDTH_LEFT)
        for (int i = 0; i < 4; i++)
            padd_x[i] -= 1;

    if (padd_dir == 1 && padd_x[3] < BREAKOUT_WIDTH_RIGHT)
        for (int i = 0; i < 4; i++)
            padd_x[i] += 1;
    padd_dir = 0;
}

void drawPaddle() {
    for (int i = 0; i < 4; i++)
        leds[convertXY(padd_x[i], padd_y)] = (255, 255, 255);
}

void drawBreakoutWalls() {
    for (int i = 0; i < HEIGHT - 1; i++) {
        // Right wall
        leds[convertXY(WIDTH - 2, i + 1)] = CHSV(255, 0, 255);
        // Left wall
        leds[convertXY(0, i + 1)] = CHSV(255, 0, 255);
        // Top wall
        leds[convertXY(i, 0)] = CHSV(255, 0, 255);
    }
    for (int i = 1; i < HEIGHT - 2; i++) {
        // Game over line
        leds[convertXY(i, GAME_OVER_Y)] = CHSV(5, 200, 200);
    }
}

void resetBreakoutBall() {
    for (int i = 0; i < 4; i++)
        padd_x[i] = i + 7;

    ball_x = padd_x[2];
    ball_y = padd_y - 2;
    ball_dx = 0;
    ball_dy = -1;
    ball_frame = 0;
}

void moveBreakoutBall() {
    ball_x += ball_dx;
    ball_y -= ball_dy;
}

void checkBreakoutBall(const int &joy_x) {
    // Checks up, down, left, and right to see if ball is hitting block
    int checkUp = checkBreakoutBlock(0, -1);
    int checkDown = checkBreakoutBlock(0, 1);
    int checkLeft = checkBreakoutBlock(-1, 0);
    int checkRight = checkBreakoutBlock(1, 0);
    int checkTopLeft = checkBreakoutBlock(-1, -1);
    int checkTopRight = checkBreakoutBlock(1, -1);
    int checkBotLeft = checkBreakoutBlock(-1, 1);
    int checkBotRight = checkBreakoutBlock(1, 1);
    // Detects if ball is hitting paddle and change direction if so
    if (ball_y == padd_y - 1) {
        for (int i = 0; i < 4; i++) {
            if (ball_x == padd_x[i])
                ball_dy *= -1;
        }
        if (ball_x == padd_x[0])
            ball_dx = -1;
        else if (ball_x == padd_x[1]) {
            if (joy_x < 120)
                ball_dx--;
            else if (ball_dx == 0)
                ball_dx = -1;
        } else if (ball_x == padd_x[2]) {
            if (joy_x > 135)
                ball_dx++;
            else if (ball_dx == 0)
                ball_dx = 1;
        } else if (ball_x == padd_x[3])
            ball_dx = 1;
        else if ((ball_x == padd_x[0] - 1 && ball_dx == 1) || (ball_x == padd_x[3] + 1 && ball_dx == -1)) {
            ball_dx *= -1;
            ball_dy *= -1;
        }

        if (ball_dx > 1)
            ball_dx = 1;
        else if (ball_dx < -1)
            ball_dx = -1;
    }
    // Detect if ball hitting walls and change direction if so
    if (ball_x > BREAKOUT_WIDTH_RIGHT || ball_x < BREAKOUT_WIDTH_LEFT)
        ball_dx *= -1;
    if (ball_y >= 29 || ball_y <= 1)
        ball_dy *= -1;

    // Detect if ball hitting block and change direction if so
    if (checkUp != -1 && ball_dy > 0) {
        changeScoreAndBlockColor(checkUp);
        ball_dy *= -1;
    }
    if (checkDown != -1 && ball_dy < 0) {
        changeScoreAndBlockColor(checkDown);
        ball_dy *= -1;
    }
    if (checkLeft != -1 && ball_dx < 0) {
        changeScoreAndBlockColor(checkLeft);
        ball_dx *= -1;
    }
    if (checkRight != -1 && ball_dx > 0) {
        changeScoreAndBlockColor(checkRight);
        ball_dx *= -1;
    } else if (checkTopLeft != -1 && ball_dx < 0 && ball_dy > 0) {
        changeScoreAndBlockColor(checkTopLeft);
        ball_dy *= -1;
        ball_dx *= -1;
    } else if (checkTopRight != -1 && ball_dx > 0 && ball_dy > 0) {
        changeScoreAndBlockColor(checkTopRight);
        ball_dy *= -1;
        ball_dx *= -1;
    } else if (checkBotLeft != -1 && ball_dx < 0 && ball_dy < 0) {
        changeScoreAndBlockColor(checkBotLeft);
        ball_dy *= -1;
        ball_dx *= -1;
    } else if (checkBotRight != -1 && ball_dx > 0 && ball_dy < 0) {
        changeScoreAndBlockColor(checkBotRight);
        ball_dx *= -1;
        ball_dy *= -1;
    }
}

void saveBreakoutBlocks() {
    int idx = 0;
    int color;

    for (int y = 0; y < 3; y++) {
        switch (y) {
        case 0:
            color = 0;
            break;
        case 1:
            color = 15;
            break;
        case 2:
            color = 50;
            break;
        default:
            break;
        }
        for (int x = 1; x <= 25; x += 3) {
            block_arr[idx++].setBlockXY(x, 3 + y * 4, color);
        }
    }
}

void loadBreakoutBlocks() {
    for (int idx = 0; idx < TOTAL_NUM_BLOCKS; idx++) {
        if (block_arr[idx].color != -1) {
            for (int i = 0; i < 6; i++) {
                leds[convertXY(block_arr[idx].block_x[i], block_arr[idx].block_y[i])] = CHSV(block_arr[idx].color, 255, 255);
            }
        }
    }
}

int checkBreakoutBlock(const int &bX, const int &bY) {
    for (int idx = 0; idx < TOTAL_NUM_BLOCKS; idx++) {
        for (int i = 0; i < 6; i++) {
            if (block_arr[idx].color != -1 && block_arr[idx].block_x[i] == ball_x + bX && block_arr[idx].block_y[i] == ball_y + bY) {
                return idx;
            }
        }
    }
    return -1;
}

bool numberOfBreakoutBlocksDestroyed() {
    int num_blocks = 0;
    for (int idx = 0; idx < TOTAL_NUM_BLOCKS; idx++) {
        if (block_arr[idx].color == -1)
            num_blocks++;
    }
    // Reset game if all blocks destroyed
    if (num_blocks == TOTAL_NUM_BLOCKS) {
        saveBreakoutBlocks();
        resetBreakoutBall();
        return true;
    }
    return false;
}

void changeScoreAndBlockColor(const int &check) {
    breakout_score += 1;
    switch (block_arr[check].color) {
    case 50:
        block_arr[check].color = -1;
        break;
    case 15:
        block_arr[check].color = 50;
        break;
    case 0:
        block_arr[check].color = 15;
        break;
    default:
        break;
    }
}