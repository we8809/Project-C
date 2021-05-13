#include "parentheses.h"

int compare_opening_index(const void* p0, const void* p1)
{
    const parenthesis_t* pt_0 = p0;
    const parenthesis_t* pt_1 = p1;

    if (pt_0->opening_index == pt_1->opening_index) {
        return ((int)pt_0->closing_index - (int)pt_1->closing_index);
    }

    return (int)pt_0->opening_index - (int)pt_1->opening_index;
}

size_t get_matching_parentheses(parenthesis_t* parentheses, size_t max_size, const char* str)
{
    data_t** pa_parenthesis_stack_list;
    size_t parenthesis_list_count = 0;

    size_t str_index = 0;
    int opening_index;

    size_t i;

    if (parentheses == NULL) {
        return 0;
    }

    if (max_size == 0) {
        return 0;
    }

    if (str == NULL) {
        return 0;
    }

    if (*(str) == '\0') {
        return 0;
    }

    memset(parentheses, 0, sizeof(parenthesis_t) * max_size);

    pa_parenthesis_stack_list = (data_t**)malloc(sizeof(data_t*) * 4);
    for (i = 0; i < 4; i++) {
        pa_parenthesis_stack_list[i] = NULL;
    }

    while (TRUE) {

        if (*(str) == '\0') {
            break;
        }

        if (parenthesis_list_count >= max_size) {
            break;
        }

        switch (*(str)) {
        case '{':
            insert_parentheses_stack(&pa_parenthesis_stack_list[FIRST_PARENTHESIS_TOP_INDEX], str_index);
            break;

        case '(':
            insert_parentheses_stack(&pa_parenthesis_stack_list[SECOND_PARENTHESIS_TOP_INDEX], str_index);
            break;

        case '[':
            insert_parentheses_stack(&pa_parenthesis_stack_list[THIRD_PARENTHESIS_TOP_INDEX], str_index);
            break;

        case '<':
            insert_parentheses_stack(&pa_parenthesis_stack_list[FOURTH_PARENTHESIS_TOP_INDEX], str_index);
            break;

        case '}':
            opening_index = pop_parentheses_stack(&pa_parenthesis_stack_list[FIRST_PARENTHESIS_TOP_INDEX]);
            if (opening_index == -1) {
                break;
            }

            parentheses[parenthesis_list_count].opening_index = opening_index;
            parentheses[parenthesis_list_count++].closing_index = str_index;

            break;

        case ')':
            opening_index = pop_parentheses_stack(&pa_parenthesis_stack_list[SECOND_PARENTHESIS_TOP_INDEX]);
            if (opening_index == -1) {
                break;
            }

            parentheses[parenthesis_list_count].opening_index = opening_index;
            parentheses[parenthesis_list_count++].closing_index = str_index;

            break;

        case ']':
            opening_index = pop_parentheses_stack(&pa_parenthesis_stack_list[THIRD_PARENTHESIS_TOP_INDEX]);
            if (opening_index == -1) {
                break;
            }

            parentheses[parenthesis_list_count].opening_index = opening_index;
            parentheses[parenthesis_list_count++].closing_index = str_index;

            break;

        case '>':
            opening_index = pop_parentheses_stack(&pa_parenthesis_stack_list[FOURTH_PARENTHESIS_TOP_INDEX]);
            if (opening_index == -1) {
                break;
            }

            parentheses[parenthesis_list_count].opening_index = opening_index;
            parentheses[parenthesis_list_count++].closing_index = str_index;

            break;

        default:
            break;
        }
        
        ++str;
        ++str_index;
    }

    qsort(parentheses, parenthesis_list_count, sizeof(data_t), compare_opening_index);

    dispose_all_parenthses_stack(&pa_parenthesis_stack_list[FIRST_PARENTHESIS_TOP_INDEX]);
    dispose_all_parenthses_stack(&pa_parenthesis_stack_list[SECOND_PARENTHESIS_TOP_INDEX]);
    dispose_all_parenthses_stack(&pa_parenthesis_stack_list[THIRD_PARENTHESIS_TOP_INDEX]);
    dispose_all_parenthses_stack(&pa_parenthesis_stack_list[FOURTH_PARENTHESIS_TOP_INDEX]);

    free(pa_parenthesis_stack_list);

    return parenthesis_list_count;
}

size_t is_empty_parentheses_stack(data_t** head)
{
    if (*head == NULL) {
        return TRUE;
    }

    return FALSE;
}

void insert_parentheses_stack(data_t** head, size_t start_index)
{
    data_t* new_node; 

    new_node = (data_t*)malloc(sizeof(data_t));
    new_node->opening_index = start_index;

    new_node->next = *head;
    *head = new_node;
}

int pop_parentheses_stack(data_t** head)
{
    int start_index;
    data_t* remove_node;

    if (is_empty_parentheses_stack(head) == TRUE) {
        return -1;
    }

    remove_node = *head;
    start_index = remove_node->opening_index;
    
    *head = remove_node->next;

    free(remove_node);

    return start_index;
}


void dispose_all_parenthses_stack(data_t** head)
{
    int ret; 

    if (is_empty_parentheses_stack(head) == TRUE) {
        return;
    }

    ret = pop_parentheses_stack(head);
    while (TRUE) {

        if (ret == -1) {
            break;
        }

        ret = pop_parentheses_stack(head);
    }

    *head = NULL;
}
