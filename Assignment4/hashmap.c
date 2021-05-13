#include "hashmap.h"

#include <assert.h>

hashmap_t* init_hashmap_malloc(size_t length, size_t (*p_hash_func)(const char* key))
{
    size_t index;
    hashmap_t* hashmap;

    assert(p_hash_func != NULL);

    if (length == 0) {
        return NULL;
    }

    hashmap = (hashmap_t*)malloc(sizeof(hashmap_t));
    memset(hashmap, 0, sizeof(hashmap_t));

    hashmap->plist = (node_t**)malloc(sizeof(node_t*) * length);
    for (index = 0; index < length; index++) {
        hashmap->plist[index] = NULL;
    }

    hashmap->hash_func = p_hash_func;
    hashmap->length = length;

    return hashmap;
}

int add_key(hashmap_t* hashmap, const char* key, const int value)
{
    size_t hash_value;

    size_t index;
    size_t key_length;

    node_t* new_node;
    node_t* prev_node;
    node_t* cur_node;

    assert(hashmap != NULL);
    assert(key != NULL);
    assert(value >= 0);

    key_length = strlen(key) + 1;

    new_node = (node_t*)malloc(sizeof(node_t));
    
    new_node->key = (char*)malloc(sizeof(char) * key_length);
    strncpy(new_node->key, key, key_length);
    new_node->value = value;
    new_node->next = NULL;

    hash_value = hashmap->hash_func(key);

    index = hash_value % (hashmap->length);

    if (hashmap->plist[index] == NULL) {

        hashmap->plist[index] = new_node;
        return TRUE;
    }

    cur_node = hashmap->plist[index];

    while (TRUE) {

        if (strcmp(cur_node->key, key) == 0) {

            free(new_node);
            return FALSE;
        }

        prev_node = cur_node;
        cur_node = cur_node->next;

        if (cur_node == NULL) {
            
            prev_node->next = new_node;
            break;
        }
    }

    return TRUE;
}

int get_value(const hashmap_t* hashmap, const char* key)
{
    size_t hash_value;

    size_t index;

    node_t* cur_node;

    assert(hashmap != NULL);
    assert(key != NULL);

    hash_value = hashmap->hash_func(key);

    index = hash_value % (hashmap->length);

    if (hashmap->plist[index] == NULL) {
        return -1;
    }

    cur_node = hashmap->plist[index];

    while (TRUE) {

        if (strcmp(cur_node->key, key) == 0) {

            return (cur_node->value);
        }

        cur_node = cur_node->next;

        if (cur_node == NULL) {

            break;
        }
    }

    return -1;
}

int update_value(hashmap_t* hashmap, const char* key, int value)
{
    node_t* cur_node;

    size_t hash_value;

    size_t index;

    assert(hashmap != NULL);
    assert(key != NULL);

    hash_value = hashmap->hash_func(key);

    index = hash_value % (hashmap->length);

    if (hashmap->plist[index] == NULL) {

        return FALSE;
    }

    cur_node = hashmap->plist[index];

    while (TRUE) {

        if (strcmp(cur_node->key, key) == 0) {
            
            cur_node->value = value;
            return TRUE;
        }

        cur_node = cur_node->next;

        if (cur_node == NULL) {
            
            break;
        }
    }

    return FALSE;
}

int remove_key(hashmap_t* hashmap, const char* key)
{
    size_t hash_value;

    size_t index;
    size_t bucket_count = 0;

    node_t* prev_node;
    node_t* cur_node;

    assert(hashmap != NULL);
    assert(key != NULL);

    hash_value = hashmap->hash_func(key);
    index = hash_value % (hashmap->length);

    if (hashmap->plist[index] == NULL) {
        return FALSE;
    }

    cur_node = hashmap->plist[index];
    prev_node = cur_node;

    while (TRUE) {

        if (cur_node == NULL) {
            break;
        }

        if (strcmp(cur_node->key, key) == 0) {

            if ((bucket_count == 0) && (cur_node->next == NULL)) {
                hashmap->plist[index] = NULL;
            } else if ((bucket_count == 0) && (cur_node->next != NULL)) {
                hashmap->plist[index] = cur_node->next;
            } else {
                prev_node->next = cur_node->next;
            }

            free(cur_node->key);
            free(cur_node);

            return TRUE;
        }

        prev_node = cur_node;
        cur_node = cur_node->next;
        ++bucket_count;
    }

    return FALSE;
}

void destroy(hashmap_t* hashmap)
{
    size_t index;

    node_t* remove_node;
    node_t* cur_node;

    assert(hashmap != NULL);

    for (index = 0; index < (hashmap->length); index++) {

        if (hashmap->plist[index] != NULL) {

            cur_node = hashmap->plist[index];

            while (TRUE) {

                if (cur_node == NULL) {
                    break;
                }

                remove_node = cur_node;
                cur_node = cur_node->next;

                free(remove_node->key);
                free(remove_node);
            }

            hashmap->plist[index] = NULL;
        }
    }

    free(hashmap->plist);
    free(hashmap);
}

void print_hashmap(hashmap_t* hashmap)
{
    size_t index;
    
    node_t* cur_node;

    for (index = 0; index < (hashmap->length); index++) {
        
        if (hashmap->plist[index] != NULL) {
            printf("[%d] = ", index);

            cur_node = hashmap->plist[index];
            while (TRUE) {
                
                if (cur_node == NULL) {
                    printf("\n");
                    break;
                }

                printf("(%s, %d) -> ", cur_node->key, cur_node->value);

                cur_node = cur_node->next;
            }
            printf("\n\n");
        }
    }
}
