#include <stdbool.h>
#include <stdlib.h>
#include "queue.h"

#define VISITED_SIZE 1000000  // Adjust based on memory constraints

struct visited_set {
    uint64_t *states;
    size_t size;
    size_t capacity;
};

void init_visited(struct visited_set *set) {
    set->capacity = VISITED_SIZE;
    set->states = malloc(set->capacity * sizeof(uint64_t));
    set->size = 0;
}

bool is_visited(struct visited_set *set, uint64_t state) {
    for (size_t i = 0; i < set->size; i++) {
        if (set->states[i] == state) return true;
    }
    return false;
}

void add_visited(struct visited_set *set, uint64_t state) {
    if (set->size >= set->capacity) {
        // Simple approach: don't add more (better: implement LRU)
        return;
    }
    set->states[set->size++] = state;
}

void free_visited(struct visited_set *set) {
    free(set->states);
}

int number_of_moves(struct game_state start) {
    if (is_solved(&start)) return 0;
    
    struct queue q = {0};
    struct visited_set visited;
    init_visited(&visited);
    
    uint64_t start_serialized = serialize(start);
    add_visited(&visited, start_serialized);
    enqueue(&q, start);
    
    while (q.data.head) {
        struct game_state current = dequeue(&q);
        
        // Try all moves
        struct game_state moves[4] = {current, current, current, current};
        move_up(&moves[0]);
        move_down(&moves[1]);
        move_left(&moves[2]);
        move_right(&moves[3]);
        
        for (int i = 0; i < 4; i++) {
            if (moves[i].num_steps > current.num_steps) {  // Valid move
                uint64_t serialized = serialize(moves[i]);
                if (!is_visited(&visited, serialized)) {
                    if (is_solved(&moves[i])) {
                        int steps = moves[i].num_steps;
                        free_visited(&visited);
                        free_list(q.data);
                        return steps;
                    }
                    add_visited(&visited, serialized);
                    enqueue(&q, moves[i]);
                }
            }
        }
    }
    
    free_visited(&visited);
    free_list(q.data);
    return -1;  // No solution found
}