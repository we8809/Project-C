#include <assert.h>
#include <limits.h>

#include "array.h"

int get_index_of(const int numbers[], const size_t element_count, const int num)
{
    size_t index;

    if (element_count == 0 || numbers[0] == INT_MIN) {
        return -1;
    }

    for (index = 0; index < element_count; index++) {
        assert(numbers[index] != INT_MIN);
        assert(index <= INT_MAX);
	    
        if (numbers[index] == num) {
            return (int)index;
        }
    }
    
    return -1;
}

int get_last_index_of(const int numbers[], const size_t element_count, const int num)
{
    size_t index;
    int index_last = -1;

    if (element_count == 0 || numbers[0] == INT_MIN) {
        return -1;
    }

    for (index = 0; index < element_count; index++) {
        assert(numbers[index] != INT_MIN);
        assert(index <= INT_MAX);

        if (numbers[index] == num) {
            index_last = (int)index;
        }
    }

    return index_last;
}

int get_max_index(const int numbers[], const size_t element_count)
{
    size_t index;
    int max_index = 0;
    int max_value = numbers[0];

    if (element_count == 0 || numbers[0] == INT_MIN) {
        return -1;
    }

    for (index = 1; index < element_count; index++) {
        assert(numbers[index] != INT_MIN);
        assert(index <= INT_MAX);

        if (max_value < numbers[index]) {
            max_value = numbers[index];
            max_index = (int)index;
        }
    }

    return max_index;
}

int get_min_index(const int numbers[], const size_t element_count)
{
    size_t index;
    int min_index = 0;
    int min_value = numbers[0];

    if (element_count == 0 || numbers[0] == INT_MIN) {
        return -1;
    }

    for (index = 1; index < element_count; index++) {
        assert(numbers[index] != INT_MIN);
        assert(index <= INT_MAX);
		
        if (min_value > numbers[index]) {
            min_value = numbers[index];
            min_index = (int)index;
        }
    }

    return min_index;
}

int is_all_positive(const int numbers[], const size_t element_count)
{
    size_t index;

    if (element_count == 0 || numbers[0] == INT_MIN) {
        return FALSE;
    }

    for (index = 0; index < element_count; index++) {
        assert(numbers[index] != INT_MIN);
        assert(index <= INT_MAX);
		
        if (numbers[index] < 0) {
            return FALSE;
        }
    }

    return TRUE;
}

int has_even(const int numbers[], const size_t element_count)
{
    size_t index;

    if (element_count == 0 || numbers[0] == INT_MIN) {
        return FALSE;
    }

    for (index = 0; index < element_count; index++) {
        assert(numbers[index] != INT_MIN);
        
        if (numbers[index] % 2 == 0) {
            return TRUE;
        }
    }

    return FALSE;
}

int insert(int numbers[], const size_t element_count, const int num, const size_t pos)
{
    size_t index;
	
    if (element_count == 0) {
        return FALSE;
    } 

    if (pos > element_count) {
        return FALSE;
    }

    for (index = element_count; index > pos; index--) {
        numbers[index] = numbers[index - 1];
    }

    numbers[pos] = num;

    return TRUE;
}

int remove_at(int numbers[], const size_t element_count, const size_t index)
{
    size_t count;

    if (element_count == 0 || numbers[0] == INT_MIN) {
        return FALSE;
    }

    if (index >= element_count) {		
        return FALSE;
    }

    for (count = index; count < element_count; count++) {
        numbers[count] = numbers[count + 1];
    }

    return TRUE;
}
