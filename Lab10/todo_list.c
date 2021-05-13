#include "todo_list.h"

#include <limits.h>

todo_list_t init_todo_list(size_t max_size)
{
    size_t index;
    todo_list_t todo_list;

    memset(&todo_list, 0, sizeof(todo_list_t));

    if (max_size == 0) {
        return todo_list;
    }

    todo_list.max_size = max_size;
    todo_list.cur_size = 0;

    todo_list.priority_list = (int32_t*)malloc(sizeof(int32_t) * max_size);
    todo_list.task_list = (char**)malloc(sizeof(char*) * max_size);

    for (index = 0; index < max_size; index++) {
        todo_list.priority_list[index] = 0;
        todo_list.task_list[index] = NULL;
    }

    return todo_list;
} 

void finalize_todo_list(todo_list_t* todo_list)
{
    size_t index;

    if (todo_list == NULL) {
        return;
    }

    if (todo_list->max_size == 0) {
        return;
    }

    for (index = 0; index < (todo_list->cur_size); index++) {
        free(todo_list->task_list[index]);
    }

    free(todo_list->priority_list);
    free(todo_list->task_list);
}

bool add_todo(todo_list_t* todo_list, const int32_t priority, const char* task)
{
    size_t index;
    size_t cur_index;
    size_t swap_index;
    size_t task_length;

    if (todo_list == NULL) {
        return false;
    }

    if ((todo_list->cur_size) >= (todo_list->max_size)) {
        return false;
    }

    if (task == NULL) {
        return false;
    }

    if (*task == '\0') {
        return false;
    }

    /* data initialization */
    cur_index = (todo_list->cur_size);
    swap_index = cur_index;
    task_length = strlen(task) + 1;

    todo_list->priority_list[cur_index] = priority;
    todo_list->task_list[cur_index] = (char*)malloc(sizeof(char) * task_length);
    snprintf(todo_list->task_list[cur_index], task_length, "%s", task);

    if (cur_index == 0) {
        
        goto complete_add;
    }

    for (index = (cur_index - 1); index >= 0; index--) {

        if (priority <= todo_list->priority_list[index]) {
            break;
        }

        swap_node(todo_list, swap_index, index);
        swap_index = index;

        if (index == 0) {
            break;
        }
    }

complete_add:
    ++(todo_list->cur_size);
    return true;
}

bool complete_todo(todo_list_t* todo_list)
{
    size_t index;
    char* remove_task;

    if (todo_list == NULL) {
        return false;
    }

    if (is_empty(todo_list) == true) {
        return false;
    }

    remove_task = todo_list->task_list[0];

    for (index = 0; index < (todo_list->cur_size); index++) {
        
        if (index == (todo_list->cur_size - 1)) {
            break;
        }

        swap_node(todo_list, index, index + 1);
    }

    todo_list->priority_list[todo_list->cur_size - 1] = 0;
    free(remove_task);
    todo_list->task_list[todo_list->cur_size - 1] = NULL;

    --(todo_list->cur_size);
    return true;
}

const char* peek_or_null(const todo_list_t* todo_list)
{
    if (todo_list == NULL) {
        return NULL;
    }

    if (is_empty(todo_list) == true) {
        return NULL;
    }

    return (const char*)(todo_list->task_list[0]);
}

size_t get_count(const todo_list_t* todo_list)
{
    if (todo_list == NULL) {
        return 0;
    }

    return (todo_list->cur_size);
}

bool is_empty(const todo_list_t* todo_list)
{
    if (todo_list == NULL) {
        return false;
    }

    if (todo_list->cur_size == 0) {
        return true;
    }

    return false;
}

void print_todo_list(const todo_list_t* todo_list)
{
    size_t index;

    if (todo_list->max_size == 0) {
        return;
    }

    if (todo_list->cur_size == 0) {
        return;
    }

    for (index = 0; index < (todo_list->cur_size); index++) {
        printf("[priority]: %d / [task]: %s\n", todo_list->priority_list[index], todo_list->task_list[index]);
    }
    printf("---------------------------------------------------------------------\n\n");
}

void swap_node(todo_list_t* todo_list, size_t idx1, size_t idx2)
{
    int32_t temp_priority;
    char* temp_task;

    temp_priority = todo_list->priority_list[idx1];
    temp_task = todo_list->task_list[idx1];

    todo_list->priority_list[idx1] = todo_list->priority_list[idx2];
    todo_list->task_list[idx1] = todo_list->task_list[idx2];
    todo_list->priority_list[idx2] = temp_priority;
    todo_list->task_list[idx2] = temp_task;
}
