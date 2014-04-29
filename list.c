#include "list.h"

#include <stdlib.h>
#include <stdio.h>

#include "token.h"


struct Node {
    Node *next;
    void *data;
};

struct List {
    Node *head;
};

static Node *end(Node *n)
{
    if (n == NULL) {
        return NULL;
    }

    Node *temp = n;

    while (temp->next != NULL) {
        temp = temp->next;
    }

    return temp;
}

static Node *get_node_at_index(Node *n, unsigned int index)
{
    if (n == NULL) {
        return NULL;
    }

    unsigned int i;
    i = 0;

    Node *temp = n;

    while (temp->next != NULL) {
        if (i == index) {
            return temp;
        }

        temp = temp->next;
        ++i;
    }

    if (i == index) {
        return temp;
    }

    return NULL;
}

Node *Node_create(void *data)
{
    Node *node = malloc(sizeof(*node));

    if (node != NULL) {
        node->next = NULL;
        node->data = data;
    }

    return node;
}

void Node_destroy(Node *node, Destructor destructor)
{
    if (node != NULL) {
        if (node->data != NULL) {
            destructor(node->data);
        }

        free(node);
        node = NULL;
    }
}

List *List_create(void)
{
    List *list = malloc(sizeof(List));

    if (list != NULL) {
        list->head = NULL;
    }

    return list;
}

void List_destroy(List *list, Destructor destructor)
{
    if (list != NULL) {
        // to_string(list);

        while (list->head != NULL) {
            Node *n = list->head->next;
            Node_destroy(list->head, destructor);
            list->head = n;
        }

        free(list);
        list = NULL;
    }
}

void List_push_back(List *l, void *data)
{
    if (l == NULL || data == NULL) {
        return;
    }

    Node *n;
    n = Node_create(data);
    if (n == NULL) {
        return;
    }

    if (l->head == NULL) {
        l->head = n;
        return;
    }

    end(l->head)->next = n;
}

void *List_get_data(List *l, unsigned int index)
{
    if (l == NULL || l->head == NULL) {
        return NULL;
    }

    Node *node;
    node = get_node_at_index(l->head, index);

    if (node != NULL) {
        if (node->data != NULL) {
            return node->data;
        }
    }

    return NULL;
}
