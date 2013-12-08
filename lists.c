#include "lists.h"

/* ------------------------------------
WE WILL NOT BE PRINTING OUT ERROR MESSAGES IF PARAMS ARE INVALID
WE WILL SIMPLY RETURN NULL, AND IT IS UP TO THE PROGRAMMER TO
CHECK THE RETURN VALUES AND INTERPRET ERRORS

THIS IS TO LET THE USER HAVE CONTROL OVER HOW HE USES THE FUNCTIONS
AND HE CAN INTERPRET THE RESULTS HIMSELF
--------------------------------------- */

/* create, initialize, and return the list structure.
If you pass a NULL comparator function, the list will
not be sorted. If you pass a comparator function, you
will be able to use listInsert which inserts in order */
List *createList(int (*compareTo) (void *, void *))
{
	List *l = malloc(sizeof(List));

	if(l == NULL)
	{
		fprintf(stderr, "Out of memory\n");
		return NULL;
	}

	/* initialize all fields to be 0/null */
	memset(l, 0, sizeof(List));

	l->compareTo = compareTo;

	return l;
}

/* allow them to create a priorityQueue by specifying
a function to determine priority order */
Queue *createPriorityQueue(int (*compareTo) (void *, void *))
{
	/* createPriorityQueue(NULL) is the same as createQueue() */
	return createList(compareTo);
}

/* just a wrapper since queues are implemented with lists, but
abstract that from the programmer. pass NULL to createlist
because we won't be sorting it */
Queue *createQueue()
{
	return createList(NULL);
}

/* also abstract away the fact a stack is a list. Use NULL
for createList because it won't be sorted */
Stack *createStack()
{
	return createList(NULL);
}

/* create and initialize the element structure */
Element *elementCreate()
{
	Element *el = malloc(sizeof(Element));

	if(el == NULL)
	{
		fprintf(stderr, "Out of memory\n");
		return NULL;
	}

	/* initialize to 0/NULL */
	memset(el, 0, sizeof(Element));

	return el;
}

/* append a "datum" to the end of the list */
Element *listAppend(List *l, void *data)
{
	if(l == NULL || data == NULL)
		return NULL;

	Element *el = elementCreate();

	if(el == NULL)
		return NULL;

	el->data = data;

	/* if sz>0 then tail is not null */
	if(l->sz++ > 0)
	{
		l->tail->next = el;
		el->prev = l->tail;
	}
	else /* if size was 0 it is both head and tail */
		l->head = el;

	l->tail = el;

	return el;
}

/* prepend the datum to the start of the list */
Element *listPrepend(List *l, void *data)
{
	if(l == NULL || data == NULL)
		return NULL;

	Element *el = elementCreate();
	
	if(el == NULL)
		return NULL;

	el->data = data;

	/* if sz>0 then head is not null */
	if(l->sz++ > 0)
	{
		l->head->prev = el;
		el->next = l->head;
	}
	else /* otherwise no elements so it's both head and tail */
		l->tail = el;

	l->head = el;

	return el;
}

/* pass the list, the data to be inserted, and a function to compare
elements (to determine order) and it will insert it. */
Element *listInsert(List *l, void *data)
{
	/* if list compareTo is null, we can't insert in any
	meaningful way */
	if(l == NULL || data == NULL || l->compareTo == NULL)
		return NULL;
	
	/* if there are no elements, or I should go before the first
	element, then listPrepend it */
	if(l->sz == 0 || l->compareTo(data, l->head->data) < 0)
		return listPrepend(l, data);

	Element *el = elementCreate();

	if(el == NULL)
		return NULL;

	el->data = data;

	Element *iter = l->head;
	while(iter != NULL)
	{
		/* break when iter points to the item right before where we
		will insert */
		if(iter->next == NULL || l->compareTo(data, iter->next->data) < 0)
			break;

		iter = iter->next;
	}

	/* iter points to the item before where we insert */
	if(iter->next != NULL)
		iter->next->prev = el;

	el->next = iter->next;
	iter->next = el;
	el->prev = iter;

	/* if this is essentially an append, then redirect the tail */
	if(l->tail == iter)
		l->tail = el;

	l->sz++;

	return el;
}

/* removes a specified element from the list */
Element *listRemoveElement(List *l, Element *el)
{
	if(l == NULL || el == NULL)
		return NULL;
	
	/* adjust if it was the head or the tail */
	if(l->head == el)
		l->head = el->next;

	if(l->tail == el)
		l->tail = el->prev;

	/* make pointers point around it */
	if(el->next != NULL)
		el->next->prev = el->prev;

	if(el->prev != NULL)
		el->prev->next = el->next;

	/* reduce size of list */
	l->sz--;

	return el;
}

/* indexing in the list starts at 1, ends at l->sz
to remove the head, pass in 1, to remove the tail, pass in l->sz */
Element *listRemoveIndex(List *l, int idx)
{
	if(l == NULL || idx < 1 || idx > l->sz)
		return NULL;
	
	Element *tmp;
	int count = 1;

	/* see if idx is closer to the head or the tail for
	faster searching on large lists */
	if(idx-1 <= l->sz-idx)
	{
		/* if less than or equal, start from head */
		tmp = l->head;

		while(count++ <= idx-1)
			tmp = tmp->next;
	}
	else
	{
		tmp = l->tail;

		while(count++ <= l->sz-idx)
			tmp = tmp->prev;
	}

	/* now tmp will point to our element */

	return listRemoveElement(l, tmp);
}

/* removes an element with the same data as *data from the list */
Element *listRemove(List *l, void *data)
{
	if(l == NULL || data == NULL)
		return NULL;

	Element *el = l->head;

	while(el != NULL)
	{
		/* !compareTo means == 0 which means they are the same. also if
		they still have the same memory address, then we will accept it
		as being a match - Useful if it isn't sorted */
		if(el->data == data || (l->compareTo != NULL && !l->compareTo(data, el->data)))
			return listRemoveElement(l, el);

		el = el->next;
	}

	/* don't print error, let them decide if it was an error */
	return NULL;
}

/* enqueue appends to the end of a list always */
Element *enqueue(Queue *q, void *data)
{
	if(q == NULL || data == NULL)
		return NULL;

	/* if compareTo function is null, treat as regular queue */
	if(q->compareTo == NULL)
		return listAppend(q, data);
	else
		return listInsert(q, data);
}

/* dequeue the leading element in a list */
Element *dequeue(Queue *q)
{
	if(q == NULL)
		return NULL;

	/* always dequeue the first element */
	return listRemoveElement(q, q->head);
}

/* push to the end of the list */
Element *push(Stack *s, void *data)
{
	if(s == NULL || data == NULL)
		return NULL;

	return listAppend(s, data);
}

/* pop from the end of the list as well */
Element *pop(Stack *s)
{
	if(s == NULL)
		return NULL;

	return listRemoveElement(s, s->tail);
}

/* check if a list (or queue or stack) is empty */
int isEmpty(List *l)
{
	if(l->sz == 0)
		return 1;
	else return 0;
}