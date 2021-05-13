#ifndef PARENTHESIS_H
#define PARENTHESIS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE    (1)
#define FALSE   (0)

typedef enum {
    FIRST_PARENTHESIS_TOP_INDEX,
    SECOND_PARENTHESIS_TOP_INDEX,
    THIRD_PARENTHESIS_TOP_INDEX,
    FOURTH_PARENTHESIS_TOP_INDEX
} parenthesis_stack_list_index_t;

typedef struct {
    size_t opening_index;
    size_t closing_index;
} parenthesis_t;

typedef struct data {
    size_t opening_index;
    struct data* next;
} data_t;

size_t get_matching_parentheses(parenthesis_t* parentheses, size_t max_size, const char* str);

size_t is_empty_parentheses_stack(data_t** head);
void insert_parentheses_stack(data_t** head, size_t start_index);
int pop_parentheses_stack(data_t** head);

void dispose_all_parenthses_stack(data_t** head);

#endif  /* PARENTHESES_H */
