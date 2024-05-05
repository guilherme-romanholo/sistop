//
// Created by guilherme on 05/05/24.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"
#include "../utils/utils.h"
#include "../kernel/kernel.h"

void process__create(const char *file) {
    char buffer[BUFFER_SIZE];
    FILE *fp;

    if ((fp = fopen(file, "r")) == NULL) {
        printf("Process file does not exist.");
        exit(1);
    }

    process_t *process = malloc(sizeof(process_t));

    process->pid = kernel->proc_id_counter++;
    process->state = NEW;

    fgets(buffer, BUFFER_SIZE, fp);
    process->name = strdup(buffer);
    fgets(buffer, BUFFER_SIZE, fp);
    process->segment_id = atoi(buffer);
    fgets(buffer, BUFFER_SIZE, fp);
    process->priority = atoi(buffer);
    fgets(buffer, BUFFER_SIZE, fp);
    process->segment_size = atoi(buffer);

    fclose(fp);

    // TODO: Read semaphores

    // TODO: Read instructions

    // TODO: Allocate memory to process

    process->state = READY;
}