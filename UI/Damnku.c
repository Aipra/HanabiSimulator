/*
 * @file Hello.c
 * @author Team ChiHeShui
 * @brief CHS
 * @version Alpha 0.1
 * @date 2020-12-29
 *
 * @copyright Copyright (c) 2020
 *
 */


#include "stdio.h"
#include "surface.h"
#include "fly.h"
#include "win.h"
#include "judge.h"

int main(int argc, char* argv[])
{

    if(SDL_Init(SDL_INIT_VIDEO)<0)
        printf("SDL��ʼ��ʧ�ܣ�������-1");

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
            break;
        }
    }


    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}