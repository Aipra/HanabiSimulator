/*
 * @file Hello.c
 * @author ChiHeShui
 * @brief CHS
 * @version 0.0.0.1
 * @date 2020-12-09
 *
 * @copyright Copyright (c) 2020
 *
 */


#include <stdio.h>
#include <SDL_image.h>
#include <SDL.h>
#include "fly.h"
#include "surface.h"
#include "win.h"
#include "judge.h"

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("chiheshui",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        window_width, window_hight,
        SDL_WINDOW_SHOWN
    );

    SDL_Event event;

    bool Uquit = true;
    while (Uquit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) Uquit = false;

        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}