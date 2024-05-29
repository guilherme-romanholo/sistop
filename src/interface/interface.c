#include "interface.h"
#include "../kernel/kernel.h"
#include <string.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

sem_t update_mutex;
sem_t data_mutex;

Interface *menu_win;
Interface *kernel_win;
Interface *mem_win;
Interface *proc_win;
Interface *sched_win;

Interface *temp_win;
char data[BUFFER_SIZE];

void Interface__init() {
    int height, width;
    char input[INPUT_SIZE];
    pthread_t thread;

    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);
    noecho();

    getmaxyx(stdscr, height, width);

    // Create menu interface
    menu_win = Interface__create_window(
            height / 4,
            width / 2,
            0, 0,
            "MENU", FALSE);

    // Create kernel interface
    kernel_win = Interface__create_window(
            height / 4,
            width / 2,
            height / 4, 0,
            "KERNEL", FALSE);

    // Create memory interface
    mem_win = Interface__create_window(
            (height / 2) + 1,
            width / 2,
            (height / 2) - 1, 0,
            "MEMORY",TRUE);

    // Create process interface
    proc_win = Interface__create_window(
            (height / 2) - 1,
            width / 2,
            0, (width / 2),
            "PROCESS",TRUE);

    // Create scheduler interface
    sched_win = Interface__create_window(
            (height / 2) + 1,
            width / 2,
            (height / 2) - 1, (width / 2),
            "SCHEDULER",TRUE);

    sem_init(&update_mutex, 0, 0);
    sem_init(&data_mutex, 0, 1);

    pthread_create(&thread, NULL, (void *) Interface__update, NULL);

    Interface__refresh_kernel_win();
    Interface__refresh_process_win();
    Interface__refresh_memory_win();

    while (TRUE) {
        char path[20] = "../synt/";

        Interface__input_menu(input);

        if (!strcmp(input, "q"))
            break;
            // TODO: Arquivo errado

        strcat(path, input);
        Kernel__syscall(CREATE_PROCESS, (void *) path);
    }

    Interface__delete_window(menu_win);
    Interface__delete_window(kernel_win);
    Interface__delete_window(mem_win);
    Interface__delete_window(sched_win);
    Interface__delete_window(proc_win);

    sem_destroy(&update_mutex);
    sem_destroy(&data_mutex);

    endwin();
}

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

void Interface__delete_window(Interface *window) {
    delwin(window->subwindow);
    delwin(window->window);
}

/// Add line in interface subwindow
/// \param interface Interface
/// \param width X position
/// \param text Text to be inserted
void Interface__add_line(Interface *interface, int width, const char *text) {
    if (interface->curr_line >= interface->max_lines) {
        wscrl(interface->subwindow, 1);
        interface->curr_line--;
    }

    mvwprintw(interface->subwindow, interface->curr_line++, width, "%s", text);
    wrefresh(interface->subwindow);
}

void Interface__input_menu(char *input) {
    int width;

    width = getmaxx(menu_win->subwindow);

    Interface__add_line(menu_win, (width / 2) - 18, " ___  ____  ___  ____  _____  ____ ");
    Interface__add_line(menu_win, (width / 2) - 18, "/ __)(_  _)/ __)(_  _)(  _  )(  _ \\");
    Interface__add_line(menu_win, (width / 2) - 18, "\\__ \\ _)(_ \\__ \\  )(   )(_)(  )___/");
    Interface__add_line(menu_win, (width / 2) - 18, "(___/(____)(___/ (__) (_____)(__)  ");

    menu_win->curr_line++;

    Interface__add_line(menu_win, (width / 2) - 9, "Press 'q' to exit");
    Interface__add_line(menu_win, (width / 2) - 9, "Process name: ");

    wclrtoeol(menu_win->subwindow);
    wrefresh(menu_win->subwindow);

    menu_win->curr_line = 0;

    echo();
    wgetnstr(menu_win->subwindow, input, INPUT_SIZE);
    noecho();
}

void Interface__refresh_kernel_win() {
    char semaphores[BUFFER_SIZE] = "";
    char sem[3] = "x ";

    for (Node *s = kernel->semaph_table->head; s != NULL ; s = s->next) {
        sem[0] = ((Semaphore *)s->content)->name;
        strcat(semaphores, sem);
    }

    werase(kernel_win->subwindow);
    kernel_win->curr_line = 0;

    Interface__send_data(kernel_win, KERNEL_MEMORY_FMT, kernel->remaining_memory);
    Interface__send_data(kernel_win, KERNEL_SEMAPH_FMT, semaphores);
}

void Interface__refresh_process_win() {
    werase(proc_win->subwindow);
    proc_win->curr_line = 0;
    wrefresh(proc_win->subwindow);

    for (Node *p = kernel->pcb->head; p != NULL ; p = p->next) {
        Process *proc = (Process *) p->content;

        Interface__send_data(proc_win, PROC_INTERFACE_FMT, proc->pid, proc->name,
                             proc->segment_id, proc->priority,
                             proc->pc, Interface__cast_process_state(proc->state));
    }
}

void Interface__refresh_memory_win() {
    werase(mem_win->subwindow);
    mem_win->curr_line = 0;
    wrefresh(mem_win->subwindow);

    for (Node *s = kernel->segment_table->head; s != NULL ; s = s->next) {
        Segment *segment = (Segment *) s->content;
        Interface__send_data(mem_win, MEMORY_INTERFACE_FMT, segment->seg_id,
                             segment->seg_size / KBYTE, segment->pages->size);
    }
}

/// Cast process int state to display string
/// \param state Process int state
/// \return String state
char *Interface__cast_process_state(int state) {
    switch (state) {
        case NEW:
            return "New";
        case READY:
            return "Ready";
        case RUNNING:
            return "Running";
        case WAITING:
            return "Waiting";
        case TERMINATED:
            return "Terminated";
        default:
            return "Error";
    }
}

void Interface__send_data(Interface *interface, const char *format, ...) {
    va_list args;

    sem_wait(&data_mutex);

    va_start(args, format);
    vsprintf(data, format, args);
    va_end(args);

    temp_win = interface;

    sem_post(&update_mutex);
}

void Interface__update() {
    while (TRUE) {
        sem_wait(&update_mutex);

        usleep(200); 

        Interface__add_line(temp_win, 1, data);

        //usleep(200);

        sem_post(&data_mutex);
    }
}