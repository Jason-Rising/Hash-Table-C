#ifndef HASH_H
#define HASH_H

typedef struct hashTable HashTable;
typedef struct record Record;

struct hashTable {
    Record* hashArray;
};

struct record{
    char* key;
};



// Create hash table
HashTable* ht_create(unsigned int table_size);

// Returns hash value given key
unsigned int hash(const char* key, const unsigned int table_size);

// Map character to specific unsigned int value
unsigned int chr(const char c);

// Insert key/value into hashtable, returns true if rehash needed
bool ht_insert(HashTable* hashtable, const char *key, const unsigned int table_size, const unsigned int step_size);

// Prints out records of hash table
void ht_print(HashTable* hashtable, const unsigned int table_size);

// Reallocates memory for hash table
void ht_change_table_size(HashTable* hashtable, const unsigned new_size);

// Reorders file strings in order of appearance in hash table
void ht_reorder_strings(HashTable* hashtable, char** strings, const unsigned int table_size);

#endif