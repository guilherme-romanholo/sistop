#ifndef SISTOP_INTERFACE_H
#define SISTOP_INTERFACE_H

#define INPUT_SIZE 10
#define SLEEP_TIME 250

#include <ncurses.h>
#include <semaphore.h>
#include <pthread.h>

typedef struct {
    // Windows
    WINDOW *window;
    WINDOW *subwindow;

    // Lines
    int max_lines;
    int curr_line;

    // Threads
    sem_t mutex;
    pthread_t thread;
} Interface;

extern Interface *kernel_interface;
extern Interface *memory_interface;
extern Interface *process_interface;

void Interface__init();
void Interface__destroy(Interface *);
Interface *Interface__create_window(int height, int width, int starty, int startx, char *title, int scroll);
void Interface__add_line(Interface *interface, int width, const char *text);
void Interface__input_menu(char *input);
void Interface__update_kernel_window();
void Interface__kernel_semaphores();
void Interface__update_memory_window();
void Interface__update_process_window();
char *Interface__cast_process_state(int);

#endif //SISTOP_INTERFACE_H
