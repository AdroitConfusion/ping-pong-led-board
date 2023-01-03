#include "..\include\games\snake.h"

int snakeX[NUM_LEDS];
int snakeY[NUM_LEDS];
int snakeLength;
int appleX;
int appleY;
int snakeDirection; //snake direction has integer values of degrees in circle
int lastDirection;
bool snakeGameOver;
int snakeColor;

unsigned long snake_last_delay;
unsigned long snake_game_delay;


void snake(const int& joy_x, const int& joy_y, const bool& butt_z, const bool& butt_c, bool& new_game) {  
    if (new_game)
        initSnake(new_game);
    
    checkController(joy_x, joy_y);
        
    if (millis() - snake_last_delay >= snake_game_delay) {
        if (!snakeGameOver) {   
            if (butt_c)
                snakeGameOver = true;

            snake_last_delay = millis();


            moveSnake();
            checkSnake();
            changeDelayAndColor();

            FastLED.clear(false);
            // Draw apple
            leds[convertXY(appleX,appleY)] = CHSV(255,255,255);
            // Draw snake
            for (int i = 1; i < snakeLength; i++)
                leds[convertXY(snakeX[i], snakeY[i])] = CHSV(snakeColor,255,255);
            //Color snake head
            leds[convertXY(snakeX[0], snakeY[0])] = CRGB(255,255,255);

        } else {
            new_game = true;
            idle = millis();
        }
    }
}

// Define variables for new game of snake
void initSnake(bool& new_game) {
    FastLED.clear(true);

    srand(millis());

    snakeX[0] = 14;
    snakeY[0] = 14;
    snakeLength = 5;
    
    snakeDirection = 0;
    lastDirection = 0;
    snakeGameOver = false;
    snakeColor = 100;

    appleX = 0;
    appleY = 14;

    snake_game_delay = 300;
    snake_last_delay = millis();
    
    new_game = false;
}

// Determine direction of snake head based on nunchuk joystick
void checkController(const int& joy_x, const int& joy_y) {
    if (joy_x < 50 && lastDirection != 0)
        snakeDirection = 180;
    else if (joy_x  > 200 && lastDirection != 180)
        snakeDirection = 0;
    else if (joy_y > 200 && lastDirection != 90)
        snakeDirection = 270;
    else if (joy_y < 50 && lastDirection != 270)
        snakeDirection = 90;
}

void moveSnake() {
    // Assign new snake segments
    for (int i = snakeLength - 1; i > 0; i--) {
        snakeX[i] = snakeX[i - 1];
        snakeY[i] = snakeY[i - 1];
    }

    // Move snake head in the given direction
    switch (snakeDirection) {
        case 180:
            snakeX[0]--;
            break;
        case 0:
            snakeX[0]++;
            break;
        case 90:
            snakeY[0]++;
            break;
        case 270:
            snakeY[0]--;
            break;
        default:
            break;
    }
    lastDirection = snakeDirection;

    // If touching a wall, teleport to opposite wall
    if (snakeX[0] < 0)
        snakeX[0] = WIDTH - 1;
    else if (snakeX[0] > WIDTH - 1)
        snakeX[0] = 0;

    if (snakeY[0] < 0)
        snakeY[0] = HEIGHT - 1;
    else if (snakeY[0] > HEIGHT - 1)
        snakeY[0] = 0;
}

void checkSnake() {
    // Determine if snake head has eaten the apple
    if (snakeX[0] == appleX && snakeY[0] == appleY) {
        snakeLength++;
        boolean appleRespawn = true;
        while (appleRespawn) {
            appleX = rand() % WIDTH;
            appleY = rand() % HEIGHT;
        //      Serial.printf("%d, %d\n", appleX, appleY);
            appleRespawn = false;
            for (int i = 0; i < snakeLength; i++) {
                if (snakeX[i] == appleX && snakeY[i] == appleY)
                    appleRespawn = true;
            }
        }
    }

    // Detect if the snake head is biting itself
    for (int i = 1; i < snakeLength; i++) {
        if (snakeX[i] == snakeX[0] && snakeY[i] == snakeY[0]) 
            snakeGameOver = true;
    }
}

//Change snake color and speed as the game goes on
void changeDelayAndColor() {
    if (snakeLength < 8) {
        snake_game_delay = 300;
        snakeColor = 100;
    }
    else if (snakeLength < 11) {
        snake_game_delay = 200;
        snakeColor = 120;
    }
    else if (snakeLength < 14) {
        snake_game_delay = 150;
        snakeColor = 140;
    }
    else if (snakeLength < 17) {
        snake_game_delay = 100;
        snakeColor = 160;
    }
    else if (snakeLength < 20) {
        snake_game_delay = 50;
        snakeColor = 180;
    }
    else {
        snake_game_delay = 25;
        snakeColor += 20;
    }
}