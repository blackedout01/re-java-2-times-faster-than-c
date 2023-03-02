/*
 * This file has been modified by github user blackedout01.
 *
 * Copyright 2021  Kazimierz Pogoda
 *
 * This file is part of java-2-times-faster-than-c.
 *
 * java-2-times-faster-than-c is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * java-2-times-faster-than-c is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with shader-web-background.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const int  MAX_PAYLOAD_SIZE   = 50;
const int  INITIAL_NODE_COUNT = 10000;
const long MUTATION_COUNT     = 1000000L;
const int  MAX_MUTATION_SIZE  = 200;

const int BLOCK_CAPACITY = INITIAL_NODE_COUNT;

typedef struct Node Node;

struct Node {
    long   id;
    int    size;
    char   *payload;
    Node   *previous;
    Node   *next;
} NodeDef;

struct NodeBlock {
    int count;
    struct NodeBlock *prev;
    char payload[BLOCK_CAPACITY*MAX_PAYLOAD_SIZE];
    Node nodes[BLOCK_CAPACITY];
    Node *first_free;
};

double almost_pseudo_random(long ordinal) {
    return fmod((sin(((double) ordinal) * 100000.0) + 1.0), 1.0);
}

void create_block(struct NodeBlock **block_ptr) {
    struct NodeBlock *block = *block_ptr;
    struct NodeBlock *next_block = calloc(1, sizeof(struct NodeBlock));
    next_block->prev = block;
    *block_ptr = next_block;
}

Node *alloc_node(struct NodeBlock **block_ptr, int payload_size) {
    Node *result;
    
    struct NodeBlock *block = *block_ptr;
    if(block->first_free) {
        result = block->first_free;
        block->first_free = block->first_free->previous;
    } else {
        if(block->count == BLOCK_CAPACITY) {
            create_block(block_ptr);
            block = *block_ptr;
        }
        
        result = block->nodes + block->count++;
        result->payload = block->payload + MAX_PAYLOAD_SIZE*(result - block->nodes);
    }
    
    return result;
}

Node *new_node(struct NodeBlock **block_ptr, long id) {
    int size = (int) (almost_pseudo_random(id) * MAX_PAYLOAD_SIZE);
    int charId = (char) id;
    Node *node = alloc_node(block_ptr, size);
    node->id = id;
    node->size = size;
    char *payload = node->payload;
    for (int i = 0; i < size; i++) {
        payload[i] = (char) i;
    }
    return node;
}

void join(Node *alfa, Node *beta) {
    alfa->previous = beta;
    alfa->next = beta;
    beta->previous = alfa;
    beta->next = alfa;
}

void delete(struct NodeBlock **block_ptr, Node *node) {
    node->next->previous = node->previous;
    node->previous->next = node->next;
    
    struct NodeBlock *block = *block_ptr;
    if(block->first_free) {
        node->previous = block->first_free;
    } else {
        node->previous = 0;
    }
    block->first_free = node;
}

void insert(Node *previous, Node *node) {
    node->next = previous->next;
    node->previous = previous;
    previous->next->previous = node;
    previous->next = node;
}

int main() {
    long node_id = 0;
    long mutation_seq = 0;
    struct NodeBlock *block = 0;
    create_block(&block);
    
    Node *head = new_node(&block, node_id++);
    join(head, new_node(&block, node_id++));
    for (int i = 2; i < INITIAL_NODE_COUNT; i++) {
        insert(head, new_node(&block, node_id++));
    }
    long node_count = INITIAL_NODE_COUNT;
    for (long i = 0; i < MUTATION_COUNT; i++) {
        int delete_count = (int) (almost_pseudo_random(mutation_seq++) * (double) MAX_MUTATION_SIZE);
        if (delete_count > (node_count - 2)) {
            delete_count = (int) node_count - 2;
        }
        for (int j = 0; j < delete_count; j++) {
            Node *to_delete = head;
            head = head->previous;
            delete(&block, to_delete);
        }
        node_count -= delete_count;
        int insert_count = (int) (almost_pseudo_random(mutation_seq++) * (double) MAX_MUTATION_SIZE);
        for (int j = 0; j < insert_count; j++) {
            insert(head, new_node(&block, node_id++));
            head = head->next;
        }
        node_count += insert_count;
    }
    long checksum = 0;
    Node *traveler = head;
    do {
        checksum += traveler->id + traveler->size;
        if (traveler->size > 0) {
            checksum += traveler->payload[0];
            checksum += traveler->payload[traveler->size - 1];
        }
    } while ((traveler = traveler->next) != head);
    printf("node count: %lu\n", node_count);
    printf("checksum: %lu\n", checksum);
    
    while(block) {
        struct NodeBlock *prev = block->prev;
        free(block);
        block = prev;
    }
}
