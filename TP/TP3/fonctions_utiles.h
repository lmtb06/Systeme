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
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <limits.h>

#define CHEMIN_CLE "test"
#define ID_CLE 1
#define TYPE_SERVEUR 1
#define TYPE_CLIENT 2

#define ADDITION 4
#define SOUSTRACTION 3
#define MULTIPLICATION 2
#define DIVISION 1

typedef struct message
{
	long mtype;
	pid_t pid_demandeur;
	int operation;
	int operande1;
	int operande2;
	int valeur;
} message;


/**
 * @brief Ecrit le message et flush dans le flux spécifié ex stdout, stderr, un fichier, etc.
 * @param flux le flux sur lequel écrire.
 * @param message le message à écrire sur le flux.
 * @param ...arguments variatiques utilisés pour le formatage
 * dans le message comme avec printf.
 */
void
print_flush(FILE *flux, char *message, ...);

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

/**
 * @brief Retourne la clé générée à partir de CHEMIN_CLE et ID_CLE
 *
 */
key_t get_cle_commune();

/**
 * @brief Crée une file à partir de la clé. Si la file exist déjà elle va la supprimer si elle a les droits et en créer un nouveau
 *
 * @param cle
 * @return int -1 si la création à échouée sinon l'id de la file
 */
int creer_file(key_t cle);
/**
 * @brief Recupére la file à partir de l'id s'il existe déjà
 *
 * @param cle
 * @return int -1 si la récupération à échoué et l'id de la file si ça a marché
 */
int recuperer_file(key_t cle);

/**
 * @brief renvoie la multiplication d'un entier par 2
 *
 * @param entier l'entier à multiplier
 * @return int l'entier fois 2
 */
int multiplier_par_2(int entier);

/**
 * @brief Attend qu'il y ait un message de type type sur
 * la file d'id id_file et copie le message dans message
 *	cette fonction est quasi un wrapper
 * @param id_file l'identifiant de la file
 * @param message le pointeur vers le message sur lequel copier les données
 * @param type le type de message attendu
 * @return 0 si succés de la reception, -1 sinon
 */
int recevoir_message(int id_file, message *message, long type);

/**
 * @brief Envoie un message de type type sur la file d'id id_file
 *
 * @param id_file l'id de la file
 * @param message le pointeur vers le message à envoyer
 * @param type le type du message
 * @return int 0 si succés de l'envoie, -1 sinon
 */
int envoyer_message(int id_file, message *message, long type);

/**
 * @brief donne la somme de a et b
 *
 * @param a la première opérande
 * @param b la deuxieme opérande
 * @return int la somme de a et b
 */
int addition(int a, int b);
/**
 * @brief donne la soustraction de a par b
 *
 * @param a la première opérande
 * @param b la deuxieme opérande
 * @return int la soustraction de a par b
 */
int soustraction(int a, int b);
/**
 * @brief donne la multiplication de a et b
 *
 * @param a la première opérande
 * @param b la deuxieme opérande
 * @return int la multiplication de a et b
 */
int multiplication(int a, int b);
/**
 * @brief donne la division entiére de a par b
 *
 * @param a la première opérande
 * @param b la deuxieme opérande
 * @return int la division entiére de a par b
 */
int division_entiere(int a, int b);
// TODO Mettre à jour les opérations si je trouve un bithack
#endif