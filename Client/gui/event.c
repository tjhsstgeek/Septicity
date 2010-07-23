#include "gui.h"
//FIXME
void (*mouse_move_array [])(component *,int,int,char) = {
	component_mouse_move,component_mouse_move,component_mouse_move,window_mouse_move
};
void (*mouse_press_array [])(component *,int,int,char) = {
	component_mouse_press,button_mouse_press,checkbox_mouse_press,window_mouse_press
};
void (*mouse_release_array [])(component *,int,int,char) = {
	component_mouse_release,component_mouse_release,component_mouse_release,window_mouse_release
};
void (*mouse_click_array [])(component *,int,int,char) = {
	component_mouse_click,button_mouse_click,checkbox_mouse_click,component_mouse_click
};
void (*mouse_enter_array [])(component *,int,int,char) = {
	component_mouse_enter,button_mouse_enter,checkbox_mouse_enter,component_mouse_enter
};
void (*mouse_leave_array [])(component *,int,int,char) = {
	component_mouse_leave,button_mouse_leave,checkbox_mouse_leave,window_mouse_leave
};
void (*focus_array [])(component *,int,int,char) = {
	component_focus,component_focus,component_focus,component_focus
};
void (*blur_array [])(component *,int,int,char) = {
	component_blur,button_blur,checkbox_blur,component_blur
};
component *last_pressed = 0;//Key events to to the last pressed
component *inside = 0;
char press = 0;


int *getRelativeLocation(component *c,int x,int y) {
	int *loc = malloc(2*sizeof(int));
	component *cur = c;
	for (;cur;cur = cur->parent) {
		x -= cur->x;
		y -= cur->y;
	}
	loc[0] = x;
	loc[1] = y;
	return loc;
}

void refocus(component *c) {//Sets the specified component as the last_pressed.
	last_pressed = c;
}
void reposition(component *c) {//Sets inside to the specified component.
	inside = c;
}

component *recurse_move(component *cur,int mx,int my,char button) {
	component *loop = cur->children;
	for (;loop;loop = loop->next) {
		int x = loop->x;
		int y = loop->y;
		int w = loop->width;
		int h = loop->height;
		if (mx >= x && mx < x+w && my >= y && my < y + h) {
			return recurse_move(loop,mx-x,my-y,button);//Locations based on parent component
		}
	}
	//This is the correct element
	mouse_move_array[(unsigned char)cur->type](cur,mx,my,button);
	if (cur != inside) {
		mouse_enter_array[(unsigned char)cur->type](cur,mx,my,button);
	}
	return cur;
}

void mouse_move(int x,int y,char button) {
	/*if (press && last_pressed) {
		component *cur = last_pressed;
		for (;cur;cur = cur->parent) {
			x -= cur->x;
			y -= cur->y;
		}
		mouse_move_array[(unsigned char)last_pressed->type](last_pressed,x,y,button);
		return;
	}*/
	component *cur = recurse_move(main_panel,x,y,button);
	if (cur != inside) {
		if (inside) {
			int *loc = getRelativeLocation(inside,x,y);
			mouse_leave_array[(unsigned char)inside->type](inside,loc[0],loc[1],button);
		}
		inside = cur;
	}
}

component *recurse_press(component *cur,int mx,int my,char button) {
	component *loop = cur->children;
	for (;loop;loop = loop->next) {
		int x = loop->x;
		int y = loop->y;
		int w = loop->width;
		int h = loop->height;
		if (mx >= x && mx < x+w && my >= y && my < y + h) {
			return recurse_press(loop,mx-x,my-y,button);//Locations based on parent component
		}
	}
	//This is the correct element
	mouse_press_array[(unsigned char)cur->type](cur,mx,my,button);
	if (last_pressed != cur) {
		focus_array[(unsigned char)cur->type](cur,mx,my,button);
	}
	return cur;
}

void mouse_press(int x,int y,char button) {
	component *cur = recurse_press(main_panel,x,y,button);
	if (last_pressed != cur) {
		if (last_pressed) {
			int *loc = getRelativeLocation(last_pressed,x,y);
			blur_array[(unsigned char)last_pressed->type](last_pressed,loc[0],loc[1],button);
		}
	}
	last_pressed = cur;
	press = 1;
}
void recurse_release(component *cur,int mx,int my,char button) {
	component *loop = cur->children;
	for (;loop;loop = loop->next) {
		int x = loop->x;
		int y = loop->y;
		int w = loop->width;
		int h = loop->height;
		if (mx >= x && mx < x+w && my >= y && my < y + h) {
			recurse_release(loop,mx-x,my-y,button);//Locations based on parent component
			return;//We found the correct object
		}
	}
	//This is the correct element
	mouse_release_array[(unsigned char)cur->type](cur,mx,my,button);
	if (cur == last_pressed)
		mouse_click_array[(unsigned char)last_pressed->type](cur,mx,my,button);
}

void mouse_release(int mx,int my,char button) {
	press = 0;
	recurse_release(main_panel,mx,my,button);
}
void key_press(char c) {
	
}
