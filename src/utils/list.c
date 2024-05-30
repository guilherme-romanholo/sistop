#include "list.h"
#include <stdio.h>
#include <stdlib.h>

/// Create a linked list
/// \return Return the created linked list
List* List__create() {
    List *list = malloc(sizeof(List));

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;

    return list;
}

/// Creates a new node with content
/// \param content Node content
/// \return Created node
Node* Node__create(void *content) {
    Node *node = malloc(sizeof(Node));

    node->content = content;
    node->next = NULL;

    return node;
}

int List__contains(List *list, void *data, int (*compare)(void *, void *)) {
    Node *current = list->head;
    while (current != NULL) {
        if (compare(current->content, data)) { // Assuming data pointers are directly comparable
            return 1; // Data already exists in the list
        }
        current = current->next;
    }
    return 0; // Data not found in the list
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
    }

    if (list->tail != NULL){
        list->tail->next = node;
    }

    list->tail = node;
    list->size++;
}

/// Remove the first node from a list
/// \param list List
/// \return Node content
void *List__remove_head(List *list) {
    if (list->head == NULL) {
        printf("List is empty.\n");
        return NULL;
    }

    Node *temp = list->head;
    void *content = list->head->content;
    list->head = list->head->next;
    free(temp);

    list->size--;

    if (list->head == NULL) {
        list->tail = NULL;
    }

    return content;
}

/// Remove node from list
/// \param list List
/// \param node Node to be removed
void List__remove_node(List *list, void *data, int (*compare)(void *, void*)) {
    if (list->head == NULL) {
        printf("List is empty.\n");
        return;
    }

    Node *current = list->head;
    Node *prev = NULL;

    // Traverse the list to find the node with the specified data
    while (current != NULL) {
        if (compare(current->content, data)) { // Assuming data pointers are directly comparable
            // Node found, remove it
            if (prev == NULL) {
                // Node to be removed is the head
                list->head = current->next;
                if (list->head == NULL) {
                    // If the list becomes empty after removal
                    list->tail = NULL;
                }
            } else {
                prev->next = current->next;
                if (current == list->tail) {
                    // If the node to be removed is the tail
                    list->tail = prev;
                }
            }
            free(current);
            list->size--;
            return;
        }
        prev = current;
        current = current->next;
    }
}

/// Free the list
/// \param list List
void List__destroy(List *list, void(*free_func)(void *)){
    Node *curr = list->head;

    while (curr != NULL) {
        Node *next = curr->next;
        free_func(curr->content);
        free(curr);
        curr = next;
    }

    free(list);
}
