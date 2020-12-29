#include "./include/fly.h"
#include "./include/win.h"
#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>

//���������ͼƬ��ַ
char* player_image[100] = { "./res/a1.png","./res/a2.png","./res/a3.png" };

struct player fly_player_init(struct SDL_Renderer* rend) {

    struct player p;
    //���Ŀǰ��0�ţ�
    SDL_Surface* i_p = IMG_Load(player_image[0]);
    p.er = SDL_CreateTextureFromSurface(rend,i_p);
    //����λ�ã����¼�λʱ�ı��������
    p.rect_player.x = 360;
    p.rect_player.y = 360;
    p.rect_player.w = i_p->w/8;
    p.rect_player.h = i_p->h/5;
    for (int j = 0; j < 3; j++) {
        for (int i = 0; i < 8; i++) {

            p.rect_back[j][i].w = i_p->w / 8;
            p.rect_back[j][i].h = i_p->h / 5;
            //�洢�����˶�״̬��ͼƬ��Ϣ
            p.rect_back[j][i].x = i * p.rect_back[j][i].w;
            p.rect_back[j][i].y = j*47;

        }
    }
        SDL_RenderClear(rend);
        SDL_RenderCopy(rend, p.er, &(p.rect_back[0][0]), &(p.rect_player));//���Ͼ�ֹʱ
        SDL_RenderPresent(rend);//ˢ����Ļ
        SDL_Delay(3);

        //�ӵ�

        return p;
}

//�������Ｐ�ӵ����˶�
void fly_player_update(struct SDL_Renderer* rend,union SDL_Event event,struct player *p) {//ƽ����¼�
    move_player(p,event);

    SDL_RenderClear(rend);
    SDL_RenderCopy(rend, p->er, &(p->rect_back[0][0]), &(p->rect_player));//��ֹ
    SDL_RenderPresent(rend);//����ȥ
    SDL_Delay(3);
}


void move_player(struct player *p,union SDL_Event event) {
    if (event.type == SDL_KEYDOWN)
        switch (event.key.keysym.sym) {
        case'w':if (p->rect_player.y >= 0) { p->rect_player.y -= speed_y; } break;
        case'a':if (p->rect_player.x >= 0) { p->rect_player.x -= speed_x; }break;
        case's':if (p->rect_player.y <= window_hight) { p->rect_player.y += speed_y; }break;
        case'd':if (p->rect_player.x <= window_width) { p->rect_player.x += speed_x; }break;
        case SDLK_UP:if (p->rect_player.y >= 0) { p->rect_player.y -= speed_y; }break;
        case SDLK_LEFT:if (p->rect_player.x >= 0) { p->rect_player.x -= speed_x; }break;
        case SDLK_DOWN:if (p->rect_player.y <= window_hight) { p->rect_player.y += speed_y; }break;
        case SDLK_RIGHT:if (p->rect_player.x <= window_width) { p->rect_player.x += speed_x; }break;
        }
}//���������ƶ�