#ifndef SISTOP_SCHEDULER_H
#define SISTOP_SCHEDULER_H

#include "../process/process.h"
#include "../utils/list.h"

typedef struct {
    Process *scheduled_proc;
    List* blocked_queue;
    List* scheduler_queue;
    int quantum;
} Scheduler;

typedef enum {
    SCHEDULE_PROCESS = 0,
    PROCESS_END = 1,
    IO_REQUESTED = 2,
    QUANTUM_END = 3,
    SEMAPH_BLOCKED = 5
} Sched_Flags;

Scheduler *Scheduler__create();

void Scheduler__schedule_process(Process *process, Scheduler *scheduler, Sched_Flags flag);

void Scheduler__unblock_process(Scheduler *scheduler, Process *process);

void Scheduler__cpu_run();

Sched_Flags exec_process(Segment *segment, Process *process, int quantum, int);

#endif //SISTOP_SCHEDULER_H
