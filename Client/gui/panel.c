#include "panel.h"

void render_panel(component *c) {
	render_component(c);
	render_children(c);
}

component *create_panel(int x,int y,int w,int h) {
	component *temp = create_component(x,y,w,h);
	temp->type = 0;
	return temp;
}

component *main_panel;
int standard_height;
int above_height;
int below_height;
void start_gui() {
	main_panel = create_panel(0,0,width,height);
	standard_height = 2*cdata['A']->height - (cdata['g']->metrics.horiBearingY>>6);
	above_height = cdata['A']->height;
	below_height = cdata['A']->height-(cdata['g']->metrics.horiBearingY>>6);
}
