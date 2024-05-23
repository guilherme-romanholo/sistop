#include <string.h>
#include <stdlib.h>
#include "interface.h"

// Função para desenhar uma borda com título em uma janela
void Interface__draw_window(Window *win, const char *title) {
    int start_x = (getmaxx(win->window) - strlen(title)) / 2;
    box(win->window, 0, 0);

    mvwprintw(win->window, 0, start_x, " %s ", title);
    wrefresh(win->window);
}

Window *Interface__create_window(int height, int width, int starty,
                                 int startx, const char *title,
                                 int max_lines, int scroll_bool) {
    Window *win = malloc(sizeof(Window));

    win->window = newwin(height, width, starty, startx);
    win->subwindow = derwin(win->window, height - 2, width - 2, 1, 1);
    win->max_lines = max_lines;
    win->current_line = 0;

    Interface__draw_window(win, title);
    scrollok(win->subwindow, scroll_bool);

    return win;
}

void Interface__add_line(Window *win, const char *text) {
    mvwprintw(win->subwindow, win->current_line, 0, "%s", text);
    wrefresh(win->subwindow);
}

// Função para adicionar uma linha de texto a uma janela com scroll
void Interface__add_line_scroll(Window *win, const char *text) {
    if (win->current_line >= win->max_lines) {
        wscrl(win->subwindow, 1);
        win->current_line--;
    }

    mvwprintw(win->subwindow, win->current_line++, 0, "%s", text);
    wrefresh(win->subwindow);
}

int Interface__create() {
    initscr();
    cbreak();
    noecho();

    int height, width;
    getmaxyx(stdscr, height, width);

    int win_height = height / 2;
    int win_width = width / 2;

    Interface__create_menu_ui(win_height / 2, win_width);
    Interface__create_memory_ui(win_height, win_width);
    Interface__create_kernel_ui((win_height / 2) + 1, win_width);

    char input[20];

    while (TRUE) {
        Interface__input_menu();

        // TODO: Make a function to input
        echo();
        wgetnstr(menu_win->window, input, 20);
        noecho();

        if (!strcmp(input, "q"))
            break;

        char path[30] = "../synt/";
        strcat(path, input);

        Process__create(path);
    }

    endwin();
    // TODO: Free semaphores, threads and windows

    return 0;
}
