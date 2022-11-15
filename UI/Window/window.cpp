//
// Created by Aipra on 2022/11/10.
//
# include "window.h"
// 绘图函数测试
status draw_test(SDL_Surface* screen_surface){
    SDL_FillRect( screen_surface, NULL, SDL_MapRGB( screen_surface->format, 0xFF, 0xFF, 0xFF ) );
}
//更新窗口上的所有表面
status update_window(SDL_Window* window){
    SDL_UpdateWindowSurface( window );
}

