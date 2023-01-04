#include "..\include\games\bird.h"

namespace Bird {
    const int WALL_WIDTH = 2;
    const int WALL_GAP = 8;
    const int WALL_SIZE = (WIDTH - WALL_GAP) * WALL_WIDTH;
    const int BIRD_ROWS = 2;
    const int BIRD_COLS = 12;
    const int BIRD_STAR_POS[BIRD_COLS][BIRD_ROWS] = {{7, 2}, {6, 1}, {5, 1}, {5, 2}, {4, 2}, {3, 2}, {2, 3}, {3, 4}, {4, 4}, {5, 4}, {6, 3}};


    int wall_arr[WALL_SIZE][2];
    int top_wall_height;

    int bird_arr[BIRD_COLS][BIRD_ROWS];
    int bird_dir;

    int frame_delay;

    unsigned long frame_count;
    unsigned long last_delay;
    unsigned long game_delay;

    void bird(const int &joy_x, const int &joy_y, const bool &butt_z, const bool &butt_c, bool &new_game) {
        if (new_game)
            initBird(new_game);
        
        checkController(joy_y);

        while (millis() - last_delay >= game_delay) {
            if (!new_game) {
                if (butt_c)
                    new_game = true;

                last_delay = millis();

                moveBird();
                checkBirdCollision(new_game);

                FastLED.clear(false);

                drawBird();
                drawWall();

                FastLED.show();

                frame_count++;
            }
        }
    }

    void initBird(bool &new_game) {
        FastLED.clear(true);

        srand(millis());

        std::copy(&BIRD_STAR_POS[0][0], &BIRD_STAR_POS[0][0] + BIRD_COLS * BIRD_ROWS, &bird_arr[0][0]);
        frame_count = 0;
        bird_dir = 0;

        last_delay = millis();
        game_delay = 60;

        new_game = false;
    }

    void checkController(const int &joy_y) {
        if (joy_y < 110)
            bird_dir = -1;
        else if (joy_y > 135)
            bird_dir = 1;
    }


    void moveBird() {
        if (bird_dir == 1 && bird_arr[0][1] > 2) {
            for (int i = 0; i < 11; i++)
                bird_arr[i][1]--;
        } else if (bird_dir == -1 && bird_arr[0][1] < HEIGHT - 3) { 
            for (int i = 0; i < 11; i++)
                bird_arr[i][1]++;
        }

        bird_dir = 0;
    }

    void drawBird() {
        int frameRate = 8;
        int frameMod = frame_count % frameRate;
        int fade = frameMod * 255 / frameRate;

        // Wing flapping animation
        if (frame_count % 2 == 0) {
            bird_arr[4][1]++;
            bird_arr[5][1]++;
            leds[convertXY(bird_arr[0][0] - 2, bird_arr[0][1] + 1)] = CRGB(255, 30, 0);
        } else {
            bird_arr[4][1]--;
            bird_arr[5][1]--;
        }

        // Bird eye 
        leds[convertXY(bird_arr[0][0] - 1, bird_arr[0][1])] = CRGB(255, 255, 0);

        for (int i = 0; i < 11; i++) {
            leds[convertXY(bird_arr[i][0], bird_arr[i][1])] = CRGB(255, 70, 0);
        }
        leds[convertXY(bird_arr[4][0], bird_arr[4][1])] = CRGB(255, 30, 0);
        leds[convertXY(bird_arr[5][0], bird_arr[5][1])] = CRGB(255, 30, 0);
    }

    void drawWall() {
        int wallRound = ((int)frame_count / 2) % (WIDTH + WALL_WIDTH - 1);
        if (wallRound == 0)
            top_wall_height = rand() % 14 + 1;

        for (int w = 0; w < WALL_WIDTH; w++) {
            // Draws upper wall_arr
            for (int i = 0; i < top_wall_height; i++) {
                wall_arr[i + (w * (WIDTH - WALL_GAP))][0] = WIDTH - 1 - wallRound + w;
                wall_arr[i + (w * (WIDTH - WALL_GAP))][1] = i;
            }
            // Draws lower wall_arr
            for (int i = top_wall_height; i < WIDTH - WALL_GAP; i++) {
                wall_arr[i + (w * (WIDTH - WALL_GAP))][0] = WIDTH - 1 - wallRound + w;
                wall_arr[i + (w * (WIDTH - WALL_GAP))][1] = i + 8;
            }
        }

        drawArray(wall_arr, 0, 0, WALL_SIZE, 120);
    }

    void checkBirdCollision(bool& new_game) {
        for (int b = 0; b < 11; b++) {
            for (int w = 0; w < WALL_SIZE; w++) {
                if (convertXY(bird_arr[b][0], bird_arr[b][1]) == convertXY(wall_arr[w][0], wall_arr[w][1]))
                    new_game = true;
            }
        }
    }
}
