CC = gcc
CFLAGS = -Wall -g -pedantic


SRCS := $(wildcard *.c)
OBJ := $(SRCS:%.c=%.o)
BINS := $(SRCS:%.c=%)

.PHONY: all clean
.PRECIOUS: %.o

all: $(BINS)

%.o: %.c
	$(CC) -o $*.o $(CFLAGS) -c $*.c

%.exec:  %.o fonctions_utiles.o
	$(CC) -o $* $(CFLAGS) $^

%pere.exec: %fils.exec %pere.o fonctions_utiles.o
	$(CC) -o $*pere $(CFLAGS) $*pere.o fonctions_utiles.o

clean:
	rm -f $(OBJ) $(BINS)