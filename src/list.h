#ifndef LIST_H
#define LIST_H


typedef struct Node Node;

typedef struct List List;

typedef void (*Destructor)(void *object);

extern Node *Node_create(void *data);
extern void Node_destroy(Node *n, Destructor destructor);

extern List *List_create(void);
extern void List_destroy(List *l, Destructor destructor);

extern void List_push_back(List *l, void *data);

extern void *List_get_data(List *l, unsigned int index);

#endif
