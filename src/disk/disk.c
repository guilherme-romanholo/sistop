#include "disk.h"
#include "../utils/list.h"
#include "../kernel/kernel.h"
#include "../scheduler/scheduler.h"
#include <stdlib.h>
#include <unistd.h>

#include "../interface/interface.h"

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

void Disk__finish_request() {
    Scheduler__unblock_process(
        kernel->scheduler,
        kernel->disk->current->process);

    Interface__send_data(io_win,
        DISK_FIN_LOG_FMT,
        kernel->disk->current->process->pid,
        kernel->disk->current->track);

    kernel->disk->current = NULL;
}

void Print__request(Process *process) {
    // Block process with print request
    process->state = WAITING;
    List__append(kernel->scheduler->blocked_queue, (void *) process);
    kernel->scheduler->sched_proc = NULL;

    Interface__send_data(io_win,
        PRINT_REQ_LOG_FMT,
        process->pid);

    Kernel__interrupt(PRINT_FINISH, (void *) process);
}

void Print__finish_request(Process *process) {
    Scheduler__unblock_process(
        kernel->scheduler,
        process);

    Interface__send_data(io_win,
        PRINT_FIN_LOG_FMT,
        process->pid);
}

void Disk__elevator() {
    usleep(1000);

    while(!kernel);
    while(!kernel->disk);

    usleep(1000);

    while (1) {

        // Elevator goes up in the disk
        while (kernel->disk->uplist->size) {
            // Remove uplist head
            kernel->disk->current = (DiskRequest *) List__remove_head(kernel->disk->uplist);

            // Send log to the interface
            Interface__send_data(io_win,
                DISK_REQ_LOG_FMT,
                kernel->disk->current->process->pid,
                kernel->disk->current->track);

            // Sleep 1 second
            sleep(1);

            // Unblock process when i/o is finished
            Kernel__interrupt(DISK_FINISH, kernel->disk->current->process);
        }

        // Elevator goes down in the disk
        while (kernel->disk->downlist->size) {
            // Remove downlist head
            kernel->disk->current = (DiskRequest *) List__remove_head(kernel->disk->downlist);

            // Send log to the interface
            Interface__send_data(io_win,
                DISK_REQ_LOG_FMT,
                kernel->disk->current->process->pid,
                kernel->disk->current->track);

            // Sleep 1 second
            sleep(1);

            // Unblock process when i/o is finished
            Kernel__interrupt(DISK_FINISH, kernel->disk->current->process);
        }
    }
}