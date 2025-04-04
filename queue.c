#include "queue.h"
#include "tile_game.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATES 100000  // Adjust based on memory constraints

typedef struct {
    uint64_t *states;
    size_t count;
} VisitedSet;

static bool is_solved(const struct game_state *state) {
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

static void visited_init(VisitedSet *vs) {
    vs->states = malloc(MAX_STATES * sizeof(uint64_t));
    vs->count = 0;
}

static bool visited_contains(VisitedSet *vs, uint64_t state) {
    for (size_t i = 0; i < vs->count; i++) {
        if (vs->states[i] == state) {
            return true;
        }
    }
    return false;
}

static void visited_add(VisitedSet *vs, uint64_t state) {
    if (vs->count < MAX_STATES) {
        vs->states[vs->count++] = state;
    }
}

static void visited_free(VisitedSet *vs) {
    free(vs->states);
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
    VisitedSet visited;
    visited_init(&visited);

    enqueue(&q, start);
    visited_add(&visited, serialize(start));

    while (q.data.head) {
        struct game_state current = dequeue(&q);

        // Generate all possible moves
        struct game_state moves[4] = {current, current, current, current};
        move_up(&moves[0]);
        move_down(&moves[1]);
        move_left(&moves[2]);
        move_right(&moves[3]);

        for (int i = 0; i < 4; i++) {
            if (moves[i].num_steps > current.num_steps) {  // Valid move
                uint64_t serialized = serialize(moves[i]);
                if (!visited_contains(&visited, serialized)) {
                    if (is_solved(&moves[i])) {
                        int steps = moves[i].num_steps;
                        visited_free(&visited);
                        free_list(q.data);
                        return steps;
                    }
                    visited_add(&visited, serialized);
                    enqueue(&q, moves[i]);
                }
            }
        }
    }

    visited_free(&visited);
    free_list(q.data);
    return -1;
}