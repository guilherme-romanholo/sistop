#include "interface.h"
#include "../process/process.h"
#include "../kernel/kernel.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

Interface *menu_interface;
Interface *kernel_interface;
Interface *memory_interface;
Interface *process_interface;
Interface *scheduler_interface;

/// Initialize project interface
void Interface__init() {
    int height, width;
    char input[INPUT_SIZE];

    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();

    getmaxyx(stdscr, height, width);

    // Create menu interface
    menu_interface = Interface__create_window(
            height / 4,
            width / 2,
            0, 0,
            "MENU", FALSE);

    // Create kernel interface
    kernel_interface = Interface__create_window(
            height / 4,
            width / 2,
            height / 4, 0,
            "KERNEL", FALSE);

    usleep(SLEEP_TIME);

    // Create memory interface
    memory_interface = Interface__create_window(
            (height / 2) + 1,
            width / 2,
            (height / 2) - 1, 0,
            "MEMORY",TRUE);

    // Create process interface
    process_interface = Interface__create_window(
            (height / 2) - 1,
            width / 2,
            0, (width / 2),
            "PROCESS",TRUE);

    usleep(SLEEP_TIME);

    // Create scheduler interface
    scheduler_interface = Interface__create_window(
            (height / 2) + 1,
            width / 2,
            (height / 2) - 1, (width / 2),
            "SCHEDULER",TRUE);

    // Init interfaces semaphores
    sem_init(&kernel_interface->mutex, 0, 1);
    sem_init(&memory_interface->mutex, 0, 0);
    sem_init(&process_interface->mutex, 0, 0);

    // Init interfaces threads
    pthread_create(&kernel_interface->thread,
                   NULL,
                   (void *) Interface__update_kernel_window,
                   NULL);

    pthread_create(&memory_interface->thread,
                   NULL,
                   (void *) Interface__update_memory_window,
                   NULL);

    pthread_create(&process_interface->thread,
                   NULL,
                   (void *) Interface__update_process_window,
                   NULL);

    while (TRUE) {
        char path[20] = "../synt/";

        Interface__input_menu(input);

        if (!strcmp(input, "q"))
            break;

        strcat(path, input);
        Kernel__syscall(CREATE_PROCESS, (void *) path);
    }

    // Free semaphores
    sem_destroy(&kernel_interface->mutex);
    sem_destroy(&memory_interface->mutex);
    sem_destroy(&process_interface->mutex);

    // Free interface
    Interface__destroy(menu_interface);
    Interface__destroy(kernel_interface);
    Interface__destroy(memory_interface);
    Interface__destroy(process_interface);
    Interface__destroy(scheduler_interface);

    endwin();
}

/*
 * ###################
 * #     GENERAL     #
 * ###################
 * */

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

/// Free an interface component
/// \param interface Interface component
void Interface__destroy(Interface *interface) {
    delwin(interface->subwindow);
    delwin(interface->window);
    free(interface);
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

/*
 * ##################
 * #      MENU      #
 * ##################
 * */

/// Input process name in menu
/// \param input Input value
void Interface__input_menu(char *input) {
    int width;

    width = getmaxx(menu_interface->subwindow);

    Interface__add_line(menu_interface, (width / 2) - 18, " ___  ____  ___  ____  _____  ____ ");
    Interface__add_line(menu_interface, (width / 2) - 18, "/ __)(_  _)/ __)(_  _)(  _  )(  _ \\");
    Interface__add_line(menu_interface, (width / 2) - 18, "\\__ \\ _)(_ \\__ \\  )(   )(_)(  )___/");
    Interface__add_line(menu_interface, (width / 2) - 18, "(___/(____)(___/ (__) (_____)(__)  ");

    menu_interface->curr_line++;

    Interface__add_line(menu_interface, (width / 2) - 9, "Press 'q' to exit");
    Interface__add_line(menu_interface, (width / 2) - 9, "Process name: ");

    wclrtoeol(menu_interface->subwindow);
    wrefresh(menu_interface->subwindow);

    menu_interface->curr_line = 0;

    echo();
    wgetnstr(menu_interface->subwindow, input, INPUT_SIZE);
    noecho();
}

/*
 * ##################
 * #     KERNEL     #
 * ##################
 * */

/// Update the kernel window
void Interface__update_kernel_window() {
    while (TRUE) {
        sem_wait(&kernel_interface->mutex);

        usleep(SLEEP_TIME);
        mvwprintw(kernel_interface->subwindow, 1, 1, "Remaining Memory: %ld bytes.", kernel->remaining_memory);

        usleep(SLEEP_TIME);
        Interface__kernel_semaphores();

        wrefresh(kernel_interface->subwindow);
    }
}

/// Return formated semaphores to print in interface
void Interface__kernel_semaphores() {
    char semaphores[BUFFER_SIZE] = "";
    char sem[6] = "x(y) ";

    for (Node *aux = kernel->semaph_table->head; aux != NULL ; aux = aux->next) {
        sem[0] = ((Semaphore *) aux->content)->name;
        sem[2] = ((Semaphore *) aux->content)->val + '0';
        strcat(semaphores, sem);
    }

    mvwprintw(kernel_interface->subwindow, 2, 1, "Semaphores: %s", semaphores);
}

/*
 * ##################
 * #     MEMORY     #
 * ##################
 * */

/// Update memory window
void Interface__update_memory_window() {
    while (TRUE) {
        sem_wait(&memory_interface->mutex);

        Segment *segment;
        char output[BUFFER_SIZE];

        usleep(SLEEP_TIME);

        werase(memory_interface->subwindow);

        usleep(SLEEP_TIME);

        for (Node *aux = kernel->segment_table->head; aux != NULL ; aux = aux->next) {
            segment = (Segment *) aux->content;

            sprintf(output, "Seg Id: %d, Seg Size: %d Kb, Num Pages: %d.", segment->seg_id,
                    segment->seg_size / KBYTE, segment->pages->size);

            usleep(SLEEP_TIME);

            Interface__add_line(memory_interface, 0, output);
        }

        memory_interface->curr_line = 0;
    }
}

/*
 * ###################
 * #     PROCESS     #
 * ###################
 * */

/// Update process window
void Interface__update_process_window() {
    Process *process;
    char output[BUFFER_SIZE];

    while (TRUE) {
        sem_wait(&process_interface->mutex);

        werase(process_interface->subwindow);

        usleep(SLEEP_TIME);

        for (Node *aux = kernel->pcb->head; aux != NULL ; aux = aux->next) {
            process = (Process *) aux->content;

            sprintf(output, "PID: %d, Name: %s, State: %s, Seg Id: %d, Priority: %d, PC: %d.",
                    process->pid, process->name, Interface__cast_process_state(process->state),
                    process->segment_id, process->priority, process->pc);

            usleep(SLEEP_TIME);

            Interface__add_line(process_interface, 0, output);
        }

        process_interface->curr_line = 0;
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

/*
 * ###################
 * #    Scheduler    #
 * ###################
 * */

void Interface__add_scheduler_log(int pid, Instruction *instr, int quantum) {
    char output[BUFFER_SIZE];

    usleep(SLEEP_TIME);

    sprintf(output, "Process %d %s: Remain %d u.t.",
            pid, Interface__cast_instruction_opcode(instr), quantum);

    Interface__add_line(scheduler_interface, 1, output);
}

char *Interface__cast_instruction_opcode(Instruction *instr) {
    char *output = malloc(sizeof(char) * BUFFER_SIZE);

    switch (instr->opcode) {
        case EXEC:
            sprintf(output, "Executing");
            break;
        case READ:
            sprintf(output, "Reading");
            break;
        case WRITE:
            sprintf(output, "Writing");
            break;
        case PRINT:
            sprintf(output, "Printing");
            break;
        case SEM_P:
            sprintf(output, "Require Sem %c", instr->sem);
            break;
        case SEM_V:
            sprintf(output, "Release Sem %c", instr->sem);
            break;
    }

    return output;
}
