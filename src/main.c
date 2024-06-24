#include "kernel/kernel.h"
#include "interface/interface.h"
#include "scheduler/scheduler.h"
#include <pthread.h>

int main(int argc, char *argv[])
{
    pthread_t sched;
    pthread_t disk;

    pthread_create(&sched,
                   NULL,
                   (void *) Scheduler__cpu_run,
                   NULL);

    pthread_create(&disk,
                   NULL,
                   (void *) Disk__elevator,
                   NULL);

    Kernel__create();
    Interface__init();

    return 0;
}
