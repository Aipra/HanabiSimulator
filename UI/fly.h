/*
* ��Ҫ��Ϊ��д�л���boss��mobs��������card�������Ի���player��
* ��һЩ�ṹ�壨������������
*/


#ifndef _FLY_
#define _FLY_
#include "SDL2/SDL_image.h"
#include "SDL/SDL.h"

extern struct player {
	SDL_Rect rect_player;//����ĳ�ʼλ��
	SDL_Rect rect_back[3][8];//3Ϊ�����˶���8Ϊ8��״̬
	SDL_Texture* er;//�������ͣ��ⲿתTexture��
}player;
#define speed_x 10
#define speed_y 10
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