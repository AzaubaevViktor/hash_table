#include "linked_list.h"
#include "hash_table.h"
#include "errors.h"

#include <stdlib.h>
#include <string.h>

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
  if (NULL != data) {
    setError(error,MEMORY_ALLOCATE_ERROR,"\0");
    return NULL;
  }
  strcpy(data->key,str);
  data->value = value;

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

  return table;
}


void **_getPtrListFromTableByString(HashTable *table, char *str) {
  return (*(table->array) + (_getHashFromString(str) % table->nElements));
}

/* Установить значение list["abs"] = 4
 * Получить значение:
 * Найти, вырвать данные
 *
 * Для общего развития:
 * Создание новой ячейки
 * Удаление ячейки
 */

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
  return ((NULL == elem) ? NULL : elem->data);
}

int setDataInTableByString(HashTable *table, char *str, u_int64_t value, Error *error) {
  LinkedList **list = (LinkedList **) _getPtrListFromTableByString(table,str);
  Data *data = NULL;
  if (NULL == *list) {
    *list = initializeList(error);
    if (NULL == *list) {
      return 1; }
  }

  data = getDataFromTableByString(table,str);

  if (NULL == data) {
    data = initializeData(str,value,error);
    if (NULL == data) {
      return 1; }
    insertDataIntoList(*list,data,error);
    if (0 != error->error) {
      return 1; }
  } else {
      data->value = value;
  }

  return 0;
}

int deleteDataFromTableByString(HashTable *table, char *str) {
  int result = 0;
  LinkedList **list = (LinkedList **) _getPtrListFromTableByString(table,str);
  result = removeDataFromList(*list,str,_isConcur);
  list = NULL;
  return result;
}
