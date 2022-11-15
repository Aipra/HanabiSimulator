//
// Created by Aipra on 2022/11/10.
//

#ifndef LOGICAL_H
#define LOGICAL_H
#ifdef __cplusplus
extern "C"{
#endif
#include <SDL/SDL.h>
#include <stdio.h>

#define OK 1
#define ERROR -1
    typedef int status;
    //逻辑处理函数测试
    status logic_test(SDL_Window*, SDL_Surface*, SDL_Event*);
#ifdef __cplusplus
}
#endif
#endif //LOGICAL_H
