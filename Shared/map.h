#ifndef __MAP_H__
#define __MAP_H__
extern char *map_error;
typedef struct {
	int height;
	int type;
	short objectcount;
	void **objects;//The items itself
} Location;
typedef struct {
	int magic;
	int width,height;
	char namesize;
	char *name;
	char players;
	int texid;
	Location **locations;//Characters/Items/Vegetation?
	short *unitcount;
	char **units;//The units that exist in this map
} MapHeader;
typedef MapHeader Map;
extern MapHeader *loadMap(const char *);
#endif
