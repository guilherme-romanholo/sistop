#ifndef SISTOP_KERNEL_H
#define SISTOP_KERNEL_H

#include "../memory/memory.h"

typedef enum {
    CREATE_PROCESS = 1,
    FINISH_PROCESS = 2,
    INTERRUPT_PROCESS = 3,
    REQ_LOAD_MEMORY = 4,
} Syscall;

typedef enum {
    FIN_LOAD_MEMORY = 10
} Interruption;

typedef struct {
    int proc_id_counter;
    List *proc_table;
    SegmentTable *seg_table;
} Kernel;

extern Kernel *kernel;

void Kernel__create();
void Kernel__syscall(Syscall, void *);
void Kernel__interrupt(Interruption , void *);

#endif //SISTOP_KERNEL_H
