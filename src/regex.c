#include "regex.h"

#include <stdlib.h>
#include <string.h>


struct Regex {
    char *value;
    size_t length;
};

Regex *Regex_create(const char *value)
{
    Regex *regex = malloc(sizeof(Regex));

    if (regex != NULL) {
        size_t length = strlen(value);

        regex->value = malloc(sizeof(char) * (length + 1));
        if (regex->value != NULL) {
            unsigned int i;
            for (i = 0; i < length; ++i) {
                regex->value[i] = value[i];
            }
            regex->value[length] = '\0';
        }

        regex->length = length;
    }

    return regex;
}

void Regex_destroy(Regex *regex)
{
    if (regex != NULL) {
        if (regex->value != NULL) {
            free(regex->value);
            regex->value = NULL;
        }

        free(regex);
        regex = NULL;
    }
}

const char *Regex_get_value(Regex *regex)
{
    if (regex == NULL) {
        return NULL;
    }

    return regex->value;
}

size_t Regex_get_length(Regex *regex)
{
    if (regex == NULL) {
        return 0;
    }

    return regex->length;
}

void Regex_set_value(Regex *regex, const char *value)
{
    if (regex == NULL) {
        return;
    }

    size_t length = strlen(value);

    regex->value = realloc(regex->value, sizeof(char) * (length + 1));
    if (regex->value != NULL) {
        unsigned int i;
        for (i = 0; i < length; ++i) {
            regex->value[i] = value[i];
        }
        regex->value[length] = '\0';
    }

    regex->length = length;
}
