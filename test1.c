#include "linked_list.h"
#include "hash_table.h"
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

int main(void)
{
  Error error;
  HashTable *table;
  Data *data = NULL;
  error.error = 0;
  strcpy((char *)(error.str),"\0");

  table = initializeHashTable(2,&error);

  setDataInTableByString(table,"abc",10,&error);
  puts("====");printHashTable(table);puts("====");
  setDataInTableByString(table,"aaaaaaaaaaaaaa",5,&error);
  puts("====");printHashTable(table);puts("====");
  setDataInTableByString(table,"abd",1,&error);
  puts("====");printHashTable(table);puts("====");
  setDataInTableByString(table,"abc",7,&error);
  puts("====");printHashTable(table);puts("====");
  printHashTable(table);puts("====");
  puts("====");printHashTable(table);puts("====");
  data = getDataFromTableByString(table,"abc");
  puts("====");printHashTable(table);puts("====");
  puts("====");
  if (NULL != data) {
    printf("Data(%p):Key='%s',val=(%"PRIu64")\n",data,data->key,data->value);
  } else {
    printf("Not fuond!\n");
  }
  deleteDataFromTableByString(table,"abc");
  puts("====");printHashTable(table);puts("====");
  setDataInTableByString(table,"abc",7,&error);
  puts("====");printHashTable(table);puts("====");
  setDataInTableByString(table,"olololololololololololol",1000000,&error);
  puts("====");printHashTable(table);puts("====");
  setDataInTableByString(table,"AhTiZEbanyTiNahui",70000000000,&error);
  puts("====");printHashTable(table);puts("====");
  data = getDataFromTableByString(table,"AhTiZEbanyTiNahui");
  puts("====");printHashTable(table);puts("====");
  puts("====");
  if (NULL != data) {
    printf("Data(%p):Key='%s',val=(%"PRIu64")\n",data,data->key,data->value);
  } else {
    printf("Not fuond!\n");
  }
  return 0;
}

