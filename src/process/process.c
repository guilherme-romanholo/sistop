#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"
#include "../utils/utils.h"
#include "../kernel/kernel.h"
#include "../semaph/semaph.h"

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
    fgets(buffer, BUFFER_SIZE, fp);
    Semaph__read_semaphores(buffer);

    // TODO: Read instructions

    fclose(fp);

    // TODO: Allocate memory to process
    Kernel__syscall(REQ_LOAD_MEMORY, (void *) process);
}