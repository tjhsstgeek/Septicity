#include "text.h"
#include <GL/gl.h>
#include <GL/glu.h>

int standard_height;

int get_word_len(char *c) {
	int a;
	for (a = 0;;a++) {
		char cur = c[a];
		if (cur == '\0' || cur == ' ')
			break;
	}
	return a;
}

int *get_text_size_len(unsigned char *text,int len) {//Placed Horizontally
	int *size = malloc(2*sizeof(int));
	size[0] = 0;
	size[1] = standard_height;//Largest height possible. 
	int a;
	for (a = 0;a < len;a++) {
		char_data *cur = cdata[text[a]];
/*		if (cur->height > size[1]) {
			size[1] = cur->height;
		}*/
		size[0] += cur->metrics.horiAdvance>>6;
	}
	return size;
}

int *get_text_size(unsigned char *text) {
	return get_text_size_len(text,strlen((char *)text));
}

void draw_char_bitmap(char_data *cur) {
	glCallList(cur->list);
}
void draw_char(unsigned char c) {
	draw_char_bitmap(cdata[c]);
}
void draw_text_unchecked(unsigned char *c,int len) {
	int a;
	for (a = 0;a < len;a++) {
		char_data *cur = cdata[c[a]];
		draw_char_bitmap(cur);
		glTranslatef(cur->metrics.horiAdvance>>6,0,0);
	}
}
void draw_text_around_blocks(unsigned char *text,int len,int limitx,int limity,int num,int **blocks) {//This should take text and write it around the blocks
	//blocks = {x,y,fx,fy}[num]
	int a = 0;
	int x = 0;
	int y = limity-above_height;
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	char_data *space = cdata[' '];
	int space_width = space->metrics.horiAdvance>>6;
	double wx,wy,wz;
	double *model = malloc(sizeof(double) * 16);
	glGetDoublev(GL_MODELVIEW_MATRIX,model);
	double *proj = malloc(sizeof(double) * 16);
	glGetDoublev(GL_PROJECTION_MATRIX,proj);
	int *view = malloc(sizeof(int) * 4);
	glGetIntegerv(GL_VIEWPORT,view);
	gluProject(0,0,0,model,proj,view,&wx,&wy,&wz);
	glScissor(wx,wy,limitx,limity);
	//We need to fit the text now
	while (a < len) {
		int word = get_word_len((char *)(text+a));
		int *size = get_text_size_len(text+a,word);
		int ex = x + size[0];
		int ty = y + above_height;
		int by = y - below_height;
		while (1) {
			int b;
			for (b = 0;b < num;b++) {
				if (ex <= blocks[b][0] || x >= blocks[b][2] || ty <= blocks[b][1] || by >= blocks[b][3]) {
					continue;
				} else {
					x = blocks[b][2];
					ex = x + size[0];
					break;
				}
			}
			if (x && ex > limitx) {
				y -= standard_height;
				x = 0;
				ex = x + size[0];
				ty = y + above_height;
				by = y - below_height;
				continue;
			}
			if (b == num) break;
		}
		glPushMatrix();
		glTranslatef(x,y,0);
		draw_text_unchecked(text+a,word);
		x = ex;
		if (x + space_width > limitx) {
			x = 0;
			y -= standard_height;
		} else {
			x += space_width;
		}
		glPopMatrix();
		free(size);
		a += word;
		a++;
		//Check for intersecting rectangles
	}
	glScissor(0,0,width,height);
	glBlendFunc(GL_ONE, GL_ZERO);
}
int draw_text_len_offset(unsigned char *text,int limitx,int limity,int len,int offsetx) {
	int a = 0;
	int x = offsetx;
	int y = limity-(cdata['A']->height);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	char_data *space = cdata[' '];
	int space_width = space->metrics.horiAdvance>>6;
	double wx,wy,wz;
	double *model = malloc(sizeof(double) * 16);
	glGetDoublev(GL_MODELVIEW_MATRIX,model);
	double *proj = malloc(sizeof(double) * 16);
	glGetDoublev(GL_PROJECTION_MATRIX,proj);
	int *view = malloc(sizeof(int) * 4);
	glGetIntegerv(GL_VIEWPORT,view);
	gluProject(0,0,0,model,proj,view,&wx,&wy,&wz);
	glScissor(wx,wy,limitx,limity);
	while (a < len) {
		int word = get_word_len((char *)(text+a));
		int *size = get_text_size_len(text+a,word);
		if (size[0] + x > limitx && x) {
			y -= standard_height;
			x = 0;
		}
		glPushMatrix();
		glTranslatef(x,y,0);
		draw_text_unchecked(text+a,word);
		x += size[0];
		if (x + space_width > limitx) {
			x = 0;
			y -= standard_height;
		} else {
			draw_char_bitmap(space);
			x += space_width;
		}
		glPopMatrix();
		free(size);
		a += word;
		a++;
	}
	glScissor(0,0,width,height);
	glBlendFunc(GL_ONE, GL_ZERO);
	return a;
}
int draw_text_offset(unsigned char *text,int limitx,int limity,int offset) {
	int len = strlen((char *)text);
	return draw_text_len_offset(text,limitx,limity,len,offset);
}
int draw_text_len(unsigned char *text,int limitx,int limity,int len) {
	return draw_text_len_offset(text,limitx,limity,len,0);
}
/*int draw_text_len(unsigned char *text,int limitx,int limity,int len) {
	int a;
	int x = 0;
	int y = 0;
	int largey = 0;// large y
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_SCISSOR_TEST);
	glScissor(0,0,limitx,limity);
	for (a = 0;a < len;a++) {
		char_data *cur = cdata[text[a]];
		if (cur->height + y > limity) break;
		if (cur->width + x > limitx) {
			y += largey;
			x = 0;
			largey = 0;
			if (cur->height + y > limity || cur->width > limitx) break;
		}
		if (cur->height > largey) largey = cur->height;
		glPushMatrix();
		glTranslatef(x,y,0);
		draw_char_bitmap(cur);
		glPopMatrix();
		x += cur->metrics.horiAdvance>>6;
	}
	glDisable(GL_SCISSOR_TEST);
	glBlendFunc(GL_ONE, GL_ZERO);
	return a;
}*/
int draw_text(unsigned char *text,int limitx,int limity) {
	int len = strlen((char *)text);
	return draw_text_len(text,limitx,limity,len);
}
