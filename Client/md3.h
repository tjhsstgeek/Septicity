#ifndef __MD3_H__
#define __MD3_H__

#include <stdint.h>
#include <stdio.h>

#define MD3_STREAM_NOT_FOUND -0x1
#define MD3_UNEXPECTED_EOF -0x2
#define MD3_FAILED_ALLOC -0x3
#define MD3_BAD_HEADER -0x4
#define MD3_FRAME_MISMATCH -0x5

typedef struct {
	float x,y,z;
} VEC3;
typedef struct {
	VEC3 minbounds,maxbounds,localorigin;
	float radius;
	char name [16];
} Frame;
typedef struct {
	char name [64];
	VEC3 origin;
	VEC3 axis[3];
} Tag;
typedef struct {
	char name [64];
	int shaderindex;
} Shader;
typedef struct {
	int indexes [3];
} Triangle;
typedef struct {
	float ST [2];
} TexCoord;//
typedef struct {
	short coord [3];
	unsigned char normal [2];
} Vertex;
typedef struct {
	int ident;
	char name [64];
	int flags;
	int numframes,numshaders,numverts,numtriangles,ofstriangles,ofsshaders,ofsst,ofsverts,ofsend;
	Shader **shaders;
	Triangle **triangles;
	TexCoord ** texcoords;
	Vertex ** vertices;
} Surface;
typedef struct {
	int version;
	char name [64];
	int flags;
	int numframes,numtags,numsurfaces,numskins,ofsframes,ofstags,ofssurfaces,ofseof;
	Frame **frames;
	Tag **tags;
	Surface **surfaces;
} MD3_Header;
typedef MD3_Header MD3;

extern void md3_clean_safe(MD3 *);
extern void md3_clean(MD3 *);
extern void md3_free_safe(MD3 *);
extern void md3_free(MD3 *);
extern MD3 *md3_create();
extern uint32_t md3_stream_search_file(FILE *);
extern uint32_t md3_stream_search_buffer(char *,uint32_t);
extern int md3_stream_check_file(FILE *, uint32_t);
extern int md3_stream_check_buffer(char *, uint32_t, uint32_t);
extern int md3_stream_read_file(FILE *, uint32_t, MD3 *);
extern int32_t md3_stream_read_buffer(char *, uint32_t, uint32_t, MD3 *);
#endif
