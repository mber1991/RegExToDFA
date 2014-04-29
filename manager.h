#ifndef MANAGER_H
#define MANAGER_H


typedef struct Manager Manager;

extern Manager *Manager_create(void);
extern void Manager_destroy(Manager *manager);

extern void Manager_run(Manager *manager);

#endif
