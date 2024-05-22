#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "interface.h"
#include "../kernel/kernel.h"

Window *menu_win;
Window *kernel_win;
Window *memory_win;
Window *process_win;
Window *execution_win;

sem_t kernel_win_sem;

// Função para desenhar uma borda com título em uma janela
void Interface__draw_window(Window *win, const char *title) {
    int start_x = (getmaxx(win->window) - strlen(title)) / 2;
    box(win->window, 0, 0);

    mvwprintw(win->window, 0, start_x, " %s ", title);
    wrefresh(win->window);
}

Window *Interface__create_window(int height, int width, int starty, int startx, const char *title, int max_lines, int scroll_bool) {
    Window *win = malloc(sizeof(Window));
    win->window = newwin(height, width, starty, startx);
    win->subwindow = derwin(win->window, height - 2, width - 2, 1, 1);
    win->max_lines = max_lines;
    win->current_line = 0;

    Interface__draw_window(win, title);
    scrollok(win->subwindow, scroll_bool); // Habilita o scroll na sub-janela

    return win;
}

// Função para adicionar uma linha de texto a uma janela com scroll
void Interface__add_line(Window *win, const char *text) {
    // Verifica se a janela atingiu ou ultrapassou o número máximo de linhas
    if (win->current_line >= win->max_lines) {
        // Aplica scroll para cima
        wscrl(win->subwindow, 1);
        win->current_line--;
    }

    // Adiciona a linha de texto à sub-janela
    mvwprintw(win->subwindow, win->current_line++, 0, "%s", text);
    wrefresh(win->subwindow);
}

void Interface__add_line_no(Window *win, const char *text) {
    // Adiciona a linha de texto à sub-janela
    mvwprintw(win->subwindow, win->current_line, 0, "%s", text);
    wrefresh(win->subwindow);
}

void *Interface__update_kernel_win() {
    while (1) {
        sem_wait(&kernel_win_sem);

        char *output = malloc(sizeof(char) * BUFFER_SIZE);

        sprintf(output, "Kernel Remaining: %d bytes.", kernel->seg_table->remaining_memory);

        Interface__add_line_no(kernel_win, output);
    }
}

void Interface__print_remaining_memory(int remaining) {
    char *output = malloc(sizeof(char) * BUFFER_SIZE);
    sprintf(output, "Kernel Remaining: %d bytes.", remaining);
    Interface__add_line(memory_win, output);
}

void Interface__print_segment(Segment *segment, char *process_name) {
    char *output = malloc(sizeof(char) * BUFFER_SIZE);

    sprintf(output, "Seg ID: %d, Seg Size: %d bytes, Proc Name: %s",segment->seg_id, segment->seg_size, process_name);

    Interface__add_line(memory_win, output);
}

int Interface__create() {
    // Inicializa a ncurses
    initscr();
    cbreak();
    noecho();

    // Obtém o tamanho da tela
    int height, width;
    getmaxyx(stdscr, height, width);

    // Calcula as dimensões de cada janela
    int win_height = height / 2;
    int win_width = width / 2;

    // Define o número máximo de linhas para scroll
    int max_lines = win_height - 2;

    // Cria as janelas
    menu_win = Interface__create_window(win_height / 2, win_width, 0, 0, "Menu", max_lines, TRUE);
    kernel_win = Interface__create_window((win_height / 2) + 1, win_width, win_height / 2, 0, "Kernel", max_lines, FALSE);
    process_win = Interface__create_window(win_height, win_width, 0, win_width, "Process", max_lines, TRUE);
    execution_win = Interface__create_window(win_height, win_width, win_height, win_width, "Execution", max_lines, TRUE);

    // Memory Window
    memory_win = Interface__create_window(win_height, win_width, win_height, 0, "Memory", max_lines, TRUE);

    // Menu de entrada de dados
    mvwprintw(menu_win->window, 1, 1, "Pressione 'q' para sair");
    wrefresh(menu_win->window);

    sem_init(&kernel_win_sem, 0, 1);

    pthread_t kernel_win_thread;
    pthread_create(&kernel_win_thread, NULL, Interface__update_kernel_win, NULL);

    // Prepara as variáveis para entrada de dados
    char input[100];

    // Loop principal
    while (1) {
        // Limpa a linha de entrada do menu
        mvwprintw(menu_win->window, 2, 1, "Process name: ");
        wclrtoeol(menu_win->window);
        wrefresh(menu_win->window);

        // Obtém a entrada do usuário
        echo();
        wgetnstr(menu_win->window, input, 100);
        noecho();

        // Verifica se o usuário quer sair
        if (strcmp(input, "q") == 0) {
            break;
        }

        char path[30] = "../synt/";
        strcat(path, input);
        Process__create(path);
    }

    // Finaliza a ncurses
    endwin();

    // Libera a memória alocada para as janelas
    free(menu_win);
    free(process_win);
    free(memory_win);
    free(execution_win);

    return 0;
}
