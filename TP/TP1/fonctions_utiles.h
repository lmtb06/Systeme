#ifndef FONCTIONS_UTILES_H
#define FONCTIONS_UTILES_H

#include <stdio.h>
#include <stdlib.h>

// affiche un message sur la sortie standart directement sans que le buffer
// soit plein
void print_flush(char *message);

// affiche le message "message" puis l'interpretation du message dans errno
// puis quitte le programme avec le exit_status défini
void print_error_and_exit(char *message, int exit_status);

// quitte le programme avec un message d'erreur si value n'est pas
// égale à zéro
void exit_if_return_not_0(int value, char* message);

//
void creer_fils_puis_exec();

#endif