#include <assert.h>
#include <limits.h>
#include <stdio.h>

#include "array.h"

void test_case_1(void);
void get_index_of_test(void);
void get_last_index_of_test(void);
void get_max_index_test(void);
void get_min_index_test(void);
void is_all_positive_test(void);
void has_even_test(void);
void insert_test(void);
void remove_at_test(void);

static int s_numbers[20] = { -1, 4, -5, 3, 10, 7, 4, 6, 5, -10, INT_MIN, INT_MIN, INT_MIN, INT_MIN, INT_MIN, INT_MIN, INT_MIN, INT_MIN, INT_MIN, INT_MIN };

static void test_remove_at(void);
static void test_insert(void);

int main(void)
{
    get_index_of_test();
    get_last_index_of_test();
    get_max_index_test();
    get_min_index_test();
    is_all_positive_test();
    has_even_test();
    insert_test();
    remove_at_test();

    assert(get_index_of(s_numbers, 10, 4) == 1);
    assert(get_last_index_of(s_numbers, 10, 4) == 6);
    assert(get_max_index(s_numbers, 10) == 4);
    assert(get_min_index(s_numbers, 10) == 9);

    test_insert();
    test_remove_at();

    return 0;
}

void get_index_of_test(void)
{
    int ary[7] = { 1, 4, 5, 6, 6, INT_MIN, INT_MIN };
    assert(3 == get_index_of(ary, 5, 6));
    assert(-1 == get_index_of(ary, 5, 2));
    /* assert(-1 == get_index_of(ary, 7, INT_MIN)); */
}

void get_last_index_of_test(void)
{
    int ary[7] = { 1, 4, 6, 6, 5, INT_MIN, INT_MIN };
    assert(3 == get_last_index_of(ary, 5, 6));
    assert(-1 == get_last_index_of(ary, 5, 2));
}

void get_max_index_test(void)
{
    int ary1[7] = { 1, 4, 6, 6, 5, INT_MIN, INT_MIN };
    int ary2[1] = { 1 };
    int ary3[3] = { INT_MIN, INT_MIN, INT_MIN };
    assert(2 == get_max_index(ary1, 5));
    assert(-1 == get_max_index(ary2, 0));
    assert(-1 == get_max_index(ary3, 3));
}

void get_min_index_test(void)
{
    int ary1[7] = { 1, 4, -6, -6, 5, INT_MIN, INT_MIN };
    int ary2[1] = { 1 };
    int ary3[3] = { INT_MAX, INT_MAX, INT_MAX };
    assert(2 == get_min_index(ary1, 5));
    assert(-1 == get_min_index(ary2, 0));
    /* printf("value: %d\n", get_min_index(ary3, 3)); */
    assert(0 == get_min_index(ary3, 3));
}

void is_all_positive_test(void)
{
    int ary1[7] = { 1, 4, -6, -6, 5, INT_MIN, INT_MIN };
    int ary2[7] = { 1, 4, 6, 6, 5, INT_MIN, INT_MIN };
   
    assert(FALSE == is_all_positive(ary1, 5));
    assert(TRUE == is_all_positive(ary2, 5));
}

void has_even_test()
{
    int ary1[7] = { 1, 3, -5, -11, -1, INT_MIN, INT_MIN };
    int ary2[7] = { 1, 4, 6, 6, 5, INT_MIN, INT_MIN };

    assert(FALSE == has_even(ary1, 5));
    assert(TRUE == has_even(ary2, 5));
}

void insert_test()
{
    int ary[8] = { 1, 4, -6, -6, 5, INT_MIN, INT_MIN, INT_MIN };
    
    assert(TRUE == insert(ary, 5, 10, 3)); /* return value: 1, ary: [ 1, 4, -6, 10, -6, 5, INT_MIN, INT_MIN ] */
    assert(1 == ary[0]);
    assert(4 == ary[1]);    
    assert(-6 == ary[2]);
    assert(10 == ary[3]);
    assert(-6 == ary[4]);
    assert(5 == ary[5]);
    assert(INT_MIN == ary[6]);
    assert(INT_MIN == ary[7]);

    assert(FALSE == insert(ary, 6, 10, 7)); /* return value: 0 */
    assert(1 == ary[0]);
    assert(4 == ary[1]);    
    assert(-6 == ary[2]);
    assert(10 == ary[3]);
    assert(-6 == ary[4]);
    assert(5 == ary[5]);
    assert(INT_MIN == ary[6]);
    assert(INT_MIN == ary[7]);

    assert(TRUE == insert(ary, 6, 11, 6)); /* return value: 1, ary: [ 1, 4, -6, 10, -6, 5, 11, INT_MIN ] */
    assert(1 == ary[0]);
    assert(4 == ary[1]);
    assert(-6 == ary[2]);
    assert(10 == ary[3]);
    assert(-6 == ary[4]);
    assert(5 == ary[5]);
    assert(11 == ary[6]);
    assert(INT_MIN == ary[7]);
}

void remove_at_test(void)
{
    int ary[7] = { 1, 4, -6, -6, 5, INT_MIN ,INT_MIN };
    assert(TRUE == remove_at(ary, 5, 1)); /* return value: 1, result1: [ 1, -6, -6, 5, INT_MIN, INT_MIN, INT_MIN ] */
    assert(1 == ary[0]);
    assert(-6 == ary[1]);
    assert(-6 == ary[2]);
    assert(5 == ary[3]);
    assert(INT_MIN == ary[4]);
    assert(INT_MIN == ary[5]);
    assert(INT_MIN == ary[6]);
    
    assert(FALSE == remove_at(ary, 4, 5)); /* return value: 0 */
    assert(1 == ary[0]);
    assert(-6 == ary[1]);
    assert(-6 == ary[2]);
    assert(5 == ary[3]);
    assert(INT_MIN == ary[4]);
    assert(INT_MIN == ary[5]);
    assert(INT_MIN == ary[6]);
}

static void test_insert(void)
{
    size_t i;
    int expected_result[12] = { -21, -1, 4, -5, 3, 10, 7, 4, 6, 5, -10, -22 };

    assert(insert(s_numbers, 10, -21, 0) == TRUE);
    assert(insert(s_numbers, 11, -22, 11) == TRUE);

    /* 
    printf("current_array: ");
    for (i = 0; i < 12; ++i) {
        printf("%d ", s_numbers[i]);
    }
    printf("\n");
    */

    for (i = 0; i < 12; i++) {
        assert(expected_result[i] == s_numbers[i]);
    }
}

static void test_remove_at(void)
{
    size_t i;
    int expected_result[10] = { -1, 4, -5, 3, 10, 7, 4, 6, 5, -10 };

    assert(get_index_of(s_numbers, 10, -21) == 0);
    assert(get_index_of(s_numbers, 12, -22) == 11);
    assert(remove_at(s_numbers, 12, 0) == TRUE);
    assert(remove_at(s_numbers, 11, 10) == TRUE);
    assert(get_index_of(s_numbers, 10, -21) == -1);
    assert(get_index_of(s_numbers, 10, -22) == -1);

    for (i = 0; i < 10; i++) {
        assert(expected_result[i] == s_numbers[i]);
    }
}
