#ifndef SISTOP_PROCESS_H
#define SISTOP_PROCESS_H

#include "../utils/utils.h"

typedef enum {
    EXEC  = 0,
    READ  = 1,
    WRITE = 2,
    PRINT = 3,
    SEM_P = 4,
    SEM_V = 5
} Opcode;

typedef struct {
    char op[OPCODE_SIZE];
    int  value;
    // char *sem;
} Instruction;

/// Process status enumeration
typedef enum {
    NEW        = 0,
    READY      = 1,
    RUNNING    = 2,
    WAITING    = 3,
    TERMINATED = 4
} ProcessStatus;

/// Process struct
typedef struct {
    // BCP Data
    int  pid;
    int  state;
    int  pc;
    char *name;
    int  segment_id;
    int  priority;
    int  segment_size;
} Process;


void Process__create(const char *);

#endif //SISTOP_PROCESS_H
