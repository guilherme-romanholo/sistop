#include "interface.h"
#include "../kernel/kernel.h"
#include <stdlib.h>

Window *kernel_win;
sem_t kernel_ui_sem;
pthread_t kernel_ui_thread;

void Interface__create_kernel_ui(int height, int width) {
    kernel_win = Interface__create_window(height, width,
                                          height - 1, 0, "Kernel",
                                          height - 2, FALSE);

    sem_init(&kernel_ui_sem, 0, 1);

    pthread_create(&kernel_ui_thread, NULL,
                   Interface__update_kernel_ui, NULL);
}

void *Interface__update_kernel_ui() {
    while (TRUE) {
        sem_wait(&kernel_ui_sem);

        char output[BUFFER_SIZE];

        werase(kernel_win->window);

        sprintf(output, "Remaining Memory: %d bytes.", kernel->seg_table->remaining_memory);

        Interface__add_line_scroll(kernel_win, output);
        Interface__add_line(kernel_win, "Semphores:");
        kernel_win->current_line--;
    }
}
