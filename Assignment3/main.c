#include "document_analyzer.h"
#include <assert.h>

void validate_default_test(void);
void validate_document1_test(void);
void validate_duplicate_output_tree_test(void);

void validate_empty_document_test(void);
void validate_one_data_document_test(void);
void validate_two_data_document_test(void);

void validate_default_test(void)
{
    const char** sentence = NULL;
    const char*** paragraph = NULL;
    size_t i = 0U;
    size_t j = 0U;

    assert(load_document("doesntexist.txt") == FALSE);
    assert(load_document("document.txt") == TRUE);

    assert(get_total_paragraph_count() == 3);
    assert(get_total_sentence_count() == 9);
    assert(get_total_word_count() == 58);

    assert(get_paragraph(3) == NULL);

    paragraph = get_paragraph(1);
    assert(paragraph != NULL);
    assert(get_paragraph_word_count(paragraph) == 14);
    assert(get_paragraph_sentence_count(paragraph) == 2);

    assert(strcmp(paragraph[i][j++], "This") == 0);
    assert(strcmp(paragraph[i][j++], "is") == 0);
    assert(strcmp(paragraph[i][j++], "the") == 0);
    assert(strcmp(paragraph[i][j++], "second") == 0);
    assert(strcmp(paragraph[i++][j++], "paragraph") == 0);

    j = 0U;

    assert(strcmp(paragraph[i][j++], "I") == 0);
    assert(strcmp(paragraph[i][j++], "wonder") == 0);
    assert(strcmp(paragraph[i][j++], "how") == 0);
    assert(strcmp(paragraph[i][j++], "many") == 0);
    assert(strcmp(paragraph[i][j++], "sentences") == 0);
    assert(strcmp(paragraph[i][j++], "you") == 0);
    assert(strcmp(paragraph[i][j++], "will") == 0);
    assert(strcmp(paragraph[i][j++], "see") == 0);
    assert(strcmp(paragraph[i][j++], "here") == 0);

    assert(get_sentence(0, 10) == NULL);

    sentence = get_sentence(0, 2);
    assert(sentence != NULL);
    assert(get_sentence_word_count(sentence) == 4);

    i = 0U;

    assert(strcmp(sentence[i++], "Is") == 0);
    assert(strcmp(sentence[i++], "this") == 0);
    assert(strcmp(sentence[i++], "too") == 0);
    assert(strcmp(sentence[i++], "easy") == 0);

    assert(print_as_tree("output.txt") == TRUE);

    dispose();

    printf("validate_default_test() complete! -> [document.txt]\n");
    printf("================================================================================\n\n");
}

void validate_document1_test(void)
{
    const char** sentence = NULL;
    const char*** paragraph = NULL;

    assert(get_paragraph(0) == NULL);
    assert(get_sentence(0, 0) == NULL);

    assert(get_total_word_count() == 0);
    assert(get_total_sentence_count() == 0);
    assert(get_total_paragraph_count() == 0U);
    assert(print_as_tree("output_document.txt") == FALSE);

    assert(load_document("document1.txt") == TRUE);

    assert(get_total_word_count() == 62U);
    assert(get_total_sentence_count() == 6U);
    assert(get_total_paragraph_count() == 3U);

    paragraph = get_paragraph(0);
    assert(paragraph != NULL);
    assert(get_paragraph_word_count(paragraph) == 19);
    assert(get_paragraph_sentence_count(paragraph) == 1);
    sentence = get_sentence(0, 0);
    assert(sentence != NULL);
    assert(get_sentence_word_count(sentence) == 19);
    assert(get_sentence(0, 1) == NULL);

    paragraph = get_paragraph(1);
    assert(paragraph != NULL);
    assert(get_paragraph_word_count(paragraph) == 28);
    assert(get_paragraph_sentence_count(paragraph) == 3);
    sentence = get_sentence(1, 0);
    assert(sentence != NULL);

    assert(get_sentence_word_count(sentence) == 10);
    sentence = get_sentence(1, 1);
    assert(sentence != NULL);
    assert(get_sentence_word_count(sentence) == 9);
    sentence = get_sentence(1, 2);
    assert(sentence != NULL);
    assert(get_sentence_word_count(sentence) == 9);
    assert(get_sentence(1, 3) == NULL);

    paragraph = get_paragraph(2);
    
    assert(paragraph != NULL);
    assert(get_paragraph_word_count(paragraph) == 15);
    assert(get_paragraph_sentence_count(paragraph) == 2);

    sentence = get_sentence(2, 0);
    assert(sentence != NULL);
    assert(get_sentence_word_count(sentence) == 8);
    sentence = get_sentence(2, 1);
    assert(sentence != NULL);
    assert(get_sentence_word_count(sentence) == 7);
    assert(get_sentence(2, 2) == NULL);

    assert(get_paragraph(3) == NULL);

    assert(print_as_tree("output.txt") == TRUE);

    dispose();

    printf("validate_document1_test() complete! -> [document1.txt]\n");
    printf("================================================================================\n\n");
}

void validate_duplicate_output_tree1_test(void)
{
    printf("validate_duplicate_output_tree_test() start!\n");

    validate_default_test();

    validate_empty_document_test();

    validate_one_data_document_test();

    printf("validate_one_data_document_test() complete! -> [one_data_document.txt]\n");
    printf("================================================================================\n\n");

    validate_document1_test();

    printf("validate_duplicate_output_tree1_test() complete!\n");
    printf("================================================================================\n\n");
}

void validate_duplicate_output_tree2_test(void)
{
    /* 파일 중복 검사 */
    assert(load_document("document.txt") == TRUE);

    assert(get_total_paragraph_count() == 3U);
    assert(get_total_sentence_count() == 9U);
    assert(get_total_word_count() == 58U);

    assert(print_as_tree("output_document.txt") == TRUE);

    /* dispose() 없이도 제대로 이전 doc에 대한 자원 해제 및 현재 doc에 대한 자원이 받아와지는가? */
    assert(load_document("empty_document.txt") == TRUE);

    assert(get_total_paragraph_count() == 0U);
    assert(get_total_sentence_count() == 0U);
    assert(get_total_word_count() == 0U);

    assert(print_as_tree("output_empty_document.txt") == FALSE);

    /* dispose() 없이도 제대로 이전 doc에 대한 자원 해제 및 현재 doc에 대한 자원이 받아와지는가? */
    assert(load_document("document.txt") == TRUE);

    assert(get_total_paragraph_count() == 3U);
    assert(get_total_sentence_count() == 9U);
    assert(get_total_word_count() == 58U);

    assert(print_as_tree("output_document.txt") == TRUE);
}

void validate_empty_document_test(void)
{
    assert(load_document("empty_document.txt") == TRUE);

    assert(get_total_paragraph_count() == 0U);
    assert(get_total_sentence_count() == 0U);
    assert(get_total_word_count() == 0U);

    assert(print_as_tree("output_empty.txt") == FALSE);
    
    dispose();

    printf("validate_empty_document_test() complete! -> [empty_document.txt]\n");
    printf("================================================================================\n\n");

    assert(load_document("empty_document1.txt") == TRUE);

    assert(get_total_paragraph_count() == 0U);
    assert(get_total_sentence_count() == 0U);
    assert(get_total_word_count() == 0U);

    assert(print_as_tree("output_empty1.txt") == FALSE);

    dispose();

    printf("validate_empty_document_test() complete! -> [empty_docuent1.txt]\n");
    printf("================================================================================\n\n");

    assert(load_document("empty_document2.txt") == TRUE);

    assert(get_total_paragraph_count() == 0U);
    assert(get_total_sentence_count() == 0U);
    assert(get_total_word_count() == 0U);

    assert(print_as_tree("output_empty2.txt") == FALSE);

    dispose();

    printf("validate_empty_document_test() complete! -> [empty_docuent2.txt]\n");
    printf("================================================================================\n\n");

    assert(load_document("empty_document3.txt") == TRUE);

    assert(get_total_paragraph_count() == 0U);
    assert(get_total_sentence_count() == 0U);
    assert(get_total_word_count() == 0U);

    assert(print_as_tree("output_empty3.txt") == FALSE);

    dispose();

    printf("validate_empty_document_test() complete! -> [empty_docuent3.txt]\n");
    printf("================================================================================\n\n");

    assert(load_document("empty_document4.txt") == TRUE);

    assert(get_total_paragraph_count() == 0U);
    assert(get_total_sentence_count() == 0U);
    assert(get_total_word_count() == 0U);

    assert(print_as_tree("output_empty4.txt") == FALSE);

    dispose();

    printf("validate_empty_document_test() complete! -> [empty_docuent4.txt]\n");
    printf("================================================================================\n\n");
}

void validate_one_data_document_test(void)
{
    const char** sentence = NULL;

    assert(load_document("one_data_document.txt") == TRUE);
    
    printf("total_paragraph_count = %d\n", get_total_paragraph_count());
    assert(get_total_paragraph_count() == 1U);
    assert(get_total_sentence_count() == 1U);
    assert(get_total_word_count() == 1U);

    sentence = get_sentence(0, 0);

    assert(strcmp(sentence[0], "13") == 0);

    print_as_tree("output_data_document.txt");

    dispose();
    
    printf("validate_one_data_document_test() complete! -> [one_data_document.txt]\n");
    printf("================================================================================\n\n");

    assert(load_document("one_data_document1.txt") == TRUE);

    assert(get_total_paragraph_count() == 1U);
    assert(get_total_sentence_count() == 1U);
    assert(get_total_word_count() == 1U);

    sentence = get_sentence(0, 0);

    assert(strcmp(sentence[0], "13") == 0);

    print_as_tree("output_one_data_document1.txt");

    dispose();

    printf("validate_one_data_document_test() complete! -> [one_data_document1.txt]\n");
    printf("================================================================================\n\n");

    assert(load_document("one_data_document2.txt") == TRUE);

    assert(get_total_paragraph_count() == 1U);
    assert(get_total_sentence_count() == 1U);
    assert(get_total_word_count() == 1U);

    sentence = get_sentence(0, 0);

    assert(strcmp(sentence[0], "13") == 0);

    print_as_tree("output2.txt");

    dispose();

    printf("validate_one_data_document_test() complete! -> [one_data_document2.txt]\n");
    printf("================================================================================\n\n");


    assert(load_document("one_data_document3.txt") == TRUE);

    assert(get_total_paragraph_count() == 1U);
    assert(get_total_sentence_count() == 1U);
    assert(get_total_word_count() == 1U);

    sentence = get_sentence(0, 0);

    assert(strcmp(sentence[0], "12") == 0);

    print_as_tree("output3.txt");

    dispose();

    printf("validate_one_data_document_test() complete! -> [one_data_document3.txt]\n");
    printf("================================================================================\n\n");

    assert(load_document("one_data_document4.txt") == TRUE);

    assert(get_total_paragraph_count() == 1U);
    assert(get_total_sentence_count() == 1U);
    assert(get_total_word_count() == 1U);

    sentence = get_sentence(0, 0);

    assert(strcmp(sentence[0], "a") == 0);

    print_as_tree("output4.txt");

    dispose();

    printf("validate_one_data_document_test() complete! -> [one_data_document4.txt]\n");
    printf("================================================================================\n\n");

    assert(load_document("one_data_document5.txt") == TRUE);

    assert(get_total_paragraph_count() == 1U);
    assert(get_total_paragraph_count() == 1U);
    assert(get_total_paragraph_count() == 1U);

    sentence = get_sentence(0, 0);

    assert(strcmp(sentence[0], "a") == 0);

    print_as_tree("output5.txt");

    dispose();

    printf("validate_one_data_document_test() complete! -> [one_data_document5.txt]\n");
    printf("================================================================================\n\n");

    assert(load_document("one_data_document6.txt") == TRUE);

    assert(get_total_paragraph_count() == 1U);
    assert(get_total_sentence_count() == 1U);
    assert(get_total_word_count() == 1U);

    sentence = get_sentence(0, 0);

    assert(strcmp(sentence[0], "a") == 0);

    print_as_tree("output6.txt");

    dispose();

    printf("validate_one_data_document_test() complete! -> [one_data_document6.txt]\n");
    printf("================================================================================\n\n");

    assert(load_document("one_data_document7.txt") == TRUE);

    assert(print_as_tree("output7.txt") == FALSE);

    dispose();

    printf("validate_one_data_document_test() complete! -> [one_data_document7.txt]\n");
    printf("================================================================================\n\n");

    assert(load_document("one_data_document8.txt") == TRUE);

    assert(get_total_paragraph_count() == 1U);
    assert(get_total_sentence_count() == 1U);
    assert(get_total_word_count() == 2U);

    sentence = get_sentence(0, 0);
    assert(get_sentence_word_count(sentence) == 2);
    assert(strcmp(sentence[0], "int") == 0);
    assert(strcmp(sentence[1], "\t") == 0);

    print_as_tree("output8.txt");

    dispose();

    printf("validate_one_data_document_test() complete! -> [one_data_document8.txt]\n");
    printf("================================================================================\n\n");
}

void validate_two_data_document_test(void)
{
    const char** sentence = NULL;
    const char*** paragraph = NULL;

    printf("validate_two_data_document_test() start\n");

    /* ====================================================================== */
    assert(load_document("two_data_document1.txt") == TRUE);

    assert(get_total_paragraph_count() == 2U);
    assert(get_total_sentence_count() == 2U);
    assert(get_total_word_count() == 2U);

    sentence = get_sentence(0, 0);
    assert(get_sentence_word_count(sentence) == 1);
    assert(strcmp(sentence[0], "a") == 0);

    paragraph = get_paragraph(1);
    assert(get_paragraph_sentence_count(paragraph) == 1);
    assert(strcmp(paragraph[0][0], "a") == 0);

    print_as_tree("output_two_data_document1.txt");

    dispose();

    printf("validate_two_data_document_test() complete! -> [two_data_document1.txt]\n");
    printf("================================================================================\n\n");

    assert(load_document("two_data_document2.txt") == TRUE);

    assert(get_total_paragraph_count() == 2U);
    assert(get_total_sentence_count() == 2U);
    assert(get_total_word_count() == 2U);

    sentence = get_sentence(0, 0);
    assert(get_sentence_word_count(sentence) == 1);
    assert(strcmp(sentence[0], "a") == 0);
    
    paragraph = get_paragraph(1);
    assert(get_paragraph_sentence_count(paragraph) == 1);
    assert(strcmp(paragraph[0][0], "a") == 0);

    print_as_tree("output_two_data_document2.txt");

    dispose();

    printf("validate_two_data_document_test() complete! -> [two_data_document2.txt]\n");
    printf("================================================================================\n\n");

    assert(load_document("two_data_document3.txt") == TRUE);

    assert(get_total_paragraph_count() == 2U);
    assert(get_total_sentence_count() == 2U);
    assert(get_total_word_count() == 2U);

    sentence = get_sentence(0, 0);
    assert(get_sentence_word_count(sentence) == 1);
    assert(strcmp(sentence[0], "\t") == 0);
    
    paragraph = get_paragraph(1);
    assert(get_paragraph_sentence_count(paragraph) == 1);
    assert(strcmp(paragraph[0][0], "\t") == 0);

    print_as_tree("output_two_data_document3.txt");

    dispose();

    printf("validate_two_data_document_test() complete! -> [two_data_document3.txt]\n");
    printf("================================================================================\n\n");

    assert(load_document("two_data_document4.txt") == TRUE);

    assert(get_total_paragraph_count() == 2U);
    assert(get_total_sentence_count() == 4U);
    assert(get_total_word_count() == 4U);

    print_as_tree("output_two_data_document4.txt");

    dispose();

    printf("validate_two_data_document_test() complete! -> [two_data_document4.txt]\n");
    printf("================================================================================\n\n");
}

int main(void)
{
    
    validate_document1_test();
    validate_default_test();
    
    
    
    validate_empty_document_test();
    
    validate_one_data_document_test();
    validate_two_data_document_test();
    
    validate_duplicate_output_tree_test(); 
    

    return 0;
}
