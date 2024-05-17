//
// Created by guilherme on 12/05/24.
//

#include "terminal.h"
#include "../process/process.h"
#include "../utils/list.h"
#include "../memory/memory.h"
#include "../kernel/kernel.h"
#include <stdio.h>
#include <string.h>

/// Init the terminal interactive screen
void Terminal__init() {
    int op, c;

    while (1) {
        printf("========== Sistop ==========\n");
        printf("| (1) Create Process       |\n");
        printf("| (2) List SegTable        |\n");
        printf("| (3) List ProcTable       |\n");
        printf("| (4) List SemTable        |\n");
        printf("| (0) Exit                 |\n");
        printf("============================\n\n");
        printf("Option: ");

        scanf("%d", &op);

        switch (op) {
            case 1:
                Terminal__input_process();
                break;
            case 2:
                Terminal__list_seg_table();
                break;
            case 3:
                Terminal__list_proc_table();
                break;
            case 4:
                Terminal__list_sem_table();
                break;
            case 0:
                return;
            default:
                break;
        }

        printf("\nDigite um NUMERO qualquer para continuar...\n");
        scanf("%d", &c);
    }
}

/// Input process from the file
void Terminal__input_process() {
    FILE *fp;
    char path[30] = "../synt/", process_file[20];

    printf("\nEnter process filename: ");
    scanf(" %s", process_file);

    strcat(path, process_file);

    if ((fp = fopen(path, "r")) == NULL) {
        printf("Process file does not exist.\n\n");
        return;
    }

    Process__create(path);

    printf("\nCreate process with success.\n");
}

/// List all segments in the kernel segment table
void Terminal__list_seg_table() {
    printf("\n");

    Node *aux = kernel->seg_table->seg_list->head;

    printf("========== Segment Table ==========\n\n");

    while (aux != NULL) {
        Segment *segment = (Segment *)aux->content;

        printf("Segment Id: %d\n", segment->seg_id);
        printf("Segment Size: %d\n\n", segment->seg_size);

        aux = aux->next;
    }

    printf("===================================\n");
}

void Terminal__list_sem_table() {
    printf("\n");

    Node *aux = kernel->sem_table->head;

    printf("========== Semaphore Table ==========\n\n");

    while (aux != NULL) {
        Semaphore *sem = (Semaphore *)aux->content;

        printf("Semaphore name: %c\n", sem->name);
        printf("Semaphore value: %d\n\n", sem->val);

        aux = aux->next;
    }

    printf("===================================\n");
}

/// List all process in the kernel process table
void Terminal__list_proc_table() {
    printf("\n");

    Node *aux = kernel->proc_table->head;

    printf("========== Process Table ==========\n\n");

    while (aux != NULL) {
        Process *process = (Process *)aux->content;

        printf("Process Name: %s", process->name);
        printf("Process Id: %d\n", process->pid);
        printf("Process State: %s\n", Terminal__cast_proc_state(process->state));
        printf("Process Priority: %d\n", process->priority);
        printf("Process Segment: %d\n\n", process->segment_id);

        aux = aux->next;
    }

    printf("===================================\n");
}

/// Cast status number to status name
char *Terminal__cast_proc_state(ProcessStatus status) {
    switch (status) {
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
    }
}



