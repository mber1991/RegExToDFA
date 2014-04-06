#ifndef REGEX_H
#define REGEX_H

#include <stddef.h>


typedef struct Regex Regex;

extern Regex *Regex_create(const char *value);
extern void Regex_destroy(Regex *regex);

extern const char *Regex_get_value(Regex *regex);
extern size_t Regex_get_length(Regex *regex);

extern void Regex_set_value(Regex *regex, const char *value);

#endif
