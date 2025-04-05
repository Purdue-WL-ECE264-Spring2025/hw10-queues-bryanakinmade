#include "linked_list.h"
#include <stdlib.h>

/* 
 * Creates a new list node with the given value
 * Returns pointer to new node or NULL if allocation fails
 */
struct list_node *new_node(size_t value) {
    // Allocate memory for new node
    struct list_node *node = malloc(sizeof(struct list_node));
    if (!node) return NULL;  // Return NULL if allocation fails
    
    // Initialize node values
    node->value = value;
    node->next = NULL;
    return node;
}

/* 
 * Inserts a new node at the head of the list
 * If memory allocation fails, function returns without modifying list
 */
void insert_at_head(struct linked_list *list, size_t value) {
    struct list_node *node = new_node(value);
    if (!node) return;  // Return if allocation failed
    
    // Make new node point to current head, then update head
    node->next = list->head;
    list->head = node;
}

/* 
 * Inserts a new node at the tail of the list
 * If memory allocation fails, function returns without modifying list
 */
void insert_at_tail(struct linked_list *list, size_t value) {
    struct list_node *node = new_node(value);
    if (!node) return;  // Return if allocation failed
    
    // If list is empty, new node becomes head
    if (!list->head) {
        list->head = node;
        return;
    }
    
    // Traverse to last node
    struct list_node *current = list->head;
    while (current->next) {
        current = current->next;
    }
    
    // Append new node to end
    current->next = node;
}

/* 
 * Removes and returns the value at the head of the list
 * Returns 0 if list is empty
 */
size_t remove_from_head(struct linked_list *list) {
    if (!list->head) return 0;  // Return 0 for empty list
    
    // Store node to remove and its value
    struct list_node *to_remove = list->head;
    size_t value = to_remove->value;
    
    // Update head pointer
    list->head = to_remove->next;
    
    // Free memory and return value
    free(to_remove);
    return value;
}

/* 
 * Removes and returns the value at the tail of the list
 * Returns 0 if list is empty
 */
size_t remove_from_tail(struct linked_list *list) {
    if (!list->head) return 0;  // Return 0 for empty list
    
    struct list_node *current = list->head;
    struct list_node *prev = NULL;
    
    // Traverse to last node
    while (current->next) {
        prev = current;
        current = current->next;
    }
    
    // Store value to return
    size_t value = current->value;
    
    // Update pointers
    if (prev) {
        prev->next = NULL;  // For lists with >1 node
    } else {
        list->head = NULL;  // For single-node lists
    }
    
    // Free memory and return value
    free(current);
    return value;
}

/* 
 * Frees all memory allocated for the linked list
 */
void free_list(struct linked_list list) {
    struct list_node *current = list.head;
    
    // Traverse list and free each node
    while (current) {
        struct list_node *next = current->next;  // Save next pointer
        free(current);                           // Free current node
        current = next;                          // Move to next node
    }
}