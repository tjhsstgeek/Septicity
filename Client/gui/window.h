#ifndef __WINDOW_H__
#define __WINDOW_H__
#include "component.h"

extern component *create_window(int,int,unsigned char *);
extern component *create_window_fixed(int,int,int,int,unsigned char *);
extern void render_window(component *);
extern void window_mouse_move(struct gui_component *,int,int,char);
extern void window_mouse_press(struct gui_component *,int,int,char);
extern void window_mouse_release(struct gui_component *,int,int,char);
extern void window_mouse_leave(struct gui_component *,int,int,char);

#endif
