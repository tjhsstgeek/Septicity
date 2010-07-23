#ifndef __UNIT_H__
#define __UNIT_H__

#include "rts.h"
#include "md3.h"

typedef struct  {
	int magic;
	char* name;
	float x, y, z;
	char* configfile;
	char* md3file;
	char* texfile;
	float length, width, height;
	MD3* model;
	int texid;

} Unit;

Unit* unit_init(char* config);



#endif
