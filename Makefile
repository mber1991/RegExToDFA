CFLAGS= -Wall -Wextra -Wfloat-equal -Wundef -Wshadow -Wpointer-arith	\
	-Wcast-align -Wstrict-prototypes -Wstrict-overflow=5		\
	-Wwrite-strings -Waggregate-return -Wcast-qual -Wswitch-default \
	-Wswitch-enum -Wconversion -Wunreachable-code			\
	-std=gnu99 -pedantic -g						\
	-O3
CC=gcc
SOURCES=$(wildcard src/*.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))
EXECUTABLE=regextodfa

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)
