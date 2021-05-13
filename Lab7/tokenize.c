#include "tokenize.h"

char** tokenize_malloc(const char* str, const char* delim)
{
    char** pa_token_list = NULL;
    char* token_p = NULL; 
    size_t str_count = 0;
    size_t str_index = 0;

    size_t token_count = 0;
    size_t token_length = 0;
    size_t token_list_index = 0;

    size_t delim_index;
    size_t delim_length;
    size_t is_continuous_delim = FALSE;

    if (str == NULL) {
        return NULL;
    }

    if (*(str) == '\0') {
        printf("hihi1\n");
        goto token_count_zero;
    }

    if (delim == NULL) {
        goto token_count_one;
    }

    if (*(delim) == '\0') {
        goto token_count_one;
    }
    
    delim_length = strlen(delim);

    token_count = count_token(str, delim);
    printf("token_count = %d\n", token_count);
    if (token_count == 0) {
        goto token_count_zero;
    }

    pa_token_list = (char**)malloc(sizeof(char*) * (token_count + 1));

    while (TRUE) {
        ++str_count;

        if (*(str) == '\0') {

            if (is_continuous_delim == TRUE) {
                pa_token_list[token_list_index] = NULL;
                break;
            }

            token_p = (char*)(str - str_count + 1);
            token_length = str_count - 1;

            pa_token_list[token_list_index] = (char*)malloc(sizeof(char) * token_length + 1);
            strncpy(pa_token_list[token_list_index], token_p, token_length);
            pa_token_list[token_list_index][token_length] = '\0';

            printf("[%d] -> \"%s\"\n", token_list_index, pa_token_list[token_list_index]);
            ++token_list_index;

            pa_token_list[token_list_index] = NULL;   
            break;
        }

        for (delim_index = 0; delim_index < delim_length; delim_index++) {
            
            if (*(str) == delim[delim_index]) {

                if (is_continuous_delim == FALSE) {

                    if (str_index == 0) {
                        str_count = 0;
                        is_continuous_delim = TRUE;
                        break;
                    }

                    is_continuous_delim = TRUE;
                    
                    token_p = (char*)(str - str_count + 1);
                    token_length = str_count - 1;

                    pa_token_list[token_list_index] = (char*)malloc(sizeof(char) * token_length + 1);
                    strncpy(pa_token_list[token_list_index], token_p, token_length);
                    pa_token_list[token_list_index][token_length] = '\0';

                    printf("[%d] -> \"%s\"\n", token_list_index, pa_token_list[token_list_index]);                   
                    ++token_list_index;
                    str_count = 0;
                    
                    break;

                } else {
                    str_count = 0;

                    break;
                }
            }
        }

        if (delim_index == delim_length) {
            is_continuous_delim = FALSE;
        }
        
        ++str;
        ++str_index;
    }

    return pa_token_list;

token_count_zero:
    pa_token_list = (char**)malloc(sizeof(char*) * 1);
    pa_token_list[0] = NULL;

    return pa_token_list;

token_count_one:
    pa_token_list = (char**)malloc(sizeof(char*) * 2);
    pa_token_list[0] = (char*)malloc(sizeof(char) * strlen(str) + 1);
    strncpy(pa_token_list[0], str, strlen(str));
    pa_token_list[0][strlen(str)] = '\0';
    pa_token_list[1] = NULL;
    
    return pa_token_list;
}

size_t count_token(const char* str, const char* delim)
{
    size_t token_count = 1;
    size_t str_index = 0;
    size_t delim_index;
    size_t str_length = strlen(str);
    size_t delim_length = strlen(delim);
    
    size_t is_continuous_delim = FALSE;

    while (TRUE) {
        if (str[str_index] == '\0') {
            break;
        }

        for (delim_index = 0; delim_index < delim_length; delim_index++) {

            if (str[str_index] == delim[delim_index]) {

                if ((str_index == (str_length - 1)) && (is_continuous_delim == TRUE)) {

                    if (token_count == 1) {
                        break;
                    }

                    --token_count;
                    break;
                }

                if (is_continuous_delim == FALSE) {

                    if (str_index == 0) {
                        is_continuous_delim = TRUE;

                        break;  
                    }

                    if (str_index == (str_length - 1)) {
                        
                        break;
                    }

                    ++token_count;
                    is_continuous_delim = TRUE;
                    break;

                } else {

                    break;
                }
            }
        }

        if (delim_index == delim_length) {
            is_continuous_delim = FALSE;
        }
       
        ++str_index;
    }

    return token_count;
}
