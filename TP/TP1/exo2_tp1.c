#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include "fonctions_utiles.h"

// Handler pour les signaux SIGALRM
void handler_alarm(int sig)
{
	// Prévient l'utilisateur qu'il a perdu et quitte le programme
	if (sig == SIGALRM)
	{
		print_flush("Perdu temps écoulé\n");
		exit(EXIT_SUCCESS);
	}
}

// Handler pour les signaux SIGINT
void handler_interuption(int sig)
{

	// arrête l'alarme et affiche le temps restant
	unsigned int temps_restant = alarm(0);
	if (sig == SIGINT)
	{
		printf("\nTemps restant: %us\n", temps_restant);
		fflush(stdout);
	}
}

int main(int argc, char *argv[])
{
	/* Pour la suite du programme on vérifie qu'il n'ya pas de problème avec
	 les fonctions, si oui on ne rééssaie pas et on quitte le programme */

	// Mise en place du handler de SIGINT
	struct sigaction sa_inter;
	sa_inter.sa_handler = handler_interuption;
	sa_inter.sa_flags = 0;
	exit_if_return_not_0(sigemptyset(&(sa_inter.sa_mask)), "Impossible de vider le masque");
	exit_if_return_not_0(sigaction(SIGINT, &sa_inter, NULL), "Impossible d'ajouter le handler du signal");

	// Mise en place du handler de SIGALRM
	struct sigaction sa_alarm;
	sa_alarm.sa_handler = handler_alarm;
	sa_alarm.sa_flags = 0;
	exit_if_return_not_0(sigemptyset(&(sa_alarm.sa_mask)), "Impossible de vider le masque");
	exit_if_return_not_0(sigaction(SIGALRM, &sa_alarm, NULL), "Impossible d'ajouter le handler du signal");

	// mise en place de l'alarme
	int n = 10;
	alarm(n);

	// attente d'un signal
	pause();

	exit(EXIT_SUCCESS);
}