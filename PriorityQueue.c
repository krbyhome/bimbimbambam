#include "PriorityQueue.h"
#include <stdio.h>
typedef long long ll;

#define MAX_PQ_CAPACITY 100000

// Функция, меняющая элементы местами
void swap(struct element *a, struct element *b)
{
  struct element temp = *b;
  *b = *a;
  *a = temp;
}
// Функция, выделяющая память для приоритетной очереди
void init(struct priority_queue *pq) {
    pq->values = (struct element*)malloc(MAX_PQ_CAPACITY);
    pq->size_of = 0;
}

void push(struct priority_queue *pq, size_t priority, void *symbol) {
    pq->values[pq->size_of].symbol = symbol;
    pq->values[pq->size_of].priority = priority;
    size_t i = pq->size_of;
    pq->size_of++;
    while (i != 0 && pq->values[i].priority < pq->values[(i-1)/2].priority) {
        swap(&pq->values[i], &pq->values[(i-1)/2]);
        i = (i-1)/2;
    }
}
//индекс ребенка меньше size_of
//и то что ребенок минимальный среди обоих детей
void *pop(struct priority_queue* pq) {
    if (pq->size_of == 0) {
        return NULL;
    }

    swap(&pq->values[0], &pq->values[pq->size_of - 1]);
    pq->size_of--;
    size_t i = 0;
    size_t comp;
    while ((2*i + 1) < pq->size_of) {
        comp = 2*i + 1;
        if (2*i + 2 < pq->size_of && pq->values[comp].priority > pq->values[2*i + 2].priority) {
            comp = 2*i + 2;
        }
        if (pq->values[comp].priority < pq->values[i].priority) {
            swap(&pq->values[comp], &pq->values[i]);
            i = comp;
        } else {
            break;
        }
    }

  return pq->values[pq->size_of].symbol;
}

bool empty(struct priority_queue *pq) {
    if (pq->size_of == 0){
        return true;
    }
    else
        return false;
}