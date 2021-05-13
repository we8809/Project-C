#include "pomoku.h"

#include <assert.h>
#include <stdio.h>

/* 수평 검사 & 수평 점수 검사 & 누적 여부 검사*/
void test_horizontal_validation(void)
{

}

/* 수직 검사 & 수직 점수 검사 & 누적 여부 검사 */
void test_vertical_validation(void)
{

}

/* 대각선 검사 & 대각선 점수 검사 & 누적 여부 검사 */
void test_diagonal_validation(void)
{

}

/* 수평 & 수직 collaboration 시 점수 검사 */
void test_horizontal_and_vertical_collab_valitaion(void)
{

}

/* 수직 & 대각 collaboration 시 점수 검사 */
void test_vertical_and_diagonal_collab_validation(void)
{

}

/* 수평 & 대각 collaboration 시 점수 검사 */
void test_horizontal_and_diagonal_collab_validation(void)
{

}

/* 수평 & 수직 & 대각 collaboration 시 점수 검사 */
void test_all_collab_validation(void)
{

}

/* init_game을 하지 않았는 데도 game을 진행 시 진행 여부 검사 */
void test_no_init_game_validation(void)
{

}

/* 삭제 시 유효성 */
void test_remove_validation(void)
{

}

/* 삽입 시 유효성 */
void test_insert_validation(void)
{

}

/* 스왑 시 유효성 */
void test_swap_valiation(void)
{

}

/* 복사 시 유효성 */
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
