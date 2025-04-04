#include "queue.h"
#include "tile_game.h"
#include <stdbool.h>
#include <stdlib.h>

bool is_solved(const struct game_state *state) {
    const uint8_t solved[4][4] = {
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

        // Try all possible moves
        struct game_state new_state;

        // Try moving up
        new_state = current;
        move_up(&new_state);
        if (is_solved(&new_state)) {
            free_list(q.data);
            return new_state.num_steps;
        }
        if (new_state.num_steps > current.num_steps) {
            enqueue(&q, new_state);
        }

        // Try moving down
        new_state = current;
        move_down(&new_state);
        if (is_solved(&new_state)) {
            free_list(q.data);
            return new_state.num_steps;
        }
        if (new_state.num_steps > current.num_steps) {
            enqueue(&q, new_state);
        }

        // Try moving left
        new_state = current;
        move_left(&new_state);
        if (is_solved(&new_state)) {
            free_list(q.data);
            return new_state.num_steps;
        }
        if (new_state.num_steps > current.num_steps) {
            enqueue(&q, new_state);
        }

        // Try moving right
        new_state = current;
        move_right(&new_state);
        if (is_solved(&new_state)) {
            free_list(q.data);
            return new_state.num_steps;
        }
        if (new_state.num_steps > current.num_steps) {
            enqueue(&q, new_state);
        }
    }

    free_list(q.data);
    return -1; // No solution found
}