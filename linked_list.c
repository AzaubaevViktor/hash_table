#include "linked_list.h"
#include <string.h>
#include <stdlib.h>

void setError(Error *error, int errorValue, char *str);

Element *initializeElement(void *data, Error *error) {
  Element *elem = calloc(sizeof(Element),1);
  if (NULL == elem) {
    setError(error,MEMORY_ALLOCATE_ERROR,"\0");
    return NULL; }
  elem->data = data;
  return elem;
}

void _bind_elements(Element *parent, Element *children, Error *error) {
  if (NULL != parent->next) {
    setError(error,LINK_ERROR,"\0");
    return; }
  parent->next = children;
}

LinkedList *initialize_list(Error *error) {
  LinkedList *list = calloc(sizeof(LinkedList),1);
  if (NULL != list) {
    setError(error,MEMORY_ALLOCATE_ERROR,"\0");
    return NULL; }

  list->root = initializeElement(NULL,error);

  if (0 != error) {
    setError(error,MEMORY_ALLOCATE_ERROR,"\0");
    return NULL; }

  list->end = list->root;

  return list;
}

void insertNewElementToList(LinkedList *list, Element *element, Error *error) {
  _bind_elements(list->end,element,error);
  if (0 != error->error) {
    return; }
  list->end = element;
}

Element *findDataInList(LinkedList *list,int (*isConcur)(void *)) {
  Element *now = NULL;
  if (NULL != list->root) {
    for (now = list->root; 0 != now; now = now->next) {
      if ((*isConcur)(now->data)) {
        return now;
      }
    }
  }
  return NULL;
}

int deleteElementFromList(LinkedList *list, int (*isConcur)(void *)) {
  Element *now = NULL, *prev = NULL;
  int isNowFirstElement, isNowLastElement,isNullElementInList;
  isNullElementInList = (NULL == list->root);

  if (!isNullElementInList) {
    for (now = list->root; NULL != now; prev = now, now = now->next) {
      if ((*isConcur)(now->data)) {

        isNowFirstElement = (NULL == prev);
        isNowLastElement = (NULL == now->next);

        switch (isNowFirstElement) {
        case 0:
          switch (isNowLastElement) {
          case 0: /* --PN-- |=> --P-- */
            prev->next = now->next;
            free(now);
            break;
          case 1: /* ---PN |=> ---P */
            list->end = prev;
            prev->next = NULL;
            free(now);
            break;
          }
          break;
        case 1:
          switch (isNowLastElement) {
          case 0: /* N---- |=> ---- */
            list->root = now->next;
            free(now);
            break;
          case 1: /* N |=> NULL */
            list->root = NULL;
            list->end = NULL;
            free(now);
            break;
          }
          break;
        }

        return 0;

      }
    }
  }
  return 1;
}

void setError(Error *error, int errorValue, char *str) {
  error->error = errorValue;
  strcpy(error->str,str);
}
