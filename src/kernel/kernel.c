#include "kernel.h"
#include "../utils/list.h"
#include <stdio.h>
#include <stdlib.h>

/// Declare Kernel
Kernel *kernel;

/// Create the Kernel
void Kernel__create() {
    kernel = malloc(sizeof(Kernel));

    if (kernel == NULL) {
        printf("Not memory to allocate kernel!");
        exit(1);
    }

    kernel->proc_id_counter = 1;
    kernel->sem_table = List__create();
    kernel->proc_table = List__create();
    kernel->seg_table = Memory__create_segment_table();
}

/// Make a syscall
/// \param call Syscall name
/// \param arg Syscall argument
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
            Memory__req_load_memory((List *)arg, kernel->seg_table);
            Kernel__interrupt(FIN_LOAD_MEMORY, (List *)arg);
            break;
    }
}

/// Make a interruption
/// \param interruption Interruption name
/// \param arg Interruption argument
void Kernel__interrupt(Interruption interruption, void *arg) {
    switch (interruption) {
        case FIN_LOAD_MEMORY:
            printf("");

            List *memory_request = (List *) arg;
            Process *process = (Process *) memory_request->head->content;
            // Append new process in the process table
            List__append(kernel->proc_table, process);

            // TODO: Add process to Scheduler
            // Process *process = ((Process *)arg);
            // process->state = READY;
            break;
    }
}