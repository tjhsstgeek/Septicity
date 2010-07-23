#ifndef __RENDER_H__
#define __RENDER_H__

typedef struct _Component {
	char type;
	struct _Component *prev;
	struct _Component *next;
	short x,y,width,height;
} Component;

typedef struct {
	char type;
	struct _Component *prev;
	struct _Component *next;
	short x,y,width,height;
	char *text;
} Button;
