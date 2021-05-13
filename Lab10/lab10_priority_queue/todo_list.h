#ifndef TODO_LIST_H
#define TODO_LIST_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

typedef struct todo_list {
    int32_t* priority_list;
    char** task_list;
    size_t cur_size;
    size_t max_size;
} todo_list_t;

todo_list_t init_todo_list(size_t max_size);
void finalize_todo_list(todo_list_t* todo_list);
bool add_todo(todo_list_t* todo_list, const int32_t priority, const char* task);
bool complete_todo(todo_list_t* todo_list);
const char* peek_or_null(const todo_list_t* todo_list);
size_t get_count(const todo_list_t* todo_list);
bool is_empty(const todo_list_t* todo_list);
void print_todo_list(const todo_list_t* todo_list);

void swap_node(todo_list_t* todo_list, size_t idx1, size_t idx2);
size_t get_parent_index(size_t idx);
size_t get_lchild_index(size_t idx);
size_t get_rchild_index(size_t idx);
size_t get_high_priority_child_index(todo_list_t* todo_list, size_t idx);

#endif /* TODO_LIST_H */
