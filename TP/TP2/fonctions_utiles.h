#ifndef FONCTIONS_UTILES_H
#define FONCTIONS_UTILES_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <stdarg.h>
#include <string.h>

/**
 * @brief Ecrit le message et flush dans le flux spécifié ex stdout, stderr, un fichier, etc.
 * @param flux le flux sur lequel écrire.
 * @param message le message à écrire sur le flux.
 * @param ...arguments variatiques utilisés pour le formatage
 * dans le message comme avec printf.
 */
void print_flush(FILE *flux, char *message, ...);

/**
 * @brief Ecrit le message avec l'interpretation du code dans errno sur le flux défini et quitte le programme avec le code de retour défini
 *
 * @param flux le flux sur lequel écrire
 * @param exit_status le code de retour
 * @param message le message à écrire
 * @param ...arguments variatiques utilisés pour le formatage
 * dans le message comme avec printf.
 */
void print_erreur_et_exit(FILE *flux, int exit_status, char *message, ...);

/**
 * @brief Quitte le programme en écrivant sur le flux le message si valeur et valeur_attendue sont différents
 *
 * @param valeur
 * @param valeur_attendue
 * @param flux le flux sur lequel écrire
 * @param exit_status le code de retour du programme
 * @param message le message à écrire
 * @param ...arguments variatiques utilisés pour le formatage
 * dans le message comme avec printf.
 */
void exit_si_different(int valeur, int valeur_attendue, FILE *flux, int exit_status, char *message, ...);

/**
 * @brief Crée un fils puis fait un recouvrement
 *
 * @param max_essais le nombre maximum d'essais de fork sans succés
 * @param temps_attente_entre_fork le temps d'attente entre chaque fork non réussi en usecondes
 * @param chemin le chemin de l'executable utilisé pour le recouvrement
 * @param nom le nom du processus
 * @param ... arguments du programme
 * @return pid_t le pid du fils si le recouvrement à réussi et -1 sinon
 */
pid_t creer_fils_exec(size_t max_essais, unsigned int temps_attente_entre_fork, char *chemin, char *nom, ...);

#endif