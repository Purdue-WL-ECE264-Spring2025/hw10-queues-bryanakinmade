#include "queue.h"
#include "tile_game.h"
#include <stdbool.h>
#include <stdlib.h>

bool is_solved(struct game_state *state) {
    uint8_t solved[4][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 0}
    };
    
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (state->tiles[i][j] != solved[i][j]) {
                return false;
            }
        }
    }
    return true;
}

void enqueue(struct queue *q, struct game_state state) {
    uint64_t serialized = serialize(state);
    insert_at_tail(&q->data, serialized);
}

struct game_state dequeue(struct queue *q) {
    uint64_t serialized = remove_from_head(&q->data);
    return deserialize(serialized);
}

int number_of_moves(struct game_state start) {
    if (is_solved(&start)) {
        return 0;
    }
    
    struct queue q = {0};
    enqueue(&q, start);
    
    while (q.data.head) {
        struct game_state current = dequeue(&q);
        
        struct game_state up = current;
        move_up(&up);
        if (is_solved(&up)) {
            free_list(q.data);
            return up.num_steps;
        }
        if (up.num_steps > current.num_steps) {
            enqueue(&q, up);
        }
        
        struct game_state down = current;
        move_down(&down);
        if (is_solved(&down)) {
            free_list(q.data);
            return down.num_steps;
        }
        if (down.num_steps > current.num_steps) {
            enqueue(&q, down);
        }
        
        struct game_state left = current;
        move_left(&left);
        if (is_solved(&left)) {
            free_list(q.data);
            return left.num_steps;
        }
        if (left.num_steps > current.num_steps) {
            enqueue(&q, left);
        }
        
        struct game_state right = current;
        move_right(&right);
        if (is_solved(&right)) {
            free_list(q.data);
            return right.num_steps;
        }
        if (right.num_steps > current.num_steps) {
            enqueue(&q, right);
        }
    }
    
    free_list(q.data);
    return -1;
}

