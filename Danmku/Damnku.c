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
#include "./include/surface.h"
#include "./include/fly.h"
#include "./include/win.h"
#include "./include/judge.h"

int main(int argc, char* argv[])
{

    if(SDL_Init(SDL_INIT_VIDEO)<0)
        printf("SDL³õÊ¼»¯Ê§°Ü£¬´íÎóÂë-1");

    SDL_Window* window;
    SDL_Renderer* rend;

    window = SDL_CreateWindow("chiheshui",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        window_width, window_hight,
        SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE
    );

    rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);



    SDL_Event event;
    _Bool Uquit = 1;

    struct player players;
    players=fly_player_init(rend);

    while (Uquit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) Uquit = 0;
            fly_player_update(rend,event,&players);
        }
    }


    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}