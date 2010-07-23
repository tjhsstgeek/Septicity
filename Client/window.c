#include "rts.h"
#include <GL/gl.h>
#include <stdio.h>
#include <string.h>


void perform_action(int sym);

typedef enum { TRANSLATE_UP, TRANSLATE_DOWN, TRANSLATE_RIGHT, TRANSLATE_LEFT, ZOOM_IN, ZOOM_OUT } Action; 

void read_config(char* config) {
	actions = malloc(sizeof(int)*1000);
	int i;
	for (i = 0; i < 1000; i++)
		actions[i] = -1;
	FILE *in = fopen(config, "r");
	fseek(in, 0, SEEK_END);
	long size = ftell(in);
	fseek(in, 0, SEEK_SET);
	while (ftell(in) != size) {
		char buffer[100];
		char* buf2;
		fgets(buffer, 100, in);
		buf2 = strtok(buffer, " \n");
		int pos = atoi(buf2);
		buf2 = strtok(NULL, " \n");
		Action act = atoi(buf2);
		actions[pos] = act;
	}
	actions[998] = 4;
	actions[999] = 5;
	fclose(in);
}

void manage_key(SDL_Event *e) {//It's easier to pass by pointer
	SDL_Event event = *e;
	int sym = event.key.keysym.sym;
	if (sym == SDLK_ESCAPE) {
		free(actions);
		exit(0);
	}
	else perform_action(sym);
}

void manage_mouse(SDL_Event *e) {
	SDL_Event event = *e;
	int button = event.button.button;
	if (button == 4)
		perform_action(998);
	if (button == 5)
		perform_action(999);
}

void perform_action(int sym) {
	if (actions[sym]==-1) return;
	switch(actions[sym]) {
		case 2:
			(*ex)+=10;
			break;
		
		case 3:
			(*ex)-=10;
			break;

		case 0:
			(*ez)-=10;
			break;

		case 1:
			(*ez)+=10;
			break;

		case 4:
			(*ez)-=5;
			(*ey)-=5;
			break;

		case 5:
			(*ez)+=5;
			(*ey)+=5;
			break;
	}

}

int listen_window_threaded(void *v) {
	SDL_Event event;//X Windows Event Catching mechanism
	while (SDL_WaitEvent(&event)) {
		if (event.type == SDL_QUIT) {
			exit(0);
			break;
		} else if (event.type == SDL_VIDEORESIZE)
			/*window = SDL_SetVideoMode(event.resize.w, event.resize.h, video_info->vfmt->BitsPerPixel,SDL_RESIZABLE|SDL_OPENGL)*/;
			//FIXME for opengl. This destroys gl pane
		else if (event.type == SDL_KEYDOWN) 
			manage_key(&event);
		else if (event.type == SDL_MOUSEBUTTONDOWN)
			manage_mouse(&event);
	}
	return 0;
}

void listen_window() {
	SDL_CreateThread(listen_window_threaded,0);
}
