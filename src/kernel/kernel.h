//
// Created by guilherme on 05/05/24.
//

#ifndef SISTOP_KERNEL_H
#define SISTOP_KERNEL_H

typedef enum {
    CREATE_PROCESS = 1,
    FINISH_PROCESS = 2,
    INTERRUPT_PROCESS = 3,
    REQ_LOAD_MEMORY = 4,
    FIN_LOAD_MEMORY = 5
} syscall_t;

typedef struct {
    int proc_id_counter;
} kernel_t;

extern kernel_t *kernel;

// Kernel constructor
void kernel__create();
// Kernel syscalls
void kernel__syscall(syscall_t, void *);

#endif //SISTOP_KERNEL_H
