#include "rts.h"
#include <SDL/SDL.h>
#include <SDL/SDL_net.h>
#include <SDL/SDL_video.h>
#include <IL/il.h>
#include <GL/gl.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

MD3_Item *head = 0;
SDL_Surface *window;
int width;
int height;
SDL_VideoInfo* video_info;
char fullscreen;
FT_Library freetype;
FT_Face face;
typedef struct {
	int list;
	int width,height;//Actual size
	FT_Glyph_Metrics metrics;
} char_data;
char_data **cdata;
int init_freetype() {
	int error = FT_Init_FreeType(&freetype);
	if (error) {
		fprintf(stderr,"Freetype failed with error code %i\n",error);
		return 1;
	}
	FILE *file = fopen("config/FreeSans.ttf","r");
	if (!file) {
		fprintf(stderr,"Font file not found\n");
		return 1;
	}
	fseek(file,0,SEEK_END);
	int size = ftell(file);
	fseek(file,0,SEEK_SET);
	char *buffer = malloc(size);
	fread(buffer,size,1,file);
	error = FT_New_Memory_Face(freetype,(const FT_Byte *)buffer,size,0,&face);
	if (error) {
		fprintf(stderr,"Freetype failed with error code %i\n",error);
		return 1;
	}
	FT_Set_Pixel_Sizes(face,16,16);
	int a;
	unsigned int *textures = malloc(sizeof(unsigned int)*256);
	glGenTextures(256,textures);
	cdata = malloc(sizeof(char_data *) * 256);
	unsigned int char_list = glGenLists(256);
	if (!char_list) {
		fprintf(stderr,"Couldn't create opengl lists for fonts\n");
		return 1;
	}
	for (a = 0;a <= 255;a++) {
		int index = FT_Get_Char_Index(face,a);
		error = FT_Load_Glyph(face,index,0);
		if (error) {
			fprintf(stderr,"Freetype failed with error code %i\n",error);
			return 1;
		}
		error = FT_Render_Glyph(face->glyph,0);
		if (error) {
			fprintf(stderr,"Freetype failed with error code %i\n",error);
			return 1;
		}
		FT_Glyph glyph;
		FT_Get_Glyph(face->glyph,&glyph);
		FT_Glyph_To_Bitmap(&glyph,0,0,1);
		FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;
		FT_Bitmap bitmap = bitmap_glyph->bitmap;
		cdata[a] = malloc(sizeof(char_data));
		int width = 2;
		while (width < bitmap.width)
			width *= 2;
		int height = 2;
		while (height < bitmap.rows)
			height *= 2;
		cdata[a]->list = char_list+a;
		cdata[a]->width = bitmap.width;
		cdata[a]->height = bitmap.rows;
		cdata[a]->metrics = face->glyph->metrics;
		unsigned char *buffer = malloc(2*width*height);
		int b;
		for (b = 0;b < height;b++) {
			int c;
			for (c = 0;c < width;c++) {
				buffer[2*(b*width+c)] = 255;
				if (b >= bitmap.rows)
					buffer[2*(b*width+c)+1] = 0;
				else if (c >= bitmap.width)
					buffer[2*(b*width+c)+1] = 0;
				else 
					buffer[2*(b*width+c)+1] = bitmap.buffer[b*bitmap.width+c];
			}
		}
		glBindTexture(GL_TEXTURE_2D,textures[a]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_LUMINANCE_ALPHA,GL_UNSIGNED_BYTE,buffer);
		free(buffer);
		glNewList(char_list+a,GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D,textures[a]);
		glPushMatrix();
		glTranslatef(face->glyph->metrics.horiBearingX>>6,(face->glyph->metrics.horiBearingY>>6)-bitmap.rows,0);
		float x = (float)bitmap.width/width;
		float y = (float)bitmap.rows/height;
		glBegin(GL_QUADS);
		glTexCoord2f(0,0);
		glVertex2f(0,bitmap.rows);
		glTexCoord2f(0,y);
		glVertex2f(0,0);
		glTexCoord2f(x,y);
		glVertex2f(bitmap.width,0);
		glTexCoord2f(x,0);
		glVertex2f(bitmap.width,bitmap.rows);
		glEnd();
		glPopMatrix();
		glEndList();
	}
	return 0;
}

int init() {
	if (SDL_Init(SDL_INIT_TIMER|SDL_INIT_AUDIO|SDL_INIT_VIDEO)) {
		fprintf(stderr,"Error initiating SDL. %s\n",SDL_GetError());
		return 1;
	}
	atexit(SDL_Quit);
	if (SDLNet_Init()) {
		fprintf(stderr,"Error initiating SDL network library. %s\n",SDL_GetError());
		return 1;
	}
	atexit(SDLNet_Quit);
	SDL_Rect **sizes = SDL_ListModes(0,SDL_FULLSCREEN|SDL_OPENGL);//Only get fullscreen sizes. It makes more sense.
	int a;
	for (a = 0;sizes[a];a++) {
		printf("Size[%i]: %i x %i\n",a,sizes[a]->w,sizes[a]->h);
	}
	char *index = malloc(4);
	int i = 0;
	while (1) {
		printf("Please enter the index of the size you want to use.\n");
		fgets(index,4,stdin);
		i = atoi(index);
		if (index[0] != '\n' && index[1] != '\n' && index[2] != '\n' && index[3] != '\n') {
			while (fgetc(stdin) != '\n');
		}
		if (i >= a) {
			printf("Invalid size\n");
			continue;
		}
		break;
	}
	while (1) {
		printf("Use fullscreen: [yn]");
		fflush(stdout);
		fgets(index,2,stdin);//Index can be reused
		if (*index != '\n') {
			while (fgetc(stdin) != '\n');
		}
		if (*index == 'y' || *index == 'Y') {
			fullscreen = 1;
		} else if (*index == 'n' || *index == 'N') {
			fullscreen = 0;
		} else {
			printf("Please enter y or n\n");
			continue;
		}
		break;
	}
	printf("%i %i\n",((fullscreen)?SDL_FULLSCREEN:0),SDL_FULLSCREEN);
	video_info = SDL_GetVideoInfo();
	window = SDL_SetVideoMode(sizes[i]->w,sizes[i]->h,0,SDL_OPENGL/*|((fullscreen)?SDL_FULLSCREEN:0)*/);
	if (!window) {
		fprintf(stderr,"Error in creating window. %s\n",SDL_GetError());
		return 1;
	}
	width = sizes[i]->w;
	height = sizes[i]->h;
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

	// DevIL init

	ilInit();
	
	if (init_freetype())
		return 1;

	//input reader init
	read_config("config/input.conf");
	
	// init camera position
	ex = malloc(sizeof(int));
	ey = malloc(sizeof(int));
	ez = malloc(sizeof(int));
	angle = malloc(sizeof(float));
	*ex = 2; *ey = 2; *ez = 0;
	*angle = 45.0f;



	return 0;
}
