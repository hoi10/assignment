#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <vector>
#include "../include/defs.h"
#include "../include/obstacle.h"

void initAudio() ;
void cleanAudio() ;
bool checkCollision(SDL_Rect a, SDL_Rect b) ;
void runGame(SDL_Renderer* renderer);
void cleanUp();
bool checkGameOver(SDL_Renderer* renderer, SDL_Rect playerRect, std::vector<Obstacle>& obstacles, int& score, float& obstacleSpeed, bool& quit) ;
void renderScore(SDL_Renderer* renderer, TTF_Font* font, int score, int level, SDL_Color labelColor, SDL_Color valueColor) ;
#endif