#include "../include/menu.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <vector>
#include <ctime>
#include "../include/defs.h"
#include "../include/char.h"


int showGameOverDialog(SDL_Renderer* renderer, int score, int totalScore) {
    SDL_Texture* menuTex = IMG_LoadTexture(renderer, "../imagine/over.png");

    TTF_Font* font = TTF_OpenFont("../Font/SF-Pro.ttf", 40);
    SDL_Color white = {255, 255, 255};
    SDL_Color yellow = {255, 255, 0};
    SDL_Color hoverColor = {255, 100, 100};
    SDL_Color red = {255, 0, 0, 255};

    bool win = score >= 10;

    SDL_Surface* titleSurface = TTF_RenderText_Solid(font, win ? "YOU WIN!" : "GAME OVER", yellow);
    SDL_Texture* titleTex = SDL_CreateTextureFromSurface(renderer, titleSurface);
    SDL_Rect titleRect = { SCREEN_WIDTH / 2 - titleSurface->w / 2, 150, titleSurface->w, titleSurface->h };

    SDL_Texture* scoreTex = nullptr;
    SDL_Rect scoreRect;
    if (!win) {
        std::string scoreText = "Level Score: " + std::to_string(score);
        SDL_Surface* scoreSurface = TTF_RenderText_Solid(font, scoreText.c_str(), white);
        scoreTex = SDL_CreateTextureFromSurface(renderer, scoreSurface);
        scoreRect = { SCREEN_WIDTH / 2 - scoreSurface->w / 2, 230, scoreSurface->w, scoreSurface->h };
        SDL_FreeSurface(scoreSurface);
    }

    SDL_Texture* totalTex = nullptr;
    SDL_Rect totalRect;
    if (!win) {
        std::string totalText = "Total Score: " + std::to_string(totalScore);
        SDL_Surface* totalSurface = TTF_RenderText_Solid(font, totalText.c_str(), red);
        totalTex = SDL_CreateTextureFromSurface(renderer, totalSurface);
        totalRect = { SCREEN_WIDTH / 2 - totalSurface->w / 2, 280, totalSurface->w, totalSurface->h };
        SDL_FreeSurface(totalSurface);
    }

    SDL_Color playColor = white;
    SDL_Color quitColor = white;

    SDL_Surface* playSurface = TTF_RenderText_Solid(font, win ? "Next Level" : "Again", playColor);
    SDL_Texture* playTex = SDL_CreateTextureFromSurface(renderer, playSurface);
    SDL_Rect playRect = { SCREEN_WIDTH / 2 - 80, win ? 290 : 360, 160, 45 };

    SDL_Surface* quitSurface = TTF_RenderText_Solid(font, "Exit", quitColor);
    SDL_Texture* quitTex = SDL_CreateTextureFromSurface(renderer, quitSurface);
    SDL_Rect quitRect = { SCREEN_WIDTH / 2 - 80, win ? 350 : 440, 160, 45 };

    int choice = -1;
    SDL_Event e;
    bool selecting = true;

    while (selecting) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                choice = 0;
                selecting = false;
            }
            if (e.type == SDL_MOUSEMOTION) {
                int x = e.motion.x, y = e.motion.y;
                playColor = (x >= playRect.x && x <= playRect.x + playRect.w &&
                             y >= playRect.y && y <= playRect.y + playRect.h) ? hoverColor : white;
                quitColor = (x >= quitRect.x && x <= quitRect.x + quitRect.w &&
                             y >= quitRect.y && y <= quitRect.y + quitRect.h) ? hoverColor : white;

                SDL_FreeSurface(playSurface);
                SDL_DestroyTexture(playTex);
                playSurface = TTF_RenderText_Solid(font, win ? "Next level" : "Again", playColor);
                playTex = SDL_CreateTextureFromSurface(renderer, playSurface);

                SDL_FreeSurface(quitSurface);
                SDL_DestroyTexture(quitTex);
                quitSurface = TTF_RenderText_Solid(font, "Exit", quitColor);
                quitTex = SDL_CreateTextureFromSurface(renderer, quitSurface);
            }
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x = e.button.x, y = e.button.y;
                if (x >= playRect.x && x <= playRect.x + playRect.w &&
                    y >= playRect.y && y <= playRect.y + playRect.h) {
                    choice = 1;
                    selecting = false;
                }
                if (x >= quitRect.x && x <= quitRect.x + quitRect.w &&
                    y >= quitRect.y && y <= quitRect.y + quitRect.h) {
                    choice = 0;
                    selecting = false;
                }
            }
        }

        SDL_RenderClear(renderer);
        if (menuTex) SDL_RenderCopy(renderer, menuTex, NULL, NULL);
        SDL_RenderCopy(renderer, titleTex, NULL, &titleRect);
        if (scoreTex) SDL_RenderCopy(renderer, scoreTex, NULL, &scoreRect);
        if (!win && totalTex != nullptr)
        SDL_RenderCopy(renderer, totalTex, NULL, &totalRect);
        SDL_RenderCopy(renderer, playTex, NULL, &playRect);
        SDL_RenderCopy(renderer, quitTex, NULL, &quitRect);
        SDL_RenderPresent(renderer);
    }

    SDL_FreeSurface(titleSurface);
    SDL_FreeSurface(playSurface);
    SDL_FreeSurface(quitSurface);
    SDL_DestroyTexture(titleTex);
    SDL_DestroyTexture(scoreTex);
    SDL_DestroyTexture(playTex);
    SDL_DestroyTexture(quitTex);
    if (totalTex) SDL_DestroyTexture(totalTex);
    if (menuTex) SDL_DestroyTexture(menuTex);
    TTF_CloseFont(font);

    return choice;
}
