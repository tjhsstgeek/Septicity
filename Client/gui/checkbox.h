#ifndef __CHECKBOX_H__
#define __CHECKBOX_H__
#include "component.h"

typedef struct {
	char inside;
	char pressed;
	char checked;//These three can be combined into one. We only use zero or one for each
	unsigned char *text;
	int s;
	int *block;
} checkbox_data;


extern component *create_checkbox(int,int,unsigned char *,char);
extern component *create_checkbox_fixed(int,int,int,int,unsigned char *,char);
extern component *create_checkbox_fixed_box(int,int,int,int,int,unsigned char *,char);
extern void render_checkbox(component *);
extern void checkbox_mouse_click(struct gui_component *,int,int,char);
extern void checkbox_mouse_press(struct gui_component *,int,int,char);
extern void checkbox_mouse_enter(struct gui_component *,int,int,char);
extern void checkbox_mouse_leave(struct gui_component *,int,int,char);
extern void checkbox_blur(struct gui_component *,int,int,char);

#endif
