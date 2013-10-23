#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "errors.h"
#include "linked_list.h"
#include <stdlib.h>

typedef struct {
  char key[11];
  u_int64_t value;
} Data;
/* TODO: Хранить полный хэш-код */

typedef struct {
    void **array;
    u_int64_t nElements;
} HashTable;

#define DEBUG

#ifdef DEBUG
void printHashTable(HashTable *table);
#endif

HashTable *initializeHashTable(u_int64_t nElements, Error *error);
int setDataInTableByString(HashTable *table, char *str, u_int64_t value, Error *error);
Data *getDataFromTableByString(HashTable *table, char *str);
int deleteDataFromTableByString(HashTable *table, char *str);

#ifndef MEMORY_ALLOCATE_ERROR
#define MEMORY_ALLOCATE_ERROR (43900)
#endif

#endif // HASH_TABLE_H
