#ifndef SISTOP_SCHEDULER_H
#define SISTOP_SCHEDULER_H

#include "../process/process.h"
#include "../utils/list.h"

typedef struct {
    Process *sched_proc;
    List* blocked_queue;
    List* sched_queue;
    int quantum;
    int new_process;
} Scheduler;

typedef enum {
    SCHEDULE_PROCESS = 10,
    PROCESS_END = 11,
    IO_REQUESTED_DISK = 12,
    IO_REQUESTED_PRINT = 13,
    QUANTUM_END = 14,
    SEMAPH_BLOCKED = 15
} SchedFlag;

Scheduler *Scheduler__create();
void Scheduler__schedule_process(Process *process, Scheduler *scheduler, SchedFlag flag, int IO_REQUEST_Track);
void Scheduler__unblock_process(Scheduler *scheduler, Process *process);
void Scheduler__cpu_run();
int Scheduler__exec_process(Segment *, Process *, int, int, int, int *);
void Scheduler__interrupt_process();

#endif //SISTOP_SCHEDULER_H
