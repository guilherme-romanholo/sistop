//
// Created by guilherme on 05/05/24.
//

#include "kernel.h"
#include <stdio.h>
#include <stdlib.h>

kernel_t *kernel;

void kernel__create() {
    // Allocate kernel
    kernel = malloc(sizeof(kernel_t));
    if (kernel == NULL) {
        printf("Not memory to allocate kernel!");
        exit(1);
    }
    // Initialize pid
    kernel->proc_id_counter = 1;
}

void kernel__syscall(syscall_t call, void *arg) {
    switch (call) {
        case CREATE_PROCESS:
            break;
    }
}