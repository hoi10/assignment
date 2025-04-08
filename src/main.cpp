#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <vector>
#include <ctime>
#include "../include/defs.h"
#include "../include/char.h"
#include "../include/menu.h"
#include "../include/over.h"
#include "../include/game.h"

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init(); 

    SDL_Window* window = SDL_CreateWindow("jump", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!showMenu(renderer)) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return 0;
    }

    initAudio();

    runGame(renderer);

    void cleanAudio() ;

    void cleanUp();

    return 0; 
} 
// g++ -std=c++11 -o game main.cpp game.cpp menu.cpp over.cpp -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer && ./game