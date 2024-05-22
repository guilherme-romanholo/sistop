#ifndef SISTOP_INTERFACE_H
#define SISTOP_INTERFACE_H

#include "../memory/memory.h"
#include <ncurses.h>

typedef struct {
    WINDOW *window;     // Janela principal
    WINDOW *subwindow;  // Sub-janela para o conteúdo com scroll
    int max_lines;      // Número máximo de linhas antes de aplicar scroll
    int current_line;   // Índice da próxima linha para adicionar conteúdo
} Window;

int Interface__create();
void Interface__draw_window(Window *, const char *);
Window *Interface__create_window(int, int, int, int, const char *, int );
void Interface__add_line(Window *, const char *);
void Interface__print_segment(Segment *, char *);
void Interface__print_remaining_memory(int);

#endif //SISTOP_INTERFACE_H
