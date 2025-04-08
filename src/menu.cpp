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

bool showMenu(SDL_Renderer* renderer) {
    SDL_Texture* menuTex = IMG_LoadTexture(renderer, "../imagine/menu.png");
    if (!menuTex) {
        std::cout << "Lỗi load menu.png: " << IMG_GetError() << std::endl;
        return false;
    }

    TTF_Font* font = TTF_OpenFont("../Font/SF-Pro.ttf", 40);
    if (!font) {
        std::cout << "Lỗi tải font: " << TTF_GetError() << std::endl;
        return false;
    }

    SDL_Color textColor = {255, 255, 255};
    SDL_Surface* titleSurface = TTF_RenderText_Solid(font, "ONLY JUMP", textColor);
    SDL_Texture* titleTextTex = SDL_CreateTextureFromSurface(renderer, titleSurface);

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, "START", textColor);
    SDL_Texture* startTextTex = SDL_CreateTextureFromSurface(renderer, textSurface);
    
    SDL_Rect titleRect = {SCREEN_WIDTH / 2 - titleSurface->w / 2, SCREEN_HEIGHT / 2 - 100, titleSurface->w, titleSurface->h};
    SDL_Rect startButton = { SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2, 100, 50 };

    bool inMenu = true;
    SDL_Event e;

    while (inMenu) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) return false;
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX = e.button.x;
                int mouseY = e.button.y;
                if (mouseX >= startButton.x && mouseX <= startButton.x + startButton.w &&
                    mouseY >= startButton.y && mouseY <= startButton.y + startButton.h) {
                    inMenu = false;
                }
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, menuTex, NULL, NULL);
        SDL_RenderCopy(renderer, titleTextTex, NULL, &titleRect);
        SDL_RenderCopy(renderer, startTextTex, NULL, &startButton);
        SDL_RenderPresent(renderer);
    }

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(startTextTex);
    SDL_DestroyTexture(menuTex);
    TTF_CloseFont(font);

    return true;
}