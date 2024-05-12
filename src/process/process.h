//
// Created by guilherme on 05/05/24.
//

#ifndef SISTOP_PROCESS_H
#define SISTOP_PROCESS_H

/// Process status enumeration
typedef enum {
    NEW = 0,
    READY = 1,
    RUNNING = 2,
    WAITING = 3,
    TERMINATED = 4
} ProcessStatus;

/// Process struct
typedef struct {
    int pid;
    int state;
    char *name;
    int segment_id;
    int priority;
    int segment_size;
} Process;

void Process__create(const char *);

#endif //SISTOP_PROCESS_H
