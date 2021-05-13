#ifndef TRANSLATE_H
#define TRANSLATE_H

#define TRUE (1)
#define FALSE (0)

/* Definition for OPTION FLAG */
#define OPTION_FLAG             (char*)("-i")
#define OPTION_FLAG_INDEX       (1U)
#define OPTION_FLAG_LENGTH      (2U)

/* Definition for RANGE CHR */
#define RANGE_CHARACTER         (char)('-')

/* Definition for Character SET */
#define MAX_CHARACTER_SET_BUFFER_SIZE (512U)


typedef enum error_code {
    ERROR_CODE_WRONG_ARGUMENTS_NUMBER = 1,
    ERROR_CODE_INVALID_FLAG,
    ERROR_CODE_INVALID_FORMAT,
    ERROR_CODE_ARGUMENT_TOO_LONG,
    ERROR_CODE_INVALID_RANGE
} error_code_t;

typedef struct {
    char chr_set_first[MAX_CHARACTER_SET_BUFFER_SIZE];
    char chr_set_second[MAX_CHARACTER_SET_BUFFER_SIZE];
    size_t chr_set_first_length;
    size_t chr_set_second_length;
} translate_set_t;

int translate(int argc, const char* argv[]);

size_t is_option_flag(const char* flag);
size_t is_escape_character(const char chr);

size_t translate_set_initialize(translate_set_t* tr_set, const char* str1, const char* str2);

size_t chr_set_escape_transform(translate_set_t* tr_set);
void chr_set_duplication_transform(translate_set_t* tr_set);
size_t chr_set_range_format_transform(translate_set_t* tr_set);

void chr_set_remove(char* str, size_t r_index);
void chr_set_reverse(char* str);

#endif /* TRANSLATE_H */
