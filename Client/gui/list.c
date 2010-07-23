#include "gui.h"
#include <GL/gl.h>

typedef struct _list_node {
	component *c;
	struct _list_node *next;
} list_node;

typedef struct {
	char num;
	char inside;
	char pressed;
	list_node *list;
} list_data;

void list_mouse_press(component *c,int x,int y,char b) {
	list_data *d = c->internal;
	if (c->mouse_press)
		c->mouse_press(c,x,y,b);
	d->pressed = 1;
}
void list_mouse_click(component *c,int x,int y,char b) {
	list_data *d = c->internal;
	d->pressed = 0;
	if (c->mouse_click)
		c->mouse_click(c,x,y,b);
}
void list_mouse_enter(component *c,int x,int y,char b) {
	list_data *d = c->internal;
	if (b != 1)
		d->pressed = 0;
	if (c->mouse_enter)
		c->mouse_enter(c,x,y,b);
	d->inside = 1;
}
void list_mouse_leave(component *c,char b) {
	list_data *d = c->internal;
	if (c->mouse_leave)
		c->mouse_leave(c,b);
	d->inside = 0;
}
void list_blur(component *c,char b) {
	list_data *d = c->internal;
	d->pressed = 0;
	if (c->blur)
		c->blur(c,b);
}

void render_list(component *c) {
	list_data *d = c->internal;
	unsigned char *text = d->text;
	glPushMatrix();
	glTranslatef(c->x,c->y,0);
	if (d->inside && d->pressed)
		glColor3f(1,0,0);
	else if (d->inside)
		glColor3f(0,1,0);
	else
		glColor3f(0,0,1);
	render_component(c);
	glColor3f(1,1,0);
	draw_text(text,c->width,c->height);
	glColor3f(1,1,1);
	render_children(c);
	glPopMatrix();
}

component *create_list_fixed(int x,int y,int w,int h,unsigned char *text) {//Fixed size
	component *temp = create_component(x,y,w,h);
	temp->type = 1;
	temp->internal = malloc(sizeof(list_data));
	list_data *data = temp->internal;
	data->text = text;
	data->inside = 0;
	data->pressed = 0;
	return temp;
}
