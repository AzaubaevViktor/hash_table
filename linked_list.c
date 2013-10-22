#include "linked_list.h"
#include "errors.h"
#include <string.h>
#include <stdlib.h>

Element *initializeElement(void *data, Error *error) {
  Element *elem = calloc(sizeof(Element),1);
  if (NULL == elem) {
    setError(error,MEMORY_ALLOCATE_ERROR,"\0");
    return NULL; }
  elem->data = data;
  return elem;
}

int _bindElements(Element *parent, Element *children, Error *error) {
  if (NULL != parent->next) {
    setError(error,LINK_ERROR,"\0");
    return 1; }
  parent->next = children;
  return 0;
}

LinkedList *initializeList(Error *error) {
  LinkedList *list = calloc(sizeof(LinkedList),1);
  if (NULL != list) {
    setError(error,MEMORY_ALLOCATE_ERROR,"\0");
    return NULL; }

  return list;
}

int _insertElementIntoList(LinkedList *list, Element *element, Error *error) {
  if (NULL == list->end) {
    list->root = element;
    list->end = element;
  } else {
    _bindElements(list->end,element,error);
    if (0 != error->error) {
      return 1; }
    list->end = element;
  }
  return 0;
}

int insertDataIntoList(LinkedList *list, void *data, Error *error) {
  Element *elem = initializeElement(data,error);
  if (0 != error->error) {
    return 1; }
  return _insertElementIntoList(list,elem,error);
}

Element *findDataInList(LinkedList *list, void *data, int (*isConcur)(void *, void *)) {
  Element *now = NULL;
  if (NULL != list->root) {
    for (now = list->root; 0 != now; now = now->next) {
      if ((*isConcur)(now->data,data)) {
        return now;
      }
    }
  }
  return NULL;
}

int removeDataFromList(LinkedList *list, void *data, int (*isConcur)(void *, void *)) {
  Element *now = NULL, *prev = NULL;
  int isNowFirstElement, isNowLastElement,isNullElementInList;
  isNullElementInList = (NULL == list->root);

  if (!isNullElementInList) {
    for (now = list->root; NULL != now; prev = now, now = now->next) {
      if ((*isConcur)(now->data,data)) {

        isNowFirstElement = (NULL == prev);
        isNowLastElement = (NULL == now->next);

        switch (isNowFirstElement) {
        case 0:
          switch (isNowLastElement) {
          case 0: /* --PN-- |=> --P-- */
            prev->next = now->next;
            free(now->data);
            free(now);
            break;
          case 1: /* ---PN |=> ---P */
            list->end = prev;
            prev->next = NULL;
            free(now->data);
            free(now);
            break;
          }
          break;
        case 1:
          switch (isNowLastElement) {
          case 0: /* N---- |=> ---- */
            list->root = now->next;
            free(now->data);
            free(now);
            break;
          case 1: /* N |=> NULL */
            list->root = NULL;
            list->end = NULL;
            free(now->data);
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

void _deleteElement(Element **elem) {
  if (NULL != *elem) {
    free((*elem)->data);
    free(*elem); }
}

void deleteList(LinkedList **list) {
  Element *now = NULL, *next = NULL;
  for (now = (*list)->root; NULL != now ;) {
    next = now->next;
    _deleteElement(&now);
    now = next;
  }
  free(*list);
}
