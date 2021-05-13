#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "hashmap.h"

#define DEFAULT_ARRAY_LENGTH (20)

static size_t hash_function(const char* key);

void validate_default_test(void)
{
    size_t i = 0;
    hashmap_t* hashmap = NULL;
    hashmap = init_hashmap_malloc(DEFAULT_ARRAY_LENGTH, hash_function);

    for (i = 0; i < 100; i++) {
        char key[100];
        int value = (int)i;
        int c;
        int dummy = 512;
        sprintf(key, "key%u", i);
        assert(add_key(hashmap, key, value) == TRUE);
        c = get_value(hashmap, key);
        assert(c == value);
        assert(add_key(hashmap, key, dummy) == FALSE);
        c = get_value(hashmap, key);
        assert(c == value);
    }

    for (i = 0; i < 100; i++) {
        char key[100];
        int value = (int)(i * i * i);
        int c;
        sprintf(key, "key%u", i);
        assert(update_value(hashmap, key, value) == TRUE);
        c = get_value(hashmap, key);
        assert(c == value);
    }

    for (i = 0; i < 100; i++) {
        char key[100];
        int c;
        sprintf(key, "key%u", i);

        assert(remove_key(hashmap, key) == TRUE);
        printf("[%s] remove\n", key);
        print_hashmap(hashmap);
        printf("\n");

        c = get_value(hashmap, key);
        assert(c == -1);
        assert(remove_key(hashmap, key) == FALSE);
    }
    for (i = 0; i < DEFAULT_ARRAY_LENGTH; i++) {
        assert((hashmap->plist)[i] == NULL);
    }
    destroy(hashmap);
}

void validate_AddNonExistingKey_test(void)
{
    hashmap_t* hashmap = NULL;
    hashmap = init_hashmap_malloc(10, hash_function);

    /* hashmap에 존재하지 않는 키(key)와 값(value)이 추가되는지 */
    /* hashmap에 존재하는 키(key)를 추가 해보기 */
    assert(add_key(hashmap, "seunghwan", 100) == TRUE); 
    print_hashmap(hashmap);

    assert(add_key(hashmap, "donghyun", 100) == TRUE);
    print_hashmap(hashmap);

    assert(add_key(hashmap, "donghyun", 20) == FALSE);

    assert(update_value(hashmap, "donghyun", 40) == TRUE);
    print_hashmap(hashmap);
    
    assert(remove_key(hashmap, "sunghwan") == FALSE);

    assert(remove_key(hashmap, "donghyun") == TRUE);
    print_hashmap(hashmap);

    destroy(hashmap);
}

int main(void)
{
    validate_AddNonExistingKey_test();
    validate_default_test();

    return 0;
}

static size_t hash_function(const char* key)
{
    size_t code = 0;
    const char* c = key;
    while (*c != '\0') {
        code += *c++;
    }
    return code;
}
