#ifndef CLIENT_H
#define CLIENT_H


typedef enum COMMAND {
    COMMAND_UNKNOWN,
    COMMAND_DFA,
    COMMAND_HELP,
    COMMAND_QUIT,
} COMMAND;

typedef struct Client Client;

extern Client *Client_create(void);
extern void Client_destroy(Client *client);

extern void Client_run(Client *client);

#endif
