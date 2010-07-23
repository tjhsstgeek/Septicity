#ifndef __BUTTON_H__
#define __BUTTON_H__
#include "component.h"

typedef struct {
	unsigned char *text;
	char inside;
	char pressed;
} button_data;

extern component *create_button(int,int,unsigned char *);
extern component *create_button_fixed(int,int,int,int,unsigned char *);
extern void render_button(component *);
extern void button_mouse_click(struct gui_component *,int,int,char);
extern void button_mouse_press(struct gui_component *,int,int,char);
extern void button_mouse_enter(struct gui_component *,int,int,char);
extern void button_mouse_leave(struct gui_component *,int,int,char);
extern void button_blur(struct gui_component *,int,int,char);
#endif
