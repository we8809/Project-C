#include <assert.h>
#include <stdio.h>

#include "tokenize.h"

void validate_token_count_test(void)
{
    const char* str = "  Tokenize this  string.  ";
    char** tokens = tokenize_malloc(str, " ");
    char** tt = tokens;

    assert(strcmp(tt[0], "Tokenize") == 0);
    assert(strcmp(tt[1], "this") == 0);
    assert(strcmp(tt[2], "string.") == 0);

    while (TRUE) {
        if (*tt == NULL) {
            break;
        }

        free(*(tt++));
    }
    
    free(tokens);

    printf("\n");

    {
        const char* str = "I like cookies. Do you like cookies? Cookies are good!";
        char** tokens = tokenize_malloc(str, " ?!.");
        char** tt = tokens;

        assert(strcmp(*tt++, "I") == 0);
        assert(strcmp(*tt++, "like") == 0);
        assert(strcmp(*tt++, "cookies") == 0);
        assert(strcmp(*tt++, "Do") == 0);
        assert(strcmp(*tt++, "you") == 0);
        assert(strcmp(*tt++, "like") == 0);
        assert(strcmp(*tt++, "cookies") == 0);
        assert(strcmp(*tt++, "Cookies") == 0);
        assert(strcmp(*tt++, "are") == 0);
        assert(strcmp(*tt++, "good") == 0);
        assert(*tt == NULL);

        tt = tokens;
        while (*tt != NULL) {
            free(*tt++);
        }
        free(tokens);
    }

    printf("\n");

    {
        const char* str = "I";
        char** tokens = tokenize_malloc(str, " ");
        char** tt = tokens;

        assert(strcmp(*tt++, "I") == 0);
        assert(*tt == NULL);

        tt = tokens;
        while (*tt != NULL) {
            free(*tt++);
        }
        free(tokens);

        puts("test4\n");
    }

    {
        const char* str = "I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I  I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I ";
        char** tokens = tokenize_malloc(str, " ");
        char** tt = tokens;

        while (*tt != NULL) {
            assert(*(*tt) == 'I');
            tt++;
        }

        assert(*tt == NULL);

        tt = tokens;
        while (*tt != NULL) {
            free(*tt++);
        }
        free(tokens);

        puts("test5\n");
    }


    printf("validate_token_count_test() -> clear!\n\n");
    printf("--------------------------------------------------------------------\n\n");
}

void validate_token_empty_test(void)
{
    {
        const char* str = "       ";
        char** tokens = tokenize_malloc(str, " ");
        char** tt = tokens;

        assert(tt[0] == NULL);

        free(tokens);
    }

    printf("\n");
    
    {
        const char* str = "";
        char** tokens = tokenize_malloc(str, "");
        char** tt = tokens;

        printf("str('') = '%s'\n", *tt);
        assert(*tt == NULL);

        tt = tokens;
        while (*tt != NULL) {
            free(*tt++);
        }
        free(tokens);

    }

    printf("validate_token_empty_test() -> clear!\n\n");
    printf("--------------------------------------------------------------------\n\n");
}

void validate_one_token_test(void)
{
    {
        const char* str = "      n";
        char** tokens = tokenize_malloc(str, " ");
        char** tt = tokens;

        assert(strcmp(tt[0], "n") == 0);

        while (TRUE) {
            if (*tt == NULL) {
                break;
            }

            free(*(tt++));
        }

        free(tokens);
    }


    printf("\n");

    {
        const char* str = "n          ";
        char** tokens = tokenize_malloc(str, " ");
        char** tt = tokens;

        assert(strcmp(tt[0], "n") == 0);

        while (TRUE) {
            if (*tt == NULL) {
                break;
            }

            free(*(tt++));
        }

        free(tokens);
    }


    printf("\n");

    {
        const char* str = " I ";
        char** tokens = tokenize_malloc(str, " ?!.");
        char** tt = tokens;

        assert(strcmp(*tt++, "I") == 0);
        assert(*tt == NULL);

        tt = tokens;
        while (*tt != NULL) {
            free(*tt++);
        }

        free(tokens);
    }

    printf("\n");

    {
        const char* str = "       ";
        char** tokens = tokenize_malloc(str, "");
        char** tt = tokens;

        assert(strcmp(*tt++, "       ") == 0);
        assert(*tt == NULL);

        tt = tokens;
        while (*tt != NULL) {
            free(*tt++);
        }

        free(tokens);
    }

    printf("validate_one_token_test() -> clear!\n\n");
    printf("--------------------------------------------------------------------\n\n");
}

int main(void)
{
    validate_token_count_test();
    validate_token_empty_test();
    validate_one_token_test();

    return 0;
}
