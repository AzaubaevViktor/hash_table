#include "linked_list.h"
#include "errors.h"
#include <string.h>
#include <stdlib.h>

#ifdef DEBUG_LINKED_LIST
#include <stdio.h>
#include <inttypes.h>

#define STANDART "\x1b[0m"
#define BOLD_WHITE "\x1b[1;37m"
#define BOLD_YELLOW "\x1b[1;33m"
#define BOLD_GREEN "\x1b[1;32m"

void printList(LinkedList *list) {
  Element *now = NULL;
  printf(BOLD_WHITE "List" STANDART " on (%p), [r->(%p),e->(%p),nColl=%"PRIu64"]:\n",list,list->root,list->end,list->nCollision);
  for(now = list->root; NULL != now; now = now->next) {
    printf("[%p] (%p)" BOLD_WHITE " -> " STANDART,now->data,now->next);
  }
  printf("NULL\n");
}

#endif

Element *initializeElement(void *data, Error *error) {
  Element *elem = calloc(sizeof(Element),1);
  if (NULL == elem) {
    setError(error,MEMORY_ALLOCATE_ERROR,"\0");
    return NULL; }
  elem->data = data;
#ifdef DEBUG_LINKED_LIST
  printf(BOLD_WHITE "InitializeElement" STANDART " on (%p)\n", elem);
#endif
  return elem;
}

int _bindElements(Element *parent, Element *children, Error *error) {
  if (NULL != parent->next) {
    setError(error,LINK_ERROR,"\0");
    return 1; }
  parent->next = children;
#ifdef DEBUG_LINKED_LIST
  printf(BOLD_WHITE "Binding elements" STANDART " on (%p) -> (%p)\n",parent, children);
#endif
  return 0;
}

LinkedList *initializeList(Error *error) {
  LinkedList *list = calloc(sizeof(LinkedList),1);
  if (NULL == list) {
    setError(error,MEMORY_ALLOCATE_ERROR,"\0");
    return NULL; }
#ifdef DEBUG_LINKED_LIST
  printf(BOLD_WHITE "InitializeList" STANDART " on (%p)\n", list);
#endif
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
  list->nCollision++;
#ifdef DEBUG_LINKED_LIST
  printf(BOLD_WHITE "Insert element into list" STANDART " (%p), root:(%p), end:(%p), nCollisioon=%"PRIu64"\n",list,list->root,list->end,list->nCollision);
#endif
  return 0;
}

int insertDataIntoList(LinkedList *list, void *data, Error *error) {
  Element *elem = initializeElement(data,error);
  if (0 != error->error) {
    return 1; }
#ifdef DEBUG_LINKED_LIST
  printf(BOLD_WHITE"Insert data into list"STANDART" on (%p), data on (%p)\n",list,data);
#endif
  return _insertElementIntoList(list,elem,error);
}

Element *findDataInList(LinkedList *list, void *data, int (*isConcur)(void *, void *)) {
  Element *now = NULL;
#ifdef DEBUG_LINKED_LIST
  printf(BOLD_WHITE "Find data in List" STANDART " on (%p), query=(%s)\n",list,(char *)data);
#endif
  if (NULL != list->root) {
    for (now = list->root; 0 != now; now = now->next) {
      if ((*isConcur)(now->data,data)) {
        return now;
      }
    }
  }
  return NULL;
}

void _deleteElement(Element **elem) {
  if (NULL != *elem) {
    free((*elem)->data);
    free(*elem); }
}

int removeDataFromList(LinkedList *list, void *data, int (*isConcur)(void *, void *)) {
  Element *now = NULL, *prev = NULL;
  int isNowFirstElement, isNowLastElement,isNullElementInList;
  isNullElementInList = (NULL == list->root);

#ifdef DEBUG_LINKED_LIST
  printf(BOLD_WHITE "Remove data from list" STANDART " on (%p), query='%s'\n",list,(char *)data);
#endif

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
            _deleteElement(&now);
            break;
          case 1: /* ---PN |=> ---P */
            list->end = prev;
            prev->next = NULL;
            _deleteElement(&now);
            break;
          }
          break;
        case 1:
          switch (isNowLastElement) {
          case 0: /* N---- |=> ---- */
            list->root = now->next;
            _deleteElement(&now);
            break;
          case 1: /* N |=> NULL */
            list->root = NULL;
            list->end = NULL;
            _deleteElement(&now);
            break;
          }
          break;
        }

        list->nCollision--;

        return 0;

      }
    }
  }
  return 1;
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
