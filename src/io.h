#ifndef IO_H
#define IO_H

#include <stdarg.h>


typedef struct IO IO;

extern IO *IO_create(const char *prompt);
extern void IO_destroy(IO *io);

extern const char *IO_read(IO *io, const char *prompt);

extern void IO_write(const char *format, ...);

#endif
