#include "linked_list.h"
#include "hash_table.h"
#include "errors.h"

#include <stdlib.h>
#include <string.h>

#ifdef DEBUG
#include <stdio.h>
#include <inttypes.h>

#define STANDART "\x1b[0m"
#define BOLD_WHITE "\x1b[1;37m"
#define BOLD_YELLOW "\x1b[1;33m"
#define BOLD_GREEN "\x1b[1;32m"

void printHashTable(HashTable *table) {
  u_int64_t i=0;
  LinkedList *list = NULL;
  printf(BOLD_GREEN"HashTable"STANDART" on (%p), %"PRIu64" elements:\n",table,table->nElements);
  for (i=0;i<table->nElements;i++) {
    list = *((table->array) + i);
    printf("%"PRIu64": ",i);
    if (NULL != list) {
      printList(list);
    } else {
      printf("None\n");
    }
  }
}

#endif

u_int64_t pHashPower[10] = {1,53,2809,148877,7890481,418195493,22164361129,1174711139837,62259690411361,3299763591802133};

int _getHashFromString(char *str) {
  char *s = NULL;
  int i = 0;
  u_int64_t hash = 0;
  for(s = str, i = 0; (*s) && (i<10) ; s++, i++) {
    hash += ((u_int64_t) (*s)) * pHashPower[i];
  }
  return hash;
}

Data *initializeData(char *str, u_int64_t value, Error *error) {
  Data *data = calloc(sizeof(Data),1);
  if (NULL == data) {
    setError(error,MEMORY_ALLOCATE_ERROR,"\0");
    return NULL;
  }
  data->key = calloc(sizeof(char),strlen(str));
  if (NULL == data) {
    setError(error,MEMORY_ALLOCATE_ERROR,"\0");
    return NULL;
  }

  strcpy(data->key,str);
  data->value = value;

#ifdef DEBUG
  printf(BOLD_GREEN"Init Data"STANDART" on (%p)={'%s',%"PRIu64"}\n",data,data->key,data->value);
#endif

  return data;
}

HashTable *initializeHashTable(u_int64_t nElements, Error *error) {
  HashTable *table = (HashTable *) calloc(sizeof(HashTable),1);
  if (NULL == table) {
    setError(error,MEMORY_ALLOCATE_ERROR,"\0");
    return NULL; }

  table->array = (void **) calloc(sizeof(void *),nElements);
  if (NULL == table->array) {
    setError(error,MEMORY_ALLOCATE_ERROR,"\0");
    free(table);
    return NULL; }

  table->nElements = nElements;

#ifdef DEBUG
  printf(BOLD_GREEN"Init HashTable"STANDART" on (%p) by %"PRIu64" elements\n",table,table->nElements);
#endif

  return table;
}


void **_getPtrListFromTableByString(HashTable *table, char *str) {
  return ((table->array) + (_getHashFromString(str) % table->nElements));
}

int _isConcur(void *data, void *str) {
  if (!strcmp(((Data *)data)->key,(char *)str)) {
    return 1;
  } else {
    return 0;
  }
  return 0;
}

Element *_findElementInTableByString(HashTable *table, char *str) {
  return findDataInList(*_getPtrListFromTableByString(table,str),str,_isConcur);
}

Data *getDataFromTableByString(HashTable *table, char *str) {
  Element *elem = _findElementInTableByString(table,str);
#ifdef DEBUG
  printf(BOLD_GREEN"Get Data from HastTable"STANDART" on (%p) by '%s'; Find Element on (%p)\n",table,str,elem);
#endif
  return ((NULL == elem) ? NULL : elem->data);
}

int setDataInTableByString(HashTable *table, char *str, u_int64_t value, Error *error) {
  void **arrayCell = _getPtrListFromTableByString(table,str);
  LinkedList *list = *arrayCell;
  Data *data = NULL;
  if (NULL == list) {
    *arrayCell = initializeList(error);
    list = *arrayCell;
    if (NULL == list) {
      return 1; }
  }

  data = getDataFromTableByString(table,str);

  if (NULL == data) {
    data = initializeData(str,value,error);
    if (NULL == data) {
      return 1; }
    insertDataIntoList(list,data,error);
    if (0 != error->error) {
      return 1; }
  } else {
      data->value = value;
  }

#ifdef DEBUG
  printf(BOLD_GREEN "In HashTable"STANDART" (%p) set data (%p) by '%s': %"PRIu64"\n",table,data,str,data->value);
#endif

  return 0;
}

int deleteDataFromTableByString(HashTable *table, char *str) {
  int result = 0;
  LinkedList **list = (LinkedList **) _getPtrListFromTableByString(table,str);
  result = removeDataFromList(*list,str,_isConcur);
  list = NULL;
#ifdef DEBUG
  printf(BOLD_GREEN"In HashTable "STANDART" (%p) delete Data From Key='%s', result=%d\n",table,str,result);
#endif
  return result;
}
