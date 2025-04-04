#include "linked_list.h"
#include <stdlib.h>
#include <stdio.h>

struct list_node *new_node(size_t value) {
    struct list_node *node = malloc(sizeof(struct list_node));
    if (!node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    node->value = value;
    node->next = NULL;
    return node;
}

void insert_at_head(struct linked_list *list, size_t value) {
    struct list_node *node = new_node(value);
    node->next = list->head;
    list->head = node;
    if (!list->tail) {
        list->tail = node;
    }
}

void insert_at_tail(struct linked_list *list, size_t value) {
    struct list_node *node = new_node(value);
    if (!list->tail) {
        list->head = list->tail = node;
    } else {
        list->tail->next = node;
        list->tail = node;
    }
}

size_t remove_from_head(struct linked_list *list) {
    if (!list->head) return (size_t)-1;
    
    struct list_node *temp = list->head;
    size_t value = temp->value;
    list->head = list->head->next;
    
    if (!list->head) {
        list->tail = NULL;
    }
    
    free(temp);
    return value;
}

size_t remove_from_tail(struct linked_list *list) {
    if (!list->tail) return (size_t)-1;
    
    size_t value = list->tail->value;
    
    if (list->head == list->tail) {
        free(list->head);
        list->head = list->tail = NULL;
        return value;
    }
    
    struct list_node *current = list->head;
    while (current->next != list->tail) {
        current = current->next;
    }
    
    free(list->tail);
    list->tail = current;
    current->next = NULL;
    
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

// Utility function to help you debugging, do not modify
void dump_list(FILE *fp, struct linked_list list) {
  fprintf(fp, "[ ");
  for (struct list_node *cur = list.head; cur != NULL; cur = cur->next) {
    fprintf(fp, "%zu ", cur->value);
  }
  fprintf(fp, "]\n");
}
