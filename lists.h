#ifndef _LISTS_H
#define _LISTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct list_el Element;
typedef struct list List, Queue, Stack;

struct list_el
{
	void *data;
	Element *next, *prev;
};

struct list
{
	Element *head, *tail;
	int sz;
};

List *createList();
Queue *createQueue();
Stack *createStack();
Element *elementCreate();
Element *listAppend(List *l, void *data);
Element *listPrepend(List *l, void *data);
Element *listInsert(List *l, void *data, int (*compareTo)(void *, void *));
Element *listRemoveElement(List *l, Element *el);
Element *listRemoveIndex(List *l, int idx);
Element *listRemove(List *l, void *data, int (*compareTo)(void *, void *));
Element *enqueue(Queue *q, void *data);
Element *dequeue(Queue *q);
Element *push(Stack *s, void *data);
Element *pop(Stack *s);

#endif /* LISTS_H */