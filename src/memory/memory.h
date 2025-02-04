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
    int used_bit;
    int total_instructions;
    int instr_per_page;
    List *instructions; // (Instruction *)
} Page;

/// Process Segment
typedef struct {
    int seg_id;
    int seg_size;
    List *pages; // (Page *)
} Segment;

void Memory__req_load_memory(List *);
void Memory__fin_load_memory(List *);
int Memory__swap_out(Segment *);

Segment *Memory__create_segment(Process *);
void Memory__create_pages(Segment *, List *);

Segment *Memory__fetch_segment(int);
Node *Memory__fetch_page(Segment *, int);
Node *Memory__fetch_instruction(Page *, int);

void Memory__free_segment(void *);
void Memory__free_page(void *);

#endif //SISTOP_MEMORY_H
