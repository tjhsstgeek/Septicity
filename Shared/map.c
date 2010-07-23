//This file is used for reading and writing maps
//This will be compiled into rts.so or rts.dll or etc

#include "map.h"
#include <stdio.h>

char *map_error = 0;

void saveMap(const char *name,int magic,char *name,int width,height,char players,int unitcount,char **units,float heightcount,float **mapheight) {
	if (!name) {
		map_error = "Name is null";
		return 0;
	}
	FILE *file = fopen(name,"r");
	if (!file) {
		map_error = "The file does not exist";
		return 0;
	}
}

MapHeader *loadMap(const char *name) {
	if (!name) {
		map_error = "Name is null";
		return 0;
	}
	FILE *file = fopen(name,"r");
	if (!file) {
		map_error = "The file does not exist";
		return 0;
	}
	MapHeader *hdr = malloc(sizeof(MapHeader));
	if (!hdr) {
		map_error = "Out of memory";
		return 0;
	}
//	MapHeader *write = hdr;
	int* write;
	fread(write,sizeof(int),1,file);
	if (*(int *)write != 'p'*256*256*256+'a'*256*256+'m'*256) {
		map_error = "Invalid magic header";
		return 0;
	}
	hdr->magic=*write;
//	*(char **)write = name;
//	write += sizeof(const char *);
	fread(write,sizeof(int),1,file); //width
	hdr->width = *write;
	fread(write,sizeof(int),1,file); //height
	hdr->height = *write;
	char size = fgetc(file); //namesize
	hdr-> namesize = size;
	fgets((hdr->name = malloc(size)),size,file); //name
	hdr->players = fgetc(file); // numplayers
	int i;
	for (i = 0; i < (hdr->width)*(hdr->height); i++) {
		hdr->locations[i] = malloc(sizeof(Location));
		fread(hdr->locations[i]->height, sizeof(int), 1, file);
		fread(hdr->locations[i]->type, sizeof(int), 1, file);
		hdr->locations[i]->objectcount = 0;
	}
	hdr->texid = 

/*	fread(write,sizeof(short),1,file); //unitcount
	short b = *(short *)write;  <-- shouldn't be located in map file;
	write += sizeof(short);
*/

	//memory leaks after this point

	*(char ***)write = malloc(sizeof(char *) * b);
	int a;
	for (a = 0;a < *(int *)write;a++) {
		char s = fgetc(file);
		(*(char ***)write)[a] = malloc(s+1);
		fgets((*(char ***)write)[a],s+1,file); //units
	}
	write += sizeof(char **);
/*	fread(write,sizeof(float),1,file);
	write += sizeof(float);
	fread(write,sizeof(float),1,file);
	write += sizeof(float);
*/

	return hdr;
}
