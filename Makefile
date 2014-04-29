CFLAGS=-Wall -Wextra -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5 -Wwrite-strings -Waggregate-return -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code -std=gnu99 -pedantic -g
CC=gcc
SOURCES=regextodfa.c dfa.c state.c regex.c parser.c lexer.c list.c token.c build.c io.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=regextodfa

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)
