//
// Created by guilherme on 08/05/24.
//

#ifndef SISTOP_MEMORY_H
#define SISTOP_MEMORY_H

#include "../utils/list.h"
#include "../process/process.h"

// Multiplicar para obter a quantidade correta em BYTES (padrão)

#define MAX_MEMORY (1 * GBYTE)
#define GBYTE (1024 * MBYTE)
#define MBYTE (1024 * KBYTE)
#define KBYTE 1024

typedef struct {
    List *seg_list;
    int remaining_memory;
} SegmentTable;

typedef struct {
    int seg_id;
    int seg_size;
    // TODO: Add pages
} Segment;

SegmentTable *Memory__create_segment_table();
Segment *Memory__create_segment(Process *);
void Memory__req_load_memory(Process *, SegmentTable *);

#endif //SISTOP_MEMORY_H
