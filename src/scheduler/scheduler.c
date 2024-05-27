#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../kernel/kernel.h"
#include "../interface/interface.h"
#include "scheduler.h"
#include <unistd.h>

/// Create scheduler
/// \return Created scheduler
Scheduler* Scheduler__create() {
    Scheduler* scheduler = malloc(sizeof(Scheduler));

    scheduler->sched_proc = NULL;
    scheduler->sched_queue = List__create();
    scheduler->blocked_queue = List__create();

    return scheduler;
}

/// Schedule process of queue head
/// \param process Queue head process
/// \param scheduler Scheduler
/// \param flag Process flag
void Scheduler__schedule_process(Process *process, Scheduler *scheduler, SchedFlag flag){
    if (flag == SCHEDULE_PROCESS) {
        process->state = RUNNING;
        scheduler->quantum = 5000 / process->priority;
        scheduler->sched_proc = process;
    } else if (flag == PROCESS_END) {
        process->state = TERMINATED;
        scheduler->sched_proc = NULL;
        kernel->pcb->size--;
    } else if (flag == QUANTUM_END || flag == IO_REQUESTED) {
        process->state = READY;
        List__append(scheduler->sched_queue, (void *) process);
        scheduler->sched_proc = NULL;
    } else if (flag == SEMAPH_BLOCKED) {
        process->state = WAITING;
        List__append(scheduler->blocked_queue,(void *) process);
        scheduler->sched_proc = NULL;
    }
}

/// Unlock waiting scheduler process
/// \param scheduler Kernel scheduler
/// \param process Process to be unlocked
void Scheduler__unblock_process(Scheduler *scheduler, Process *process){
    Process *proc_aux;
    //printf("Entrando na função semV\n\n");
    //Search and remove the process from the blocked queue
    for (Node *aux = scheduler->blocked_queue->head; aux != NULL; aux = aux->next) {
        proc_aux = (Process *) aux->content;

        if(proc_aux->pid == process->pid) {
            //printf("Removendo processo %d da lista de bloqueados\n\n", process->pid);
            List__remove_node(scheduler->blocked_queue, aux);
        }
    }

    //Append the process in the scheduler queue again
    List__append(scheduler->sched_queue, (void *) process);
}

/// Run the scheduler in CPU
void Scheduler__cpu_run(){
    Segment *segment;
    Page *page;
    SchedFlag flag;

    while (!kernel);

    while (1) {
        if (kernel->scheduler->sched_proc == NULL){

            if (kernel->pcb->size == 0) {
                //sleep(3);
            }else{
                Process* process = (Process *) List__remove_head(kernel->scheduler->sched_queue);
                Scheduler__schedule_process(process, kernel->scheduler, SCHEDULE_PROCESS);
            }

        } else {
            segment = Memory__fetch_segment(kernel->scheduler->sched_proc->segment_id);
            page = (Page *) segment->pages->head->content;
            
            flag = Scheduler__exec_process(segment, kernel->scheduler->sched_proc,
                                           kernel->scheduler->quantum, page->instr_per_page,
                                           page->total_instructions);

            Scheduler__schedule_process(kernel->scheduler->sched_proc, kernel->scheduler, flag);
        }
    }
}

int Scheduler__exec_process(Segment *seg, Process *proc, int quantum, int instr_per_page, int total_instr) {
    int blocked, page_id;
    SchedFlag flag;
    Node *p, *i;
    Page *page;
    Instruction *instruction;
    Semaphore *sem;

    page_id = floor((double) proc->pc / instr_per_page);

    for (p = Memory__fetch_page(seg, page_id); (p != NULL) && (quantum > 0); p = p->next) {
        page = (Page *) p->content;

        for (i = Memory__fetch_instruction(page, (proc->pc % instr_per_page)); (i != NULL) && (quantum > 0) ; i = i->next) {
            sleep(2);
            instruction = (Instruction *) i->content;

            switch (instruction->opcode) {
                case EXEC:
                    quantum = 0;
                    proc->pc++;
                    Interface__send_data(sched_win, SCHED_EXEC_FMT, proc->pid, instruction->value, quantum);
                    break;

                case SEM_P:
                    sem = Semaph__semaphore_search(instruction->sem);
                    blocked = Semaph__semaphore_P(sem, proc);
                    Interface__send_data(sched_win, SCHED_SEMP_FMT, proc->pid, instruction->sem, quantum);
                    if (blocked) {
                        quantum = 0;
                        flag = SEMAPH_BLOCKED;
                        proc->pc++;
                        Interface__send_data(sched_win, SCHED_SEM_BLOCK_FMT, proc->pid, instruction->sem);
                    } else {
                        quantum -= 200;
                        proc->pc++;
                    }
                    break;

                case SEM_V:
                    sem = Semaph__semaphore_search(instruction->sem);
                    Semaph__semaphore_V(sem);
                    quantum -= 200;
                    proc->pc++;
                    Interface__send_data(sched_win, SCHED_SEMV_FMT, proc->pid, instruction->sem, quantum);
                    break;

                case PRINT:
                    quantum = 0;
                    proc->pc++;
                    flag = IO_REQUESTED;
                    Interface__send_data(sched_win, SCHED_PRINT_FMT, proc->pid, instruction->value, quantum);
                    break;

                case READ:
                    quantum = 0;
                    proc->pc++;
                    flag = IO_REQUESTED;
                    Interface__send_data(sched_win, SCHED_READ_FMT, proc->pid, instruction->value, quantum);
                    break;

                case WRITE:
                    quantum = 0;
                    proc->pc++;
                    flag = IO_REQUESTED;
                    Interface__send_data(sched_win, SCHED_WRITE_FMT, proc->pid, instruction->value, quantum);
                    break;
            }

            if (quantum < 0) {
                instruction->value = -1 * quantum;
                proc->pc--;
                flag = QUANTUM_END;
                Interface__send_data(sched_win, SCHED_QUANTUM_TIMEOUT_FMT, proc->pid);
            }

            Interface__refresh_process_win();
        }

    }

    if (p == NULL && proc->pc >= total_instr)
        flag = PROCESS_END;

    return flag;
}
