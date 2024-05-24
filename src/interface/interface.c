#include "interface.h"
#include "../process/process.h"
#include "../kernel/kernel.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

Interface *menu_interface;
Interface *kernel_interface;

/// Initialize project interface
void Interface__init() {
    int height, width;
    char input[INPUT_SIZE];

    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();

    getmaxyx(stdscr, height, width);

    // Create menu interface
    menu_interface = Interface__create_window(
            height / 4,
            width / 2,
            0, 0,
            "MENU");

    // Create kernel interface
    kernel_interface = Interface__create_window(
            height / 4,
            width / 2,
            height / 4, 0,
            "KERNEL");

    sem_init(&kernel_interface->mutex, 0, 1);

    pthread_attr_t thread_attr;
    pthread_attr_init(&thread_attr);
    pthread_attr_setscope(&thread_attr, PTHREAD_SCOPE_SYSTEM);

    pthread_create(&kernel_interface->thread,
                   NULL,
                   (void *)Interface__update_kernel_window,
                   NULL);

    while (TRUE) {
        char path[20] = "../synt/";

        Interface__input_menu(input);

        if (!strcmp(input, "q"))
            break;

        strcat(path, input);
        Process__create(path);
    }

    // TODO: Free interface

    endwin();
}

/// Create an interface component
/// \param height Window height
/// \param width Window width
/// \param starty Window Y start position
/// \param startx Window X start position
/// \param title Window title
/// \return Interface component
Interface *Interface__create_window(int height, int width, int starty, int startx, char *title) {
    Interface *interface = malloc(sizeof(Interface));

    // Initialize values
    interface->window = newwin(height, width, starty, startx);
    interface->subwindow = derwin(interface->window, height - 2, width - 2, 1, 1);
    interface->max_lines = height - 2;
    interface->curr_line = starty;

    // Create border with title
    box(interface->window, 0, 0);
    mvwprintw(interface->window, 0, 1, " %s ", title);
    wrefresh(interface->window);

    return interface;
}

void Interface__add_line(Interface *interface, int width, const char *text) {
    mvwprintw(interface->subwindow, interface->curr_line++, width, "%s", text);
    wrefresh(interface->subwindow);
}

void Interface__input_menu(char *input) {
    int width;

    width = getmaxx(menu_interface->subwindow);

    Interface__add_line(menu_interface, (width / 2) - 18, " ___  ____  ___  ____  _____  ____ ");
    Interface__add_line(menu_interface, (width / 2) - 18, "/ __)(_  _)/ __)(_  _)(  _  )(  _ \\");
    Interface__add_line(menu_interface, (width / 2) - 18, "\\__ \\ _)(_ \\__ \\  )(   )(_)(  )___/");
    Interface__add_line(menu_interface, (width / 2) - 18, "(___/(____)(___/ (__) (_____)(__)  ");

    menu_interface->curr_line++;

    Interface__add_line(menu_interface, (width / 2) - 9, "Press 'q' to exit");
    Interface__add_line(menu_interface, (width / 2) - 9, "Process name: ");

    wclrtoeol(menu_interface->subwindow);
    wrefresh(menu_interface->subwindow);

    menu_interface->curr_line = 0;

    echo();
    wgetnstr(menu_interface->subwindow, input, INPUT_SIZE);
    noecho();
}

void Interface__update_kernel_window() {
    while (TRUE) {
        sem_wait(&kernel_interface->mutex);

        usleep(1);

        mvwprintw(kernel_interface->window, 1, 1, "Remaining Memory: %d bytes.", kernel->seg_table->remaining_memory);
        wrefresh(kernel_interface->window);
    }
}