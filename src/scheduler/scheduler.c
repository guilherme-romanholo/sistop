#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../kernel/kernel.h"
#include "../process/process.h"
#include "../semaph/semaph.h"
#include "scheduler.h"

Scheduler* Scheduler__create() {
    Scheduler* scheduler = malloc(sizeof(Scheduler));

    scheduler->scheduled_proc = NULL;
    scheduler->scheduler_queue = List__create();
    scheduler->scheduler_queue = List__create();
    scheduler->blocked_queue = List__create();

    return scheduler;
}

void Scheduler__schedule_process(Process *process, Scheduler *scheduler, Sched_Flags flag){
    //Basicamente, temos uma lista de processos prontos para escalonar, então pegamos
    // a cabeça da lista para executar, depois de executar (ou interromper por algum motivo)
    // Re-insere ele na lista de prontos e executa a cabeça de novo

    if(flag == SCHEDULE_PROCESS){

        scheduler->quantum = 5000/process->priority;
        scheduler->scheduled_proc = (Process *)List__remove_head(scheduler->scheduler_queue);
        process->state = RUNNING;

    }else if(flag == PROCESS_END){

        scheduler->scheduled_proc = NULL;
        process->state = TERMINATED;


    }else if(flag == QUANTUM_END && flag == IO_REQUESTED){

        process->state = READY;
        List__append(scheduler->scheduler_queue, (void *)process);
        scheduler->scheduler_queue = NULL;

    }else if(flag == SEMAPH_BLOCKED){

        process->state = WAITING;
        List__append(scheduler->blocked_queue,(void *)process);
        scheduler->scheduler_queue = NULL;

    }

}

void Scheduler__unblock_process(Scheduler *scheduler, Process *process){
    Node *aux = scheduler->blocked_queue->head;
    Process *proc_aux;

    //Search and remove the process from the blocked queue
    while(aux){
        proc_aux = (Process *)aux->content;

        if(proc_aux->pid == process->pid)
            List__remove_node(scheduler->blocked_queue,aux);

        aux = aux->next;
    }

    //Append the process in the scheduler queue again
    List__append(scheduler->scheduler_queue,(void *)process);

}

void Scheduler__cpu_run(){
    Segment *segment;
    Page *page;
    Sched_Flags flag;

    while(!kernel);
    

    while(1){
        

        if(kernel->scheduler->scheduled_proc == NULL){
            if(kernel->pcb->size != 0){
                Process* process = (Process *)List__remove_head(kernel->scheduler->scheduler_queue);
                Scheduler__schedule_process(process,kernel->scheduler, SCHEDULE_PROCESS);
            }
        }else{
            segment = Memory__fetch_segment(kernel->scheduler->scheduled_proc->segment_id);
            page = (Page *)segment->pages->head;
            
            flag = exec_process(segment, kernel->scheduler->scheduled_proc, kernel->scheduler->quantum, page->num_instructions_page);

            Scheduler__schedule_process(kernel->scheduler->scheduled_proc, kernel->scheduler, flag);

        }


    }
}

Sched_Flags exec_process(Segment *segment, Process *process, int quantum, int num_instructions_page){
    int blocked = 0;
    int page_id = ceil((double)process->pc/num_instructions_page);

    Sched_Flags flag;

    Node *aux_page = Memory__fetch_page(segment,page_id); 
    Page *curr_page = (Page *)aux_page->content;

    Node *aux_instr = curr_page->instructions->head;
    Instruction *instr;

    //Percorre a lista de páginas que estão na memória
    while(aux_page && quantum > 0){
        curr_page = (Page *)aux_page->content;
        
        //Para cada página, percorre a lista de instruções e vai "executando"
        while(aux_instr && quantum > 0){
            instr = (Instruction *)aux_instr->content;

            if(instr->opcode == EXEC){
                quantum -= instr->value;
                process->pc++;
            }else if(instr->opcode == SEM_P){
                Semaphore *sem = Semaph__semaphore_search(instr->sem);
                blocked = Semaph__semaphore_P(sem, process);

                if(blocked){
                    quantum = 0;
                    flag = SEMAPH_BLOCKED;
                }
                else{
                    quantum -= 200;
                    process->pc++;
                }
                //interrompe o processo
            }else if(instr->opcode == SEM_V){
                Semaphore *sem = Semaph__semaphore_search(instr->sem);
                Semaph__semaphore_V(sem);
                quantum -= 200;
                process->pc++;
            }else{
                quantum = 0;
                process->pc++;
                flag = IO_REQUESTED;
                //interrompe o processo
            }

            aux_instr = aux_instr->next;

            if(quantum < 0){
                instr->value = -1 * quantum; //Recebe o tanto que falta para completar
                flag = QUANTUM_END;
            }
        }

        aux_page = aux_page->next;
    }

    if(aux_page == NULL)
        flag = PROCESS_END;

    return flag;
}