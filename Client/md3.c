#include "md3.h"
#include <stdio.h>
#include <malloc.h>

MD3 *load_md3(const char *file) {
	if (!file)
		return 0;
	FILE *in = fopen(file,"r");
	if (!in)
		return 0;
	int a,b;
	//It says that the file doesn't have to start at zero 
	/*while (fgetc(in) != 0x49 || (++a&&fgetc(in) != 0x44) || (++a&&fgetc(in) != 0x50) || (++a&&fgetc(in) != 0x33)) {
		fseek(in,-a,SEEK_CUR);
		a = 0;//FIXME
	}*/
	if (fgetc(in) != 0x49 || fgetc(in) != 0x44 || fgetc(in) != 0x50 || fgetc(in) != 0x33)
		return 0;
	MD3 *cur = malloc(sizeof(MD3));
	if (!cur)
		return 0;
	fread(cur,sizeof(int)*10+64,1,in);
	fseek(in,cur->ofsframes,SEEK_SET);
	cur->frames = malloc(sizeof(Frame *) * cur->numframes);
	for (a = 0;a < cur->numframes;a++) {
		cur->frames[a] = malloc(sizeof(Frame));
		fread(cur->frames[a],sizeof(Frame),1,in);
	}
	fseek(in,cur->ofstags,SEEK_SET);
	cur->tags = malloc(sizeof(Surface *) * cur->numtags);
	for (a = 0;a < cur->numtags;a++) {
		cur->tags[a] = malloc(sizeof(Tag));
		fread(cur->tags[a],sizeof(Tag),1,in);
	}
	fseek(in,cur->ofssurfaces,SEEK_SET);
	cur->surfaces = malloc(sizeof(Surface *) * cur->numsurfaces);
	for (a = 0;a < cur->numsurfaces;a++) {
		long loc = ftell(in);
		//printf("%i\n",loc);
		cur->surfaces[a] = malloc(sizeof(Surface));
		fread(cur->surfaces[a],sizeof(Surface)-sizeof(Shader **) - sizeof(Triangle **) - sizeof(TexCoord **) - sizeof(Vertex **),1,in);
		Surface *sur = cur->surfaces[a];
		sur->shaders = malloc(sizeof(Shader *) * sur->numshaders);
		//printf("%i %i %i %i %i %i %i %i %s\n",sur->numshaders,sur->numverts,sur->numtriangles,sur->ofsshaders,sur->ofstriangles,sur->ofsst,sur->ofsverts,sur->ofsend,sur->name);
		fseek(in,loc + sur->ofsshaders,SEEK_SET);
		for (b = 0;b < sur->numshaders;b++) {
			sur->shaders[b] = malloc(sizeof(Shader));
			fread(sur->shaders[b],sizeof(Shader),1,in);
		}
		fseek(in,loc + sur->ofstriangles,SEEK_SET);
		sur->triangles = malloc(sizeof(Triangle *) * sur->numtriangles);
		for (b = 0;b < sur->numtriangles;b++) {
			sur->triangles[b] = malloc(sizeof(Triangle));
			fread(sur->triangles[b],sizeof(Triangle),1,in);
		}
		fseek(in,loc + sur->ofsst,SEEK_SET);
		sur->texcoords = malloc(sizeof(TexCoord *) * sur->numverts);
		for (b = 0;b < sur->numverts;b++) {
			sur->texcoords[b] = malloc(sizeof(TexCoord));
			fread(sur->texcoords[b],sizeof(TexCoord),1,in);
		}
		fseek(in,loc + sur->ofsverts,SEEK_SET);
		sur->vertices = malloc(sizeof(Vertex *) * sur->numverts);
		for (b = 0;b < sur->numverts;b++) {
			sur->vertices[b] = malloc(sizeof(Vertex));
			fread(sur->vertices[b],sizeof(Vertex),1,in);
		}
		fseek(in,sur->ofsend,SEEK_SET);
	}
	return cur;
}
void free_md3(MD3 *cur) {
	if (!cur)
		return;
	int a,b;
	for (a = 0;a < cur->numframes;a++) {
		free(cur->frames[a]);
	}
	free(cur->frames);
	for (a = 0;a < cur->numtags;a++) {
		free(cur->tags[a]);
	}
	free(cur->tags);
	for (a = 0;a < cur->numsurfaces;a++) {
		Surface *sur = cur->surfaces[a];
		for (b = 0;b < sur->numshaders;b++) {
			free(sur->shaders[a]);
		}
		free(sur->shaders);
		for (b = 0;b < sur->numtriangles;b++) {
			free(sur->triangles[a]);
		}
		free(sur->triangles);
		for (b = 0;b < sur->numverts;b++) {
			free(sur->texcoords[a]);
		}
		free(sur->texcoords);
		for (b = 0;b < sur->numverts;b++) {
			free(sur->vertices[a]);
		}
		free(sur->vertices);
	}
	free(cur->surfaces);
}
