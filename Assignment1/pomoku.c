#include "pomoku.h"

#include <limits.h>
#include <stdio.h>

/* Is game on? -> flag */

/* game board array! */
static signed char s_game_board[MAXIMUM_BOARD_ROW_SIZE][MAXIMUM_BOARD_COLUMN_SIZE] = { 0, };

static size_t s_cur_row = INITIAL_BOARD_ROW_SIZE;
static size_t s_cur_column = INITIAL_BOARD_COLUMN_SIZE;

static size_t s_black_score = 0;
static size_t s_white_score = 0;

static size_t get_horizontal_score(const size_t row, const size_t col, const color_t color);
static size_t get_vertical_score(const size_t row, const size_t col, const color_t color);
static size_t get_left_diagonal_score(const size_t row, const size_t col, const color_t color);
static size_t get_right_diagonal_score(const size_t row, const size_t col, const color_t color);

void init_game(void)
{
    size_t index_row;
    size_t index_col;
    
    for (index_row = 0; index_row < MAXIMUM_BOARD_ROW_SIZE; index_row++) {
        for (index_col = 0; index_col < MAXIMUM_BOARD_COLUMN_SIZE; index_col++) {
            s_game_board[index_row][index_col] = COLOR_EMPTY;
        }
    }

    /* set board's current row & column */
    s_cur_row = INITIAL_BOARD_ROW_SIZE;
    s_cur_column = INITIAL_BOARD_COLUMN_SIZE;
	
    /* set player's score */
    s_black_score = 0;
    s_white_score = 0;
}

size_t get_row_count(void)
{
    return s_cur_row;
}

size_t get_column_count(void)
{
    return s_cur_column;
}

int get_score(const color_t color)
{
    int score = 0;

    switch (color) {
    case COLOR_BLACK:
        score = (int)s_black_score;
        break;
    case COLOR_WHITE:
        score = (int)s_white_score;
        break;
    default:
        score = -1;
        break;
    }

    return score;
}

int get_color(const size_t row, const size_t col)
{
    if ((row >= MAXIMUM_BOARD_ROW_SIZE) || (col >= MAXIMUM_BOARD_COLUMN_SIZE)) {
        return -1;
    }

    if ((row >= s_cur_row) || (col >= s_cur_column)) {
        return -1;
    }

    if (s_game_board[row][col] == COLOR_BLACK) {
        return COLOR_BLACK;
    } else if (s_game_board[row][col] == COLOR_WHITE) {	
        return COLOR_WHITE;
    }

    return -1;
}

int is_placeable(const size_t row, const size_t col)
{
    /* 위치시킬 돌의 위치가 최대 보드 사이즈를 넘어가는 위치를 가리킬 경우 */
    if ((row >= MAXIMUM_BOARD_ROW_SIZE) || (col >= MAXIMUM_BOARD_COLUMN_SIZE)) {
        return FALSE;
    }

    /* 위치시킬 돌의 위치가 현재 보드 사이즈를 넘어가는 위치를 가리킬 경우 */
    if ((row >= s_cur_row) || (col >= s_cur_column)) {
        return FALSE;
    }

    /* 위치시킬 돌의 위치가 빈 공간이 아닐 경우 */
    if (s_game_board[row][col] != COLOR_EMPTY) {
        return FALSE;
    }

    return TRUE;
}

int place_stone(const color_t color, const size_t row, const size_t col)
{
    size_t score;
    
    /* 위치시킬 돌의 위치가 놓을 수 있는 위치인가? 놓지 못할 위치일 경우 돌을 놓지 못함 */
    if (is_placeable(row, col) == FALSE) {
        return FALSE;
    }

    /* 위치시킬 돌의 색깔이 흰색과 흑색 둘 다 아닐 경우 돌을 놓지 못함 */
    if ((color != COLOR_BLACK) && (color != COLOR_WHITE)) {
        return FALSE;
    }

    /* 돌을 놓게 됨 */
    s_game_board[row][col] = color;

    /* 놓은 돌의 위치를 기준으로 수평, 수직, left 대각, right 대각 검사 및 점수 도출 */
    score = get_horizontal_score(row, col, color) + get_vertical_score(row, col, color) + get_left_diagonal_score(row, col, color) + get_right_diagonal_score(row, col, color);

    if (color == COLOR_BLACK) {
        s_black_score += score;
    } else {
        s_white_score += score;
    }

    return TRUE;
}

/* special moves */
int insert_row(const color_t color, const size_t row)
{
    size_t index_row;
    size_t index_column;

    if (s_cur_row >= MAXIMUM_BOARD_ROW_SIZE) {
        return FALSE;
    }

    if (row > s_cur_row) {
        return FALSE;
    }

    /* check player's(color type) score */
    if (color == COLOR_BLACK) {
        if (s_black_score < CONSUME_INSERT_SCORE) {
            return FALSE;
        }

        s_black_score -= CONSUME_INSERT_SCORE;
    } else if (color == COLOR_WHITE) {
        if (s_white_score < CONSUME_INSERT_SCORE) {
            return FALSE;
        }

        s_white_score -= CONSUME_INSERT_SCORE;
    } else {
        return FALSE;
    }

    /* 삽입할 열의 위치가 현재 보드판의 최종 열의 인덱스보다 작은 인덱스에 삽입할 경우 */
    if (row <= (s_cur_row - 1)) {
        for (index_row = (s_cur_row - 1); index_row >= row; index_row--) {

            for (index_column = 0; index_column < s_cur_column; index_column++) {

                s_game_board[index_row + 1][index_column] = s_game_board[index_row][index_column];
            }

            if (index_row == 0) {
                break;
            }
        }
    }
    
    for (index_column = 0; index_column < s_cur_column; index_column++) {
        s_game_board[row][index_column] = COLOR_EMPTY;
    }

    s_cur_row++;
	
    return TRUE;
}

int insert_column(const color_t color, const size_t col)
{
    size_t index_row;
    size_t index_column;

    if (s_cur_column >= MAXIMUM_BOARD_COLUMN_SIZE) {
        return FALSE;
    }

    if (col > s_cur_column) {
        return FALSE;
    }

    /* check player's(color type) score */
    if (color == COLOR_BLACK) {
        if (s_black_score < CONSUME_INSERT_SCORE) {
            return FALSE;
        }

        s_black_score -= CONSUME_INSERT_SCORE;
    } else if (color == COLOR_WHITE) {
        if (s_white_score < CONSUME_INSERT_SCORE) {
            return FALSE;
        }

        s_white_score -= CONSUME_INSERT_SCORE;
    } else {
        return FALSE;
    }

    /* 삽입할 행의 위치가 현재 보드판의 최종 행의 인덱스보다 작은 인덱스에 삽입할 경우 */
    if (col <= (s_cur_column - 1)) {
        for (index_column = (s_cur_column - 1); index_column >= col; index_column--) {

            for (index_row = 0; index_row < s_cur_row; index_row++) {

                s_game_board[index_row][index_column + 1] = s_game_board[index_row][index_column];
            }

            if (index_column == 0) {
                break;
            }
        }
    }

    for (index_row = 0; index_row < s_cur_row; index_row++) {
        s_game_board[index_row][col] = COLOR_EMPTY;
    }

    s_cur_column++;

    return TRUE;
}

int remove_row(const color_t color, const size_t row)
{
    size_t index_row;
    size_t index_column;

    if (s_cur_row <= MINIMUM_BOARD_ROW_SIZE) {
        return FALSE;
    }

    if (row >= s_cur_row) {
        return FALSE;
    }

    /* check player's(color type) score */
    if (color == COLOR_BLACK) {
        if (s_black_score < CONSUME_REMOVE_SCORE) {
            return FALSE;
        }

        s_black_score -= CONSUME_REMOVE_SCORE;
    } else if (color == COLOR_WHITE) {
        if (s_white_score < CONSUME_REMOVE_SCORE) {
            return FALSE;
        }

        s_white_score -= CONSUME_REMOVE_SCORE;
    } else {
        return FALSE;
    }
	
    if (row < (s_cur_row - 1)) {
        for (index_row = row; index_row < s_cur_row; index_row++) {

            if (index_row == (s_cur_row - 1)) {
                break;
            }

            for (index_column = 0; index_column < s_cur_column; index_column++) {

                s_game_board[index_row][index_column] = s_game_board[index_row + 1][index_column];
            }
        }
    } 

    for (index_column = 0; index_column < s_cur_column; index_column++) {
        s_game_board[s_cur_row - 1][index_column] = COLOR_EMPTY;
    }

    s_cur_row--;

    return TRUE;
}

int remove_column(const color_t color, const size_t col)
{
    size_t index_row;
    size_t index_column;

    if (s_cur_column == MINIMUM_BOARD_COLUMN_SIZE) {
        return FALSE;
    }

    if (col >= s_cur_column) {
        return FALSE;
    }

    /* check player's(color type) score */
    if (color == COLOR_BLACK) {
        if (s_black_score < CONSUME_REMOVE_SCORE) {
            return FALSE;
        }

        s_black_score -= CONSUME_REMOVE_SCORE;

    } else if (color == COLOR_WHITE) {
        if (s_white_score < CONSUME_REMOVE_SCORE) {
            return FALSE;
        }

        s_white_score -= CONSUME_REMOVE_SCORE;

    } else {
        return FALSE;
    }

    if (col < (s_cur_column - 1)) {
        for (index_column = col; index_column < s_cur_column; index_column++) {
            if (index_column == (s_cur_column - 1)) {
                break;
            }

            for (index_row = 0; index_row < s_cur_row; index_row++) {

                s_game_board[index_row][index_column] = s_game_board[index_row][index_column + 1];
            }
        }
    }
	
    for (index_row = 0; index_row < s_cur_row; index_row++) {
        s_game_board[index_row][s_cur_column - 1] = COLOR_EMPTY;
    }

    s_cur_column--;

    return TRUE;
}

int swap_rows(const color_t color, const size_t row0, const size_t row1)
{
    char temporary_row_array[20] = { 0, };
    size_t index_column;

    if ((row0 >= s_cur_row) || (row1 >= s_cur_row)) {
        return FALSE;
    }

    if (row0 == row1) {
        return FALSE;
    }

    /* check player's(color type) score */
    if (color == COLOR_BLACK) {

        if (s_black_score < CONSUME_SWAP_SCORE) {
            return FALSE;
        }

        s_black_score -= CONSUME_SWAP_SCORE;

    } else if (color == COLOR_WHITE) {

        if (s_white_score < CONSUME_SWAP_SCORE) {
            return FALSE;
        }

        s_white_score -= CONSUME_SWAP_SCORE;

    } else {
        return FALSE;
    }

    for (index_column = 0; index_column < s_cur_column; index_column++) {
        temporary_row_array[index_column] = s_game_board[row0][index_column];
    }

    for (index_column = 0; index_column < s_cur_column; index_column++) {
        s_game_board[row0][index_column] = s_game_board[row1][index_column];
        s_game_board[row1][index_column] = temporary_row_array[index_column];
    }

    return TRUE;
}

int swap_columns(const color_t color, const size_t col0, const size_t col1)
{
    char temporary_column_array[20] = { 0, };
    size_t index_row;

    if ((col0 >= s_cur_column) || (col1 >= s_cur_column)) {
        return FALSE;
    }

    if (col0 == col1) {
        return FALSE;
    }
	
    /* check player's(color type) score */
    if (color == COLOR_BLACK) {

        if (s_black_score < CONSUME_SWAP_SCORE) {
            return FALSE;
        }

        s_black_score -= CONSUME_SWAP_SCORE;

    } else if (color == COLOR_WHITE) {

        if (s_white_score < CONSUME_SWAP_SCORE) {
            return FALSE;
        }

        s_white_score -= CONSUME_SWAP_SCORE;

    } else {
        return FALSE;
    }

    for (index_row = 0; index_row < s_cur_row; index_row++) {
        temporary_column_array[index_row] = s_game_board[index_row][col0];
    }

    for (index_row = 0; index_row < s_cur_row; index_row++) {
        s_game_board[index_row][col0] = s_game_board[index_row][col1];
        s_game_board[index_row][col1] = temporary_column_array[index_row];
    }

    return TRUE;
}

int copy_row(const color_t color, const size_t src, const size_t dst)
{
    size_t index_column;

    if ((src >= s_cur_row) || (dst >= s_cur_row)) {
        return FALSE;
    }

    if (src == dst) {
        return FALSE;
    }
	
    /* check player's(color type) score */
    if (color == COLOR_BLACK) {
        if (s_black_score < CONSUME_COPY_SCORE) {
            return FALSE;
        }

        s_black_score -= CONSUME_COPY_SCORE;
    } else if (color == COLOR_WHITE) {
        if (s_white_score < CONSUME_COPY_SCORE) {
            return FALSE;
        }

        s_white_score -= CONSUME_COPY_SCORE;
    } else {
        return FALSE;
    }

    for (index_column = 0; index_column < s_cur_column; index_column++) {
        s_game_board[dst][index_column] = s_game_board[src][index_column];
    }

    return TRUE;
}

int copy_column(const color_t color, const size_t src, const size_t dst)
{
    size_t index_row;

    if ((src >= s_cur_column) || (dst >= s_cur_column)) {
        return FALSE;
    }
	
    if ((src < 0) || (dst < 0)) {
        return FALSE;
    }

    /* check player's(color type) score */
    if (color == COLOR_BLACK) {

        if (s_black_score < CONSUME_COPY_SCORE) {
            return FALSE;
        }

        s_black_score -= CONSUME_COPY_SCORE;

    } else if (color == COLOR_WHITE) {

        if (s_white_score < CONSUME_COPY_SCORE) {
            return FALSE;
        }

        s_white_score -= CONSUME_COPY_SCORE;

    } else {
        return FALSE;
    }

    for (index_row = 0; index_row < s_cur_row; index_row++) {
        s_game_board[index_row][dst] = s_game_board[index_row][src];
    }

    return TRUE;
}

static size_t get_horizontal_score(const size_t row, const size_t col, const color_t color)
{
    size_t horizontal_score_count = 1;
    size_t index_row = row;
    size_t index_col;

    if (color != COLOR_WHITE && color != COLOR_BLACK) {
        return 0;
    }

    /* left scan*/
    index_col = col;

    while (1) {
        if (index_col <= 0) {
            break;
        }

        if (s_game_board[index_row][--index_col] != color) {
            break;
        }

        horizontal_score_count++;
    }

    /* right scan */
    index_col = col;

    while (1) {
        if (index_col >= (s_cur_column - 1)) {
            break;
        }

        if (s_game_board[index_row][++index_col] != color) {
            break;
        }

        horizontal_score_count++;
    }

    if (horizontal_score_count < 5) {
        return 0;
    }

    horizontal_score_count -= 4;
    
    return horizontal_score_count;
}

static size_t get_vertical_score(const size_t row, const size_t col, const color_t color)
{
    size_t vertical_score_count = 1;
    size_t index_row;
    size_t index_col = col;

    if (color != COLOR_WHITE && color != COLOR_BLACK) {
        return 0;
    }

    /* up scan */
    index_row = row;

    while (1) {
        if (index_row <= 0) {
            break;
        }

        if (s_game_board[--index_row][index_col] != color) {
            break;
        }

        vertical_score_count++;
    }

    /* down scan */
    index_row = row;

    while (1) {
        if (index_row >= (s_cur_row - 1)) {
            break;
        }

        if (s_game_board[++index_row][index_col] != color) {
            break;
        }

        vertical_score_count++;
    }

    if (vertical_score_count < 5) {
        return 0;
    }

    vertical_score_count -= 4;

    return vertical_score_count;
}

static size_t get_left_diagonal_score(const size_t row, const size_t col, const color_t color)
{
    size_t diagonal_score_count = 1;
    size_t index_row;
    size_t index_col;

    if (color != COLOR_WHITE && color != COLOR_BLACK) {
        return 0;
    }

    /* left - up */
    index_row = row;
    index_col = col;

    while (1) {
        if ((index_row <= 0) || (index_col <= 0)) {
            break;
        }

        if (s_game_board[--index_row][--index_col] != color) {
            break;
        }

        diagonal_score_count++;
    }

    /* right - down */
    index_row = row;
    index_col = col;

    while (1) {
        if ((index_row >= (s_cur_row - 1)) || (index_col >= (s_cur_column - 1))) {
            break;
        }

        if (s_game_board[++index_row][++index_col] != color) {
            break;
        }

        diagonal_score_count++;
    }

    if (diagonal_score_count < 5) {
        return 0;
    }

    diagonal_score_count -= 4;

    return diagonal_score_count;
}

static size_t get_right_diagonal_score(const size_t row, const size_t col, const color_t color)
{
    size_t diagonal_score_count = 1;
    size_t index_row;
    size_t index_col;

    if (color != COLOR_WHITE && color != COLOR_BLACK) {
        return 0;
    }

    /* left - down */
    index_row = row;
    index_col = col;

    while (1) {
        if ((index_row >= (s_cur_row - 1)) || (index_col <= 0)) {
            break;
        }

        if (s_game_board[++index_row][--index_col] != color) {
            break;
        }

        diagonal_score_count++;
    }

    /* right - up */
    index_row = row;
    index_col = col;

    while (1) {
        if ((index_row <= 0) || (index_col >= (s_cur_column - 1))) {
            break;
        }

        if (s_game_board[--index_row][++index_col] != color) {
            break;
        }

        diagonal_score_count++;
    }

    if (diagonal_score_count < 5) {
        return 0;
    }

    diagonal_score_count -= 4;

    return diagonal_score_count;
}

/* print current board state display! */
/*
void print_board(void)
{
    size_t count_row;
    size_t count_column;
    
    printf("[POKUMU BOARD]\n");
    
    printf("     ");
    
    for (count_column = 0; count_column < s_cur_column; count_column++)
    {
        printf("- ");
    }
    printf("\n");
    
    for (count_row = 0; count_row < s_cur_row; count_row++)
    {
        printf("%02d. ", count_row + 1);
        printf("|");
        
        for (count_column = 0; count_column < s_cur_column; count_column++)
        {
            switch (s_game_board[count_row][count_column])
            {
            case -1:
                printf(" ");
                break;
            case COLOR_BLACK:
                printf("@");
                break;
            case COLOR_WHITE:
                printf("O");
                break;
            default:
                printf("x");
                break;
            }
            printf("|");
        }
        printf("\n");
    
        printf("     ");
        for (count_column = 0; count_column < s_cur_column; count_column++)
        {
            printf("- ");
        }
        printf("\n");
    }
    
    printf("[SCORE] \n");
    printf("black = %d\n", s_black_score);
    printf("white = %d\n", s_white_score);
}
*/
