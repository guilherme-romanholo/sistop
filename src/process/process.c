#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"
#include "../kernel/kernel.h"
#include "../interface/interface.h"

/// Create a process
/// \param file File for synthetic process
void Process__create(const char *file) {
    char buffer[BUFFER_SIZE];
    FILE *fp = fopen(file, "r");

    if (!fp) {
      Interface__send_data(proc_win, "%s not exist. Place process in synt folder.", file);
      return;
    }

    Process *process = malloc(sizeof(Process));

    process->pid = kernel->proc_id_counter++;
    process->state = NEW;
    process->pc = 0;


    fgets(buffer, BUFFER_SIZE, fp);
    process->name = strdup(buffer);
    process->name[strlen(process->name) - 1] = '\0';

    fgets(buffer, BUFFER_SIZE, fp);
    process->segment_id = atoi(buffer);

    fgets(buffer, BUFFER_SIZE, fp);
    process->priority = atoi(buffer);

    fgets(buffer, BUFFER_SIZE, fp);
    process->segment_size = atoi(buffer) * KBYTE;

    fgets(buffer, BUFFER_SIZE, fp);
    Semaph__read_semaphores(buffer);

    fgets(buffer, BUFFER_SIZE, fp);

    List *instructions = Process__read_instructions(fp, buffer);

    fclose(fp);

    List *memory_request = List__create();
    List__append(memory_request, (void *) process);
    List__append(memory_request, (void *) instructions);

    Kernel__syscall(REQ_LOAD_MEMORY, (void *) memory_request);
}

/// Read instruction from process file
/// \param fp File pointer to synthetic process
/// \param buffer Buffer to read file
/// \return List of instructions
List *Process__read_instructions(FILE *fp, char *buffer) {
    List *instr_list = List__create();
    Instruction *instruction;

    while (fgets(buffer, BUFFER_SIZE, fp) != NULL) {
        instruction = Process__parse_instruction(buffer);
        List__append(instr_list, (void *) instruction);
    }

    return instr_list;
}

/// Parse string instructions to the Instruction type
/// \param content String instruction from file
/// \return Parsed instructions
Instruction *Process__parse_instruction(char *content) {
    Instruction *instruction = malloc(sizeof(Instruction));
    char *delimiter = " ";
    char *op = strtok(content, delimiter);
    char *value;

    if (op) {
        Process__cast_opcode(instruction, op);

        value = strtok(NULL, delimiter);
        if (value)
            instruction->value = atoi(value);
    }

    return instruction;
}

/// Cast process string opcode to integer
/// \param instr Instruction to insert opcode
/// \param opcode String opcode
void Process__cast_opcode(Instruction *instr, char *opcode) {
    if (!strcmp("exec", opcode)) {
        instr->opcode = EXEC;
    } else if (!strcmp("read", opcode)) {
        instr->opcode = READ;
    } else if (!strcmp("write", opcode)) {
        instr->opcode = WRITE;
    } else if (!strcmp("print", opcode)) {
        instr->opcode = PRINT;
    } else if (!strncmp("P(", opcode, 2)) {
        instr->opcode = SEM_P;
        instr->value = 200;
        instr->sem = opcode[2];
    } else if (!strncmp("V(", opcode, 2)) {
        instr->opcode = SEM_V;
        instr->value = 200;
        instr->sem = opcode[2];
    }
}

/// Finish the process, clears the memory from the segment + retires from PCB
/// \param process Process to be finished
void Process__finish(Process *process){
    Segment *segment = Memory__fetch_segment(process->segment_id);

    kernel->remaining_memory += segment->seg_size;

    List__remove_node(kernel->pcb, (void *) process, Utils__compare_process);
    Process_free_process(process);

    List__remove_node(kernel->segment_table, (void *) segment, Utils__compare_segment);
    Memory__free_segment(segment);

    Interface__refresh_memory_win();
    Interface__refresh_kernel_win();
}

void Process_free_process(void *p) {
    Process *process = (Process *) p;

    free(process->name);
    free(process);
}
