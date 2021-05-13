#ifndef TOKENIZE_H
#define TOKENIZE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE    (1U)
#define FALSE   (0U)

char** tokenize_malloc(const char* str, const char* delim);
size_t count_token(const char* str, const char* delim);

#endif  /* TOKENIZE_H */
