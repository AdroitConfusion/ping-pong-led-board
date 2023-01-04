#include "..\include\games\connect_four.h"

namespace ConnectFour {
const int GAME_WIDTH_LEFT = 4;
const int GAME_WIDTH_RIGHT = 25;

const int GAME_HEIGHT_TOP = 5;
const int GAME_HEIGHT_BOTTOM = 26;

int saved_blocks[WIDTH][HEIGHT];

int color;
int curr_block_x[4];
int curr_block_y[4];
int block_count;
int block_dir;
bool block_dropped;

unsigned long frame_count;
unsigned long game_delay;
unsigned long last_delay;

void connectFour() {
    if (new_game)
        initConnectFour();

    checkController();

    if (millis() - last_delay >= game_delay) {
        if (!new_game) {
            if (butt_c)
                new_game = true;

            last_delay = millis();

            // Serial.println("Joy X: " + joy_x);

            moveCurrBlock();

            if (butt_z && frame_count % 2 == 0 && saved_blocks[curr_block_x[1]][curr_block_y[1] + 4] == -1)
                dropCurrBlock();

            FastLED.clear(false);

            drawSavedBlocks();
            drawCurrBlock();
            drawWalls();

            FastLED.show();

            frame_count++;
        }
    }
}

void initConnectFour() {
    FastLED.clear(true);

    memset(&saved_blocks, -1, sizeof(saved_blocks[0][0]) * NUM_LEDS);

    color = 0;
    resetBlock();
    block_count = 0;
    block_dir = 0;
    block_dropped = false;

    frame_count = 0;
    game_delay = 300;
    last_delay = millis();

    new_game = false;
}

void checkController() {
    if (joy_x < 50)
        block_dir = -1;
    else if (joy_x > 200)
        block_dir = 1;

    if (butt_z)
        block_dropped = true;
}

void moveCurrBlock() {
    if (block_dir == -1 && curr_block_x[0] - 3 > GAME_WIDTH_LEFT)
        for (int &i : curr_block_x)
            i -= 3;
    else if (block_dir == 1 && curr_block_x[3] + 3 < GAME_WIDTH_RIGHT)
        for (int &i : curr_block_x)
            i += 3;
    block_dir = 0;
}

void clearCurrBlock() {
    for (int i = 0; i < 4; i++)
        leds[convertXY(curr_block_x[i], curr_block_y[i])] = 0;
}

void drawCurrBlock() {
    for (int i = 0; i < 4; i++)
        leds[convertXY(curr_block_x[i], curr_block_y[i])] = CHSV(color, 255, 255);
}

void drawWalls() {
    for (int j = 5; j < 26; j++)
        for (int i = 4; i < 26; i += 3)
            leds[convertXY(i, j)] = CHSV(255, 0, 120);

    for (int j = 5; j < 27; j += 3)
        for (int i = 4; i < 26; i++)
            leds[convertXY(i, j)] = CHSV(255, 0, 120);
    // for (int j = GAME_HEIGHT_TOP; j <= GAME_HEIGHT_BOTTOM; j++)
    //     for (int i = GAME_WIDTH_LEFT; i <= GAME_WIDTH_RIGHT; i += 3)
    //         leds[convertXY(i, j)] = CHSV(255, 0, 120);
    // for (int j = GAME_HEIGHT_TOP; j <= GAME_HEIGHT_BOTTOM; j += 3)
    //     for (int i = GAME_WIDTH_LEFT; i <= GAME_WIDTH_RIGHT; i++)
    //         leds[convertXY(i, j)] = CHSV(255, 0, 120);
}

void dropCurrBlock() {

    FastLED.clear(false);

    

    drawSavedBlocks();

    while (curr_block_y[1] < GAME_HEIGHT_BOTTOM - 1 && saved_blocks[curr_block_x[1]][curr_block_y[1] + 2] == -1) {
        clearCurrBlock();
        
        for (int i = 0; i < 4; i++)
            curr_block_y[i] += 1;

        drawCurrBlock();
        drawWalls();
        FastLED.show();
        delay(50);
    }

    saveCurrBlock();

    block_dropped = false;


    if ((checkWinCondition(3, 0) + checkWinCondition(-3, 0)) >= 3 || (checkWinCondition(0, 3) + checkWinCondition(0, -3)) >= 3 || (checkWinCondition(3, 3) + checkWinCondition(-3, -3)) >= 3 || (checkWinCondition(-3, 3) + checkWinCondition(3, -3)) >= 3)
        new_game = true;
    else {
        if (color == 0)
            color = 140;
        else
            color = 0;
    }
    block_count++;
    resetBlock();
}

void saveCurrBlock() {
    for (int k = 0; k < 4; k++) {
        saved_blocks[curr_block_x[k]][curr_block_y[k]] = color;
    }
}

void resetBlock() {
    curr_block_x[0] = 5;
    curr_block_x[1] = 5;
    curr_block_x[2] = 6;
    curr_block_x[3] = 6;

    curr_block_y[0] = 1;
    curr_block_y[1] = 2;
    curr_block_y[2] = 1;
    curr_block_y[3] = 2;
}

void drawSavedBlocks() {
    for (int i = 0; i <= GAME_WIDTH_RIGHT; i++) {
        for (int j = 0; j <= GAME_HEIGHT_BOTTOM; j++) {
            if (saved_blocks[i][j] != -1)
                leds[convertXY(i, j)] = CHSV(saved_blocks[i][j], 255, 255);
        }
    }
}

int checkWinCondition(int init_x, int init_y) {
    int adj_block_count = 0;
    for (int i = 1; i < 4; i++) {
        int offset_x = curr_block_x[0] + i * init_x;
        int offset_y = curr_block_y[0] + i * init_y;
        if (offset_x >= 0 && offset_x <= WIDTH && offset_y >= 0 && offset_y <= HEIGHT && saved_blocks[offset_x][offset_y] == color)
            adj_block_count++;
        else
            break;
    }
    return adj_block_count;
}
} // namespace ConnectFour
