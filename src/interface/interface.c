#include "interface.h"

void Interface__init() {
    int height, width;

    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();

    getmaxyx(stdscr, height, width);

    WINDOW *menu_window = Interface__create_window(
            height / 2,
            width / 2,
            0, 0,
            "MENU");


    mvwprintw(menu_window, 1, 1, "Hello World!");
    wrefresh(menu_window);
    wgetch(menu_window);

    endwin();
}

WINDOW *Interface__create_window(int height, int width, int starty, int startx, char *title) {
    WINDOW *window = newwin(height, width, 0, 0);

    box(window, 0, 0);
    mvwprintw(window, 0, 1, " %s ", title);

    return window;
}
