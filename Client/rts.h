#ifndef __RTS_H__
#define __RTS_H__
#include <SDL/SDL.h>
#include "md3.h"
extern int init();
extern void read_config(char* file);
extern void render();
extern void listen_window();
extern SDL_Surface *window;
extern int width;
extern int height;
extern SDL_VideoInfo* video_info;
extern char fullscreen;
extern void render_screen();
typedef struct _MD3_ITEM {
	MD3 *item;
	struct _MD3_ITEM *next;
} MD3_Item;
MD3_Item *head;
int* actions;
int *ex, *ey, *ez;
float *angle;
#endif
