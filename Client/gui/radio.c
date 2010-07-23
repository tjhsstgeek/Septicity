#include "radio.h"
#include <GL/gl.h>

typedef struct {
	char inside;
	char pressed;
	char checked;//These three can be combined into one. We only use zero or one for each
	unsigned char *text;
	int s;
	int *block;
} radio_data;

void radio_mouse_press(component *c,int x,int y,char b) {
	radio_data *d = c->internal;
	if (c->mouse_press)
		c->mouse_press(c,x,y,b);
	d->pressed = 1;
}
void radio_mouse_click(component *c,int x,int y,char b) {
	radio_data *d = c->internal;
	d->pressed = 0;
	d->checked = 1-d->checked;
	if (c->mouse_click)
		c->mouse_click(c,x,y,b);
}
void radio_mouse_enter(component *c,int x,int y,char b) {
	radio_data *d = c->internal;
	if (b != 1)
		d->pressed = 0;
	if (c->mouse_enter)
		c->mouse_enter(c,x,y,b);
	d->inside = 1;
}
void radio_mouse_leave(component *c,int x,int y,char b) {
	radio_data *d = c->internal;
	if (c->mouse_leave)
		c->mouse_leave(c,x,y,b);
	d->inside = 0;
}
void radio_blur(component *c,int x,int y,char b) {
	radio_data *d = c->internal;
	d->pressed = 0;
	if (c->blur)
		c->blur(c,x,y,b);
}

void render_radio(component *c) {
	radio_data *d = c->internal;
	unsigned char *text = d->text;
	glPushMatrix();
	glTranslatef(c->x,c->y,0);
	glColor3f(0,0,0);
	/*glBegin(GL_QUADS);
	glVertex2i(0,c->height);
	glVertex2i(d->s,c->height);
	glVertex2i(d->s,c->height-d->s);
	glVertex2i(0,c->height-d->s);
	glEnd();*/
	//Circle
	draw_text_around_blocks(text,strlen((char *)text),c->width,c->height,1,&d->block);
	glColor3f(1,1,1);
	/*glBegin(GL_QUADS);
	glVertex2i(1,c->height-1);
	glVertex2i(d->s-1,c->height-1);
	glVertex2i(d->s-1,c->height-d->s+1);
	glVertex2i(1,c->height-d->s+1);
	glEnd();*/
	//Circle
	glColor3f(0,0,0);
	if (d->checked) {
		/*glBegin(GL_TRIANGLES);
		glVertex2i(d->s,c->height);
		glVertex2i(d->s/2,c->height-d->s);
		glVertex2i(d->s/2,c->height-d->s+(d->s/3));
		glVertex2i(d->s/2,c->height-d->s);
		glVertex2i(d->s/2,c->height-d->s+(d->s/3));
		glVertex2i(0,c->height-d->s/2);
		glEnd();*/
	}	
	/*else if (d->inside && d->pressed)
		glColor3f(1,0,0);
	else if (d->inside)
		glColor3f(0,1,0);
	else
		glColor3f(0,0,1);*/
	glColor3f(1,1,1);
	render_children(c);
	glPopMatrix();
}

component *create_radio_fixed_box(int x,int y,int w,int h,int s,unsigned char *text,char checked) {//Fixed size
	component *temp = create_component(x,y,w,h);
	temp->type = 2;
	temp->internal = malloc(sizeof(radio_data));
	radio_data *data = temp->internal;
	data->checked = checked;
	data->inside = 0;
	data->pressed = 0;
	data->text = text;
	data->s = s;
	data->block = malloc(sizeof(int)*4);
	data->block[0] = 0;
	data->block[1] = h-s;
	data->block[2] = s;
	data->block[3] = h;
	return temp;
}

component *create_radio_fixed(int x,int y,int w,int h,unsigned char *text,char checked) {//Fixed size
	int s = 0;
	if (w < h) {
		s = w;
	} else {
		s = h;
	}
	if (s > standard_height)
		s = standard_height;
	return create_radio_fixed_box(x,y,w,h,s,text,checked);
}
component *create_radio(int x,int y,unsigned char *text,char checked) {//Fits the text
	int *size = get_text_size(text);
	return create_radio_fixed(x,y,size[1]+size[0],size[1],text,checked);
}
