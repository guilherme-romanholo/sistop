#ifndef SISTOP_INTERFACE_H
#define SISTOP_INTERFACE_H

#include <ncurses.h>

void Interface__init();
WINDOW *Interface__create_window(int height, int width, int starty, int startx, char *title);

#endif //SISTOP_INTERFACE_H
