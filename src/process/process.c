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

    // Initialize process
    process->pid = kernel->proc_id_counter++;
    process->state = NEW;
    process->pc = 0;

    // Read data
    fgets(buffer, BUFFER_SIZE, fp);
    process->name = strdup(buffer);
    fgets(buffer, BUFFER_SIZE, fp);
    process->segment_id = atoi(buffer);
    fgets(buffer, BUFFER_SIZE, fp);
    process->priority = atoi(buffer);
    fgets(buffer, BUFFER_SIZE, fp);
    process->segment_size = atoi(buffer) * KBYTE;

    // Read semaphores to kernel table
    // TODO: Remover não dar append em semaforos já existentes
    fgets(buffer, BUFFER_SIZE, fp);
    Semaph__read_semaphores(buffer);

    // Read one blank line
    fgets(buffer, BUFFER_SIZE, fp);

    // Read instructions
    List *instructions = Process__read_instructions(fp, buffer);

    // TODO: DEBUG
    Node *head = instructions->head;
    while (head != NULL) {
        Instruction *i = (Instruction *) head->content;
        printf("%d, %d, %c\n",i->opcode, i->value, i->sem);
        head = head->next;
    }

    fclose(fp);

    // TODO: Allocate memory to process
    Kernel__syscall(REQ_LOAD_MEMORY, (void *) process);
}

List *Process__read_instructions(FILE *fp, char *buffer) {
    // Create instruction list
    List *instr_list = List__create();
    Instruction *instruction;

    // Leitura de todas as instruções do arquivo
    while (fgets(buffer, BUFFER_SIZE, fp) != NULL) {
        instruction = Process__parse_instruction(buffer);
        List__append(instr_list, instruction);
    }

    return instr_list;
}

Instruction *Process__parse_instruction(char *content) {
    Instruction *instruction = malloc(sizeof(Instruction));
    char *delimiter = " "; // Assuming this is the delimiter between operation and value
    char *op = strtok(content, delimiter);
    char *value;

    if (op) {
        // Assign operation
        Process__cast_opcode(instruction, op);

        // Extract value using strtok again
        value = strtok(NULL, delimiter);
        if (value)
            instruction->value = atoi(value); // Convert value to integer
    }

    return instruction;
}

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
        instr->sem = opcode[2];
    } else if (!strncmp("V(", opcode, 2)) {
        instr->opcode = SEM_V;
        instr->sem = opcode[2];
    }
}
