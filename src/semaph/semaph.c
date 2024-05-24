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
        //Verifica se o semáforo já está na lista antes de cadastrá-lo
        if(Semaph__semaphore_search(buffer[i]) == NULL){
            sem = Semaph__semaphore_init(buffer[i], 1);
            List__append(kernel->sem_table, (void*)sem);
        }
        // TODO: Se existir incrementar o val dele?
    }
}

Semaphore *Semaph__semaphore_search(char name) {
    Node *aux = kernel->sem_table->head;
    Semaphore *sem;

    //Percorre a lista de semáforos verificando se o semáforo já está na lista
    while(aux != NULL){
        sem = (Semaphore *)aux->content;

        if(sem->name == name){
            // printf("Semáforo %c já foi cadastrado!\n",sem->name);
            return sem;
        }

        aux = aux->next;
    }

    // printf("Semáforo %c não foi encontrado!\n", name);
    return NULL;
}

void Semaph__semaphore_P(Semaphore *sem, Process *proc) {
    int blocked = 0;

    sem_wait(&sem->mutex);
    sem->val--;

    if (sem->val < 0) {
        List__append(sem->waiters, (void *) proc);
        //Chama a função para bloquear o processo que está rodando e pediu o semáforo
        blocked = 1;
    }

    sem_post(&sem->mutex);
}

void Semaph__semaphore_V(Semaphore *sem, Process *process) {
    sem_wait(&sem->mutex);
    sem->val++;

    if (sem->val <= 0) {
        //Process *proc = list_remove_head(sem->waiters)->content;

        //Chama a função para tirar o processo do estado de bloqueado
    }

    sem_post(&sem->mutex);
}
