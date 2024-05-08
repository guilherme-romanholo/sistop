//
// Created by guilherme on 07/05/24.
//

#ifndef SISTOP_LIST_H
#define SISTOP_LIST_H

typedef struct node {
    void *content;
    struct node *next;
    struct node *prev;
} node_t;

typedef struct list {
    node_t *head;
    node_t *tail;
    int size;
} list_t;

list_t* list__create();
void list__append(list_t*, void*);
node_t* node__create(void*);


#endif //SISTOP_LIST_H
