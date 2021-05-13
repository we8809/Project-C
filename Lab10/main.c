#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "todo_list.h"

void validate_default_test(void)
{
    todo_list_t todo_list = init_todo_list(9);
    assert(is_empty(&todo_list));
    assert(add_todo(&todo_list, 10, "A"));

    print_todo_list(&todo_list);
    assert(add_todo(&todo_list, 22, "B"));

    print_todo_list(&todo_list);
    assert(add_todo(&todo_list, 3, "C"));
    assert(add_todo(&todo_list, 34, "D"));
    assert(add_todo(&todo_list, 20, "E"));
    assert(add_todo(&todo_list, 59, "F"));
    assert(add_todo(&todo_list, 60, "G"));

    printf("hihi1\n");
    print_todo_list(&todo_list);
    assert(add_todo(&todo_list, 55, "H"));

    printf("hihi2\n");
    print_todo_list(&todo_list);
    assert(add_todo(&todo_list, 22, "I"));
    assert(get_count(&todo_list) == 9);
    assert(!is_empty(&todo_list));
    assert(!add_todo(&todo_list, -5, "J"));
    assert(get_count(&todo_list) == 9);

    print_todo_list(&todo_list);
    assert(strcmp("G", peek_or_null(&todo_list)) == 0);
    assert(complete_todo(&todo_list));
    assert(strcmp("F", peek_or_null(&todo_list)) == 0);
    assert(complete_todo(&todo_list));
    assert(strcmp("H", peek_or_null(&todo_list)) == 0);
    assert(complete_todo(&todo_list));
    assert(strcmp("D", peek_or_null(&todo_list)) == 0);
    assert(complete_todo(&todo_list));
    assert(strcmp("B", peek_or_null(&todo_list)) == 0);
    assert(complete_todo(&todo_list));
    assert(strcmp("I", peek_or_null(&todo_list)) == 0);
    assert(complete_todo(&todo_list));
    assert(strcmp("E", peek_or_null(&todo_list)) == 0);
    assert(complete_todo(&todo_list));
    assert(strcmp("A", peek_or_null(&todo_list)) == 0);
    assert(complete_todo(&todo_list));
    assert(strcmp("C", peek_or_null(&todo_list)) == 0);
    assert(complete_todo(&todo_list));
    assert(is_empty(&todo_list));
    assert(!complete_todo(&todo_list));
    finalize_todo_list(&todo_list);

    printf("validate_default_test() clear\n\n");
}

void validate_get_count_more_than_max_size_test(void)
{
    /* 
        todo-list가 가득찼을 때, add_todo()함수가 실패하는지 확인
        todo-list에 task추가가 실패했을 때, get_count()함수의 반환값이 바뀌지 않는지 확인
    */

    todo_list_t todo_list;
    todo_list = init_todo_list(3);

    assert(false == complete_todo(&todo_list));

    assert(true == add_todo(&todo_list, 10, "dong-hyun"));
    assert(true == add_todo(&todo_list, 10, "seung-hwan"));
    assert(true == add_todo(&todo_list, 10, "jong-gyung"));

    assert(3 == get_count(&todo_list));

    assert(false == add_todo(&todo_list, 10, "no-person"));

    assert(3 == get_count(&todo_list));

    finalize_todo_list(&todo_list);

    printf("validate_get_count_more_than_max_size_test() clear\n\n");
}

void validate_complete_todo_is_empty(void)
{
    todo_list_t todo_list;
    todo_list = init_todo_list(3);

    assert(false == complete_todo(&todo_list));

    assert(true == add_todo(&todo_list, 10, "dong-hyun"));
    assert(true == add_todo(&todo_list, 10, "seung-hwan"));
    assert(true == add_todo(&todo_list, 10, "jong-gyung"));
    
    assert(0 == strcmp(peek_or_null(&todo_list), "dong-hyun"));
    assert(true == complete_todo(&todo_list));

    print_todo_list(&todo_list);

    assert(0 == strcmp(peek_or_null(&todo_list), "seung-hwan"));
    assert(true == complete_todo(&todo_list));

    assert(0 == strcmp(peek_or_null(&todo_list), "jong-gyung"));
    assert(true == complete_todo(&todo_list));

    assert(is_empty(&todo_list) == true);
    assert(peek_or_null(&todo_list) == NULL);

    finalize_todo_list(&todo_list);

    printf("validate_complete_todo_is_empty() clear\n\n");
}

void validate_even_size_test(void)
{
    todo_list_t todo_list;
    todo_list = init_todo_list(8);

    assert(false == complete_todo(&todo_list));

    add_todo(&todo_list, 1, "A");
    add_todo(&todo_list, 1, "B");
    add_todo(&todo_list, 1, "C");
    add_todo(&todo_list, 1, "D");
    add_todo(&todo_list, 1, "E");
    add_todo(&todo_list, 1, "F");
    add_todo(&todo_list, 1, "G");
    add_todo(&todo_list, 1, "H");

    printf("start\n");
    print_todo_list(&todo_list);
    assert(strcmp(peek_or_null(&todo_list), "A") == 0);
    assert(true == complete_todo(&todo_list));

    print_todo_list(&todo_list);
    assert(strcmp(peek_or_null(&todo_list), "B") == 0);
    assert(true == complete_todo(&todo_list));

    print_todo_list(&todo_list);
    assert(strcmp(peek_or_null(&todo_list), "C") == 0);
    assert(true == complete_todo(&todo_list));

    print_todo_list(&todo_list);
    assert(strcmp(peek_or_null(&todo_list), "D") == 0);
    assert(true == complete_todo(&todo_list));

    print_todo_list(&todo_list);
    assert(strcmp(peek_or_null(&todo_list), "E") == 0);
    assert(true == complete_todo(&todo_list));

    assert(strcmp(peek_or_null(&todo_list), "F") == 0);
    assert(true == complete_todo(&todo_list));

    assert(strcmp(peek_or_null(&todo_list), "G") == 0);
    assert(true == complete_todo(&todo_list));

    assert(strcmp(peek_or_null(&todo_list), "H") == 0);
    assert(true == complete_todo(&todo_list));

    assert(complete_todo(&todo_list) == false);
    assert(is_empty(&todo_list) == true);
    assert(peek_or_null(&todo_list) == NULL);

    finalize_todo_list(&todo_list);

    printf("validate_even_size_test() clear\n\n");
}

void validate_empty_test(void)
{
    todo_list_t todo_list;
    todo_list = init_todo_list(0);

    assert(add_todo(&todo_list, 1, "hi") == false);
    print_todo_list(&todo_list);

    assert(complete_todo(&todo_list) == false);
    assert(peek_or_null(&todo_list) == NULL);
    assert(get_count(&todo_list) == 0);

    finalize_todo_list(&todo_list);

    printf("validate_empty_test() clear\n\n");
}

void validate_one_node_test(void)
{
    todo_list_t todo_list;
    todo_list = init_todo_list(1);

    assert(add_todo(&todo_list, 1, "hi") == true);
    assert(get_count(&todo_list) == 1);

    assert(strcmp(peek_or_null(&todo_list), "hi") == 0);
    assert(complete_todo(&todo_list) == true);

    assert(get_count(&todo_list) == 0);
    assert(is_empty(&todo_list) == true);

    finalize_todo_list(&todo_list);

    printf("validate_one_node_test() clear\n\n");
}


int main(void)
{
    validate_even_size_test();

    
    validate_one_node_test();
    validate_empty_test();
    validate_complete_todo_is_empty();
    validate_get_count_more_than_max_size_test();
    validate_default_test();
    

    return 0;
}
