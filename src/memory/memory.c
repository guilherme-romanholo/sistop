#include "memory.h"
#include <stdlib.h>

SegmentTable *Memory__create_segment_table() {
    SegmentTable *seg_table = malloc(sizeof(SegmentTable));

    seg_table->seg_list = List__create();
    seg_table->remaining_memory = MAX_MEMORY;

    return seg_table;
}

Segment *Memory__create_segment(Process *process) {
    Segment *segment = malloc(sizeof(Segment));

    segment->seg_id = process->segment_id;
    segment->seg_size = process->segment_size;

    // TODO: Add pages with instructions

    return segment;
}


void Memory__req_load_memory(Process *process, SegmentTable *seg_table) {
    Segment *segment = Memory__create_segment(process);

    int remaining = seg_table->remaining_memory - process->segment_size;
    seg_table->remaining_memory = remaining ? (remaining > 0) : 0;

    // TODO: Add Swap

    // TODO: Add code to segment page

    List__append(seg_table->seg_list, (void *)segment);
}
