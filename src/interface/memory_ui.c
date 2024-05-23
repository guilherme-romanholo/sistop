#include "interface.h"

Window *memory_win;
sem_t memory_ui_sem;

void Interface__create_memory_ui(int height, int width) {
    memory_win = Interface__create_window(height, width,
                                          height, 0, "Memory",
                                          height - 2, TRUE);
    sem_init(&memory_ui_sem, 0, 0);
}