#include "memory.h"
#include <stdlib.h>
#include <math.h>
#include "../interface/interface.h"

/// Create the kernel segment table
/// \return Returns segment table to the kernel
SegmentTable *Memory__create_segment_table() {
    SegmentTable *seg_table = malloc(sizeof(SegmentTable));

    seg_table->seg_list = List__create();
    seg_table->remaining_memory = MAX_MEMORY;

    return seg_table;
}

/// Make a memory request to the process
/// \param memory_request Process that make the request
/// \param seg_table Kernel segment table
void Memory__req_load_memory(List *memory_request, SegmentTable *seg_table) {
    // Cast process and instructions
    Process *process = (Process *) memory_request->head->content;
    List *instructions = (List *) memory_request->tail->content;

    // Create segment and pages
    Segment *segment = Memory__create_segment(process);
    Memory__create_pages(segment, instructions);

    // Updated remaining memory
    int remaining = seg_table->remaining_memory - process->segment_size;
    seg_table->remaining_memory = (remaining > 0) ? remaining : 0;

    // TODO: Add Swap

    // Append segment into kernel segment table
    List__append(seg_table->seg_list, (void *)segment);

    // Print in the ncurses
    Interface__print_segment(segment, process->name);
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

void Memory__create_pages(Segment *segment, List *instructions) {
    int num_pages, instructions_per_page, qtd_instr;

    num_pages = ceil((double)segment->seg_size / PAGE_SIZE);
    instructions_per_page = ceil((double)instructions->size / num_pages);

    for (int i = 0; i < num_pages; i++){
        Page *page = malloc(sizeof(Page));
        page->page_size = PAGE_SIZE;
        page->page_id = i;
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