#ifndef __THEME_H__
#define __THEME_H__

enum {
	ThemeTop = 1,ThemeBottom,ThemeLeft,ThemeRight,ThemeInterior,ThemeClose,ThemeMaximize,ThemeMinimize,ThemeUnmaximize
}

typedef struct {
	short rules;//Rules for component & 1 = NoTop 2 = NoBottom 4 = NoLeft 8 = NoRight 16 = NoInterior etc. Follow enum
	short x;
	short y;
	short width;
	short height;//If components exceed these sizes then LOL.
} Component;

typedef struct _TCD {
	char key; //0 - Color 1 - Text 2 - 
	int value;//RGBA      char *
	struct _TCD *next;
} ThemeComponentData;

typedef struct _TC {
	int part;//The part of the component it affects
	struct _TC *next;
} ThemeComponent;

typedef struct {
	char *filename;
	char *themename;
	unsigned char left,right,top,bottom;//Deny anything bigger than 255
	
} Theme;

#endif
