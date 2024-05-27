#ifndef SISTOP_INTERFACE_H
#define SISTOP_INTERFACE_H

#include <ncurses.h>

#define INPUT_SIZE 20

#define KERNEL_MEMORY_FMT "Remaining Memory: %d bytes."
#define KERNEL_SEMAPH_FMT "Semaphores: %s"

#define PROC_INTERFACE_FMT "PID %d, Name: %s, Seg ID: %d, State: %s, Priority: %d, PC: %d"
#define MEMORY_INTERFACE_FMT "Segment ID: %d, Segment Size: %d Kb, Num Pages: %d."

#define SCHED_PRINT_FMT "PID: %d, Print: %d ut, Remaing quantum: %d ut."
#define SCHED_WRITE_FMT "PID: %d, Write: %d ut, Remaing quantum: %d ut."
#define SCHED_READ_FMT "PID: %d, Read: %d ut, Remaing quantum: %d ut."
#define SCHED_EXEC_FMT "PID: %d, Exec: %d ut, Remaing quantum: %d ut."
#define SCHED_SEMV_FMT "PID: %d, Release Semaph %c, Remaing quantum: %d ut."
#define SCHED_SEMP_FMT "PID: %d, Request Semaph %c, Remaing quantum: %d ut."
#define SCHED_SEM_BLOCK_FMT "PID: %d, Blocked by Semaph %c."
#define SCHED_QUANTUM_TIMEOUT_FMT "PID: %d, Quantum timeout."


typedef struct {
    WINDOW *window;
    WINDOW *subwindow;
    int max_lines;
    int curr_line;
} Interface;

extern Interface *sched_win;

void Interface__init();
Interface *Interface__create_window(int, int, int, int, char *, int);
void Interface__delete_window(Interface *window);
void Interface__add_line(Interface *, int, const char *);
void Interface__input_menu(char *);
void Interface__send_data(Interface *, const char *, ...);
void Interface__update();
void Interface__refresh_kernel_win();
void Interface__refresh_process_win();
char *Interface__cast_process_state(int);
void Interface__refresh_memory_win();

#endif //SISTOP_INTERFACE_H
