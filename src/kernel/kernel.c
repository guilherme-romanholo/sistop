#include "kernel.h"
#include <stdio.h>
#include <stdlib.h>

Kernel *kernel;

void Kernel__create() {
    kernel = malloc(sizeof(Kernel));

    if (kernel == NULL) {
        printf("Not memory to allocate kernel!");
        exit(1);
    }

    kernel->proc_id_counter = 1;
    kernel->proc_table = List__create();
    kernel->seg_table = Memory__create_segment_table();
}

void Kernel__syscall(Syscall call, void *arg) {
    switch (call) {
        case CREATE_PROCESS:
            Process__create((const char *)arg);
            break;
        case FINISH_PROCESS:
            break;
        case INTERRUPT_PROCESS:
            break;
        case REQ_LOAD_MEMORY:
            Memory__req_load_memory((Process *)arg, kernel->seg_table);
            Kernel__interrupt(FIN_LOAD_MEMORY, (Process *)arg);
            break;
    }
}

void Kernel__interrupt(Interruption interruption, void *arg) {
    switch (interruption) {
        case FIN_LOAD_MEMORY:
            List__append(kernel->proc_table, (Process *)arg);
            // TODO: Add process to Scheduler
            // Process *process = ((Process *)arg);
            // process->state = READY;
    }
}