//
// Created by guilherme on 07/05/24.
//

#include "list.h"
#include <stdio.h>
#include <stdlib.h>

/// Create a linked list
/// \return Return the created linked list
List* List__create() {
    List *list = malloc(sizeof(List));

    if (list == NULL) {
        printf("Not memory to allocate list.");
        exit(1);
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;

    return list;
}

/// Append a new node in the list
/// \param list List to append node
/// \param content Node content
void List__append(List *list, void *content) {
    Node *node = Node__create(content);

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

/// Creates a new node with content
/// \param content Node content
/// \return Created node
Node* Node__create(void *content) {
    Node *node = malloc(sizeof(Node));

    if (node == NULL) {
        printf("Not memory to allocate node.");
        return NULL;
    }

    node->content = content;
    node->next = NULL;
    node->prev = NULL;

    return node;
}