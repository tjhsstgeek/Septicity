#include "rts.h"
#include "../Shared/map.h"
#include "unit.h"
#include <SDL/SDL_opengl.h>
#include <math.h>
#include <IL/il.h>
#include <assert.h>
#include <GL/glu.h>

void render_normals(char*);
GLuint generate_texture();

void render_model(MD3 *item) {
	int del = generate_texture();

	// Rotation code 
/*	angle+=0.4f;
	if (angle>=360.0)
		angle = 0.0;
	glRotatef(angle, 1.0, 0.0, 0.0);
	glRotatef(angle, 0.0, 1.0, 0.0);
	glRotatef(angle, 0.0, 0.0, 1.0);
*/
	// Model drawing code
	glBegin(GL_TRIANGLES);
	int i, j, k;
	for (i = 0; i < item->numsurfaces; i++) {
		for (j = 0; j < item->surfaces[i]->numtriangles; j++) {
			for (k = 0; k < 3; k++) {
				Surface* surf = item->surfaces[i];
				glTexCoord2f( surf->texcoords[surf->triangles[j]->indexes[k]]->ST[0], surf->texcoords[surf->triangles[j]->indexes[k]]->ST[1]);
//				printf("%f\n", surf->texcoords[surf->triangles[j]->indexes[k]]->ST[1]);

				int vertind = surf->triangles[j]->indexes[k];
				render_normals(surf->vertices[vertind] -> normal);
				glVertex3f ((float)surf->vertices [vertind]->coord[0]/64.0, 
	   					(float) surf->vertices [vertind]->coord[1]/64.0, 
						(float) surf->vertices [vertind]->coord[2]/64.0);
//				printf("%f ", surf->vertices[vertind]->coord[0]/64.0);
//				printf("%f ", surf->vertices[vertind]->coord[1]/64.0);
//				printf("%f\n", surf->vertices[vertind]->coord[2]/64.0);


			}
		}
	}
	glEnd();
	glDeleteTextures(1, &del);

}

void render_normals(char* normal) {
	glNormal3f(cos(normal[0]) * sin(normal[1]),sin(normal[0]) * cos(normal[1]), cos(normal[1]) );
//	printf("%f %f %f\n", cos(normal[0])*sin(normal[1]), sin(normal[0])*cos(normal[1]), cos(normal[1]));
}

GLuint generate_texture() {
	GLuint image;
	ILuint texid;
	ilGenImages(1, &texid);
	ilBindImage(texid);
	if(ilLoadImage("../models/models/mapobjects/kt_utilities/water_can.tga")) {
//		printf("loaded\n");
	}
	ILboolean suc = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
	if (suc) {
//		printf("success\n");
	}
	glGenTextures(1, &image);
	glBindTexture(GL_TEXTURE_2D, image);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); /* We will use linear
										            interpolation for magnification filter */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); /* We will use linear
											        interpolation for minifying filter */
	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH),
	     ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
	     ilGetData()); /* Texture specification */
	ilDeleteImages(1, &texid);
	return image;
}

void move_camera() {
//	glRotatef(*angle*-1, 1, 0, 0);
	glTranslatef(-*ex, -*ey, -*ez);
//	printf("%d %d %d\n", *ex, *ey, *ez);
}


int render_helper(void *v) {
	MD3* item = load_md3("../models/models/mapobjects/kt_utilities/watercan.md3");
	while (1) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		render_model(item);
		move_camera();
		glColor3f(1,1,1);
		glBegin(GL_QUADS);
			glVertex3f(0,0,0);
			glVertex3f(0,0,-1000);
			glVertex3f(1000,0,-1000);
			glVertex3f(1000, 0, 0);
		glEnd();
//		for (v = 0; v< head;i = i->next) {
//			render_item(i->item);
//		}
		glFinish();
		SDL_GL_SwapBuffers();
		SDL_Event event;
		SDL_Delay(33);
	}
	return 1;
}

void render_helperV2 (Map* map) {
	int i, j;
/*	for(i = 0; i < height; i++)
		for (j = 0; j < width; j++)
			render_model(map->objects[i][j]);
	render_map(map); */
}

void render_screen() {
	glShadeModel(GL_SMOOTH);
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.0, 1.0, -1, 1, 1.0, 1000.0);
	glEnable(GL_POINT_SMOOTH|GL_LINE_SMOOTH);
	glHint(GL_POLYGON_SMOOTH_HINT,GL_NICEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//SDL_CreateThread(render_helper,0);
	render_helper(0);
}
