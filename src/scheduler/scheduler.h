#ifndef SISTOP_SCHEDULER_H
#define SISTOP_SCHEDULER_H

#include "../process/process.h"
#include "../utils/list.h"

typedef struct {
    Process *scheduled_proc;
    List* blocked_queue;
    int quantum;
} Scheduler;

void Scheduler__create(Scheduler *);

#endif //SISTOP_SCHEDULER_H
