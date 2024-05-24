#ifndef SISTOP_INTERFACE_H
#define SISTOP_INTERFACE_H

#define INPUT_SIZE 10

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

void Interface__init();
Interface *Interface__create_window(int height, int width, int starty, int startx, char *title);
void Interface__input_menu(char *input);
void Interface__update_kernel_window();

#endif //SISTOP_INTERFACE_H
