#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "text.h"

struct gui_component {
	char type;
	struct gui_component *prev;
	struct gui_component *next;
	struct gui_component *children;//The first component to render. Like a stack.
	struct gui_component *parent;
	int x,y,width,height;
	int (*mouse_click)(struct gui_component *,int,int,char);//if returns non-zero then no propagation
	int (*mouse_press)(struct gui_component *,int,int,char);
	int (*mouse_release)(struct gui_component *,int,int,char);
	int (*mouse_move)(struct gui_component *,int,int,char);
	int (*mouse_enter)(struct gui_component *,int,int,char);
	int (*mouse_leave)(struct gui_component *,int,int,char);//There need to be an array of listeners
	int (*focus)(struct gui_component *,int,int,char);
	int (*blur)(struct gui_component *,int,int,char);
	void *internal;
};

typedef struct gui_component component;

extern void render_gui(component *);
extern component *create_component(int,int,int,int);

extern void render_component(component *);
extern void render_children(component *);

extern void component_add_child(component *,component *);
extern void component_remove_child_at_index(component *,int);
extern void component_remove_child(component *,component *);

extern void component_mouse_click(struct gui_component *,int,int,char);
extern void component_mouse_press(struct gui_component *,int,int,char);
extern void component_mouse_release(struct gui_component *,int,int,char);
extern void component_mouse_move(struct gui_component *,int,int,char);
extern void component_mouse_enter(struct gui_component *,int,int,char);
extern void component_mouse_leave(struct gui_component *,int,int,char);
extern void component_focus(struct gui_component *,int,int,char);
extern void component_blur(struct gui_component *,int,int,char);

/*extern void attach_mouseclick_listener(int (*)(struct gui_component *,int,int,char));
extern void attach_mousepress_listener(int (*)(struct gui_component *,int,int,char));
extern void attach_mouserelease_listener(int (*)(struct gui_component *,int,int,char));
extern void attach_mousemove_listener(int (*)(struct gui_component *,int,int,char));
extern void attach_mouseenter_listener(int (*)(struct gui_component *,int,int,char));
extern void attach_mouseleave_listener(int (*)(struct gui_component *,int,int,char));
extern void attach_focus_listener(int (*)(struct gui_component *,int,int,char));
extern void attach_blur_listener(int (*)(struct gui_component *,int,int,char));

extern void refocus(component *);
extern void reposition(component *);
Not Implemented*/
#endif
