#include "kernel.h"
#include "../interface/interface.h"
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

    kernel->remaining_memory = MAX_MEMORY;
    kernel->proc_id_counter = 1;
    kernel->segment_table = List__create();
    kernel->semaph_table = List__create();
    kernel->pcb = List__create();
    kernel->scheduler = Scheduler__create();
    kernel->disk = Disk__create();
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
            Process__finish((Process *)arg);
            Interface__refresh_process_win();
            break;
        case REQ_LOAD_MEMORY:
            Memory__req_load_memory((List *)arg);
            Kernel__interrupt(FIN_LOAD_MEMORY, (List *)arg);
            break;
        case DISK_REQUEST:
            Disk__request((DiskRequest *) arg);
            break;
        case PRINT_REQUEST:
            Print__request((Process *) arg);
            break;
    }
}

/// Make a interruption
/// \param interruption Interruption name
/// \param arg Interruption argument
void Kernel__interrupt(Interruption interruption, void *arg) {
    switch (interruption) {
        case FIN_LOAD_MEMORY:
            Memory__fin_load_memory((List *) arg);
            break;
        case INTERRUPT_PROCESS:
            Scheduler__interrupt_process();
            break;
        case DISK_FINISH:
            Disk__finish_request();
            break;
        case PRINT_FINISH:
            Print__finish_request((Process *) arg);
            break;
    }
}