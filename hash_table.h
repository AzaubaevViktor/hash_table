#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "errors.h"
#include "linked_list.h"
#include <stdlib.h>

typedef struct {
  char key[11];
  u_int64_t value;
} Data;

typedef struct {
    void **array;
    u_int64_t nElements;
} HashTable;

#ifndef MEMORY_ALLOCATE_ERROR
#define MEMORY_ALLOCATE_ERROR (43900)
#endif

#endif // HASH_TABLE_H
