#ifndef HUFFMAN_TEST1_PRIORITYQUEUE_H
#define HUFFMAN_TEST1_PRIORITYQUEUE_H

#include <stdlib.h>
#include <stdbool.h>

// Пара символ и количество его повторений в файле
struct element {
    void* symbol;
    size_t priority;
};
// Приоритетная очередь
struct priority_queue {
    struct element* values;
    size_t size_of;
};

void push(struct priority_queue *pq, size_t priority, void* symbol);

void *pop(struct priority_queue *pq);

void init(struct priority_queue *queue);

bool empty(struct priority_queue *pq);

size_t queue_size();

#endif