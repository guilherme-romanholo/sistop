#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"
#include "../kernel/kernel.h"

/// Create a process
/// \param file File for synthetic process
void Process__create(const char *file) {
    char buffer[BUFFER_SIZE];
    FILE *fp = fopen(file, "r");
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
        List__append(instr_list, instruction);
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
    Segment *seg_aux;
    Process *proc_aux;
    Node *s, *p;

    // Search the segment from process in segment_table 
    for (s = kernel->segment_table->head; s != NULL ; s = s->next) {
        seg_aux = (Segment *) s->content;

        if(seg_aux->seg_id == process->segment_id)
            break;
    }

    // Clears segment from memory
    kernel->remaining_memory += seg_aux->seg_size;

    do {
        p = seg_aux->pages->head->next;
        List__remove_head(seg_aux->pages); // TODO: Sem retorno vai?
    } while (p != NULL);

    /*for (Node *p = seg_aux->pages->head; p != NULL ; p = p->next)
        List__remove_node(seg_aux->pages, p); */

    // Removes segment from segment table
    List__remove_node(kernel->segment_table, s);

    // Search the segment from process in segment_table 
    for (s = kernel->pcb->head; s != NULL ; s = s->next) {
        proc_aux = (Process *) s->content;

        if(proc_aux->pid == process->pid)
            break;
    }

    List__remove_node(kernel->pcb, s);

    free(process);
}

