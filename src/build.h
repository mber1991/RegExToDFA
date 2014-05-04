#ifndef BUILD_H
#define BUILD_H

#include <stddef.h>

#include "list.h"


extern void build_dfa(List *tokens, List **groups, size_t group_count);

#endif
