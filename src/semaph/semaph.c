#include "semaph.h"
#include "../kernel/kernel.h"
#include "../process/process.h"
#include <stdlib.h>

Semaphore *Semaph__semaphore_init(char name, const int val) {
    Semaphore *sem = malloc(sizeof(Semaphore));
    
    sem->name = name;
    sem->val = val;
    sem->waiters = List__create();
    sem_init(&sem->mutex, 0, 1);

    return sem;
}

void Semaph__read_semaphores(char *buffer) {
    Semaphore *sem;
    int num_sem;

    num_sem = strlen(buffer) / 2;

    for (int i = 0; i <= num_sem; i += 2) {
        sem = Semaph__semaphore_init(buffer[i], 1);
        List__append(kernel->sem_table, (void*)sem);
    }
}