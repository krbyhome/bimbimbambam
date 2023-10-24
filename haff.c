#define ALPHABET_SIZE 256

#include <stdio.h>

#include "PriorityQueue.h"

struct node {
    int sym;
    size_t freq;
    struct haff_code *code;
    struct node *left;
    struct node *right;
};

struct haff_code {
    size_t len;
    int *val;
};

int* cpy_array(const int* arr, int size) {
    int *copied = calloc(ALPHABET_SIZE, sizeof(int));

    for (size_t i = 0; i < size; ++i) {
        *(copied + i) = *(arr + i);
    }

    return copied;
}

void set_code(struct node* curr_node, struct haff_code *prev_code) {
    if (curr_node->sym != -1) {
        curr_node->code = prev_code;

        return;
    }

    struct haff_code *left_code = malloc(sizeof(struct haff_code));
    left_code->len = prev_code->len;
    left_code->val = cpy_array(prev_code->val, prev_code->len);
    *(left_code->val + left_code->len) = 0;
    left_code->len++;
    set_code(curr_node->left, left_code);

    struct haff_code *right_code = malloc(sizeof(struct haff_code));
    right_code->len = prev_code->len;
    right_code->val = cpy_array(prev_code->val, prev_code->len);
    *(right_code->val + right_code->len) = 1;
    right_code->len++;
    set_code(curr_node->right, right_code);
}

int *char_to_byte(char c) {
    int *bites = calloc(8, sizeof(int));
    for (int i = 0; i < 9; ++i) {
        int bit;
        if (c & (1 << i)) {
            bit = 1;
        } else {
            bit = 0;
        }

        bites[7 - i] = bit;
    }

    return bites;
}

int encode(FILE *input, FILE *output) {
    struct node **node_arr = calloc(ALPHABET_SIZE, sizeof(struct node*));
    int node_counter = 0;
    int symbols_counter = 0;

    int symbol;
    struct node *current;
    while ((symbol = getc(input)) != EOF) {
        current = *(node_arr + symbol);
        if (!current) {
            current = malloc(sizeof(struct node));

            current->sym = symbol;
            current->freq = 0;
            current->code = 0;
            current->left = 0;
            current->right = 0;
            *(node_arr + symbol) = current;
            node_counter++;
        }

        current->freq++;
        symbols_counter++;
    }

    struct priority_queue pq;
    init(&pq);

    for (size_t i = 0; i < ALPHABET_SIZE; ++i) {
        current = *(node_arr + i);
        if (current) {
            push(&pq, current->freq, current);
        }
    }

    while (pq.size_of != 1) {
        struct node *first_min = pop(&pq);
        struct node *second_min = pop(&pq);
        struct node *new_node = malloc(sizeof(struct node));
        new_node->sym = -1;
        new_node->freq = first_min->freq + second_min->freq;
        new_node->left = first_min;
        new_node->right = second_min;

        push(&pq, new_node->freq, new_node);
    }

    struct node *root = pop(&pq);

    struct haff_code *code = malloc(sizeof(struct haff_code));
    code->len = 0;
    code->val = calloc(ALPHABET_SIZE, sizeof(int));
    set_code(root, code);

    fwrite(&node_counter, sizeof(int), 1, output);
    fwrite(&symbols_counter, sizeof(int), 1, output);
    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        current = *(node_arr + i);
        if (current) {
            fwrite(current, sizeof(struct node), 1, output);
        }
    }

    rewind(input);
    char write_code = 0;
    int code_size = 0;

    while((symbol = getc(input)) != EOF) {
        current = *(node_arr + symbol);
        int len = current->code->len;
        int *cur_code = current->code->val;
        for (int i = 0; i < len; ++i) {
            if (*(cur_code + i)) {
                write_code = (write_code << 1) | 1;
            } else {
                write_code = write_code << 1;
            }
        }
        code_size++;
        if (code_size == 8) {
            fwrite(&write_code, sizeof(char), 1, output);
            write_code = 0;
            code_size = 0;
        }
    }

    if (code_size != 0) {
        write_code <<= (8 - code_size);
        fwrite(&write_code, sizeof(char), 1, output);
    }

    free(node_arr);
}

int decode(FILE *input, FILE* output);
