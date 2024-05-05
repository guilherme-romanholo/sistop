//
// Created by guilherme on 05/05/24.
//

#ifndef SISTOP_PROCESS_H
#define SISTOP_PROCESS_H

// Status
typedef enum {
    NEW = 0,
    READY = 1,
    RUNNING = 2,
    WAITING = 3,
    TERMINATED = 4
} pstatus_t;

// Process PCB
typedef struct {
    int pid;
    int state;
    char *name;
    int segment_id;
    int priority;
    int segment_size;
} process_t;

void process__create(const char *);

#endif //SISTOP_PROCESS_H
