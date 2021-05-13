#ifndef MY_STRING_H
#define MY_STRING_H

#include <limits.h>

void reverse(char* str);
int index_of(const char* str, const char* word);
void reverse_by_words(char* str);
char* tokenize(char* str, const char* delims);
char* reverse_tokenize(char* str, const char* delims);
int get_length(const char* str);

#endif      /* MY_SYRING_H */
