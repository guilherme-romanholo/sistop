#include "memory.h"
#include "../kernel/kernel.h"
#include "../interface/interface.h"
#include <stdlib.h>
#include <math.h>

/// Make a memory request to the process
/// \param memory_request Process that make the request
void Memory__req_load_memory(List *memory_request) {
    Process *process = (Process *) memory_request->head->content;
    Segment *segment = Memory__create_segment(process);
    List *instructions = (List *) memory_request->tail->content;

    Memory__create_pages(segment, instructions);

    long remaining = kernel->remaining_memory - process->segment_size;
    kernel->remaining_memory = (remaining > 0) ? remaining : 0;

    if (remaining < 0)
        kernel->remaining_memory += Memory__swap_out(segment);

    List__append(kernel->segment_table, (void *)segment);
}

/// Finalize memory creation
/// \param memory_request Process of memory request
void Memory__fin_load_memory(List *memory_request) {
    Process *process = (Process *) memory_request->head->content;

    process->state = READY;
    List__append(kernel->pcb, (void *)process);

    // TODO: Add process to Scheduler
    List__append(kernel->scheduler->sched_queue, (void *)process);

    sem_post(&kernel_interface->mutex);
    sem_post(&memory_interface->mutex);
    sem_post(&process_interface->mutex);
}

/// Create a segment for the process
/// \param process Process that request segment
/// \return Returns the new segment
Segment *Memory__create_segment(Process *process) {
    Segment *segment = malloc(sizeof(Segment));

    segment->seg_id = process->segment_id;
    segment->seg_size = process->segment_size;
    segment->pages = List__create();

    return segment;
}

/// Fetch segment in segment kernel table by id
/// \param seg_id Id of segment
/// \return NULL or Fetched Segment
Segment *Memory__fetch_segment(int seg_id) {
    Segment *temp;

    for (Node *aux = kernel->segment_table->head; aux != NULL ; aux = aux->next) {
        temp = (Segment *) aux->content;
        if (temp->seg_id == seg_id)
            return temp;
    }

    return NULL;
}

/// Create pages with instructions for the new process
/// \param segment
/// \param instructions
void Memory__create_pages(Segment *segment, List *instructions) {
    int num_pages, instructions_per_page, qtd_instr;

    num_pages = ceil((double)segment->seg_size / PAGE_SIZE);
    instructions_per_page = ceil((double)instructions->size / num_pages);

    for (int i = 0; i < num_pages; i++){
        Page *page = malloc(sizeof(Page));
        page->page_size = PAGE_SIZE;
        page->page_id = i;
        page->instr_per_page = instructions_per_page;
        page->used_bit = 0;
        page->total_instructions = instructions->size;
        page->instructions = List__create();

        qtd_instr = 0;

        while (qtd_instr < instructions_per_page && instructions->size != 0) {
            void *instr = List__remove_head(instructions);
            List__append(page->instructions, (void *) instr);
            qtd_instr++;
        }

        List__append(segment->pages, (void *) page);
    }
}

Node *Memory__fetch_page(Segment *seg, int page_id) {
    Node *aux;
    Page *temp;

    for (aux = seg->pages->head; aux != NULL ; aux = aux->next) {
        temp = (Page *) aux->content;
        if (temp->page_id == page_id)
            return aux;
    }

    return NULL;
}

Node *Memory__fetch_instruction(Page *page, int num_instruction) {
    Node *aux = page->instructions->head;
    Instruction *temp;

    for (int i = num_instruction; i > 0; i--){
        aux = aux->next;
    }

    return aux;
}

int Memory__swap_out(Segment *segment) {
    int free_mem = 0;
    Segment *seg_aux;
    Page *page_aux;

    for (Node *s = kernel->segment_table->head; s != NULL ; s = s->next) {
        seg_aux = (Segment *) s->content;

        for (Node *p = seg_aux->pages->head; p != NULL ; p = p->next) {
            page_aux = (Page *) p->content;

            if (page_aux->used_bit == 1)
                page_aux->used_bit = 0;
            else
                free_mem += page_aux->page_size;

            if (free_mem >= segment->seg_size)
                break;
        }
    }

    return free_mem;
}