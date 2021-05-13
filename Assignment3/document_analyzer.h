#ifndef DOCUMENT_ANALYZER_H
#define DOCUMENT_ANALYZER_H

#ifndef _CRT_NO_SECURE_WARNINGS
#define _CRT_NO_SECURE_WARNINGS
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define TRUE        (1)
#define FALSE       (0)

#define PARAGRAPH_DELIMS    (const char*)("\n")
#define SENTENCE_DELIMS     (const char*)(".!?")
#define WORD_DELIMS         (const char*)(" ,")

int load_document(const char* document);
void dispose(void);

size_t get_total_word_count(void);
size_t get_total_sentence_count(void);
size_t get_total_paragraph_count(void);

const char*** get_paragraph(const size_t paragraph_index);
size_t get_paragraph_word_count(const char*** paragraph);
size_t get_paragraph_sentence_count(const char*** paragraph);

const char** get_sentence(const size_t paragraph_index, const size_t sentence_index);
size_t get_sentence_word_count(const char** sentence);

size_t get_data_paragraph_count(const char* data, size_t data_length);
size_t get_data_sentence_count(const char* data, size_t data_length);
size_t get_data_word_count(const char* data, size_t data_length);

int print_as_tree(const char* filename);

#endif /* DOCUMENT_ANALYZER_H */
