#ifndef __MD3_H__
#define __MD3_H__

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
extern MD3 *load_md3(const char *);
extern void free_md3(MD3 *);
#endif
