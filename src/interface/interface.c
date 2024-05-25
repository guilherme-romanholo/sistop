#include "interface.h"
#include "../process/process.h"
#include "../kernel/kernel.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

Interface *menu_interface;
Interface *kernel_interface;
Interface *memory_interface;

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
            "MENU", FALSE);

    // Create kernel interface
    kernel_interface = Interface__create_window(
            height / 4,
            width / 2,
            height / 4, 0,
            "KERNEL", FALSE);

    // Create memory interface
    memory_interface = Interface__create_window(
            (height / 2) + 1,
            width / 2,
            (height / 2) - 1, 0,
            "MEMORY",TRUE);

    // Init interfaces semaphores
    sem_init(&kernel_interface->mutex, 0, 1);
    sem_init(&memory_interface->mutex, 0, 0);

    // Init interfaces threads
    pthread_create(&kernel_interface->thread,
                   NULL,
                   (void *) Interface__update_kernel_window,
                   NULL);

    pthread_create(&memory_interface->thread,
                   NULL,
                   (void *) Interface__update_memory_window,
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

/*
 * ###################
 * #     GENERAL     #
 * ###################
 * */

/// Create an interface component
/// \param height Window height
/// \param width Window width
/// \param starty Window Y start position
/// \param startx Window X start position
/// \param title Window title
/// \return Interface component
Interface *Interface__create_window(int height, int width, int starty, int startx, char *title, int scroll) {
    Interface *interface = malloc(sizeof(Interface));

    // Initialize values
    interface->window = newwin(height, width, starty, startx);
    interface->subwindow = derwin(interface->window, height - 2, width - 2, 1, 1);
    interface->max_lines = height - 2;
    interface->curr_line = 0;
    scrollok(interface->subwindow, scroll);

    // Create border with title
    box(interface->window, 0, 0);
    mvwprintw(interface->window, 0, 1, " %s ", title);
    wrefresh(interface->window);

    return interface;
}

void Interface__add_line(Interface *interface, int width, const char *text) {
    if (interface->curr_line >= interface->max_lines) {
        wscrl(interface->subwindow, 1);
        interface->curr_line--;
    }

    mvwprintw(interface->subwindow, interface->curr_line++, width, "%s", text);
    wrefresh(interface->subwindow);
}

/*
 * ##################
 * #      MENU      #
 * ##################
 * */

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

/*
 * ##################
 * #     KERNEL     #
 * ##################
 * */

void Interface__update_kernel_window() {
    while (TRUE) {
        sem_wait(&kernel_interface->mutex);

        usleep(SLEEP_TIME);

        mvwprintw(kernel_interface->subwindow, 1, 1, "Remaining Memory: %d bytes.", kernel->seg_table->remaining_memory);

        usleep(SLEEP_TIME);

        char semaphores[BUFFER_SIZE] = "";
        char sem[6] = "x(y) ";

        for (Node *aux = kernel->sem_table->head; aux != NULL ; aux = aux->next) {
            sem[0] = ((Semaphore *) aux->content)->name;
            sem[2] = ((Semaphore *) aux->content)->val + '0';
            strcat(semaphores, sem);
        }
        mvwprintw(kernel_interface->subwindow, 2, 1, "Semaphores: %s", semaphores);

        wrefresh(kernel_interface->subwindow);
    }
}

/*
 * ##################
 * #     MEMORY     #
 * ##################
 * */

void Interface__update_memory_window() {
    while (TRUE) {
        sem_wait(&memory_interface->mutex);

        Segment *segment;
        char output[BUFFER_SIZE];

        usleep(SLEEP_TIME);

        werase(memory_interface->subwindow);

        for (Node *aux = kernel->seg_table->seg_list->head; aux != NULL ; aux = aux->next) {
            segment = (Segment *) aux->content;

            sprintf(output, "Seg Id: %d, Seg Size: %d, Num Pages: %d.", segment->seg_id,
                    segment->seg_size, segment->pages->size);

            usleep(SLEEP_TIME);

            Interface__add_line(memory_interface, 0, output);
        }

        memory_interface->curr_line = 0;
    }
}
