#include "client.h"


int main(int argc, char **argv)
{
    (void) argc;
    (void) argv;

    Client *client;
    client = Client_create();

    Client_run(client);

    Client_destroy(client);

    return 0;
}
