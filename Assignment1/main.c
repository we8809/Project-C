#include "pomoku.h"

#include <assert.h>
#include <stdio.h>

/* ���� �˻� & ���� ���� �˻� & ���� ���� �˻�*/
void test_horizontal_validation(void)
{

}

/* ���� �˻� & ���� ���� �˻� & ���� ���� �˻� */
void test_vertical_validation(void)
{

}

/* �밢�� �˻� & �밢�� ���� �˻� & ���� ���� �˻� */
void test_diagonal_validation(void)
{

}

/* ���� & ���� collaboration �� ���� �˻� */
void test_horizontal_and_vertical_collab_valitaion(void)
{

}

/* ���� & �밢 collaboration �� ���� �˻� */
void test_vertical_and_diagonal_collab_validation(void)
{

}

/* ���� & �밢 collaboration �� ���� �˻� */
void test_horizontal_and_diagonal_collab_validation(void)
{

}

/* ���� & ���� & �밢 collaboration �� ���� �˻� */
void test_all_collab_validation(void)
{

}

/* init_game�� ���� �ʾҴ� ���� game�� ���� �� ���� ���� �˻� */
void test_no_init_game_validation(void)
{

}

/* ���� �� ��ȿ�� */
void test_remove_validation(void)
{

}

/* ���� �� ��ȿ�� */
void test_insert_validation(void)
{

}

/* ���� �� ��ȿ�� */
void test_swap_valiation(void)
{

}

/* ���� �� ��ȿ�� */
void test_copy_validation(void)
{

}



int main(void)
{
    size_t index_row;
    size_t index_col;

    init_game();

    for (index_row = 0; index_row < 15; index_row++) {
        for (index_col = 0; index_col < 15; index_col++) {
            if (index_row % 2 == 0) {
                place_stone(COLOR_BLACK, index_row, (index_col * 2));
            } else {
                place_stone(COLOR_BLACK, index_row, (index_col * 2) + 1);
            }
                
        }
    }

    /*
    print_board();
    */

    assert(remove_column(COLOR_BLACK, 5) == TRUE);
    
    /*
    print_board();
    */
    
    printf("current row = %d\n", get_row_count());
    printf("cuurent col = %d\n", get_column_count());

    return 0;
}
