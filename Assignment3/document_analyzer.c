#include "document_analyzer.h"

static size_t s_document_loaded_flag = FALSE;
static size_t s_empty_document_loaded_flag = FALSE;

static char**** s_pa_document = NULL;

static size_t s_total_paragraph_count = 0;
static size_t s_total_sentence_count = 0;
static size_t s_total_word_count = 0;

static size_t* s_pa_sentence_count_list = NULL;
static size_t** s_pa_word_count_list = NULL;

int load_document(const char* document)
{
    FILE* document_file_p = NULL;

    int data;
    char* pa_document_data = NULL;
    size_t data_index = 0;
    long document_start_index;
    long document_end_index;
    size_t document_length;

    size_t paragraph_index = 0;
    size_t paragraph_count = 0;
    size_t sentence_count = 0;
    size_t sentence_index = 0;
    size_t word_count = 0;
    size_t word_length = 0;
    size_t word_index = 0;

    char* paragraph;
    char* sentence;
    char* word;
    
    char** pa_temporary_paragraph_list = NULL;
    char** pa_temporary_sentence_list = NULL;

    size_t empty_find_after_paragraph_scan_flag = FALSE;
    size_t empty_find_after_sentence_scan_flag = FALSE;

    if (document == NULL) {
        return FALSE;
    }

    if (*(document) == '\0') {
        return FALSE;
    }

    if (s_document_loaded_flag == TRUE) {
        dispose();
    }

    document_file_p = fopen(document, "r");
    if (document_file_p == NULL) {
        perror("Document not in here");
        return FALSE;
    }

    document_start_index = ftell(document_file_p);
    fseek(document_file_p, 0, SEEK_END);
    document_end_index = ftell(document_file_p);
    rewind(document_file_p);

    document_length = (size_t)document_end_index - (size_t)document_start_index + 1;
    if (document_length <= 1) {
        s_document_loaded_flag = TRUE;
        s_empty_document_loaded_flag = TRUE;
        
        fclose(document_file_p);
        return TRUE;
    }

    pa_document_data = (char*)malloc(sizeof(char) * document_length);
    if (pa_document_data == NULL) {
        fprintf(stderr, "pa_document_data[]'s allocate failed!\n");
        
        fclose(document_file_p);
        return FALSE;
    }
    memset(pa_document_data, 0, document_length);

    data = fgetc(document_file_p);
    while (TRUE) {
        if (data == EOF) {
            pa_document_data[data_index] = '\0';
            break;
        }

        if ((char)data == '\n') {
            --document_length;
        }

        pa_document_data[data_index++] = (char)data;
        data = fgetc(document_file_p);
    }

    s_total_paragraph_count = get_data_paragraph_count(pa_document_data, document_length - 1);
    if (s_total_paragraph_count == 0) {

        goto empty_found_after_document_reading;
    }

    s_pa_document = (char****)malloc(sizeof(char***) * s_total_paragraph_count);
    s_pa_sentence_count_list = (size_t*)malloc(sizeof(size_t) * s_total_paragraph_count);

    pa_temporary_paragraph_list = (char**)malloc(sizeof(char*) * s_total_paragraph_count);

    paragraph = strtok(pa_document_data, PARAGRAPH_DELIMS);
    while (TRUE) {
        if (paragraph == NULL) {

            sentence_count = 0;
            break;
        }

        sentence_count = get_data_sentence_count(paragraph, strlen(paragraph));
        if ((s_total_paragraph_count == 1) && (sentence_count == 0)) {
            
            empty_find_after_paragraph_scan_flag = TRUE;
            goto empty_found_after_document_reading;
        }

        s_total_sentence_count += sentence_count;

        pa_temporary_paragraph_list[paragraph_count] = paragraph;

        s_pa_sentence_count_list[paragraph_count] = sentence_count;
        s_pa_document[paragraph_count++] = (char***)malloc(sizeof(char**) * sentence_count);
        paragraph = strtok(NULL, PARAGRAPH_DELIMS);

        sentence_count = 0;
    }

    s_pa_word_count_list = (size_t**)malloc(sizeof(size_t*) * s_total_paragraph_count);

    for (paragraph_index = 0; paragraph_index < paragraph_count; paragraph_index++) {

        sentence_count = get_data_sentence_count(pa_temporary_paragraph_list[paragraph_index], strlen(pa_temporary_paragraph_list[paragraph_index]));

        pa_temporary_sentence_list = (char**)malloc(sizeof(char*) * sentence_count);
        s_pa_word_count_list[paragraph_index] = (size_t*)malloc(sizeof(size_t) * sentence_count);

        sentence_count = 0;

        sentence = strtok(pa_temporary_paragraph_list[paragraph_index], SENTENCE_DELIMS);
        while (TRUE) {
            if (sentence == NULL) {

                word_count = 0;
                break;
            }

            word_count = get_data_word_count(sentence, strlen(sentence));
            if ((s_total_paragraph_count == 1) && (s_total_sentence_count == 1) && (word_count == 0)) {

                empty_find_after_sentence_scan_flag = TRUE;
                goto empty_found_after_document_reading;
            }

            s_pa_word_count_list[paragraph_index][sentence_count] = word_count;
            s_total_word_count += word_count; 

            pa_temporary_sentence_list[sentence_count] = sentence;
            s_pa_document[paragraph_index][sentence_count++] = (char**)malloc(sizeof(char*) * word_count);

            sentence = strtok(NULL, SENTENCE_DELIMS);

            word_count = 0;
        }
        

        for (sentence_index = 0; sentence_index < sentence_count; sentence_index++) {

            word = strtok(pa_temporary_sentence_list[sentence_index], WORD_DELIMS);

            if (word == NULL) {

                word_length = strlen(pa_temporary_sentence_list[sentence_index]);

                s_pa_document[paragraph_index][sentence_index][word_index] = (char*)malloc(sizeof(char) * (word_length + 1));
                memset(s_pa_document[paragraph_index][sentence_index][word_index], 0, (word_length + 1));

                strncpy(s_pa_document[paragraph_index][sentence_index][word_index], pa_temporary_sentence_list[sentence_index], word_length);

                s_pa_document[paragraph_index][sentence_index][word_index][word_length] = '\0';

            } else {

                while (TRUE) {
                    if (word == NULL) {
                        break;
                    }

                    word_length = strlen(word);

                    s_pa_document[paragraph_index][sentence_index][word_index] = (char*)malloc(sizeof(char) * (word_length + 1));

                    strncpy(s_pa_document[paragraph_index][sentence_index][word_index], word, word_length);
                    s_pa_document[paragraph_index][sentence_index][word_index++][word_length] = '\0';

                    word = strtok(NULL, WORD_DELIMS);
                }

                word_index = 0;
            }
        }

        for (sentence_index = 0; sentence_index < sentence_count; sentence_index++) {
            pa_temporary_sentence_list[sentence_index] = NULL;
        }
        free(pa_temporary_sentence_list);
    }

    for (paragraph_index = 0; paragraph_index < s_total_paragraph_count; paragraph_index++) {
        pa_temporary_paragraph_list[paragraph_index] = NULL;
    }
    free(pa_temporary_paragraph_list);

    memset(pa_document_data, 0, document_length);
    free(pa_document_data);

    pa_temporary_paragraph_list = NULL;
    pa_temporary_sentence_list = NULL;
    pa_document_data = NULL;

    printf("loading complete!!\n");
    fclose(document_file_p);

    s_document_loaded_flag = TRUE;
    
    return TRUE;

empty_found_after_document_reading:

    memset(pa_document_data, 0, document_length);
    free(pa_document_data);
    pa_document_data = NULL;

    s_document_loaded_flag = TRUE;
    s_empty_document_loaded_flag = TRUE;

    s_total_paragraph_count = 0;
    s_total_sentence_count = 0;
    s_total_word_count = 0;

    fclose(document_file_p);

    if (empty_find_after_paragraph_scan_flag == TRUE) {
        goto empty_found_after_paragraph_scan;
    }

    return TRUE;

empty_found_after_paragraph_scan:

    s_pa_document[0] = NULL;
    free(s_pa_document);
    s_pa_document = NULL;
    
    s_pa_sentence_count_list[0] = 0;
    free(s_pa_sentence_count_list);
    s_pa_sentence_count_list = NULL;

    pa_temporary_paragraph_list[0] = NULL;
    free(pa_temporary_paragraph_list);
    pa_temporary_paragraph_list = NULL;

    if (empty_find_after_sentence_scan_flag == TRUE) {
        goto empty_found_after_sentence_scan;
    }
    
    return TRUE;

empty_found_after_sentence_scan:

    s_pa_word_count_list[0][0] = 0;
    free(s_pa_word_count_list[0]);
    free(s_pa_word_count_list);
    s_pa_word_count_list = NULL;

    pa_temporary_sentence_list[0] = NULL;
    free(pa_temporary_sentence_list);
    pa_temporary_sentence_list = NULL;

    return TRUE;
}

void dispose(void)
{
    size_t paragraph_index;
    size_t sentence_index;
    size_t word_index;

    if (s_document_loaded_flag == FALSE) {

        fprintf(stderr, "dispose() not execute -> file has not opened, please load_document() and get the file resource!\n");
        return;
    }

    if (s_empty_document_loaded_flag == TRUE) {

        s_document_loaded_flag = FALSE;
        s_empty_document_loaded_flag = FALSE;

        return;
    }

    for (paragraph_index = 0; paragraph_index < s_total_paragraph_count; paragraph_index++) {
        
        for (sentence_index = 0; sentence_index < s_pa_sentence_count_list[paragraph_index]; sentence_index++) {

            for (word_index = 0; word_index < s_pa_word_count_list[paragraph_index][sentence_index]; word_index++) {

                memset(s_pa_document[paragraph_index][sentence_index][word_index], 0, strlen(s_pa_document[paragraph_index][sentence_index][word_index]));
                
                free(s_pa_document[paragraph_index][sentence_index][word_index]);
                s_pa_document[paragraph_index][sentence_index][word_index] = NULL;
            }
            
            free(s_pa_document[paragraph_index][sentence_index]);
            s_pa_document[paragraph_index][sentence_index] = NULL;
        }

        free(s_pa_document[paragraph_index]);
        s_pa_document[paragraph_index] = NULL;
    }
    
    free(s_pa_document);
    
    for (paragraph_index = 0; paragraph_index < s_total_paragraph_count; paragraph_index++) {
        
        memset(s_pa_word_count_list[paragraph_index], 0, s_pa_sentence_count_list[paragraph_index]);
        free(s_pa_word_count_list[paragraph_index]);

        s_pa_word_count_list[paragraph_index] = NULL;
    }

    free(s_pa_word_count_list);

    memset(s_pa_sentence_count_list, 0, s_total_paragraph_count);
    free(s_pa_sentence_count_list);

    s_pa_word_count_list = NULL;
    s_pa_sentence_count_list = NULL;
    s_pa_document = NULL;

    s_total_paragraph_count = 0;
    s_total_sentence_count = 0;
    s_total_word_count = 0;

    s_document_loaded_flag = FALSE;

    printf("dispose() exit!\n");
}

size_t get_total_word_count(void)
{
    if (s_document_loaded_flag == FALSE) {
        return 0;
    }

    if (s_empty_document_loaded_flag == TRUE) {
        return 0;
    }
        
    return s_total_word_count;
}

size_t get_total_sentence_count(void)
{
    if (s_document_loaded_flag == FALSE) {
        return 0;
    }

    if (s_empty_document_loaded_flag == TRUE) {
        return 0;
    }

    return s_total_sentence_count;
}

size_t get_total_paragraph_count(void)
{
    if (s_document_loaded_flag == FALSE) {
        return 0;
    }

    if (s_empty_document_loaded_flag == TRUE) {
        return 0;
    }

    return s_total_paragraph_count;
}

const char*** get_paragraph(const size_t paragraph_index)
{
    if (s_document_loaded_flag == FALSE) {
        return NULL;
    }

    if (s_empty_document_loaded_flag == TRUE) {
        return NULL;
    }

    if (paragraph_index >= s_total_paragraph_count) {
        return NULL;
    }

    return (const char***)s_pa_document[paragraph_index];
}

size_t get_paragraph_word_count(const char*** paragraph)
{
    size_t paragraph_word_count = 0;

    size_t paragraph_index;
    size_t sentence_index;

    size_t paragraph_find_flag = FALSE;

    if (s_document_loaded_flag == FALSE) {
        return 0;
    }

    if (s_empty_document_loaded_flag == TRUE) {
        return 0;
    }

    for (paragraph_index = 0; paragraph_index < s_total_paragraph_count; paragraph_index++) {

        if ((const char***)s_pa_document[paragraph_index] == paragraph) {
            paragraph_find_flag = TRUE;
            break;
        }
    }

    if (paragraph_find_flag == FALSE) {
        return 0;
    }

    for (sentence_index = 0; sentence_index < s_pa_sentence_count_list[paragraph_index]; sentence_index++) {
        
        paragraph_word_count += s_pa_word_count_list[paragraph_index][sentence_index];
    }

    return paragraph_word_count;
}

size_t get_paragraph_sentence_count(const char*** paragraph)
{
    size_t paragraph_sentence_count = 0;

    size_t paragraph_index;

    size_t paragraph_find_flag = FALSE;

    if (s_document_loaded_flag == FALSE) {
        return 0;
    }

    if (s_empty_document_loaded_flag == TRUE) {
        return 0;
    }

    for (paragraph_index = 0; paragraph_index < s_total_paragraph_count; paragraph_index++) {

        if ((const char***)s_pa_document[paragraph_index] == paragraph) {
            paragraph_find_flag = TRUE;
            break;
        }
    }

    if (paragraph_find_flag == FALSE) {
        return 0;
    }

    paragraph_sentence_count = s_pa_sentence_count_list[paragraph_index];

    return paragraph_sentence_count;
}

const char** get_sentence(const size_t paragraph_index, const size_t sentence_index)
{
    if (s_document_loaded_flag == FALSE) {
        return NULL;
    }

    if (s_empty_document_loaded_flag == TRUE) {
        return NULL;
    }

    if (paragraph_index >= s_total_paragraph_count) {
        return NULL;
    }

    if (sentence_index >= s_pa_sentence_count_list[paragraph_index]) {
        return NULL;
    }

    return (const char**)s_pa_document[paragraph_index][sentence_index];
}

size_t get_sentence_word_count(const char** sentence)
{
    size_t paragraph_index;
    size_t sentence_index;

    if (s_document_loaded_flag == FALSE) {
        return 0;
    }

    if (s_empty_document_loaded_flag == TRUE) {
        return 0;
    }

    for (paragraph_index = 0; paragraph_index < s_total_paragraph_count; paragraph_index++) {
        
        for (sentence_index = 0; sentence_index < s_pa_sentence_count_list[paragraph_index]; sentence_index++) {
            
            if ((const char**)s_pa_document[paragraph_index][sentence_index] == sentence) {
                goto find_sentence;
            }
        }
    }

    return 0;

find_sentence:

    return s_pa_word_count_list[paragraph_index][sentence_index];
}

int print_as_tree(const char* filename)
{
    FILE* output_file_p = NULL;

    size_t paragraph_index;
    size_t sentence_index;
    size_t word_index;

    if (filename == NULL) {
        return FALSE;
    }

    if (*(filename) == '\0') {
        return FALSE;
    }

    if (s_document_loaded_flag == FALSE) {
        return FALSE;
    }

    if (s_empty_document_loaded_flag == TRUE) {
        return FALSE;
    }

    output_file_p = fopen(filename, "w+");
    if (output_file_p == NULL) {
        perror("output_file not opening");
        return FALSE;
    }

    printf("total_paragraph_count = %d\n\n", s_total_paragraph_count);
    for (paragraph_index = 0; paragraph_index < s_total_paragraph_count; paragraph_index++) {
        
        fprintf(output_file_p, "Paragraph %d:\n", paragraph_index);
        printf("paragraph %d\n", paragraph_index);
        printf("      sentence_count = %d\n", s_pa_sentence_count_list[paragraph_index]);

        for (sentence_index = 0; sentence_index < s_pa_sentence_count_list[paragraph_index]; sentence_index++) {

            fprintf(output_file_p, "    Sentence %d:\n", sentence_index);
            printf("          word_count = %d\n", s_pa_word_count_list[paragraph_index][sentence_index]);

            for (word_index = 0; word_index < s_pa_word_count_list[paragraph_index][sentence_index]; word_index++) {

                fprintf(output_file_p, "        %s", s_pa_document[paragraph_index][sentence_index][word_index]);
                
                if ((word_index >= (s_pa_word_count_list[paragraph_index][sentence_index] - 1)) && (paragraph_index == (s_total_paragraph_count - 1)) && (sentence_index == (s_pa_sentence_count_list[paragraph_index] - 1))) {
                    break;
                }

                fputc('\n', output_file_p);
            }
        }

        if (paragraph_index == (s_total_paragraph_count - 1)) {
            break;
        }

        fputc('\n', output_file_p);
    }

    printf("print_as_tree() complete -> [%s]\n", filename);
    fclose(output_file_p);

    return TRUE;
}

size_t get_data_paragraph_count(const char* data, size_t data_length)
{
    size_t data_index;

    size_t paragraph_count = 0;
    
    size_t paragraph_delims_count = 0;
    size_t continuous_delims_flag = FALSE;

    for (data_index = 0; data_index < data_length; data_index++) {

        if (data[data_index] == '\n') {
            ++paragraph_delims_count;

            if (data_index == 0) {
                continuous_delims_flag = TRUE;
                continue;
            }

            if ((data_index == (data_length - 1)) && (continuous_delims_flag == TRUE)) {
                --paragraph_count;
                break;
            }

            if (continuous_delims_flag == FALSE) {
                
                if (data_index == (data_length - 1)) {
                    break;
                }

                ++paragraph_count;
                continuous_delims_flag = TRUE;

            }

        } else {
            continuous_delims_flag = FALSE;
        }
    }

    if (paragraph_delims_count == data_length) {
        return 0;
    }

    if (paragraph_count == 0) {
        return 1;
    }

    paragraph_count += 1;

    return paragraph_count;
}

size_t get_data_sentence_count(const char* data, size_t data_length)
{
    size_t data_index;

    size_t sentence_count = 0;
    size_t sentence_delims_count = 0;

    size_t continuous_delims_flag = FALSE;

    for (data_index = 0; data_index < data_length; data_index++) {

        if (data[data_index] == '.' || data[data_index] == '?' || data[data_index] == '!') {
            ++sentence_delims_count;

            if (data_index == 0) {
                continuous_delims_flag = TRUE;
                continue;
            }

            if (continuous_delims_flag == FALSE) {

                ++sentence_count;
                continuous_delims_flag = TRUE;
            }

        } else {
            continuous_delims_flag = FALSE;
        }
    }

    if (sentence_delims_count == data_length) {
        return 0;
    }

    if (sentence_count == 0) {
        return 1;
    }

    return sentence_count;
}

size_t get_data_word_count(const char* data, size_t data_length)
{
    size_t data_index;

    size_t word_count = 0;
    size_t word_delims_count = 0;

    size_t continuous_delims_flag = FALSE;

    for (data_index = 0; data_index < data_length; data_index++) {

        if (data[data_index] == ' ' || data[data_index] == ',') {

            ++word_delims_count;

            if (data_index == 0) {

                continuous_delims_flag = TRUE;
                continue;
            }

            if ((data_index == (data_length - 1)) && (continuous_delims_flag == TRUE)) {
                --word_count;
            }

            if (continuous_delims_flag == FALSE) {

                ++word_count;
                continuous_delims_flag = TRUE;
            }

        } else {
            continuous_delims_flag = FALSE;
        }
    }

    if (word_delims_count == data_length) {
        return 0;
    }

    if (word_count == 0) {
        return 1;
    }

    word_count += 1;

    return word_count;
}
