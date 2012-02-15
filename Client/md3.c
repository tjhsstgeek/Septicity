#include "md3.h"
#include <stdio.h>
#include <malloc.h>
#include <stdint.h>
#include <string.h>

void md3_clean_safe(MD3 *cur) {
	int a,b;
	if (cur->frames) {
		for (a = 0;a < cur->numframes;a++) {
			if (cur->frames[a])
				free(cur->frames[a]);
		}
		free(cur->frames);
	}
	if (cur->tags) {
		for (a = 0;a < cur->numtags;a++) {
			if (cur->tags[a])
				free(cur->tags[a]);
		}
		free(cur->tags);
	}
	if (cur->surfaces) {
		for (a = 0;a < cur->numsurfaces;a++) {
			Surface *sur = cur->surfaces[a];
			if (sur) {
				if (sur->shaders) {
					for (b = 0;b < sur->numshaders;b++) {
						if (sur->shaders[a])
							free(sur->shaders[a]);
					}
					free(sur->shaders);
				}
				if (sur->triangles) {
					for (b = 0;b < sur->numtriangles;b++) {
						if (sur->triangles[a])
							free(sur->triangles[a]);
					}
					free(sur->triangles);
				}
				if (sur->texcoords) {
					for (b = 0;b < sur->numverts;b++) {
						if (sur->texcoords[a])
							free(sur->texcoords[a]);
					}
					free(sur->texcoords);
				}
				if (sur->vertices) {
					for (b = 0;b < sur->numverts *
					               sur->numframes;b++) {
						if (sur->vertices[a])
							free(sur->vertices[a]);
					}
					free(sur->vertices);
				}
				free(sur);
			}
		}
		free(cur->surfaces);
	}
	memset(cur,0,sizeof(MD3));
}
void md3_clean(MD3 *cur) {
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
		for (b = 0;b < sur->numverts * sur->numframes;b++) {
			free(sur->vertices[a]);
		}
		free(sur->vertices);
		free(sur);
	}
	free(cur->surfaces);
	memset(cur,0,sizeof(MD3));
}

void md3_free_safe(MD3 *cur) {
	md3_clean_safe(cur);
	free(cur);
}
void md3_free(MD3 *cur) {
	md3_clean(cur);
	free(cur);
}
MD3 *md3_create() {
	return calloc(1,sizeof(MD3));//Fills with zeros
}
uint32_t md3_stream_search_file(FILE *in) {
	uint8_t need [] = {0x49,0x44,0x50,0x33};
	int loc = 0;
	for (;loc < 4;loc++) {
		int check = fgetc(in);
		if (check == -1) {
			return ftell(in);
		}
		if (check != need[loc]) {//We didn't find the entire piece. 
			while (check != 0x49) {//Find the first part and continue from there
				check = fgetc(in);
				if (check == -1) {
					return ftell(in);
				}
			}
			loc = 0;
		}
	}
	return ftell(in) - 4;
}
uint32_t md3_stream_search_buffer(char *in,uint32_t len) {
	uint32_t where = 0;
	uint8_t need [] = {0x49, 0x44, 0x50, 0x33};
	int loc = 0;
	for (;loc < 4;loc++) {
		if (where >= len) {
			return where;
		}
		int check = in[where++];
		if (check != need[loc]) {
			while (check != 0x49) {
				if (where >= len) {
					return where;
				}
				check = in[where++];
			}
			loc = 0;
		}
	}
	return where - 4;
}
int32_t md3_stream_check_file(FILE *in, uint32_t loc) {
	int a = 0;
	uint8_t need [] = {0x49, 0x44, 0x50, 0x33};
	fseek(in,loc,SEEK_SET);
	uint8_t check [4];
	if (!fread(check, 4, 1, in))
		return MD3_UNEXPECTED_EOF;
	return (memcmp(check, need, 4))?MD3_BAD_HEADER:0;
}
int32_t md3_stream_check_buffer(char *in, uint32_t len, uint32_t loc) {
	if (len < 4 + loc)
		return MD3_UNEXPECTED_EOF;
	uint8_t need [] = {0x49,0x44,0x50,0x33};
	return (memcmp(need,in + loc,4))?MD3_BAD_HEADER:0;
}
int32_t md3_stream_read_file(FILE *in, uint32_t loc, MD3 *cur) {
	if (fread(cur,sizeof(int)*10 + 64,1,in) != 1) {
		return MD3_UNEXPECTED_EOF;
	}
	fseek(in,cur->ofsframes + loc,SEEK_SET);
	cur->frames = calloc(1,sizeof(Frame *) * cur->numframes);
	if (!cur->frames) {
		return MD3_FAILED_ALLOC;
	}
	uint32_t a;
	for (a = 0;a < cur->numframes;a++) {
		cur->frames[a] = malloc(sizeof(Frame));
		if (!cur->frames[a] ||
		    !fread(cur->frames[a], sizeof(Frame), 1, in)) {
			md3_clean_safe(cur);
			return MD3_UNEXPECTED_EOF;
		}
	}
	fseek(in,cur->ofstags + loc,SEEK_SET);
	cur->tags = calloc(1,sizeof(Surface *) * cur->numtags);
	if (!cur->tags) {
		md3_clean_safe(cur);
		return MD3_FAILED_ALLOC;
	}
	for (a = 0;a < cur->numtags;a++) {
		cur->tags[a] = malloc(sizeof(Tag));
		if (!cur->tags[a] || !fread(cur->tags[a],sizeof(Tag),1,in)) {
			md3_clean_safe(cur);
			return MD3_UNEXPECTED_EOF;
		}
	}
	fseek(in,cur->ofssurfaces + loc,SEEK_SET);
	cur->surfaces = calloc(1,sizeof(Surface *) * cur->numsurfaces);
	if (!cur->surfaces) {
		md3_clean_safe(cur);
		return MD3_FAILED_ALLOC;
	}
	for (a = 0;a < cur->numsurfaces;a++) {
		long loc = ftell(in);
		//printf("%i\n",loc);
		cur->surfaces[a] = calloc(1,sizeof(Surface));
		if (!cur->surfaces[a]
		 || !fread(cur->surfaces[a], sizeof(Surface) - sizeof(Shader **)
		                           - sizeof(Triangle **)
		                           - sizeof(TexCoord **)
		                           - sizeof(Vertex **), 1, in)) {
			md3_clean_safe(cur);
			return MD3_UNEXPECTED_EOF;
		}
		Surface *sur = cur->surfaces[a];
		sur->shaders = calloc(1, sizeof(Shader *) * sur->numshaders);
		sur->triangles = calloc(1,
		                        sizeof(Triangle *) * sur->numtriangles);
		sur->texcoords = calloc(1, sizeof(TexCoord *) * sur->numverts);
		sur->vertices = calloc(1, sizeof(Vertex *) * sur->numverts);
		if (!sur->shaders || !sur->triangles || !sur->texcoords ||
		    !sur->vertices) {
			md3_clean_safe(cur);
			return MD3_FAILED_ALLOC;
		}
		//printf("%i %i %i %i %i %i %i %i %s\n",sur->numshaders,sur->numverts,sur->numtriangles,sur->ofsshaders,sur->ofstriangles,sur->ofsst,sur->ofsverts,sur->ofsend,sur->name);
		fseek(in,loc + sur->ofsshaders,SEEK_SET);
		uint32_t b;
		for (b = 0;b < sur->numshaders;b++) {
			sur->shaders[b] = malloc(sizeof(Shader));
			if (!sur->shaders[b] ||
			    !fread(sur->shaders[b], sizeof(Shader), 1, in)) {
				md3_clean_safe(cur);
				return MD3_UNEXPECTED_EOF;
			}
		}
		fseek(in, loc + sur->ofstriangles, SEEK_SET);
		for (b = 0;b < sur->numtriangles;b++) {
			sur->triangles[b] = malloc(sizeof(Triangle));
			if (!sur->triangles[b] ||
			    !fread(sur->triangles[b],sizeof(Triangle),1,in)) {
				md3_clean_safe(cur);
				return MD3_UNEXPECTED_EOF;
			}
		}
		fseek(in,loc + sur->ofsst,SEEK_SET);
		for (b = 0;b < sur->numverts;b++) {
			sur->texcoords[b] = malloc(sizeof(TexCoord));
			if (!sur->texcoords[b] ||
			    !fread(sur->texcoords[b], sizeof(TexCoord), 1,
			           in)) {
				md3_clean_safe(cur);
				return MD3_UNEXPECTED_EOF;
			}
		}
		fseek(in,loc + sur->ofsverts,SEEK_SET);
		for (b = 0;b < sur->numverts;b++) {
			sur->vertices[b] = malloc(sizeof(Vertex));
			if (!sur->vertices[b] ||
			    !fread(sur->vertices[b], sizeof(Vertex), 1, in)) {
				md3_clean_safe(cur);
				return MD3_UNEXPECTED_EOF;
			}
		}
		fseek(in, sur->ofsend, SEEK_SET);
	}
	return 0;
}
int32_t md3_stream_read_buffer(char *in,uint32_t len,uint32_t loc,MD3 *cur) {
	char *start = in;
	char *end = in + len;
	char *mid = in + loc;
	if (start + sizeof(int) * 10 + 64 > end) {
		return MD3_UNEXPECTED_EOF;
	}
	in = mid;
	memcpy(cur,in,sizeof(int) * 10 + 64);
	in = mid + cur->ofsframes;
	if (in < start || in + sizeof(Frame) * cur->numframes > end) {
		return MD3_UNEXPECTED_EOF;
	}
	cur->frames = calloc(1,sizeof(Frame *) * cur->numframes);
	if (!cur->frames) {
		return MD3_FAILED_ALLOC;
	}
	uint32_t a;
	for (a = 0;a < cur->numframes;a++) {
		cur->frames[a] = malloc(sizeof(Frame));
		if (!cur->frames[a]) {
			md3_clean_safe(cur);
			return MD3_UNEXPECTED_EOF;
		}
		memcpy(cur->frames[a],in,sizeof(Frame));
		in += sizeof(Frame);
	}
	in = mid + cur->ofstags;
	if (in < start || in + sizeof(Tag) * cur->numtags > end) {
		return MD3_UNEXPECTED_EOF;
	}
	cur->tags = calloc(1,sizeof(Tag *) * cur->numtags);
	if (!cur->tags) {
		md3_clean_safe(cur);
		return MD3_FAILED_ALLOC;
	}
	for (a = 0;a < cur->numtags;a++) {
		cur->tags[a] = malloc(sizeof(Tag));
		if (!cur->tags[a]) {
			md3_clean_safe(cur);
			return MD3_UNEXPECTED_EOF;
		}
		memcpy(cur->tags[a],in,sizeof(Tag));
		in += sizeof(Tag);
	}
	in = mid + cur->ofssurfaces;
	if (in < start || in + (sizeof(int) * 11 + 64) * cur->numtags > end) {//If we don't have the minimum, don't try. 
		return MD3_UNEXPECTED_EOF;
	}
	cur->surfaces = calloc(1,sizeof(Surface *) * cur->numsurfaces);
	if (!cur->surfaces) {
		md3_clean_safe(cur);
		return MD3_FAILED_ALLOC;
	}
	for (a = 0;a < cur->numsurfaces;a++) {
		char *sloc = in;
		if (in + sizeof(int) * 11 + 64 > end) {
			md3_clean_safe(cur);
			return MD3_UNEXPECTED_EOF;
		}
		cur->surfaces[a] = calloc(1,sizeof(Surface));
		if (!cur->surfaces[a]) {
			md3_clean_safe(cur);
			return MD3_FAILED_ALLOC;
		}
		memcpy(cur->surfaces[a],in,sizeof(int) * 11 + 64);
		Surface *sur = cur->surfaces[a];
		if (sur->numframes != cur->numframes) {
			md3_clean_safe(cur);
			return MD3_FRAME_MISMATCH;
		}
		sur->shaders = calloc(1,sizeof(Shader *) * sur->numshaders);
		sur->triangles = calloc(1,sizeof(Triangle *) * sur->numtriangles);
		sur->texcoords = calloc(1,sizeof(TexCoord *) * sur->numverts);
		sur->vertices = calloc(1,sizeof(Vertex *) * sur->numverts * sur->numframes);
		if (!sur->shaders || !sur->triangles || !sur->texcoords || !sur->vertices) {
			md3_clean_safe(cur);
			return MD3_FAILED_ALLOC;
		}
		in = sloc + sur->ofsshaders;
		if (start > in || in + sizeof(Shader) * sur->numshaders > end) {
			md3_clean_safe(cur);
			return MD3_UNEXPECTED_EOF;
		}
		uint32_t b;
		for (b = 0;b < sur->numshaders;b++) {
			sur->shaders[b] = malloc(sizeof(Shader));
			if (!sur->shaders[b]) {
				md3_clean_safe(cur);
				return MD3_FAILED_ALLOC;
			}
			memcpy(sur->shaders[b],in,sizeof(Shader));
			in += sizeof(Shader);
		}
		in = sloc + sur->ofstriangles;
		if (start > in || in + sizeof(Triangle) * sur->numtriangles > end) {
			md3_clean_safe(cur);
			return MD3_UNEXPECTED_EOF;
		}
		for (b = 0;b < sur->numtriangles;b++) {
			sur->triangles[b] = malloc(sizeof(Triangle));
			if (!sur->triangles[b]) {
				md3_clean_safe(cur);
				return MD3_FAILED_ALLOC;
			}
			memcpy(sur->triangles[b],in,sizeof(Triangle));
			in += sizeof(Triangle);
		}
		in = sloc + sur->ofsst;
		if (start > in || in + sizeof(TexCoord) * sur->numverts > end) {
			md3_clean_safe(cur);
			return MD3_UNEXPECTED_EOF;
		}
		for (b = 0;b < sur->numverts;b++) {
			sur->texcoords[b] = malloc(sizeof(TexCoord));
			if (!sur->texcoords[b]) {
				md3_clean_safe(cur);
				return MD3_FAILED_ALLOC;
			}
			memcpy(sur->texcoords[b],in,sizeof(TexCoord));
			in += sizeof(TexCoord);
		}
		in = sloc + sur->ofsverts;
		if (start > in || in + sizeof(TexCoord) * sur->numverts * sur->numframes > end) {
			md3_clean_safe(cur);
			return MD3_UNEXPECTED_EOF;
		}
		for (b = 0;b < sur->numverts * sur->numframes;b++) {
			sur->vertices[b] = malloc(sizeof(Vertex));
			if (!sur->vertices[b]) {
				md3_clean_safe(cur);
				return MD3_FAILED_ALLOC;
			}
			memcpy(sur->vertices[b],in,sizeof(Vertex));
			in += sizeof(Vertex);
		}
		in = sloc + sur->ofsend;
		if (in < start) {
			md3_clean_safe(cur);
			return MD3_UNEXPECTED_EOF;
		}
	}
	return 0;
}
