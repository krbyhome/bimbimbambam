#include <stdio.h>
#include "PriorityQueue.h"

int main() {
    struct priority_queue pq;
    init(&pq);

    int pr[5] = {
    5, 4, 3, 2, 1
    };

    char *bimbim[5] = {
        "aa", "bb", "cc", "dd", "ee"
        };

    for (size_t i = 0; i < 5; ++i) {
        push(&pq, pr[i], bimbim[i]);
    }

    while (!empty(&pq)) {
        char* min = (char*)pop(&pq);
        printf("%s\n", min);
    }


    return 0;
}
