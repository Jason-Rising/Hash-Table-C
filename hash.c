#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#include "hash.h"

#define BIT_RANGE 64
#define STR_MAX 256

// Create hash table
HashTable* ht_create(unsigned int table_size){
    // Allocate table memory
    HashTable* hashtable = malloc(sizeof(HashTable));
    assert(hashtable);

    // Allocate memory for each record
    hashtable->hashArray = malloc(sizeof(Record) * table_size);

    // Set each record to NULL
    int i;
    for (i = 0; i < table_size; i++){
        hashtable->hashArray[i].key = NULL;
    }
    return hashtable;
}

// Returns hash value given key
unsigned int hash(const char* key, const unsigned int table_size){
    unsigned int value = 0, i = 0;
    while(key[i] != '\0'){
        unsigned int letter = chr(key[i]);
        value = (value * BIT_RANGE + letter) % table_size;
        i++;
  }
  return value;
}

// Map character to specific unsigned int value
unsigned int chr(const char c){
  if (c >= 48 && c <=57) return c + 4;
  if (c >= 65 && c <= 90) return c - 39;
  if (c >= 97 && c <= 122) return c - 97;
  // Invalid char
  fprintf(stderr, "[ERROR]: Invalid character in string '%c'", c);
  return EXIT_FAILURE;
}

// Insert key/value into hashtable, returns true if rehash needed
bool ht_insert(HashTable* hashtable, const char *key, const unsigned int table_size, const unsigned int step_size){
    unsigned int slot = hash(key, table_size);
    // try to look up an entry
    char* value = hashtable->hashArray[slot].key;

    // no entry means slot is empty, insert immediately
    if (value == NULL){
        hashtable->hashArray[slot].key = malloc(strlen(key) + 1);
        strcpy(hashtable->hashArray[slot].key, key);
        return false;
    }
    // collision look for next available spot (open addressing)
    int start = slot;
    while(hashtable->hashArray[slot].key != NULL){
        slot = (slot + step_size) % table_size;
        // Looped back to start
        if (slot == start) return true;
    }
    // Found empty position
    hashtable->hashArray[slot].key = malloc(strlen(key) + 1);
    strcpy(hashtable->hashArray[slot].key, key);
    return false;
}

// Prints out records of hash table
void ht_print(HashTable* hashtable, const unsigned int table_size){
    int i = 0;
    for(i = 0; i < table_size; i++){
        printf("%d: ", i);
        if (hashtable->hashArray[i].key == NULL){
            printf("\n");
            continue;
        }
        printf("%s\n", hashtable->hashArray[i].key);
    }
}


// Reallocates memory for hash table
void ht_change_table_size(HashTable* hashtable, const unsigned new_size){
    hashtable->hashArray = realloc(hashtable->hashArray, sizeof(Record) * new_size);
    assert(hashtable->hashArray);
    
    // Set each record to NULL
    int i;
    for (i = 0; i < new_size; i++){
        hashtable->hashArray[i].key = NULL;
    }
}

// Reorders file strings in order of appearance in hash table
void ht_reorder_strings(HashTable* hashtable, char** strings, const unsigned int table_size){
    int i, index = 0;
    for (i = 0; i < table_size; i++){
        char* value = hashtable->hashArray[i].key;
        if (value != NULL){
            // Found active record, swap hashtable value into string array
            free(strings[index]);
            strings[index] = malloc(strlen(value) + 1);
            strcpy(strings[index++], value);
            // Reset hashtable record value
            free(hashtable->hashArray[i].key);
            hashtable->hashArray[i].key = NULL;
        }
    }
}

int main(int argc, char **argv){
    int i = 0, n, table_size, step_size;
    char str[STR_MAX + 1];
    char** strings;

    // Retrieve file n, m, k values
    if (scanf("%d %d %d", &n, &table_size, &step_size) == 3){
            HashTable* ht = ht_create(table_size);
            strings = malloc(n * sizeof(strings));
            // Append file words to string array
            while(scanf("%s", str) == 1){
                strings[i] = malloc(strlen(str) + 1);
                strcpy(strings[i], str);
                i++;
            }

            // Insert file strings into hash table
            i = 0;
            while (i < n){
                // If true; indicates rehash is needed
                if (ht_insert(ht, strings[i++], table_size, step_size)){
                    // Reorder strings array in order of strings in hash table
                    ht_reorder_strings(ht, strings, table_size);

                    table_size *= 2;
                    // Update hash table size
                    ht_change_table_size(ht, table_size);       
                    
                    // Resetting i to 0 restarts hashing process
                    i = 0;
                }
            }
            ht_print(ht, table_size);
    }else{
        fprintf(stderr, "[ERROR]: Invalid file format, couldn't read values m, n, k");
        return EXIT_FAILURE;
    }


    return 0;
}

