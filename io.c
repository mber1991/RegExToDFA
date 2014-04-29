#include "io.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


struct IO {
    char *buffer;
};

static const char *prompt = "regextodfa> ";

static void clear_buffer(char *buffer)
{
    if (buffer != NULL) {
        free(buffer);
        buffer = NULL;
    }
}

IO *IO_create(void)
{
    IO *io;
    io = malloc(sizeof(IO));

    if (io != NULL) {
        io->buffer = NULL;
    }

    return io;
}

void IO_destroy(IO *io)
{
    if (io != NULL) {
        clear_buffer(io->buffer);
        free(io);
        io = NULL;
    }
}

const char *IO_read(IO *io)
{
    clear_buffer(io->buffer);

    printf("%s", prompt);

    size_t size;
    size = 0;
    if (getline(&io->buffer, &size, stdin) == -1) {
        return NULL;
    }

    size_t buffer_length = strlen(io->buffer);

    if (io->buffer[buffer_length - 1] == '\n') {
        io->buffer[buffer_length - 1] = '\0';
    }

    return io->buffer;
}

void IO_write(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    vprintf(format, args);

    va_end(args);
}
