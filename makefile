CC = gcc
PROGRAM = teletype
CFLAGS = -Iinclude -Wall -Wextra

_OBJ = main.o libtermio.o
_DEPS = libtermio.h

SDIR = src
IDIR = include
OBJ = $(_OBJ:%=$(SDIR)/%)
DEPS = $(_DEPS:%=$(IDIR)/%)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(PROGRAM) clean

$(PROGRAM): $(OBJ)
	$(CC) -o $@ $^

clean:
	rm -f $(OBJ)

purge:
	rm -f $(OBJ) $(PROGRAM)
