#include "interface.h"

Window *menu_win;

void Interface__create_menu_ui(int height, int width) {
    menu_win = Interface__create_window(height, width,
                                          0, 0, "Menu",
                                          height - 2, FALSE);

    mvwprintw(menu_win->window, 1, 1, "Press 'q' to exit");
}

void Interface__input_menu() {
    mvwprintw(menu_win->window, 2, 1, "Process name: ");
    wclrtoeol(menu_win->window);
    wrefresh(menu_win->window);
}
