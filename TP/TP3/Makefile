CC = gcc
CFLAGS = -Wall -g -pedantic


SRCS := $(wildcard *.c)
OBJ := $(SRCS:%.c=%.o)
BINS := exo3-client exo3-serveur

.PHONY: all clean
.PRECIOUS: %.o

all: $(BINS)

%.o: %.c
	@$(CC) -o $*.o $(CFLAGS) -c $*.c

$(BINS): % : %.o fonctions_utiles.o
	@$(CC) -o $* $(CFLAGS) $^
	@echo "Executable crée : $@"

clean:
	@echo "Suppression des fichiers objets : $(OBJ)"
	@rm -f $(OBJ)
	@echo "Suppression des executables : $(BINS)"
	@rm -f $(BINS)