#ifndef SISTOP_KERNEL_H
#define SISTOP_KERNEL_H

#include "../memory/memory.h"
#include "../semaph/semaph.h"
#include "../scheduler/scheduler.h"

/// Syscall enumeration
typedef enum {
    CREATE_PROCESS = 1,
    FINISH_PROCESS = 2,
    REQ_LOAD_MEMORY = 3,
} Syscall;

/// Interruption enumeration
typedef enum {
    FIN_LOAD_MEMORY = 4,
    INTERRUPT_PROCESS = 5,
} Interruption;

/// Kernel struct
typedef struct {
    int proc_id_counter;
    long remaining_memory;
    Scheduler *scheduler;
    List *pcb;
    List *semaph_table;
    List *segment_table;
} Kernel;

/// Export kernel to other files
extern Kernel *kernel;

void Kernel__create();
void Kernel__syscall(Syscall, void *);
void Kernel__interrupt(Interruption , void *);

#endif //SISTOP_KERNEL_H
