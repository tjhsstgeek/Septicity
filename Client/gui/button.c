#include "button.h"
#include <GL/gl.h>

void button_mouse_press(component *c,int x,int y,char b) {
	button_data *d = c->internal;
	if (c->mouse_press)
		c->mouse_press(c,x,y,b);
	d->pressed = 1;
}
void button_mouse_click(component *c,int x,int y,char b) {
	button_data *d = c->internal;
	d->pressed = 0;
	if (c->mouse_click)
		c->mouse_click(c,x,y,b);
}
void button_mouse_enter(component *c,int x,int y,char b) {
	button_data *d = c->internal;
	if (b != 1)
		d->pressed = 0;
	if (c->mouse_enter)
		c->mouse_enter(c,x,y,b);
	d->inside = 1;
}
void button_mouse_leave(component *c,int x,int y,char b) {
	button_data *d = c->internal;
	if (c->mouse_leave)
		c->mouse_leave(c,x,y,b);
	d->inside = 0;
}
void button_blur(component *c,int x,int y,char b) {
	button_data *d = c->internal;
	d->pressed = 0;
	if (c->blur)
		c->blur(c,x,y,b);
}

void render_button(component *c) {
	button_data *d = c->internal;
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

component *create_button_fixed(int x,int y,int w,int h,unsigned char *text) {//Fixed size
	component *temp = create_component(x,y,w,h);
	temp->type = 1;
	temp->internal = malloc(sizeof(button_data));
	button_data *data = temp->internal;
	data->text = text;
	data->inside = 0;
	data->pressed = 0;
	return temp;
}
component *create_button(int x,int y,unsigned char *text) {//Fits the text
	int *size = get_text_size(text);
	return create_button_fixed(x,y,size[0],size[1],text);
}
