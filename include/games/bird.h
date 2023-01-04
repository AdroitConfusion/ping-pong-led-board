#include "draw.h"

namespace Bird {
    void bird(const int &joy_x, const int &joy_y, const bool &butt_z, const bool &butt_c, bool &new_game);
    void initBird(bool &new_game);
    void checkController(const int &joy_y);

    void moveBird();

    void drawBird();
    void drawWall();
    void checkBirdCollision(bool& new_game);
}