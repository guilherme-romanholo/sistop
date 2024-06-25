#include "disk.h"
#include "../utils/list.h"
#include "../kernel/kernel.h"
#include "../scheduler/scheduler.h"
#include <stdlib.h>
#include <unistd.h>

Disk *Disk__create() {
    Disk *disk = malloc(sizeof(Disk));

    disk->current = NULL;
    disk->uplist = List__create();
    disk->downlist = List__create();

    return disk;
}

void Disk__request(DiskRequest *disk_request) {
    // TODO: sem_wait (com elevador) para poder acessar
    if (kernel->disk->current == NULL) {
        // Caso não tenha nenhuma E/S faz do novo
        kernel->disk->current = disk_request;
        //Por padrão insere na lista de up
        List__ordered_insert(kernel->disk->uplist, disk_request, Utils__compare_tracks_up);

    } else if (kernel->disk->current->track < disk_request->track) {
        // Caso já tenha, tenta inserir na lista de subida
        List__ordered_insert(kernel->disk->uplist, disk_request, Utils__compare_tracks_up);

    } else {
        
        // Caso já tenha, tenta inserir na lista de descida
        List__ordered_insert(kernel->disk->downlist, disk_request, Utils__compare_tracks_down);
    }
}

void Disk__elevator() {
    while (!kernel);
    while(!kernel->disk);

    sleep(10);
    while (1){

        while (kernel->disk->uplist->size != 0){
            //Basicamente ele pega a cabeça da lista de up e `faz` a entrada e saída, depois ele pula pro próximo da lista, que seria a nova cabeça
            kernel->disk->current = (DiskRequest *) List__remove_head(kernel->disk->uplist);

            printf("Fazendo IO do processo %d na trilha %d\n", kernel->disk->current->process->pid, kernel->disk->current->track);
            sleep(3);

            //Depois de fazer a E/S, desbloqueia o processo e faz tudo de novo até a lista ficar vazia
            Scheduler__unblock_process(kernel->scheduler, kernel->disk->current->process);

        }

        //Quando a lista de up termina, vai pra head da lista de down e faz o mesmo processo até a lista ficar vazia, assim ele fica subindo e descendo
        kernel->disk->current = kernel->disk->downlist->head; // Interliga o final da Uplist com o começo da Downlist

        while (kernel->disk->downlist->size != 0){
            kernel->disk->current = (DiskRequest *) List__remove_head(kernel->disk->downlist->head);
            
            printf("Fazendo IO do processo %d na trilha %d\n", kernel->disk->current->process->pid, kernel->disk->current->track);
            sleep(3);

            Scheduler__unblock_process(kernel->scheduler, kernel->disk->current->process);
        }
        //Quando termina a lista de down, volta pra head da lista de up
        kernel->disk->current = kernel->disk->uplist->head;
    }
}