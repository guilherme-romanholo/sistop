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
void Disk__elevator();

#endif //SISTOP_DISK_H
