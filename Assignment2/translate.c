#include "translate.h"

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#define MAX_LENGTH  (1024U)

int translate(int argc, const char* argv[])
{
    int chr;
    translate_set_t tr_set = { 0, };

    size_t chr_index;
    size_t err_code = 0;
    size_t flag_option_setted = FALSE;

    fprintf(stderr, "c to - : %d - %d\n", 'c', '-');

    /* ERROR - wrong arguments number */
    if ((argc < 3) || (argc > 4)) {
        return ERROR_CODE_WRONG_ARGUMENTS_NUMBER;
    }

    /* FLAG VARIABLE CHECK */
    if (argc == 4) {
        if (is_option_flag(argv[OPTION_FLAG_INDEX]) == FALSE) {
            return ERROR_CODE_INVALID_FLAG;
        } else {
            flag_option_setted = TRUE;
        }
    }
    
    if (flag_option_setted == TRUE) {
        err_code = translate_set_initialize(&tr_set, argv[2], argv[3]);

    } else {
        err_code = translate_set_initialize(&tr_set, argv[1], argv[2]);
    }

    switch (err_code) {
    case ERROR_CODE_ARGUMENT_TOO_LONG:
        return err_code;
    case ERROR_CODE_INVALID_FORMAT:
        return err_code;
    case ERROR_CODE_INVALID_RANGE:
        return err_code;
    default:
        break;
    }

    fprintf(stderr, "-------------------------result------------------------\n");
    fprintf(stderr, "first_set = %s\n", tr_set.chr_set_first);
    fprintf(stderr, "first_set_length = %d\n", tr_set.chr_set_first_length);
    fprintf(stderr, "second_set = %s\n", tr_set.chr_set_second);
    fprintf(stderr, "second_set_length = %d\n\n", tr_set.chr_set_second_length);

    /* translate */
    while ((chr = fgetc(stdin)) != EOF) {
        for (chr_index = 0; chr_index < (tr_set.chr_set_first_length); chr_index++) {
            if (flag_option_setted == TRUE) {
                if ((char)(chr) == tr_set.chr_set_first[chr_index]) {
                    chr = tr_set.chr_set_second[chr_index];
                    break;
                } 

                if ((char)(chr) == (tr_set.chr_set_first[chr_index] & ~0x20)) {
                    chr = tr_set.chr_set_second[chr_index];
                    break;
                }

                if ((char)(chr) == (tr_set.chr_set_first[chr_index] | 0x20)) {
                    chr = tr_set.chr_set_second[chr_index];
                    break;
                }

            } else {
                if ((char)(chr) == tr_set.chr_set_first[chr_index]) {
                    chr = tr_set.chr_set_second[chr_index];
                    break;
                }
            }
        }
        
        fputc(chr, stdout);
    }

    return 0;
}

size_t translate_set_initialize(translate_set_t* tr_set, const char* str1, const char* str2)
{
    size_t err_code = 0;
    size_t chr_index = 0;
    size_t str1_len = strlen(str1);
    size_t str2_len = strlen(str2);

    if ((str1_len == 0) || (str2_len == 0)) {
        return ERROR_CODE_INVALID_FORMAT;
    }

    if ((str1_len >= MAX_CHARACTER_SET_BUFFER_SIZE) || (str2_len >= MAX_CHARACTER_SET_BUFFER_SIZE)) {
        return ERROR_CODE_ARGUMENT_TOO_LONG;
    }

    /* initialize tr_set */
    strncpy(tr_set->chr_set_first, str1, str1_len);
    strncpy(tr_set->chr_set_second, str2, str2_len);

    tr_set->chr_set_first[MAX_CHARACTER_SET_BUFFER_SIZE - 1] = '\0';
    tr_set->chr_set_second[MAX_CHARACTER_SET_BUFFER_SIZE - 1] = '\0';

    tr_set->chr_set_first_length = str1_len;
    tr_set->chr_set_second_length = str2_len;

    /* examine of character set's format */
    err_code = chr_set_escape_transform(tr_set);
    if (err_code == ERROR_CODE_INVALID_FORMAT) {
        return err_code;
    }

    fprintf(stderr, "----------------escape_transform---------------\n");
    fprintf(stderr, "first_set = %s\n", tr_set->chr_set_first);
    fprintf(stderr, "first_set_length = %d\n", tr_set->chr_set_first_length);
    fprintf(stderr, "second_set = %s\n", tr_set->chr_set_second);
    fprintf(stderr, "second_set_length = %d\n\n", tr_set->chr_set_second_length);
    
    /* examine of character set's range */
    err_code = chr_set_range_format_transform(tr_set);
    if ((err_code == ERROR_CODE_INVALID_RANGE) || (err_code == ERROR_CODE_ARGUMENT_TOO_LONG)) {
        return err_code;
    }

    fprintf(stderr, "----------------range_transform---------------\n");
    fprintf(stderr, "first_set = %s\n", tr_set->chr_set_first);
    fprintf(stderr, "first_set_length = %d\n", tr_set->chr_set_first_length);
    fprintf(stderr, "second_set = %s\n", tr_set->chr_set_second);
    fprintf(stderr, "second_set_length = %d\n\n", tr_set->chr_set_second_length);
    
    
    /* aligning tr_set's width */
    if ((tr_set->chr_set_first_length) < (tr_set->chr_set_second_length)) {
        tr_set->chr_set_second[tr_set->chr_set_first_length] = '\0';
        tr_set->chr_set_second_length = tr_set->chr_set_first_length;

    } else if ((tr_set->chr_set_first_length) > (tr_set->chr_set_second_length)) {

        for (chr_index = 0; chr_index < (tr_set->chr_set_first_length - tr_set->chr_set_second_length); chr_index++) {
            tr_set->chr_set_second[(tr_set->chr_set_second_length) + chr_index] = tr_set->chr_set_second[(tr_set->chr_set_second_length) - 1];
        }

        tr_set->chr_set_second[(tr_set->chr_set_second_length) + chr_index] = '\0';
        tr_set->chr_set_second_length = tr_set->chr_set_first_length;
    }

    tr_set->chr_set_first[MAX_CHARACTER_SET_BUFFER_SIZE - 1] = '\0';
    tr_set->chr_set_second[MAX_CHARACTER_SET_BUFFER_SIZE - 1] = '\0';

    tr_set->chr_set_first_length = strlen(tr_set->chr_set_first);
    tr_set->chr_set_second_length = strlen(tr_set->chr_set_second);
    
    /* examine of character set's duplication, then transform the character set */
    chr_set_duplication_transform(tr_set);

    return 0;
}

void chr_set_duplication_transform(translate_set_t* tr_set)
{
    char tmp_chr_set[MAX_CHARACTER_SET_BUFFER_SIZE] = { 0, };
    char* token = NULL;
    char* delims = " ";

    int chr_index;
    size_t tmp_chr_count = 0;
    size_t tmp_chr_index;
    size_t duplicate_chr_flag = FALSE;

    for (chr_index = (int)(tr_set->chr_set_first_length - 1); chr_index >= 0; chr_index--) {

        for (tmp_chr_index = 0; tmp_chr_index < strlen(tmp_chr_set); tmp_chr_index++) {
            if (tmp_chr_set[tmp_chr_index] == tr_set->chr_set_first[chr_index]) {
                duplicate_chr_flag = TRUE;
                break;
            }
        }

        if (duplicate_chr_flag == TRUE) {
            tr_set->chr_set_second[chr_index] = ' ';

            duplicate_chr_flag = FALSE;
            continue;
        }

        tmp_chr_set[tmp_chr_count] = tr_set->chr_set_first[chr_index];
        tmp_chr_count++;
    }

    chr_set_reverse(tmp_chr_set);

    strncpy(tr_set->chr_set_first, tmp_chr_set, strlen(tmp_chr_set));
    tr_set->chr_set_first[strlen(tmp_chr_set)] = '\0';
    tr_set->chr_set_first_length = strlen(tr_set->chr_set_first);

    for (tmp_chr_index = 0; tmp_chr_index < MAX_CHARACTER_SET_BUFFER_SIZE; tmp_chr_index++) {
        tmp_chr_set[tmp_chr_index] = 0;
    }

    token = strtok(tr_set->chr_set_second, delims);
    strncat(tmp_chr_set, token, strlen(token));

    while ((token = strtok(NULL, delims)) != NULL) {
        strncat(tmp_chr_set, token, strlen(token));
    }

    strncpy(tr_set->chr_set_second, tmp_chr_set, strlen(tmp_chr_set));
    tr_set->chr_set_second[strlen(tmp_chr_set)] = '\0';
    tr_set->chr_set_second_length = strlen(tr_set->chr_set_second);
}

size_t chr_set_escape_transform(translate_set_t* tr_set)
{
    size_t chr_index;

    for (chr_index = 0; chr_index < (tr_set->chr_set_first_length); chr_index++) {

        if (tr_set->chr_set_first[chr_index] == '\\') {

            switch (tr_set->chr_set_first[chr_index + 1]) {
            case '\\':
                tr_set->chr_set_first[chr_index] = '\\';
                break;
            case 'a':
                tr_set->chr_set_first[chr_index] = '\a';
                break;
            case 'b':
                tr_set->chr_set_first[chr_index] = '\b';
                break;
            case 'f':
                tr_set->chr_set_first[chr_index] = '\f';
                break;
            case 'n':
                tr_set->chr_set_first[chr_index] = '\n';
                break;
            case 'r':
                tr_set->chr_set_first[chr_index] = '\r';
                break;
            case 't':
                tr_set->chr_set_first[chr_index] = '\t';
                break;
            case 'v':
                tr_set->chr_set_first[chr_index] = '\v';
                break;
            case '\'':
                tr_set->chr_set_first[chr_index] = '\'';
                break;
            case '\"':
                tr_set->chr_set_first[chr_index] = '\"';
                break;
            default:
                return ERROR_CODE_INVALID_FORMAT;
            }

            chr_set_remove(tr_set->chr_set_first, chr_index + 1);

            (tr_set->chr_set_first_length)--;
        }
    }

    for (chr_index = 0; chr_index < (tr_set->chr_set_second_length); chr_index++) {

        if (tr_set->chr_set_second[chr_index] == '\\') {
            
            switch (tr_set->chr_set_second[chr_index + 1]) {
            case '\\':
                tr_set->chr_set_second[chr_index] = '\\';
                break;
            case 'a':
                tr_set->chr_set_second[chr_index] = '\a';
                break;
            case 'b':
                tr_set->chr_set_second[chr_index] = '\b';
                break;
            case 'f':
                tr_set->chr_set_second[chr_index] = '\f';
                break;
            case 'n':
                tr_set->chr_set_second[chr_index] = '\n';
                break;
            case 'r':
                tr_set->chr_set_second[chr_index] = '\r';
                break;
            case 't':
                tr_set->chr_set_second[chr_index] = '\t';
                break;
            case 'v':
                tr_set->chr_set_second[chr_index] = '\v';
                break;
            case '\'':
                tr_set->chr_set_second[chr_index] = '\'';
                break;
            case '\"':
                tr_set->chr_set_second[chr_index] = '\"';
                break;
            default:
                return ERROR_CODE_INVALID_FORMAT;
            }

            chr_set_remove(tr_set->chr_set_second, chr_index + 1);

            (tr_set->chr_set_second_length)--;
        }
    }
    
    return 0;
}

size_t chr_set_range_format_transform(translate_set_t* tr_set)
{
    /* 
        a to z : 97 - 122
        A to Z : 65 - 90
        '-' : 45

        문자 범위 지정자는 무조건
        [이스케이프 문자]-[이스케이프 문자]
        [알파벳]-[알파벳]
        으로 전달 됨.
    */
    size_t chr_index;
    size_t add_chr_index;
    size_t range_chr_index;
    size_t range_length;
    size_t range_chr_count = 0;

    char first_range_chr;
    char second_range_chr;
    char temp_chr_set[MAX_CHARACTER_SET_BUFFER_SIZE] = { 0, };
    char range_chr_set[MAX_CHARACTER_SET_BUFFER_SIZE] = { 0, };

    for (chr_index = 0; chr_index < (tr_set->chr_set_first_length); chr_index++) {
        if (tr_set->chr_set_first[chr_index] == RANGE_CHARACTER) {
            if (chr_index == ((tr_set->chr_set_first_length) - 1)) {
                break;
            }

            range_chr_index = chr_index;
            fprintf(stderr, "chr_set = %s\n", tr_set->chr_set_first);
            fprintf(stderr, "range_chr_index = %d\n\n", range_chr_index);
            
            if (range_chr_index == 0) {
                continue;
            }

            first_range_chr = tr_set->chr_set_first[range_chr_index - 1];
            second_range_chr = tr_set->chr_set_first[range_chr_index + 1];

            /* 이스케이프는 이스케이프 범위 & 알파벳은 알파벳 범위로 들어온다고 가정 */

            if (first_range_chr > second_range_chr) {
                return ERROR_CODE_INVALID_RANGE;

            } else if (first_range_chr == second_range_chr) {
                chr_set_remove(tr_set->chr_set_first, range_chr_index);
                chr_set_remove(tr_set->chr_set_first, range_chr_index);

                tr_set->chr_set_first_length -= 2;

                range_length = 1;
                chr_index = (range_chr_index - 1) + range_length;

            } else {
                if ((range_chr_index + 2) < (tr_set->chr_set_first_length)) {
                    strncpy(temp_chr_set, &(tr_set->chr_set_first[range_chr_index + 2]), MAX_CHARACTER_SET_BUFFER_SIZE);
                }

                for (add_chr_index = (size_t)first_range_chr; add_chr_index < ((size_t)second_range_chr + 1); add_chr_index++) {
                    if (add_chr_index <= 32) {
                        if (is_escape_character((char)(add_chr_index)) == TRUE) {
                            range_chr_set[range_chr_count] = (char)add_chr_index;
                            range_chr_count++;
                        }
                    } else {
                        range_chr_set[range_chr_count] = add_chr_index;
                        range_chr_count++;
                    }
                }

                range_length = strlen(range_chr_set);
                if (((tr_set->chr_set_first_length) - 3 + range_length) >= (MAX_CHARACTER_SET_BUFFER_SIZE - 1)) {
                    return ERROR_CODE_ARGUMENT_TOO_LONG;
                }

                tr_set->chr_set_first[range_chr_index - 1] = '\0';

                strncat(tr_set->chr_set_first, range_chr_set, range_length);
                strncat(tr_set->chr_set_first, temp_chr_set, strlen(temp_chr_set));

                tr_set->chr_set_first[MAX_CHARACTER_SET_BUFFER_SIZE - 1] = '\0';
                tr_set->chr_set_first_length = strlen(tr_set->chr_set_first);
                
                chr_index = (range_chr_index - 1) + range_length;

                range_chr_count = 0;
                memset(range_chr_set, 0, MAX_CHARACTER_SET_BUFFER_SIZE);
                memset(temp_chr_set, 0, MAX_CHARACTER_SET_BUFFER_SIZE);
            }
        }
    }

    for (chr_index = 0; chr_index < (tr_set->chr_set_second_length); chr_index++) {
        if (tr_set->chr_set_second[chr_index] == RANGE_CHARACTER) {
            if (chr_index == ((tr_set->chr_set_second_length) - 1)) {
                break;
            }

            range_chr_index = chr_index;

            if (range_chr_index == 0) {
                continue;
            }

            first_range_chr = tr_set->chr_set_second[range_chr_index - 1];
            second_range_chr = tr_set->chr_set_second[range_chr_index + 1];

            /* 이스케이프는 이스케이프 범위 & 알파벳은 알파벳 범위로 들어온다고 가정 */

            if (first_range_chr > second_range_chr) {
                return ERROR_CODE_INVALID_RANGE;

            } else if (first_range_chr == second_range_chr) {
                chr_set_remove(tr_set->chr_set_second, range_chr_index);
                chr_set_remove(tr_set->chr_set_second, range_chr_index);

                tr_set->chr_set_second_length -= 2;

                range_length = 1;
                chr_index = (range_chr_index - 1) + range_length;

            } else {
                if ((range_chr_index + 2) < (tr_set->chr_set_second_length)) {
                    strncpy(temp_chr_set, &(tr_set->chr_set_second[range_chr_index + 2]), MAX_CHARACTER_SET_BUFFER_SIZE);
                }

                for (add_chr_index = (size_t)first_range_chr; add_chr_index < ((size_t)second_range_chr + 1); add_chr_index++) {
                    if (add_chr_index <= 32) {
                        if (is_escape_character((char)(add_chr_index)) == TRUE) {
                            range_chr_set[range_chr_count] = (char)add_chr_index;
                            range_chr_count++;
                        }
                    } else {
                        range_chr_set[range_chr_count] = add_chr_index;
                        range_chr_count++;
                    }
                }

                range_length = strlen(range_chr_set);
                if (((tr_set->chr_set_second_length) - 3 + range_length) >= (MAX_CHARACTER_SET_BUFFER_SIZE - 1)) {
                    return ERROR_CODE_ARGUMENT_TOO_LONG;
                }

                tr_set->chr_set_second[range_chr_index - 1] = '\0';

                strncat(tr_set->chr_set_second, range_chr_set, range_length);
                strncat(tr_set->chr_set_second, temp_chr_set, strlen(temp_chr_set));

                tr_set->chr_set_second_length = strlen(tr_set->chr_set_second);
                tr_set->chr_set_second[MAX_CHARACTER_SET_BUFFER_SIZE - 1] = '\0';

                chr_index = (range_chr_index - 1) + range_length;

                range_chr_count = 0;
                memset(range_chr_set, 0, MAX_CHARACTER_SET_BUFFER_SIZE);
                memset(temp_chr_set, 0, MAX_CHARACTER_SET_BUFFER_SIZE);
            }
        }
    }

    return TRUE;
}

size_t is_option_flag(const char* flag)
{
    if (strncmp(flag, OPTION_FLAG, OPTION_FLAG_LENGTH) == 0) {
        return TRUE;
    }

    return FALSE;
}

size_t is_escape_character(const char chr)
{
    switch (chr) {
    case '\\':
    case '\a':
    case '\b':
    case '\f':
    case '\n':
    case '\r':
    case '\t':
    case '\v':
    case '\'':
    case '\"':
        return TRUE;
    default:
        return FALSE;
    }

    return FALSE;
}

void chr_set_remove(char* str, size_t rindex) 
{
    size_t str_length;
    size_t str_index;

    if (str == NULL) {
        return;
    }

    str_length = strlen(str);

    if (str_length == 0) {
        return;
    }

    if (rindex >= str_length) {
        return;
    }

    for (str_index = rindex; str_index < str_length; str_index++) {
        if (str_index >= (str_length - 1)) {
            str[str_length - 1] = '\0';
            break;
        }

        str[str_index] = str[str_index + 1];
    }
}

void chr_set_reverse(char* str)
{
    char c;
    size_t index;
    int length = strlen(str);

    if (length == 0) {
        return;
    }

    for (index = 0; index < (size_t)(length / 2); index++) {
        if (*(str + index) == *(str + (length - index - 1))) {
            continue;
        }

        c = *(str + index);
        *(str + index) = *(str + (length - index - 1));
        *(str + (length - index - 1)) = c;
    }
}
