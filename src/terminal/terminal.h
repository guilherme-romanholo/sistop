//
// Created by guilherme on 12/05/24.
//

#ifndef SISTOP_TERMINAL_H
#define SISTOP_TERMINAL_H

#include "../process/process.h"

void Terminal__init();
void Terminal__input_process();
void Terminal__list_seg_table();
void Terminal__list_proc_table();
void Terminal__list_sem_table();
char *Terminal__cast_proc_state(ProcessStatus);

#endif //SISTOP_TERMINAL_H
