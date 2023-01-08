#include "..\include\games\space_invaders.h"

namespace SpaceInvaders {

int score;

int spaceShipX[6];
int spaceShipColor;
int spaceShipDot;

bool ShipCollision;

int numRounds;

int shootFrameCount;

Turret turret;

Alien alien_arr[NUM_ALIENS];
int num_alien_bullets;
int num_aliens_alive;
int Alien::animation_counter = 0;
int Alien::dir = 1;
std::vector<Bullet> Alien::bullet_vec;
int Alien::max_bullets;
int aliens_max_x;
int aliens_min_x;
int aliens_max_y;
int aliens_min_y;

Barrier barrier_arr[NUM_BARRIERS];

unsigned long frame_count;
unsigned long last_delay;
unsigned long game_delay;

void spaceInvaders() {
    if (new_game)
        initSpaceInvaders();

    checkController();

    while (millis() - last_delay >= game_delay) {
        if (!new_game) {
            if (butt_c)
                new_game = true;

            last_delay = millis();

            turret.moveTurret();
            if (butt_z && frame_count % 10 == 0)
                turret.addBullet();
            turret.moveBullets();

            if (frame_count % 5 == 0) {
                animateAliens();
            }
            if (frame_count % 10 == 0) {
                moveAliens();
                chooseRandomAlienAndShoot();
            }

            Alien::moveBullets();

            FastLED.clear(false);

            drawTurret();
            drawBarriers();
            drawAliens();

            FastLED.show();

            frame_count++;
        }
    }
}

void initSpaceInvaders() {
    FastLED.clear(true);

    srand(millis());

    // Reset turret
    turret.resetTurret(11, 27);
    turret.setLives(TURRET_NUM_LIVES);

    // Reset barriers
    int idx = 0;
    for (int i = 3; i <= 21; i += 9)
        barrier_arr[idx++].resetBarrier(i, 23);

    // Reset aliens
    for (int row = 0; row < ALIENS_ROWS; row++) {
        for (int col = 0; col < ALIENS_COLS; col++) {
            int idx = row * ALIENS_COLS + col;
            alien_arr[idx].resetAlien(1 + col * 4, row * 3, row * 15);
        }
    }
    getAliensMaxAndMins();
    num_aliens_alive = NUM_ALIENS;
    Alien::max_bullets = 5;

    last_delay = millis();
    game_delay = 100;

    new_game = false;
}

void checkController() {
    if (joy_x < 110)
        turret.turret_dir = -1;
    else if (joy_x > 135)
        turret.turret_dir = 1;
}

void drawTurret() {
    for (int i = 1; i < 4; i++) {
        leds[convertXY(turret.x_arr[i], turret.y_arr[i])] = CHSV(160 - 15 * (TURRET_NUM_LIVES - turret.lives), 255 - 15 * (TURRET_NUM_LIVES - turret.lives), 255);
    }
    leds[convertXY(turret.x_arr[0], turret.y_arr[0])] = CHSV(160, 255, 255);

    for (const Bullet &bullet : turret.bullet_vec) {
        leds[convertXY(bullet.curr_x, bullet.curr_y)] = bullet.color;
    }
}

void drawBarriers() {
    for (Barrier &barrier : barrier_arr) {
        barrier.drawBarrier();
    }
}

void drawAliens() {
    for (Alien &alien : alien_arr) {
        if (alien.visible) {
            for (int idx = 0; idx < ALIEN_SIZE; idx++) {
                leds[convertXY(alien.x_arr[idx], alien.y_arr[idx])] = CHSV(alien.alien_color, 255, 255);
            }
        }
    }
    for (Bullet &bullet : Alien::bullet_vec) {
        leds[convertXY(bullet.curr_x, bullet.curr_y)] = bullet.color, 255, 255;
    }
}

void getAliensMaxAndMins() {

    aliens_max_y = GAME_HEIGHT_TOP;
    aliens_min_y = GAME_HEIGHT_BOTTOM;

    aliens_max_x = GAME_WIDTH_LEFT;
    aliens_min_x = GAME_WIDTH_RIGHT;

    for (Alien &alien : alien_arr) {
        if (alien.visible) {
            aliens_max_y = std::max(aliens_max_y, alien.max_y);
            aliens_min_y = std::min(aliens_min_y, alien.min_y);
            aliens_max_x = std::max(aliens_max_x, alien.max_x);
            aliens_min_x = std::min(aliens_min_x, alien.min_x);
        }
    }
}

void animateAliens() {
    for (Alien &alien : alien_arr)
        if (alien.visible)
            alien.setXY();
    Alien::animation_counter++;
}

void moveAliens() {
    int change_max;
    for (Alien &alien : alien_arr) {
        if (aliens_max_x < GAME_WIDTH_RIGHT && Alien::dir == 1) {
            for (int idx = 0; idx < ALIEN_SIZE; idx++)
                alien.x_arr[idx]++;
            change_max = 0;
        } else if (aliens_min_x > GAME_WIDTH_LEFT && Alien::dir == -1) {
            for (int idx = 0; idx < ALIEN_SIZE; idx++)
                alien.x_arr[idx]--;
            change_max = 1;
        } else {
            for (int idx = 0; idx < ALIEN_SIZE; idx++)
                alien.y_arr[idx]++;
            change_max = 2;
        }
    }
    switch (change_max) {
    case 0:
        aliens_max_x++;
        aliens_min_x++;
        break;
    case 1:
        aliens_max_x--;
        aliens_min_x--;
        break;
    case 2:
        aliens_max_y++;
        aliens_min_y++;
        Alien::dir *= -1;
        break;
    default:
        break;
    }
}

bool checkBarrierCollision(Bullet &bullet) {
    if (leds[convertXY(bullet.curr_x, bullet.curr_y)] != CRGB(0, 0, 0)) {
        for (Barrier &barrier : barrier_arr) {
            for (int idx = 0; idx < BARRIER_SIZE; idx++) {
                if (bullet.curr_x == barrier.x_arr[idx] && bullet.curr_y == barrier.y_arr[idx] && barrier.colors[idx] > END_COLOR) {
                    barrier.colors[idx] -= COLOR_CHANGE_ON_HIT;
                    return true;
                }
            }
        }
    }
    return false;
}

bool checkAlienCollision(Bullet &bullet) {
    for (Alien &alien : alien_arr) {
        if (alien.visible) {
            for (int idx = 0; idx < ALIEN_SIZE; idx++) {
                if (bullet.curr_x == alien.x_arr[idx] && bullet.curr_y == alien.y_arr[idx]) {
                    alien.visible = false;
                    getAliensMaxAndMins();
                    return true;
                }
            }
        }
    }
    return false;
}

bool checkTurretCollision(Bullet &bullet) {
    for (int idx = 0; idx < TURRET_SIZE; idx++) {
        if (bullet.curr_x == turret.x_arr[idx] && bullet.curr_y == turret.y_arr[idx]) {
            if (turret.lives == 1)
                new_game = true;
            else
                turret.lives--;
            return true;
        }
    }
    return false;
}

void chooseRandomAlienAndShoot() {
    int idx = rand() % NUM_ALIENS;
    while (!alien_arr[idx].visible) {
        idx = rand() % NUM_ALIENS;
    }
    alien_arr[idx].addBullet();
}

// void drawSpaceShip(int counter) {
//     for (int i = 0; i < 6; i++) {
//         if (i == 0 || i == 4)
//             leds[convertXY(spaceShipX[i], i % 2)] = CHSV(spaceShipColor, 255, 100);
//         else
//             leds[convertXY(spaceShipX[i], i % 2)] = CHSV(spaceShipColor, 255, 255);
//     }
//     leds[convertXY(spaceShipX[0] + spaceShipDot % 3, 1)] = CHSV(254, 255, 255);
// }

// void resetSpaceShip() {
//     spaceShipX[0] = 0;
//     spaceShipX[1] = 0;
//     spaceShipX[2] = 1;
//     spaceShipX[3] = 1;
//     spaceShipX[4] = 2;
//     spaceShipX[5] = 2;
//     spaceShipColor = -1;
// }

// void moveSpaceShip() {
//     if (spaceShipX[5] > GAME_WIDTH_RIGHT)
//         resetSpaceShip();
//     else {
//         spaceShipColor = 0;
//         for (int i = 0; i < 6; i++)
//             spaceShipX[i]++;
//     }
// }

// bool checkSpaceShipCollision() {
//     if (spaceShipX[0] <= PointX && spaceShipX[4] >= PointX && (PointY == 0 || PointY == 1) && spaceShipColor != -1) {
//         resetSpaceShip();
//         score += 5;
//         return true;
//     }
//     return false;
// }

// void moveAlienBullets() {
//     for (int i = 0; i < shootingAliensNum; i++) {
//         if (alien_arr[i].bullet_visible)
//             alien_arr[i].shootAlienBullet(checkBarrierCollision(alien_arr[i].bullet_x, alien_arr[i].bullet_y), GAME_HEIGHT_TOP);
//     }
// }

// void drawAlienBullets() {
//     for (int i = 0; i < shootingAliensNum; i++) {
//         if (alien_arr[i].bullet_visible)
//             leds[convertXY(alien_arr[i].bullet_x, alien_arr[i].bullet_y)] = CHSV(0, 0, 255);
//     }
// }

// void goToNextRound() {
//     frameCount = 0;
//     projectileVisible = false;
//     animation_counter = 0;
//     moveLeft = false;

//     for (int idx = 0; idx < NUM_ALIENS; idx++) {
//         alien_arr[idx].setXY(10 * (int)(idx / 7), 3 + (4 * idx % NUM_ALIENS), (int)(idx / 7) * 4);
//         alien_arr[idx].setAlienBulletXY();
//         alien_arr[idx].bullet_visible = false;
//     }
// }

// int checkAllAliensPVisible() {
//     int num = 0;
//     for (int i = 0; i < shootingAliensNum; i++) {
//         if (alien_arr[i].bullet_visible)
//             num++;
//     }
//     return num;
// }
} // namespace SpaceInvaders