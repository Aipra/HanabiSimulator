/*
* 主要是为了写敌机（boss和mobs）符卡（card）还有自机（player）
* 的一些结构体（参数）及函数
*/


#ifndef _FLY_
#define _FLY_
#include <SDL_image.h>
#include <SDL.h>
extern struct player {
	SDL_Rect rect_player;//人物的初始位置
	SDL_Rect rect_back[3][8];//3为左右运动，8为8种状态
	SDL_Texture* er;//人物造型，外部转Texture用
}player;

#define speed_x 3
#define speed_y 5
#ifdef __cplusplus
extern "C"{
#endif//__cplusplus

	typedef struct player PLAYER;

	extern struct player fly_player_init(struct SDL_Renderer* rend);
	extern void fly_player_update(struct SDL_Renderer* rend, union SDL_Event event, struct player* p);

	extern void move_player(struct player* p, SDL_Event event);
	//extern void fly_card(struct SDL_Rect position);



#ifdef __cplusplus
}
#endif



#endif