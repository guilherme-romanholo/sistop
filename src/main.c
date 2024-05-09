#include "kernel/kernel.h"
#include "process/process.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    Kernel__create();
    Kernel__syscall(CREATE_PROCESS, "synt");

    Process *p1 = ((Process *)kernel->proc_table->head->content);
    printf("Pid: %d\nName: %s\n",p1->pid, p1->name);

    Segment *s1 = ((Segment *)kernel->seg_table->seg_list->head->content);
    printf("Seg id: %d\nSeg size: %d\n", s1->seg_id, s1->seg_size);
}
