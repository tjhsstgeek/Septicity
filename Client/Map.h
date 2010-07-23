#ifndef __MAP_H__
#define __MAP_H__

typedef struct _LOC {
	struct _LOC **surround;
} Location;

typedef struct {
	Location ***objects;
} Map;

//Use a triangle pathing technique where units can only move from one triangle to another bordering triangle

//SDL

//Use Server - Client configuration
//WHere client talks to server

#endif
