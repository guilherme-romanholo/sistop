#ifndef SISTOP_DISK_H
#define SISTOP_DISK_H

#include "../utils/list.h"
#include "../process/process.h"

typedef struct disk_request {
    Process *process;
    int track;
} DiskRequest;

typedef struct disk {
    DiskRequest *current;
    List *uplist; // (DiskRequest *)
    List *downlist; // (DiskRequest *)
} Disk;

Disk *Disk__create();
void Disk__request();
void Disk__finish_request();
void Disk__elevator();
void Print__request(Process *);
void Print__finish_request(Process *);

#endif //SISTOP_DISK_H
