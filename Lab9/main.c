#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "macros.h"

void validate_max_test(void)
{
    float f_max;
    float f1 = -2.1f;
    float f2 = -1.85f;

    short s_max;
    short s1 = 2;
    short s2 = 3;

    char c_max;
    char c1 = 'a';
    char c2 = 'b';

    int mix_max;
    int i1 = -5;
    int i2 = 18;

    f_max = MAX(f1, f2);
    printf("max = %f\n", f_max);

    s_max = MAX(s1, s2);
    printf("max = %d\n", s_max);

    c_max = MAX(c1, c2);
    printf("max = %c\n", c_max);

    mix_max = MAX(c1, f1);
    printf("max = %d\n", mix_max);

    mix_max = MAX(i1, i2);
    printf("max = %d\n", mix_max);

    assert(10 == MAX(10, 5));
    assert(10.0f == MAX(10.0f, 5.0f));
    assert(10.0 == MAX(10.0, 5.0f));
    assert(1.1 == MAX(1.1, 1));

    printf("------------------------------------- MAX() clear --------------------------------------\n\n");
}

void validate_min_test(void)
{
    float f_min;
    float f1 = -2.1f;
    float f2 = -1.85f;

    short s_min;
    short s1 = 2;
    short s2 = 3;

    char c_min;
    char c1 = 'a';
    char c2 = 'b';

    unsigned int ui1 = 3;
    int i1 = -3;

    int mix_min;

    f_min = MIN(f1, f2);
    printf("min = %f\n", f_min);

    s_min = MIN(s1, s2);
    printf("min = %d\n", s_min);

    c_min = MIN(c1, c2);
    printf("min = %c\n", c_min);

    mix_min = MIN(ui1, f1);
    printf("min = %d\n", mix_min);

    mix_min = MIN(i1, s1);
    printf("min = %d\n", mix_min);

    mix_min = MIN(f2, c2);
    printf("min = %d\n", mix_min);

    assert(5 == MIN(10.f, 5.0f));
    assert(5.0f == MIN(10.0, 5.0f));

    printf("------------------------------------- MIN() clear --------------------------------------\n\n");
}

void validate_abs_test(void)
{
    float f_abs;
    float f1 = -2.1f;
    float f2 = -1.85f;

    short s_abs;
    short s1 = -2;

    char c_abs;
    char c1 = 'a';
    
    f_abs = ABS(f1);
    printf("abs = %f\n", f_abs);

    f_abs = ABS(f2);
    printf("abs = %f\n", f_abs);

    s_abs = ABS(s1);
    printf("abs = %d\n", s_abs);

    c_abs = ABS(c1);
    printf("abs = %d\n", c_abs);

    assert(1 == ABS(1));
    assert(1 == ABS(-1));
    assert(1.0f == ABS(1.0f));
    assert(1.0f == ABS(-1.0f));
    assert(1.0 == ABS(1.0));
    assert(1.0 == ABS(-1.0));

    printf("------------------------------------- ABS() clear --------------------------------------\n\n");
}

void validate_range_test(void)
{  
    int i = 0;
    int arr[20];
    int* curr;

    curr = arr;

    printf("[range]: ");
    RANGE(i, i < 0, 4) {
        printf("%d, ", i);
    }
    printf("\n");

    printf("------------------------------------- RANGE() clear --------------------------------------\n\n");
}

void validate_range_desc_test(void)
{
    int i = 0;
    int arr[20];
    int* curr;

    curr = arr;

    printf("[range_desc]: ");
    RANGE_DESC(i, 3 & 6, -4.0f) {
        printf("%d, ", i);
    }
    printf("\n");

    printf("------------------------------------- RANGE_DESC() clear --------------------------------------\n\n");
}

void validate_set_tset(void)
{
    int i = 0;
    int arr[20];
    float f_arr[10];
    int* pa_arr = NULL;
    
    SET(arr, 0, 20, 0);
    for (i = 0; i < 20; i++) {
        assert(arr[i] == 0);
    }
    SET(arr, 5, 5, 100);
    for (i = 0; i < 5; i++) {
        assert(arr[i] == 0);
    }
    for (i = 5; i < 10; i++) {
        assert(arr[i] == 100);
    }
    for (i = 10; i < 20; i++) {
        assert(arr[i] == 0);
    }
    

    SET(arr, 0, 5, 1 << 2);
    printf("[set]: ");
    for (i = 0; i < 20; i++) {
        printf("%d, ", arr[i]);
    }
    printf("\n");

    SET(arr, 0, 0, 1 << 2);
    printf("[set]: ");
    for (i = 0; i < 20; i++) {
        printf("%d, ", arr[i]);
    }
    printf("\n");

    SET(f_arr, 5, 2, 5.2f);
    printf("[set]: ");
    for (i = 0; i < 10; i++) {
        printf("%f, ", f_arr[i]);
    }
    printf("\n");   

    pa_arr = (int*)malloc(sizeof(int) * 10);
    
    SET(pa_arr, 0, 5, 10);
    printf("[set]: ");
    for (i = 0; i < 10; i++) {
        printf("%d, ", pa_arr[i]);
    }
    printf("\n");

    free(pa_arr);

    SET(arr, 0, 10, 0);
    SET(arr, 1, 1, 10);
    SET(arr, 3, 4, 25);
    printf("[set]: ");
    for (i = 0; i < 10; i++) {
        printf("%d, ", arr[i]);
    }
    printf("\n");

    printf("------------------------------------- SET() clear --------------------------------------\n\n");
}

int main(void)
{
    validate_max_test();
    validate_min_test();
    validate_abs_test();
    validate_range_test();
    validate_range_desc_test();
    validate_set_tset();

    return 0;
}
