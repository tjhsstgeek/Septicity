#include "rts.h"
#include "md3.h"

int main(int argc,char ** argv) {
//	if (argc < 2) {
//		return 1;
//	}
//	MD3 *m = load_md3(argv[1]);
//	printf("%i %s %i %i %i %i %i %i %i %i %i\n",m->version,m->name,m->flags,m->numframes,m->numtags,m->numsurfaces,m->numskins,m->ofsframes,m->ofstags,m->ofssurfaces,m->ofseof);

	if (init()) {
		return 1;
	}
	listen_window();
	render_screen();
	return 0;
}
