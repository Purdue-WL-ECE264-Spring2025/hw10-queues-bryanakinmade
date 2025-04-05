#include "queue.h"
#include "tile_game.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// Maximum number of game states to keep track of (adjust for memory constraints)
#define MAX_STATES 100000

// Structure to track visited game states
typedef struct {
    uint64_t *states;  // Array of serialized game states
    size_t count;      // Number of states currently stored
} VisitedSet;

/* Checks if the current game state matches the solved configuration */
static bool is_solved(const struct game_state *state) {
    // The solved configuration of the 15-puzzle
    const uint8_t solved[4][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 0}
    };

    // Compare each tile with the solved configuration
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (state->tiles[i][j] != solved[i][j]) {
                return false;
            }
        }
    }
    return true;
}

/* Initializes the visited set data structure */
static void visited_init(VisitedSet *vs) {
    vs->states = malloc(MAX_STATES * sizeof(uint64_t));
    vs->count = 0;
}

/* Checks if a game state has already been visited */
static bool visited_contains(VisitedSet *vs, uint64_t state) {
    // Linear search through visited states
    for (size_t i = 0; i < vs->count; i++) {
        if (vs->states[i] == state) {
            return true;
        }
    }
    return false;
}

/* Adds a new game state to the visited set */
static void visited_add(VisitedSet *vs, uint64_t state) {
    // Only add if we haven't reached capacity
    if (vs->count < MAX_STATES) {
        vs->states[vs->count++] = state;
    }
}

/* Frees memory allocated for the visited set */
static void visited_free(VisitedSet *vs) {
    free(vs->states);
}

/* Adds a game state to the queue (serializes it first) */
void enqueue(struct queue *q, struct game_state state) {
    uint64_t serialized = serialize(state);
    insert_at_tail(&q->data, serialized);
}

/* Removes a game state from the queue (deserializes it) */
struct game_state dequeue(struct queue *q) {
    uint64_t serialized = remove_from_head(&q->data);
    return deserialize(serialized);
}

/* 
 * Calculates the minimum number of moves to solve the puzzle using BFS
 * Returns -1 if no solution is found
 */
int number_of_moves(struct game_state start) {
    // Check if already solved
    if (is_solved(&start)) {
        return 0;
    }

    struct queue q = {0};  // Initialize queue
    VisitedSet visited;    // Track visited states
    visited_init(&visited);

    // Start with initial state
    enqueue(&q, start);
    visited_add(&visited, serialize(start));

    // Process states in the queue
    while (q.data.head) {
        struct game_state current = dequeue(&q);

        // Generate all possible moves from current state
        struct game_state moves[4] = {current, current, current, current};
        move_up(&moves[0]);
        move_down(&moves[1]);
        move_left(&moves[2]);
        move_right(&moves[3]);

        // Check each possible move
        for (int i = 0; i < 4; i++) {
            // Valid move if number of steps increased
            if (moves[i].num_steps > current.num_steps) {
                uint64_t serialized = serialize(moves[i]);
                
                // Only process new states
                if (!visited_contains(&visited, serialized)) {
                    // Check if this move solves the puzzle
                    if (is_solved(&moves[i])) {
                        int steps = moves[i].num_steps;
                        visited_free(&visited);
                        free_list(q.data);
                        return steps;
                    }
                    
                    // Mark as visited and add to queue
                    visited_add(&visited, serialized);
                    enqueue(&q, moves[i]);
                }
            }
        }
    }

    // Clean up and return -1 if no solution found
    visited_free(&visited);
    free_list(q.data);
    return -1;
}