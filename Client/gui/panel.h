#ifndef __PANEL_H__
#define __PANEL_H__
#include "component.h"

extern component *create_panel(int,int,int,int);
extern void render_panel(component *);
extern void start_gui();
extern component *main_panel;

#endif
