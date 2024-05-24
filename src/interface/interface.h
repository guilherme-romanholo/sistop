#ifndef SISTOP_INTERFACE_H
#define SISTOP_INTERFACE_H

#include "../memory/memory.h"
#include <semaphore.h>
#include <pthread.h>
#include <ncurses.h>

typedef struct {
    WINDOW *window;     // Janela principal
    WINDOW *subwindow;  // Sub-janela para o conteúdo com scroll
    int max_lines;      // Número máximo de linhas antes de aplicar scroll
    int current_line;   // Índice da próxima linha para adicionar conteúdo
} Window;

// ========== Interface ==========
int Interface__create();
void Interface__draw_window(Window *, const char *);
Window *Interface__create_window(int, int, int, int, const char *, int,int );
void Interface__add_line(Window *, const char *);
void Interface__add_line_scroll(Window *, const char *);

// ========== Memory ==========
void Interface__create_memory_ui(int height, int width);

// ========== Menu ==========
extern Window *menu_win;
void Interface__create_menu_ui(int height, int width);
void Interface__input_menu();

// ========== Kernel ==========
extern sem_t ksem_ui;
void Interface__create_kernel_ui(int height, int width);
void *Interface__update_kernel_ui();

#endif //SISTOP_INTERFACE_H
