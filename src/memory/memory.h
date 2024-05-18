//
// Created by guilherme on 08/05/24.
//

#ifndef SISTOP_MEMORY_H
#define SISTOP_MEMORY_H

#include "../utils/list.h"
#include "../process/process.h"

/// Memory units to use everything with BYTEs
/// Example: 32Kb = 32 * KBYTE
#define MAX_MEMORY (1 * GBYTE)
#define GBYTE (1024 * MBYTE)
#define MBYTE (1024 * KBYTE)
#define KBYTE 1024

/// Memory Page Size
#define PAGE_SIZE (8 * KBYTE)

/// Process Page
typedef struct {
    int page_id;
    int page_size;
    List *instructions; // (Instruction *)
} Page;

/// Kernel Segment Table
typedef struct {
    List *seg_list; // (Segment *)
    int remaining_memory;
} SegmentTable;

/// Process Segment
typedef struct {
    int seg_id;
    int seg_size;
    List *pages; // (Page *)
} Segment;

SegmentTable *Memory__create_segment_table();
Segment *Memory__create_segment(Process *);
void Memory__create_pages(Segment *, List *);
void Memory__req_load_memory(List *, SegmentTable *);

#endif //SISTOP_MEMORY_H
