#ifndef _CHAR__H
#define _CHAR__H

const int PLAYER_WIDTH = 117;
const int PLAYER_HEIGHT = 141;
const int GROUND_Y = SCREEN_HEIGHT - PLAYER_HEIGHT - 80;
const int JUMP_HEIGHT = 150;
const float GRAVITY = 0.8f;

extern float obstacleSpeed; 
const int OBSTACLE_WIDTH = 85;
const int OBSTACLE_HEIGHT = 85;
const float SPEED_INCREMENT = 0.01f;
const int OBSTACLE_Y = SCREEN_HEIGHT - OBSTACLE_HEIGHT - 100;

const float BG_SPEED = 3.0f;
#endif