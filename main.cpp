#include "Window/window.hpp"
#include "Logical/logical.hpp"


int main(int argc, char* argv[]);

int main(int argc, char* argv[])
{
//    if(argc == 0) return ERROR;
    SDL_Window *window=NULL;
    SDL_Surface *screen_surface=NULL;

    // 初始化SDL，创建窗口
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        exit(ERROR);
    }else{
        // 创建窗口
        window = SDL_CreateWindow( "HanabiSimulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN );
        if( window == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            exit(ERROR);
        }else{
            screen_surface = SDL_GetWindowSurface( window );
        }
    }

    // 绘制表面
    draw_test(screen_surface);
    // 更新表面
    update_window(window);
    // 接受输入
    SDL_Event e;
    int quit=0;
    while (!quit){
        while(SDL_PollEvent(&e)){
            logic_test(window, screen_surface, &e);
            if(e.type==SDL_QUIT){
                quit=1;
            }
        }
    }

    //销毁窗口
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}