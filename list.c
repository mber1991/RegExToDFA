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

static void to_string(List *l)
{
    if (l == NULL) {
        return;
    }

    Node *temp = l->head;

    while (temp != NULL) {
        if (temp->data != NULL) {
            Token *token = temp->data;
            if (token->value != NULL) {
                printf("\"%s\", at: %u",
                       token->value,
                       token->begin);

                if (token->begin != token->end) {
                    printf("-%u", token->end);
                }

                switch (token->type) {
                    case TOKEN_SYMBOL:
                        printf(", type: Symbol");
                        break;
                    case TOKEN_LITERAL:
                        printf(", type: Literal");
                        break;
                    case TOKEN_ESCAPE_CHAR:
                        printf(", type: Escape Character");
                        break;
                    case TOKEN_UNKNOWN:
                        printf(", type: Unknown");
                        break;
                    default:
                        break;
                }
                printf("\n");
            }
        }

        if (temp->next == NULL) {
            break;
        }

        temp = temp->next;
    }

    printf("\n");
}

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

Node *Node_create(void *data)
{
    Node *node = malloc(sizeof(*node));

    if (node != NULL) {
        node->next = NULL;
        node->data = data;
    }

    return node;
}

void Node_destroy(Node *node)
{
    if (node != NULL) {
        if (node->data != NULL) {
            Token *token = node->data;
            if (token->value != NULL) {
                free(token->value);
                token->value = NULL;
            }
            free(token);
            token = NULL;
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

void List_destroy(List *list)
{
    if (list != NULL) {
        to_string(list);

        while (list->head != NULL) {
            Node *n = list->head->next;
            Node_destroy(list->head);
            list->head = n;
        }

        free(list);
        list = NULL;
    }
}

void List_push_back(List *l, Node *n)
{
    if (l == NULL || n == NULL) {
        return;
    }

    if (l->head == NULL) {
        l->head = n;
        return;
    }

    end(l->head)->next = n;
}
