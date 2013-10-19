#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct _Element {
  struct _Element *next;
  void *data;
} Element;

typedef struct {
  Element *root;
  Element *end;
} LinkedList;

typedef struct {
  int error;
  char str[100];
} Error;

#define MEMORY_ALLOCATE_ERROR (72100)
#define LINK_ERROR (72101)

#endif // LINKED_LIST_H
