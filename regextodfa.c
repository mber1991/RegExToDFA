#include "manager.h"


int main(int argc, char **argv)
{
    (void) argc;
    (void) argv;

    Manager *manager;
    manager = Manager_create();

    Manager_run(manager);

    Manager_destroy(manager);

    return 0;
}
