#include "draw.h"
#include "unordered_set"

namespace SpaceInvaders {
    
const int GAME_WIDTH_LEFT = 0;
const int GAME_WIDTH_RIGHT = 29;

const int GAME_HEIGHT_TOP = 0;
const int GAME_HEIGHT_BOTTOM = 29;

const int ALIENS_ROWS = 4;
const int ALIENS_COLS = 7;
const int NUM_ALIENS = ALIENS_ROWS * ALIENS_COLS;
const int ALIEN_SIZE = 5;
const int MAX_ALIEN_BULLETS = 15;

const int TURRET_SIZE = 4;
const int TURRET_NUM_LIVES = 3;

const int START_COLOR = 110;
const int BARRIER_HITPOINTS = 3;
const int COLOR_CHANGE_ON_HIT = 12;
const int END_COLOR = START_COLOR - BARRIER_HITPOINTS * COLOR_CHANGE_ON_HIT;

const int NUM_BARRIERS = 3;
const int BARRIER_SIZE = 8;

void spaceInvaders();
void initSpaceInvaders();
void checkController();
void drawTurret();
void drawBarriers();
void moveAliens();
void drawAliens();
void getAliensMaxAndMins();
void animateAliens();
void chooseRandomAlienAndShoot();

struct Bullet {
    int curr_x;
    int curr_y;

    CHSV color;
    int dir;

    bool visible;

    Bullet(const int &init_x, const int &init_y, const CHSV &color, const int &dir) {
        this->color = color;
        this->dir = dir;
        curr_x = init_x;
        curr_y = init_y;
        visible = true;
    }

    void moveBullet(const bool &collision) {
        // Reset bullet if it go outs of bounds or hits some entity
        if (curr_y == GAME_HEIGHT_TOP || curr_y == GAME_HEIGHT_BOTTOM || collision)
            visible = false;
        else
            curr_y += dir;
    }
};

bool checkBarrierCollision(Bullet &bullet);
bool checkAlienCollision(Bullet &bullet);
bool checkTurretCollision(Bullet &bullet);

struct Turret {
    int x_arr[TURRET_SIZE];
    int y_arr[TURRET_SIZE];
    int lives;
    int color;

    int &max_x = x_arr[2];
    int &min_x = x_arr[1];
    int &bullet_start_x = x_arr[0];
    int &bullet_start_y = y_arr[3];
    int dir;
    bool shoot;

    std::vector<Bullet> bullet_vec;

    Turret() {}

    void setLives(const int &lives) {
        this->lives = lives;
    }

    void resetTurret(const int &init_x, const int &init_y) {
        x_arr[0] = init_x;
        x_arr[1] = init_x - 1;
        x_arr[2] = init_x + 1;
        x_arr[3] = init_x;

        y_arr[0] = init_y;
        y_arr[1] = init_y;
        y_arr[2] = init_y;
        y_arr[3] = init_y - 1;

        bullet_vec.clear();

        dir = 0;
        shoot = false;
    }

    void moveTurret() {
        if (dir == -1 && min_x > GAME_WIDTH_LEFT) {
            for (int &x : x_arr)
                x--;
        }

        if (dir == 1 && max_x < GAME_WIDTH_RIGHT) {
            for (int &x : x_arr)
                x++;
        }
        dir = 0;
    }

    void addBullet() {
        if (shoot == true)
            bullet_vec.push_back(Bullet(bullet_start_x, bullet_start_y, CHSV(160, 150, 255), -1));
        shoot = false;
    }

    void moveBullets() {
        bullet_vec.erase(std::remove_if(bullet_vec.begin(),
                                        bullet_vec.end(),
                                        [&](const Bullet bullet) -> bool { return !bullet.visible; }),
                         bullet_vec.end());
        for (Bullet &bullet : bullet_vec)
            bullet.moveBullet(checkBarrierCollision(bullet) || checkAlienCollision(bullet));
    }
};

struct Barrier {
    int x_arr[BARRIER_SIZE];
    int y_arr[BARRIER_SIZE];
    int colors[BARRIER_SIZE];

    Barrier() {}

    void resetBarrier(const int &init_x, const int &init_y) {
        for (int i = 0; i <= 5; i++) {
            y_arr[i] = init_y;
            x_arr[i] = init_x + i;
        }
        x_arr[6] = init_x;
        x_arr[7] = init_x + 5;

        y_arr[6] = y_arr[7] = init_y + 1;

        std::fill(colors, colors + BARRIER_SIZE, START_COLOR);
    }

    void drawBarrier() {
        for (int i = 0; i < BARRIER_SIZE; i++)
            if (colors[i] != END_COLOR)
                leds[convertXY(x_arr[i], y_arr[i])] = CHSV(colors[i], 255, 255);
    }
};

struct Alien {
    int x_arr[5];
    int y_arr[5];
    int &max_x = x_arr[4];
    int &min_x = x_arr[3];

    int &max_y = y_arr[1];
    int &min_y = y_arr[3];

    int &bullet_start_x = x_arr[0];
    int &bullet_start_y = y_arr[0];

    int alien_color;
    bool visible;

    static int dir;
    static int animation_counter;
    static std::vector<Bullet> bullet_vec;
    static int max_bullets;

    Alien() {}

    void resetAlien(const int &init_x, const int &init_y, const int &alien_color) {
        // Initialize alien
        this->alien_color = alien_color;
        x_arr[0] = init_x;
        y_arr[0] = init_y;
        visible = true;
        animation_counter = 0;
        dir = 1;
        setXY();

        bullet_vec.clear();

        // Initialize bullet
        // bullet = Bullet(init_x, init_y, 0, 1);
    }

    void setXY() {
        if (animation_counter % 2 == 0) {
            x_arr[1] = x_arr[0] - 1;
            x_arr[2] = x_arr[0] + 1;
            x_arr[3] = x_arr[0] - 1;
            x_arr[4] = x_arr[0] + 1;

            y_arr[1] = y_arr[0] + 1;
            y_arr[2] = y_arr[0] + 1;
            y_arr[3] = y_arr[0];
            y_arr[4] = y_arr[0];
        } else {
            x_arr[1] = x_arr[0];
            x_arr[2] = x_arr[0];
        }
    }

    void addBullet() {
        if (bullet_vec.size() < max_bullets)
            bullet_vec.push_back(Bullet(bullet_start_x, bullet_start_y, CHSV(START_COLOR, 150, 255), 1));
    }

    static void moveBullets() {
        bullet_vec.erase(std::remove_if(bullet_vec.begin(),
                                        bullet_vec.end(),
                                        [&](const Bullet bullet) -> bool { return !bullet.visible; }),
                         bullet_vec.end());

        for (Bullet &bullet : bullet_vec)
            bullet.moveBullet(checkBarrierCollision(bullet) || checkTurretCollision(bullet));
    }
};

} // namespace SpaceInvaders