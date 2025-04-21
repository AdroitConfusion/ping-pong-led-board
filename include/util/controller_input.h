#ifndef CONTROLLER_INPUT_H
#define CONTROLLER_INPUT_H

// Struct for passing controller input cleanly
struct ControllerInput {
    int joy_x;
    int joy_y;
    bool butt_z;
    bool butt_c;
};

#endif // CONTROLLER_INPUT_H