CC = gcc
CFLAGS = -Wall -g -pedantic
DEPENDANCES = fonctions_utiles.h
OBJ = fonctions_utiles.o

all: exo1_tp1 exo2_tp1 Pere Timer cleano
.PHONY: clean cleano

%.o: %.c $(DEPENDANCES)
	$(CC) -o $@ $(CFLAGS) -c $<

exo1_tp1: exo1_tp1.o $(OBJ)
	$(CC) -o $@ $(CFLAGS) $^

exo2_tp1: exo2_tp1.o $(OBJ)
	$(CC) -o $@ $(CFLAGS) $^

Pere: exo3_tp1.o $(OBJ) recouvrant
	$(CC) -o $@ $(CFLAGS) exo3_tp1.o $(OBJ)

recouvrant: exo3_fils_tp1.o $(OBJ)
	$(CC) -o $@ $(CFLAGS) $^

Timer: exo4_tp1.o $(OBJ) fils_timer
	$(CC) -o $@ $(CFLAGS) exo4_tp1.o $(OBJ)

fils_timer: exo4_fils_tp1.o $(OBJ)
	$(CC) -o $@ $(CFLAGS) $^

clean: cleano
	rm -f exo1_tp1 exo2_tp1 Pere recouvrant Timer fils_timer

cleano:
	rm -f *.o