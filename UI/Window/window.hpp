//
// Created by Aipra on 2022/11/10.
//
// 显示主窗口

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus
// SDL基本绘图库
#include <SDL/SDL.h>
// 标准输入输出
#include <stdio.h>
#ifndef _WIN_
#define _WIN_
//定义窗口大小
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
// 定义执行状态
#define OK 1
#define ERROR -1
typedef int status;
// 绘图函数测试
status draw_test(SDL_Surface*);
//更新窗口上的所有表面
status update_window(SDL_Window*);

// 销毁窗口直接调用SDL自带的SDL_DestroyWindow(SDL_window*)函数即可
#ifdef __cplusplus
}
#endif







#endif
