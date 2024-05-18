#include "memory.h"
#include <stdlib.h>

/// Create the kernel segment table
/// \return Returns segment table to the kernel
SegmentTable *Memory__create_segment_table() {
    SegmentTable *seg_table = malloc(sizeof(SegmentTable));

    seg_table->seg_list = List__create();
    seg_table->remaining_memory = MAX_MEMORY;

    return seg_table;
}

/// Create a segment for the process
/// \param process Process that request segment
/// \return Returns the new segment
Segment *Memory__create_segment(Process *process) {
    Segment *segment = malloc(sizeof(Segment));
    int instructions_per_page, qtd_instructions, num_instructions = 0;

    segment->seg_id = process->segment_id;
    segment->seg_size = process->segment_size;

    // Seta as quantidades de paginas
    // segment->num_pages = segment->seg_size / PAGE_SIZE;
    // instructions_per_page = num_instructions / segment->num_pages;

    // // Aloca espaço para as paginas no segmento
    // segment->pages = malloc (segment->num_pages * sizeof(Page));

    // for (int i = 0; i < segment->num_pages; i++){

    //     segment->pages[i].page_size = PAGE_SIZE;
    //     qtd_instructions = 0; // Pra saber qual posição de instrução esta armazenando
    //     segment->pages[i].instructions = malloc (instructions_per_page * sizeof (Instruction)); // Aloca vetor para armazenar as instruções

    //     while (qtd_instructions < instructions_per_page && instr->head != NULL){ // TODO: Condições corretas?

    //         // TODO: Verificar se é semaforo

    //         segment->pages[i].instructions[qtd_instructions++] = Separar_op_value(instr->head->content);

    //         instr->head = instr->head->next;
    //     }

    //     segment->pages[i].num_instructions = qtd_instructions;
    // }

    return segment;
}

/// Make a memory request to the process
/// \param process Process that make the request
/// \param seg_table Kernel segment table
void Memory__req_load_memory(Process *process, SegmentTable *seg_table) {
    Segment *segment = Memory__create_segment(process);

    int remaining = seg_table->remaining_memory - process->segment_size;
    seg_table->remaining_memory = (remaining > 0) ? remaining : 0;

    // TODO: Add Swap

    // TODO: Add code to segment page

    List__append(seg_table->seg_list, (void *)segment);
}