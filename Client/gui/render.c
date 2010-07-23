#include "gui.h"

void (*render_array [])(component *) = {
	render_panel,render_button,render_checkbox,render_window
};
void render_gui(component *c) {
	render_array[(unsigned char)c->type](c);
}
