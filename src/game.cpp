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
#include "../include/obstacle.h"

Mix_Music* music = NULL;
Mix_Chunk* jump = NULL;
Mix_Chunk* over = NULL;
Mix_Chunk* next = NULL;

void initAudio() {
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    music = Mix_LoadMUS("../sound/background.mp3");
    jump = Mix_LoadWAV("../sound/jump.mp3");
    next = Mix_LoadWAV("../sound/next_level.mp3"); 
    over = Mix_LoadWAV("../sound/over.mp3");
}

void cleanAudio() {
    Mix_FreeMusic(music);
    Mix_FreeChunk(jump);
    Mix_FreeChunk(next);
    Mix_FreeChunk(over);
    Mix_CloseAudio();
}

bool checkCollision(SDL_Rect a, SDL_Rect b) {
    return (a.x < b.x + b.w && a.x + a.w > b.x &&
            a.y < b.y + b.h && a.y + a.h > b.y); 
}

float obstacleSpeed = 5.0f;
int score = 0, totalScore = 0 ; 
int level = 1 ;
SDL_Color red = {255, 0, 0, 255};
SDL_Color white = {255, 255, 255, 255};

bool checkGameOver(SDL_Renderer* renderer, SDL_Rect playerRect, std::vector<Obstacle>& obstacles, int& score, float& obstacleSpeed, bool& quit) {
    for (auto& ob : obstacles) {
        SDL_Rect obstacleRect = {ob.x, ob.y, OBSTACLE_WIDTH, OBSTACLE_HEIGHT};
        if (checkCollision(playerRect, obstacleRect)) {
            totalScore = 10*(level-1) + score ;
            Mix_HaltMusic();
            Mix_PlayChannel(-1, over, 0);
            int choice = showGameOverDialog(renderer, score, totalScore );
            if (choice == 1 ) {
                level = 1 ;
                score = 0;
                totalScore = 0 ;
                obstacleSpeed = 5.0f;
                runGame(renderer); 
            } else {
                quit = true;
            }
            return true;
        }
    }

    if (score >= 10) {
        Mix_HaltMusic();
        Mix_PlayChannel(-1, next, 0);
        int choice = showGameOverDialog(renderer, score, totalScore );
        if ( choice == 1) {
            score = 0;
            level ++ ;
            runGame(renderer); 
        } else {
            quit = true;
        }
        return true;
    }

    return false;
}

SDL_Color labelColor = {255, 255, 255};  
SDL_Color valueColor = {255, 255, 0};    

void renderScore(SDL_Renderer* renderer, TTF_Font* font, int score, int level, SDL_Color labelColor, SDL_Color valueColor) {
    SDL_Surface* levelLabelSurface = TTF_RenderText_Solid(font, "Level: ", labelColor);
    SDL_Surface* levelValueSurface = TTF_RenderText_Solid(font, std::to_string(level).c_str(), valueColor);

    SDL_Texture* levelLabelTexture = SDL_CreateTextureFromSurface(renderer, levelLabelSurface);
    SDL_Texture* levelValueTexture = SDL_CreateTextureFromSurface(renderer, levelValueSurface);

    SDL_Rect levelLabelRect = {10, 10, levelLabelSurface->w, levelLabelSurface->h};
    SDL_Rect levelValueRect = {10 + levelLabelSurface->w, 10, levelValueSurface->w, levelValueSurface->h};

    SDL_RenderCopy(renderer, levelLabelTexture, NULL, &levelLabelRect);
    SDL_RenderCopy(renderer, levelValueTexture, NULL, &levelValueRect);

    SDL_Surface* scoreLabelSurface = TTF_RenderText_Solid(font, "Score: ", labelColor);
    SDL_Surface* scoreValueSurface = TTF_RenderText_Solid(font, std::to_string(score).c_str(), valueColor);

    SDL_Texture* scoreLabelTexture = SDL_CreateTextureFromSurface(renderer, scoreLabelSurface);
    SDL_Texture* scoreValueTexture = SDL_CreateTextureFromSurface(renderer, scoreValueSurface);

    SDL_Rect scoreLabelRect = {10, 20 + levelLabelSurface->h, scoreLabelSurface->w, scoreLabelSurface->h};
    SDL_Rect scoreValueRect = {10 + scoreLabelSurface->w, 20 + levelLabelSurface->h, scoreValueSurface->w, scoreValueSurface->h};

    SDL_RenderCopy(renderer, scoreLabelTexture, NULL, &scoreLabelRect);
    SDL_RenderCopy(renderer, scoreValueTexture, NULL, &scoreValueRect);

    SDL_FreeSurface(levelLabelSurface);
    SDL_FreeSurface(levelValueSurface);
    SDL_DestroyTexture(levelLabelTexture);
    SDL_DestroyTexture(levelValueTexture);

    SDL_FreeSurface(scoreLabelSurface);
    SDL_FreeSurface(scoreValueSurface);
    SDL_DestroyTexture(scoreLabelTexture);
    SDL_DestroyTexture(scoreValueTexture);
}

void runGame(SDL_Renderer* renderer){
    TTF_Font* font = TTF_OpenFont("../Font/SF-Pro.ttf", 24);
    SDL_Texture* bg = IMG_LoadTexture(renderer, "../imagine/bg2.png");
    SDL_Texture* playerTex = IMG_LoadTexture(renderer, "../imagine/player.png");
    SDL_Texture* obstacleTex = IMG_LoadTexture(renderer, "../imagine/obstacle.png");
    if (music) Mix_PlayMusic(music, -1); 
    
    float bgX = 0;
    int playerY = GROUND_Y;
    bool isJumping = false;
    float velocityY = 0;
    std::vector<Obstacle> obstacles;
    srand(time(0));

    Uint32 lastSpawnTime = SDL_GetTicks();
    Uint32 lastFrameTime = SDL_GetTicks();
    bool quit = false;
    SDL_Event e;

    while (!quit) {
        Uint32 currentFrameTime = SDL_GetTicks();
        float deltaTime = (currentFrameTime - lastFrameTime) / 1000.0f;
        lastFrameTime = currentFrameTime;

        if (SDL_GetTicks() - lastSpawnTime > 5000) {  
            int numNewObstacles = 1 + rand() % 2; 
            for (int i = 0; i < numNewObstacles; i++) {  
                int offsetY = (rand() % 50) - 25; 
                obstacles.emplace_back(SCREEN_WIDTH + i * 50, OBSTACLE_Y + offsetY);  
            }  
            lastSpawnTime = SDL_GetTicks();  
        } 

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = true;
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE && !isJumping) {
                Mix_PlayChannel(-1, jump, 0);
                isJumping = true;
                velocityY = -17;
            }
        }

        bgX -= BG_SPEED * deltaTime * 60;
        if (bgX <= -SCREEN_WIDTH) bgX = 0;

        if (isJumping) {
            playerY += velocityY;
            velocityY += GRAVITY;
            if (playerY >= GROUND_Y) {
                playerY = GROUND_Y;
                isJumping = false;
            }
        }

        if (SDL_GetTicks() - lastSpawnTime > 2000) {
            obstacles.emplace_back(SCREEN_WIDTH, OBSTACLE_Y);
            lastSpawnTime = SDL_GetTicks();
        }

        obstacleSpeed += SPEED_INCREMENT * deltaTime * 60;
        for (auto &ob : obstacles) ob.x -= obstacleSpeed;
        if (!obstacles.empty() && obstacles[0].x < -OBSTACLE_WIDTH) obstacles.erase(obstacles.begin());

        for (auto &ob : obstacles) {
            if (!ob.passed && ob.x + OBSTACLE_WIDTH < 500) {
                ob.passed = true;
                score++;
            } 
        }

        
        SDL_RenderClear(renderer);
        SDL_Rect bg1 = { static_cast<int>(bgX), 0, SCREEN_WIDTH, SCREEN_HEIGHT };
        SDL_Rect bg2 = { static_cast<int>(bgX) + SCREEN_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
        SDL_RenderCopy(renderer, bg, nullptr, &bg1);
        SDL_RenderCopy(renderer, bg, nullptr, &bg2);

        SDL_Rect playerDst = {500, playerY, PLAYER_WIDTH, PLAYER_HEIGHT};
        SDL_RenderCopy(renderer, playerTex, nullptr, &playerDst);

        for (auto &ob : obstacles) {
            SDL_Rect obDst = {ob.x, ob.y, OBSTACLE_WIDTH, OBSTACLE_HEIGHT};
            SDL_RenderCopy(renderer, obstacleTex, nullptr, &obDst);
        }
 
        
        renderScore(renderer, font, score, level, labelColor, valueColor);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);

        SDL_Rect playerRect = {500, playerY, PLAYER_WIDTH, PLAYER_HEIGHT};
        if (checkGameOver(renderer, playerRect, obstacles, score, obstacleSpeed, quit)) break;

    }
}

void cleanUp(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* bg, SDL_Texture* playerTex, SDL_Texture* obstacleTex, TTF_Font* font){
    SDL_DestroyTexture(bg);
    SDL_DestroyTexture(playerTex);
    SDL_DestroyTexture(obstacleTex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}