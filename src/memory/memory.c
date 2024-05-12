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

    segment->seg_id = process->segment_id;
    segment->seg_size = process->segment_size;

    // TODO: Add pages with instructions

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