#ifndef LIST_H
#define LIST_H


typedef struct Node Node;

typedef struct List List;

extern Node *Node_create(void *data);
extern void Node_destroy(Node *n);

extern List *List_create(void);
extern void List_destroy(List *l);

extern void List_push_back(List *l, Node *n);

extern void *List_get_data(List *l, unsigned int index);

#endif
