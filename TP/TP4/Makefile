CC = gcc
CFLAGS = -Wall -g -pedantic


SRCS := $(wildcard *.c)
OBJ := $(SRCS:%.c=%.o)
BINS := exo1

.PHONY: all clean
.PRECIOUS: %.o

all: $(BINS)

%.o: %.c
	@$(CC) -o $*.o $(CFLAGS) -c $*.c

$(BINS): % : %.o fonctions_utiles.o mes_semaphores.o
	@$(CC) -o $* $(CFLAGS) $^
	@echo "Executable(s) crée : $@"

clean:
	@echo "Suppression des fichiers objets s'ils existent : $(OBJ)"
	@rm -f $(OBJ)
	@echo "Suppression des executables s'ils existent : $(BINS)"
	@rm -f $(BINS)