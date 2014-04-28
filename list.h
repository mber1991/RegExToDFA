#ifndef LIST_H
#define LIST_H

#include <stddef.h>


typedef struct Node Node;

typedef struct List List;

typedef void (*Destructor)(void *object);

extern Node *Node_create(void *data);
extern void Node_destroy(Node *n, Destructor destructor);

extern List *List_create(void);
extern void List_destroy(List *l, Destructor destructor);

extern void List_push_back(List *l, void *data);

extern void *List_get_data(List *l, unsigned int index);

extern void List_clear(List *list, Destructor destructor);

extern size_t List_get_size(List *list);



extern void List_to_string(List *l);

#endif
