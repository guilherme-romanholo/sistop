//
// Created by guilherme on 07/05/24.
//

#include "list.h"
#include <stdio.h>
#include <stdlib.h>

list_t* list__create() {
    list_t *list = malloc(sizeof(list_t));

    if (list == NULL) {
        printf("Not memory to allocate list.");
        exit(1);
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;

    return list;
}

void list__append(list_t *list, void *content) {
    node_t *node = node__create(content);

    if (node == NULL)
        return;

    if (list->head == NULL) {
        list->head = node;
    } else {
        list->tail->next = node;
        node->prev = list->tail;
    }

    list->tail = node;
    list->size++;
}

node_t* node__create(void *content) {
    node_t *node = malloc(sizeof(node_t));

    if (node == NULL) {
        printf("Not memory to allocate node.");
        return NULL;
    }

    node->content = content;
    node->next = NULL;
    node->prev = NULL;

    return node;
}