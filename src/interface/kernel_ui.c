#include "interface.h"
#include "../kernel/kernel.h"
#include <stdlib.h>

Window *kernel_win;
sem_t ksem_ui;
pthread_t kernel_ui_thread;

void Interface__create_kernel_ui(int height, int width) {
    kernel_win = Interface__create_window(height, width,
                                          height - 1, 0, "Kernel",
                                          height - 2, FALSE);

    sem_init(&ksem_ui, 0, 1);

    pthread_create(&kernel_ui_thread, NULL,
                   Interface__update_kernel_ui, NULL);
}

void *Interface__update_kernel_ui() {
    while (TRUE) {
        char remaining[BUFFER_SIZE];
        char semaphores[BUFFER_SIZE] = "Semaphores: ";
        char sem_name[3] = "x ";

        sem_wait(&ksem_ui);

        werase(kernel_win->window);

        sprintf(remaining, "Remaining Memory: %d bytes.", kernel->seg_table->remaining_memory);
        Interface__add_line_scroll(kernel_win, remaining);

        for (Node *aux = kernel->sem_table->head; aux != NULL; aux = aux->next) {
            sem_name[0] = ((Semaphore *) aux->content)->name;
            strcat(semaphores, sem_name);
        }
        Interface__add_line(kernel_win, semaphores);

        kernel_win->current_line--;
    }
}
