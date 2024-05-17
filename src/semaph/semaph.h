#ifndef SISTOP_SEMAPH_H
#define SISTOP_SEMAPH_H

#include "../utils/list.h"
#include <semaphore.h>
#include <string.h>

typedef struct {
    char name;
    int val;
    List *waiters;
    sem_t  mutex;
} Semaphore;

Semaphore *Semaph__semaphore_init(char, const int);
void Semaph__read_semaphores(char *);

#endif //SISTOP_SEMAPH_H