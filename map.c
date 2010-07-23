#include "Shared/map.h"

int main(int argc,char **argv) {
	if (argc < 2)
		return 1;
	MapHeader *m = loadMap(argv[1]);
	printf("%p\n",m);
	if (map_error)
		printf("ERROR: %s\n",map_error);
}
