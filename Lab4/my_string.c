#include "my_string.h"

void reverse(char* str)
{
    char c;
    size_t index;
    int length = get_length(str);

    if ((length == 0) || (length == -1)) {
        return;
    }

    for (index = 0; index < (size_t)(length / 2); index++) {
        c = *(str + index);
        *(str + index) = *(str + (length - index - 1));
        *(str + (length - index - 1)) = c;
    }
}

int index_of(const char* str, const char* word)
{
    size_t str_index;
    size_t word_index;
    size_t right_count = 0;
    int str_length = get_length(str);
    int word_length = get_length(word);

    if (str_length == -1) {
        return -1;
    }

    if ((word_length == 0) || (word_length == -1)) {
        return 0;
    }

    if (str_length < word_length) {
        return -1;
    }

    for (str_index = 0; str_index < (size_t)str_length; str_index++) {
        if (*(str + str_index) == *(word)) {
            
            right_count++;
            
            for (word_index = 1; word_index < (size_t)word_length; word_index++) {
                if (*(word + word_index) != *(str + str_index + word_index)) {
                    break;
                }

                right_count++;
            }

            if (right_count == (size_t)word_length) {
                return str_index;
            }

            right_count = 0;
        }
    }

    return -1;
}

void reverse_by_words(char* str)
{
    size_t str_index;
    int str_length = get_length(str);
    char* word_start_address = str;

    if ((str_length == 0) || (str_length == -1)) {
        return;
    }

    for (str_index = 0; str_index < (size_t)str_length; str_index++) {
        if (*(str + str_index) == ' ') {
            *(str + str_index) = '\0';

            reverse(word_start_address);
            word_start_address = str + str_index + 1;
        }
    }

    reverse(word_start_address);

    for (str_index = 0; str_index < (size_t)str_length; str_index++) {
        if (*(str + str_index) == '\0') {
            *(str + str_index) = ' ';
        }
    }
}

static char* s_token_start_address = NULL;

char* tokenize(char* str, const char* delims)
{
    size_t prev_delim = 0;
    size_t cur_delim = 0;

    size_t str_index;
    size_t delims_index;
    int str_length = 0;
    int delims_length = get_length(delims);

    /* first token */
    if (str != NULL) {
        str_length = get_length(str);
        
        if ((str_length == 0) || (str_length == -1)) {
            return NULL;
        }

        if ((delims_length == 0) || (delims_length == -1)) {
            return str;
        }

        str_index = 0;
        while (str_index < (size_t)str_length) {
            for (delims_index = 0; delims_index < (size_t)delims_length; delims_index++) {
                if (*(str + str_index) == *(delims + delims_index)) {
                    cur_delim = 1;
                }
            }

            if (cur_delim == 0) {
                str += str_index;
                break;
            }

            cur_delim = 0;
            str_index++;
        }

        str_length = get_length(str);

        if ((str_length == 0) || (str_length == -1)) {
            return NULL;
        }

        cur_delim = 0;
        str_index = 0;

        while (str_index < (size_t)str_length) {
            for (delims_index = 0; delims_index < (size_t)delims_length; delims_index++) {
                if (*(str + str_index) == *(delims + delims_index)) {
                    cur_delim = 1;
                    break;
                }
            }

            if (delims_index == (size_t)delims_length) {
                cur_delim = 0;
            }

            if ((cur_delim == 1) && (prev_delim == 0)) {
                *(str + str_index) = '\0';
                prev_delim = 1;
            }

            if ((cur_delim == 1) && (prev_delim == 1)) {
                *(str + str_index) = '\0';
            }

            if ((cur_delim == 0) && (prev_delim == 1)) {
                break;
            }

            str_index++;
        }

        s_token_start_address = str + str_index;
        return str;
    } 

    /* remained token */
    str_length = get_length(s_token_start_address);

    if ((str_length == 0) || (str_length == -1)) {
        return NULL;
    }

    if ((delims_length == 0) || (delims_length == -1)) {
        return str;
    }

    str_index = 0;

    while (str_index < (size_t)str_length) {
        for (delims_index = 0; delims_index < (size_t)delims_length; delims_index++) {
            if (*(s_token_start_address + str_index) == *(delims + delims_index)) {
                cur_delim = 1;
                break;
            }
        }

        if (delims_index == (size_t)delims_length) {
            cur_delim = 0;
        }

        if ((cur_delim == 1) && (prev_delim == 0)) {
            *(s_token_start_address + str_index) = '\0';
            prev_delim = 1;
        }

        if ((cur_delim == 1) && (prev_delim == 1)) {
            *(s_token_start_address + str_index) = '\0';
        }

        if ((cur_delim == 0) && (prev_delim == 1)) {
            break;
        }

        str_index++;
    }

    s_token_start_address += str_index;
    return s_token_start_address - str_index;
}

char* reverse_tokenize(char* str, const char* delims)
{
    size_t prev_delim = 0;
    size_t cur_delim = 0;

    size_t str_index;
    size_t delims_index;
    int str_length = 0;
    int delims_length = get_length(delims);

    /* first token */
    if (str != NULL) {
        str_length = get_length(str);

        if ((str_length == 0) || (str_length == -1)) {
            return NULL;
        }

        if ((delims_length == 0) || (delims_length == -1)) {
            return str;
        }

        str_index = 0;
        while (str_index < (size_t)str_length) {
            for (delims_index = 0; delims_index < (size_t)delims_length; delims_index++) {
                if (*(str + str_index) == *(delims + delims_index)) {
                    cur_delim = 1;
                }
            }

            if (cur_delim == 0) {
                str += str_index;
                break;
            }

            cur_delim = 0;
            str_index++;
        }

        str_length = get_length(str);

        if ((str_length == 0) || (str_length == -1)) {
            return NULL;
        }

        cur_delim = 0;
        str_index = 0;

        while (str_index < (size_t)str_length) {
            for (delims_index = 0; delims_index < (size_t)delims_length; delims_index++) {
                if (*(str + str_index) == *(delims + delims_index)) {
                    cur_delim = 1;
                    break;
                }
            }

            if (delims_index == (size_t)delims_length) {
                cur_delim = 0;
            }

            if ((cur_delim == 1) && (prev_delim == 0)) {
                *(str + str_index) = '\0';
                prev_delim = 1;
            }

            if ((cur_delim == 1) && (prev_delim == 1)) {
                *(str + str_index) = '\0';
            }

            if ((cur_delim == 0) && (prev_delim == 1)) {
                break;
            }

            str_index++;
        }

        s_token_start_address = str + str_index;

        reverse(str);
        return str;
    }

    /* remained token */
    str_length = get_length(s_token_start_address);

    if ((str_length == 0) || (str_length == -1)) {
        return NULL;
    }

    if ((delims_length == 0) || (delims_length == -1)) {
        return s_token_start_address;
    }

    str_index = 0;

    while (str_index < (size_t)str_length) {
        for (delims_index = 0; delims_index < (size_t)delims_length; delims_index++) {
            if (*(s_token_start_address + str_index) == *(delims + delims_index)) {
                cur_delim = 1;
                break;
            }
        }

        if (delims_index == (size_t)delims_length) {
            cur_delim = 0;
        }

        if ((cur_delim == 1) && (prev_delim == 0)) {
            *(s_token_start_address + str_index) = '\0';
            prev_delim = 1;
        }

        if ((cur_delim == 1) && (prev_delim == 1)) {
            *(s_token_start_address + str_index) = '\0';
        }

        if ((cur_delim == 0) && (prev_delim == 1)) {
            break;
        }

        str_index++;
    }

    s_token_start_address += str_index;

    reverse(s_token_start_address - str_index);
    return s_token_start_address - str_index;
}

int get_length(const char* str)
{
    int length = 0;

    if (str == NULL) {
        return -1;
    }

    if (*(str) == '\0') {
        return 0;
    }

    while (*(str) != '\0') {
        length++;
        str++;
    }
    
    return length;
}
