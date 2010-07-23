#ifndef __RADIO_H__
#define __RADIO_H__
#include "component.h"
#include "linkedlist.h"

typedef struct {
	linkedlist *group;
	int selected;
} radio_group;

radio_group *create_radio_group();

typedef struct {
	char inside;
	char pressed;
	char checked;//These three can be combined into one. We only use zero or one for each
	unsigned char *text;
	int s;
	int *block;
} radio_data;

void radio_mouse_press(component *c,int x,int y,char b);
void radio_mouse_click(component *c,int x,int y,char b);
void radio_mouse_enter(component *c,int x,int y,char b);
void radio_mouse_leave(component *c,int x,int y,char b) ;
void radio_blur(component *c,int x,int y,char b);
void render_radio(component *c);
component *create_radio_fixed_box(int x,int y,int w,int h,int s,unsigned char *text,char checked);
component *create_radio_fixed(int x,int y,int w,int h,unsigned char *text,char checked);
component *create_radio(int x,int y,unsigned char *text,char checked);

#endif
