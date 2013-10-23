#include "linked_list.h"
#include "hash_table.h"
#include <stdio.h>
#include <string.h>

int main(void)
{
  Error error;
  HashTable *table;
  Data *data = NULL;
  error.error = 0;
  strcpy(&error.error,"\0");

  table = initializeHashTable(5,&error);

  setDataInTableByString(table,"abc",10,&error);
  setDataInTableByString(table,"dcd",5,&error);
  setDataInTableByString(table,"aaab",1,&error);
  setDataInTableByString(table,"abc",7,&error);
  printHashTable(table);puts("====");
  data = getDataFromTableByString(table,"abc");
  puts("====");
  if (NULL != data) {
  printf("Data(%p):Key='%s',val=(%lli)\n",data,data->key,data->value);
  } else {
    printf("Not fuond!\n");
  }
  deleteDataFromTableByString(table,"dcd");
  printHashTable(table);puts("====");
  return 0;
}

