#ifndef __TEXT_H__
#define __TEXT_H__

#include <ft2build.h>
#include FT_FREETYPE_H

typedef struct {
	int list;
	int width,height;//Actual size
	FT_Glyph_Metrics metrics;
} char_data;

extern char_data **cdata;

extern int width;//Placement
extern int height;//

extern int standard_height;
extern int above_height;
extern int below_height;

extern void draw_char(unsigned char);
extern int draw_text(unsigned char *,int,int);//Returns how many chars were actually drawn
extern int draw_text_len(unsigned char *,int,int,int);
extern int draw_text_offset(unsigned char *,int,int,int);
extern int draw_text_len_offset(unsigned char *,int,int,int,int);
extern void draw_text_around_blocks(unsigned char *,int,int,int,int,int **);
extern int *get_text_size(unsigned char *);
extern int *get_text_size_len(unsigned char *,int);

#endif
