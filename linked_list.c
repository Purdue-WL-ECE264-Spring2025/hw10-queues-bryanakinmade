#include "linked_list.h"
#include <stdlib.h>

struct list_node *new_node(size_t value) {
    struct list_node *node = malloc(sizeof(struct list_node));
    if (!node) return NULL;
    node->value = value;
    node->next = NULL;
    return node;
}

void insert_at_head(struct linked_list *list, size_t value) {
    struct list_node *node = new_node(value);
    if (!node) return;
    node->next = list->head;
    list->head = node;
}

void insert_at_tail(struct linked_list *list, size_t value) {
    struct list_node *node = new_node(value);
    if (!node) return;
    
    if (!list->head) {
        list->head = node;
        return;
    }
    
    struct list_node *current = list->head;
    while (current->next) {
        current = current->next;
    }
    current->next = node;
}

size_t remove_from_head(struct linked_list *list) {
    if (!list->head) return 0;
    
    struct list_node *to_remove = list->head;
    size_t value = to_remove->value;
    list->head = to_remove->next;
    free(to_remove);
    return value;
}

size_t remove_from_tail(struct linked_list *list) {
    if (!list->head) return 0;
    
    struct list_node *current = list->head;
    struct list_node *prev = NULL;
    
    while (current->next) {
        prev = current;
        current = current->next;
    }
    
    size_t value = current->value;
    if (prev) {
        prev->next = NULL;
    } else {
        list->head = NULL;
    }
    free(current);
    return value;
}

void free_list(struct linked_list list) {
    struct list_node *current = list.head;
    while (current) {
        struct list_node *next = current->next;
        free(current);
        current = next;
    }
}