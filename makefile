CC = gcc
PROGRAM = teletype
CFLAGS = -Iinclude -Wall -Wextra -g

_OBJ = main.o libtermio.o teletype.o
_DEPS = libtermio.h teletype.h

SDIR = src
IDIR = include
OBJ = $(_OBJ:%=$(SDIR)/%)
DEPS = $(_DEPS:%=$(IDIR)/%)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(PROGRAM) clean

$(PROGRAM): $(OBJ)
	$(CC) -o tty $^

clean:
	rm -f $(OBJ)

purge:
	rm -f $(OBJ) $(PROGRAM)
