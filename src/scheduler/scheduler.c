#include <stdio.h>
#include "scheduler.h"

/// Create scheduler
/// \param scheduler Scheduler struct
void Scheduler__create(Scheduler *scheduler) {
    scheduler->scheduled_proc = NULL;
    scheduler->blocked_queue = List__create();
    scheduler->quantum = 5000;
}

/*
int exec_process(Process *process){
    int quantum = 5000/process->priority;
    int process_end = 0, pc = process->pc;
    while(quantum > 0 && !(process_end)){
        if(strcmp(process->code[pc].op,"exec") == 0){
            printf("Processo %d executando: %d u.t.; restam %d u.t.\n", process->pid,process->code[pc].value, quantum);
            sleep(3);
        }else if(strcmp(process->code[pc].op,"read") == 0){
            printf("Processo %d lendo: %d u.t.; restam %d u.t.\n", process->pid,process->code[pc].value, quantum);
            sleep(3);
        }else if(strcmp(process->code[pc].op,"write") == 0){
            printf("Processo %d escrevendo: %d u.t.; restam %d u.t.\n", process->pid,process->code[pc].value, quantum);
            sleep(3);
        }else if(strcmp(process->code[pc].op,"print") == 0){
            printf("Processo %d printando: %d u.t.; restam %d u.t.\n", process->pid,process->code[pc].value, quantum);
            sleep(3);
        }

        if(quantum < process->code[pc].value){
            process->code[pc].value -= quantum;
            quantum = 0;
        }else{
            quantum = quantum - process->code[pc].value;
            pc++;
        }

        if(pc > 11)
            process_end = 1;
    }
    process->pc = pc;
    return process_end;
}*/
