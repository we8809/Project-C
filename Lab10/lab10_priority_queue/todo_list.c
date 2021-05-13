#include "todo_list.h"

#include <limits.h>

todo_list_t init_todo_list(size_t max_size)
{
    size_t size;
    todo_list_t todo_list;

    memset(&todo_list, 0, sizeof(todo_list_t));

    if (max_size == 0) {
        return todo_list;
    }

    todo_list.max_size = max_size;
    todo_list.cur_size = 0;

    todo_list.priority_list = (int32_t*)malloc(sizeof(int32_t) * max_size);
    todo_list.task_list = (char**)malloc(sizeof(char*) * max_size);

    for (size = 0; size < max_size; size++) {
        todo_list.priority_list[size] = 0;
        todo_list.task_list[size] = NULL;
    }

    return todo_list;
} 

void finalize_todo_list(todo_list_t* todo_list)
{
    size_t size;

    if (todo_list == NULL) {
        return;
    }

    if (todo_list->max_size == 0) {
        return;
    }

    for (size = 0; size < (todo_list->cur_size); size++) {
        free(todo_list->task_list[size]);
    }

    free(todo_list->priority_list);
    free(todo_list->task_list);
}

bool add_todo(todo_list_t* todo_list, const int32_t priority, const char* task)
{
    size_t cur_index;
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
    cur_index = todo_list->cur_size;
    task_length = strlen(task) + 1;

    todo_list->priority_list[cur_index] = priority;
    todo_list->task_list[cur_index] = (char*)malloc(sizeof(char) * task_length);
    snprintf(todo_list->task_list[cur_index], task_length, "%s", task);

    while (true) {

        if (cur_index == 0) {
            break;
        }

        if (todo_list->priority_list[get_parent_index(cur_index)] < priority) {

            swap_node(todo_list, get_parent_index(cur_index), cur_index);
            cur_index = get_parent_index(cur_index);

        } else {
            
            break;
        }
    }

    ++(todo_list->cur_size);
    return true;
}

bool complete_todo(todo_list_t* todo_list)
{
    size_t index;
    size_t child_index;
    size_t parent_index = 0;

    int32_t last_priority;
    char* last_task;
    char* remove_task;

    if (todo_list == NULL) {
        return false;
    }

    if (is_empty(todo_list) == true) {
        return false;
    }

    remove_task = todo_list->task_list[0];
    last_priority = todo_list->priority_list[todo_list->cur_size - 1];
    last_task = todo_list->task_list[todo_list->cur_size - 1];

    while (true) {

        child_index = get_high_priority_child_index(todo_list, parent_index);

        if (child_index == UINT32_MAX) {
            break;
        }

        if (last_priority > todo_list->priority_list[child_index]) {
            break;
        }

        swap_node(todo_list, parent_index, child_index);
        parent_index = child_index;
    }

    todo_list->priority_list[parent_index] = last_priority;
    todo_list->task_list[parent_index] = last_task;
    
    /* reset */
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
    size_t size;

    if (todo_list->max_size == 0) {
        return;
    }

    if (todo_list->cur_size == 0) {
        return;
    }

    for (size = 0; size < (todo_list->cur_size); size++) {
        printf("[priority]: %d / [task]: %s\n", todo_list->priority_list[size], todo_list->task_list[size]);
    }
    printf("---------------------------------------------------------------------\n\n");
}

size_t get_parent_index(size_t idx)
{
    if (idx % 2 == 0) {
        return (idx / 2 - 1);
    }

    return (idx / 2);
}

size_t get_lchild_index(size_t idx)
{
    return (idx * 2 + 1);
}

size_t get_rchild_index(size_t idx)
{
    return (idx * 2 + 2);
}

size_t get_high_priority_child_index(todo_list_t* todo_list, size_t idx)
{
    size_t lchild_index = get_lchild_index(idx);
    size_t rchild_index = get_rchild_index(idx);

    if (lchild_index > (todo_list->cur_size - 1)) {

        return UINT32_MAX;

    } else if (lchild_index == (todo_list->cur_size - 1)) {

        return lchild_index;

    } else {

        if (todo_list->priority_list[lchild_index] >= todo_list->priority_list[rchild_index]) {
        
            return lchild_index;
        
        } else {
        
            return rchild_index;
        }
    }
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
