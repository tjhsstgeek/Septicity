#include "window.h"
#include <GL/gl.h>

typedef struct {
	unsigned char *text;
	char pressed;
	int offx;
	int offy;
} window_data;

void window_mouse_leave(component *c,int x,int y,char b) {
	window_data *d = c->internal;
	if (c->mouse_press)
		c->mouse_leave(c,x,y,b);
	if (d->pressed) {
		c->x += x-d->offx;
		c->y += y-d->offy;
	}
	reposition(c);
}
void window_mouse_move(component *c,int x,int y,char b) {
	window_data *d = c->internal;
	if (c->mouse_press)
		c->mouse_press(c,x,y,b);
	if (d->pressed) {
		c->x += x-d->offx;
		c->y += y-d->offy;
	}
}
void window_mouse_press(component *c,int x,int y,char b) {
	window_data *d = c->internal;
	if (c->mouse_press)
		c->mouse_press(c,x,y,b);
	d->pressed = 1;
	d->offx = x;
	d->offy = y;
}
void window_mouse_release(component *c,int x,int y,char b) {
	window_data *d = c->internal;
	if (c->mouse_click)
		c->mouse_click(c,x,y,b);
	d->pressed = 0;
}

void render_window(component *c) {
	window_data *d = c->internal;
	unsigned char *text = d->text;
	glPushMatrix();
	glTranslatef(c->x,c->y,0);
	glColor3f(0,0,0);
	render_component(c);
	//glColor3f(1,1,0);
	//draw_text(text,c->width,c->height);
	glColor3f(1,1,1);
	render_children(c);
	glPopMatrix();
}

component *create_window_fixed(int x,int y,int w,int h,unsigned char *text) {//Fixed size
	component *temp = create_component(x,y,w,h);
	temp->type = 3;
	temp->internal = malloc(sizeof(window_data));
	window_data *data = temp->internal;
	data->text = text;
	data->pressed = 0;
	return temp;
}
component *create_window(int x,int y,unsigned char *text) {//Fits the text
	int *size = get_text_size(text);
	return create_window_fixed(x,y,size[0],size[1],text);
}
