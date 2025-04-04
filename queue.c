#include "queue.h"
#include "tile_game.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define HASH_SIZE 1000003  // Large prime number for hash table

typedef struct hash_entry {
    uint64_t key;
    struct hash_entry *next;
} hash_entry;

typedef struct {
    hash_entry **entries;
} hash_table;

hash_table *create_hash_table() {
    hash_table *ht = malloc(sizeof(hash_table));
    ht->entries = calloc(HASH_SIZE, sizeof(hash_entry*));
    return ht;
}

void free_hash_table(hash_table *ht) {
    for (int i = 0; i < HASH_SIZE; i++) {
        hash_entry *entry = ht->entries[i];
        while (entry) {
            hash_entry *next = entry->next;
            free(entry);
            entry = next;
        }
    }
    free(ht->entries);
    free(ht);
}

bool hash_insert(hash_table *ht, uint64_t key) {
    uint64_t index = key % HASH_SIZE;
    hash_entry *entry = ht->entries[index];
    
    while (entry) {
        if (entry->key == key) return false;
        entry = entry->next;
    }
    
    hash_entry *new_entry = malloc(sizeof(hash_entry));
    new_entry->key = key;
    new_entry->next = ht->entries[index];
    ht->entries[index] = new_entry;
    return true;
}

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
    hash_table *visited = create_hash_table();
    enqueue(&q, start);
    hash_insert(visited, serialize(start));
    
    while (q.list.head) {
        struct game_state current = dequeue(&q);
        
        if (is_solved(&current)) {
            free_list(q.list);
            free_hash_table(visited);
            return current.num_steps;
        }
        
        int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        
        for (int i = 0; i < 4; i++) {
            int new_row = current.empty_row + directions[i][0];
            int new_col = current.empty_col + directions[i][1];
            
            if (new_row >= 0 && new_row < 4 && new_col >= 0 && new_col < 4) {
                struct game_state new_state = current;
                new_state.tiles[current.empty_row][current.empty_col] = 
                    current.tiles[new_row][new_col];
                new_state.tiles[new_row][new_col] = 0;
                new_state.empty_row = new_row;
                new_state.empty_col = new_col;
                new_state.num_steps++;
                
                uint64_t serialized = serialize(new_state);
                if (hash_insert(visited, serialized)) {
                    enqueue(&q, new_state);
                }
            }
        }
    }
    
    free_list(q.list);
    free_hash_table(visited);
    return -1;
}