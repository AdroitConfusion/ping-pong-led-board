#include "../include/games/breakout.h"

namespace Breakout {

BreakoutGame::BreakoutGame() {
    initGame();
}

void BreakoutGame::update(const ControllerInput &input) {
    if (new_game) {
        initGame();
        return;
    }

    checkController(input);

    if (millis() - last_delay >= game_delay) {
        if (ball_y == GAME_OVER_Y || input.butt_c) {
            new_game = true;
            return;
        }

        last_delay = millis();

        movePaddle();

        uint8_t rate = 2;
        uint8_t mod = frame % rate;
        uint8_t fade = mod * 255 / rate;

        if (mod == 0) {
            prev_x1 = ball_x;
            prev_y1 = ball_y;
            prev_x2 = prev_x1 - ball_dx;
            prev_y2 = prev_y1 + ball_dy;

            checkBallCollision(input);
            moveBall();
        }

        FastLED.clear(false);

        leds[convertXY(prev_x2, prev_y2)] = CHSV(235, 255, 255 - fade);
        leds[convertXY(prev_x1, prev_y1)] = CHSV(235, 255, 255);
        leds[convertXY(ball_x, ball_y)] = CHSV(235, 255, fade);

        drawPaddle();
        drawWalls();
        loadBlocks();

        if (checkWinCondition()) {
            score += 5;
        }

        FastLED.show();
        frame++;
    }
}

void BreakoutGame::initGame() {
    FastLED.clear(true);
    srand(millis());

    score = 0;
    frame = 0;

    for (uint8_t i = 0; i < 4; ++i) {
        padd_x[i] = i + 7;
    }
    padd_y = 20;
    padd_dir = 0;

    saveBlocks();
    resetBall();

    last_delay = millis();
    game_delay = 100;
    new_game = false;
}

void BreakoutGame::checkController(const ControllerInput &input) {
    if (input.joy_x < 50)
        padd_dir = -1;
    else if (input.joy_x > 200)
        padd_dir = 1;
}

void BreakoutGame::movePaddle() {
    if (padd_dir == -1 && padd_x[0] > WIDTH_LEFT)
        for (uint8_t &x : padd_x)
            x -= 1;
    if (padd_dir == 1 && padd_x[3] < WIDTH_RIGHT)
        for (uint8_t &x : padd_x)
            x += 1;
    padd_dir = 0;
}

void BreakoutGame::drawPaddle() {
    for (uint8_t i = 0; i < 4; ++i)
        leds[convertXY(padd_x[i], padd_y)] = CHSV(0, 0, 255);
}

void BreakoutGame::drawWalls() {
    for (uint8_t i = 0; i < HEIGHT - 1; ++i) {
        leds[convertXY(WIDTH - 2, i + 1)] = CHSV(255, 0, 255);
        leds[convertXY(0, i + 1)] = CHSV(255, 0, 255);
        leds[convertXY(i, 0)] = CHSV(255, 0, 255);
    }
    for (uint8_t i = 1; i < HEIGHT - 2; ++i)
        leds[convertXY(i, GAME_OVER_Y)] = CHSV(5, 200, 200);
}

void BreakoutGame::resetBall() {
    for (uint8_t i = 0; i < 4; ++i)
        padd_x[i] = i + 7;

    ball_x = padd_x[2];
    ball_y = padd_y - 2;
    ball_dx = 0;
    ball_dy = -1;
}

void BreakoutGame::moveBall() {
    ball_x += ball_dx;
    ball_y -= ball_dy;
}

void BreakoutGame::checkBallCollision(const ControllerInput &input) {
    int up = checkBlock(0, -1);
    int down = checkBlock(0, 1);
    int left = checkBlock(-1, 0);
    int right = checkBlock(1, 0);
    int tl = checkBlock(-1, -1);
    int tr = checkBlock(1, -1);
    int bl = checkBlock(-1, 1);
    int br = checkBlock(1, 1);

    if (ball_y == padd_y - 1) {
        for (uint8_t i = 0; i < 4; ++i) {
            if (ball_x == padd_x[i])
                ball_dy *= -1;
        }
        if (ball_x == padd_x[0])
            ball_dx = -1;
        else if (ball_x == padd_x[1]) {
            if (input.joy_x < 120)
                ball_dx--;
            else if (ball_dx == 0)
                ball_dx = -1;
        } else if (ball_x == padd_x[2]) {
            if (input.joy_x > 135)
                ball_dx++;
            else if (ball_dx == 0)
                ball_dx = 1;
        } else if (ball_x == padd_x[3])
            ball_dx = 1;

        if ((ball_x == padd_x[0] - 1 && ball_dx == 1) || (ball_x == padd_x[3] + 1 && ball_dx == -1)) {
            ball_dx *= -1;
            ball_dy *= -1;
        }

        if (ball_dx > 1)
            ball_dx = 1;
        if (ball_dx < -1)
            ball_dx = -1;
    }

    if (ball_x > WIDTH_RIGHT || ball_x < WIDTH_LEFT)
        ball_dx *= -1;
    if (ball_y >= 29 || ball_y <= 1)
        ball_dy *= -1;

    if (up != -1 && ball_dy > 0) {
        hitBlock(up);
        ball_dy *= -1;
    }
    if (down != -1 && ball_dy < 0) {
        hitBlock(down);
        ball_dy *= -1;
    }
    if (left != -1 && ball_dx < 0) {
        hitBlock(left);
        ball_dx *= -1;
    }
    if (right != -1 && ball_dx > 0) {
        hitBlock(right);
        ball_dx *= -1;
    } else if (tl != -1 && ball_dx < 0 && ball_dy > 0) {
        hitBlock(tl);
        ball_dx *= -1;
        ball_dy *= -1;
    } else if (tr != -1 && ball_dx > 0 && ball_dy > 0) {
        hitBlock(tr);
        ball_dx *= -1;
        ball_dy *= -1;
    } else if (bl != -1 && ball_dx < 0 && ball_dy < 0) {
        hitBlock(bl);
        ball_dx *= -1;
        ball_dy *= -1;
    } else if (br != -1 && ball_dx > 0 && ball_dy < 0) {
        hitBlock(br);
        ball_dx *= -1;
        ball_dy *= -1;
    }
}

void BreakoutGame::saveBlocks() {
    uint8_t idx = 0;
    int8_t color;
    for (uint8_t y = 0; y < 3; y++) {
        color = (y == 0) ? 0 : (y == 1) ? 15
                                        : 50;
        for (uint8_t x = 1; x <= 25; x += 3) {
            blocks[idx++].setBlockXY(x, 3 + y * 4, color);
        }
    }
}

void BreakoutGame::loadBlocks() {
    for (uint8_t i = 0; i < TOTAL_NUM_BLOCKS; ++i) {
        if (blocks[i].color != -1) {
            for (uint8_t j = 0; j < 6; ++j) {
                leds[convertXY(blocks[i].block_x[j], blocks[i].block_y[j])] =
                    CHSV(blocks[i].color, 255, 255);
            }
        }
    }
}

int BreakoutGame::checkBlock(int8_t offset_x, int8_t offset_y) {
    for (uint8_t i = 0; i < TOTAL_NUM_BLOCKS; ++i) {
        for (uint8_t j = 0; j < 6; ++j) {
            if (blocks[i].color != -1 &&
                blocks[i].block_x[j] == ball_x + offset_x &&
                blocks[i].block_y[j] == ball_y + offset_y) {
                return i;
            }
        }
    }
    return -1;
}

bool BreakoutGame::checkWinCondition() {
    uint8_t destroyed = 0;
    for (uint8_t i = 0; i < TOTAL_NUM_BLOCKS; ++i) {
        if (blocks[i].color == -1)
            ++destroyed;
    }
    if (destroyed == TOTAL_NUM_BLOCKS) {
        saveBlocks();
        resetBall();
        return true;
    }
    return false;
}

void BreakoutGame::hitBlock(int idx) {
    score++;
    switch (blocks[idx].color) {
    case 50:
        blocks[idx].color = -1;
        break;
    case 15:
        blocks[idx].color = 50;
        break;
    case 0:
        blocks[idx].color = 15;
        break;
    }
}

} // namespace Breakout
