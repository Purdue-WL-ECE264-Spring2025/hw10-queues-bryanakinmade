#include "queue.h"
#include "tile_game.h"
#include <stdbool.h>

// Helper function to check if board is solved
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
    insert_at_tail(&q->list, serialized);
}

struct game_state dequeue(struct queue *q) {
    uint64_t serialized = remove_from_head(&q->list);
    return deserialize(serialized);
}

int number_of_moves(struct game_state start) {
    struct queue q = {0};
    enqueue(&q, start);
    
    // Visited tracking (using a simple hash set)
    #define VISITED_SIZE (1 << 20) // ~1 million entries
    bool *visited = calloc(VISITED_SIZE, sizeof(bool));
    
    while (q.list.head) {
        struct game_state current = dequeue(&q);
        uint64_t hash = serialize(current) % VISITED_SIZE;
        
        if (is_solved(&current)) {
            free_list(q.list);
            free(visited);
            return current.num_steps;
        }
        
        if (visited[hash]) continue;
        visited[hash] = true;
        
        // Generate all possible moves
        int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        
        for (int i = 0; i < 4; i++) {
            int new_row = current.empty_row + directions[i][0];
            int new_col = current.empty_col + directions[i][1];
            
            if (new_row >= 0 && new_row < 4 && new_col >= 0 && new_col < 4) {
                struct game_state new_state = current;
                // Swap tiles
                new_state.tiles[current.empty_row][current.empty_col] = 
                    current.tiles[new_row][new_col];
                new_state.tiles[new_row][new_col] = 0;
                new_state.empty_row = new_row;
                new_state.empty_col = new_col;
                new_state.num_steps++;
                
                uint64_t new_hash = serialize(new_state) % VISITED_SIZE;
                if (!visited[new_hash]) {
                    enqueue(&q, new_state);
                }
            }
        }
    }
    
    free_list(q.list);
    free(visited);
    return -1; // No solution found
}