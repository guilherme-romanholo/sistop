#include "disk.h"
#include "../utils/list.h"
#include "../kernel/kernel.h"
#include <stdlib.h>

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

    while (1){

        while (kernel->disk->uplist->size != 0){
            List__remove_head(kernel->disk->uplist->head);
            //kernel->disk->uplist->head = kernel->disk->uplist->head->next;
        }
        kernel->disk->current = kernel->disk->downlist->head; // Interliga o final da Uplist com o começo da Downlist

        while (kernel->disk->downlist->size != 0){
            List__remove_head(kernel->disk->downlist->head);
            //kernel->disk->downlist->head = kernel->disk->downlist->head->next;
        }
        kernel->disk->current = kernel->disk->uplist->head;
    }
}