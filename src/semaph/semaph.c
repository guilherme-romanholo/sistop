#include "semaph.h"
#include "../kernel/kernel.h"
#include <stdlib.h>

/// Initialize semaphores
/// \param name Semaphore name
/// \param val Semaphore value
/// \return Return created semaphore
Semaphore *Semaph__semaphore_init(char name, const int val) {
    Semaphore *sem = malloc(sizeof(Semaphore));
    
    sem->name = name;
    sem->val = val;
    sem->waiters = List__create();
    sem_init(&sem->mutex, 0, 1);

    return sem;
}

/// Read semaphores from input buffer
/// \param buffer Input buffer
void Semaph__read_semaphores(char *buffer) {
    Semaphore *sem;
    int num_sem;

    num_sem = (int) strlen(buffer) / 2;

    for (int i = 0; i <= num_sem; i += 2) {
        if(Semaph__semaphore_search(buffer[i]) == NULL){
            sem = Semaph__semaphore_init(buffer[i], 1);
            List__append(kernel->sem_table, (void*)sem);
        }
    }
}

/// Search semaphore in kernel semaphores table
/// \param name Semaphore name
/// \return NULL or Semaphore
Semaphore *Semaph__semaphore_search(char name) {
    Node *aux = kernel->sem_table->head;
    Semaphore *sem;

    while(aux != NULL){
        sem = (Semaphore *)aux->content;

        if(sem->name == name){
            return sem;
        }

        aux = aux->next;
    }

    return NULL;
}

/// Semaphore wait
/// \param sem Required semaphore
/// \param proc Process that require semaphore
void Semaph__semaphore_P(Semaphore *sem, Process *proc) {
    int blocked = 0;

    sem_wait(&sem->mutex);
    sem->val--;

    if (sem->val < 0) {
        List__append(sem->waiters, (void *) proc);
        // TODO: Sleep function?
        blocked = 1;
    }

    sem_post(&sem->mutex);
}

/// Semaphore wait
/// \param sem Required semaphore
/// \param proc Process that require semaphore
void Semaph__semaphore_V(Semaphore *sem, Process *process) {
    sem_wait(&sem->mutex);
    sem->val++;

    if (sem->val <= 0) {
        Process *proc = (Process *) List__remove_head(sem->waiters);

        //TODO: Chama a função para tirar o processo do estado de bloqueado
    }

    sem_post(&sem->mutex);
}