#include "component.h"
#include <GL/gl.h>
#include <stdlib.h>

void component_add_child(component *parent,component *child) {
	component *t = parent->children;
	parent->children = child;
	child->next = t;
}

void component_mouse_move(component *c,int x,int y,char b) {
	if (c->mouse_move)
		c->mouse_move(c,x,y,b);
}
void component_mouse_press(component *c,int x,int y,char b) {
	if (c->mouse_press)
		c->mouse_press(c,x,y,b);
}
void component_mouse_release(component *c,int x,int y,char b) {
	if (c->mouse_release)
		c->mouse_release(c,x,y,b);
}
void component_mouse_click(component *c,int x,int y,char b) {
	if (c->mouse_click)
		c->mouse_click(c,x,y,b);
}
void component_mouse_enter(component *c,int x,int y,char b) {
	if (c->mouse_enter)
		c->mouse_enter(c,x,y,b);
}
void component_mouse_leave(component *c,int x,int y,char b) {
	if (c->mouse_leave)
		c->mouse_leave(c,x,y,b);
}
void component_focus(component *c,int x,int y,char b) {
	if (c->focus)
		c->focus(c,x,y,b);
}
void component_blur(component *c,int x,int y,char b) {
	if (c->blur)
		c->blur(c,x,y,b);
}

void render_child(component *c) {
	if (!c) return;
	render_child(c->next);
	render_gui(c);
	render_children(c);
}

void render_children(component *c) {
	render_child(c->children);
}

void render_component(component *c) {
	glBegin(GL_QUADS);
	glVertex2i(0,0);
	glVertex2i(c->width,0);
	glVertex2i(c->width,c->height);
	glVertex2i(0,c->height);
	glEnd();
}

component *create_component(int x,int y,int w,int h) {
	component *temp = malloc(sizeof(component));
	memset(temp,0,sizeof(component));//Required
	temp->x = x;
	temp->y = y;
	temp->width = w;
	temp->height = h;
	return temp;
}
