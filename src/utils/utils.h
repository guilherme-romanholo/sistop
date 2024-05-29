//
// Created by guilherme on 05/05/24.
//

#ifndef SISTOP_UTILS_H
#define SISTOP_UTILS_H

#define BUFFER_SIZE 256
#define OPCODE_SIZE 10

int Utils__compare_segment(void *, void *);
int Utils__compare_process(void *, void *);
int Utils__compare_page(void *, void *);

#endif //SISTOP_UTILS_H
