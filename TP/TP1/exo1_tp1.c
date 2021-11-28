#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include "fonctions_utiles.h"

int nb_ctrl_c = 0;

// la fonction du handler pour les signaux
void handler_pour_signal(int sig)
{
	switch (sig)
	{
	case SIGINT:
		nb_ctrl_c++;
		if (nb_ctrl_c == 1)
		{
			print_flush("\nSignal SIGINT bien reçu\n");
		}
		else if (nb_ctrl_c == 2)
		{
			print_flush("\nSignal SIGINT bien reçu au revoir\n");
			exit(EXIT_SUCCESS);
		}
		break;
	}
}

int main(int argc, char *argv[])
{
	/* Pour la suite du programme on vérifie qu'il n'ya pas de problème avec
	 les fonctions, si oui on ne rééssaie pas et on quitte le programme */

	// Préparation de la structure
	struct sigaction sa;
	sa.sa_handler = handler_pour_signal;
	sa.sa_flags = 0;
	exit_if_return_not_0(sigemptyset(&(sa.sa_mask)), "Impossible de vider le masque");

	// Installation effective du handler
	exit_if_return_not_0(sigaction(SIGINT, &sa, NULL), "Impossible d'ajouter le handler du signal");

	// Installation du masque
	exit_if_return_not_0(sigaddset(&(sa.sa_mask), SIGINT), "Impossible d'ajouter le signal au masque");

	exit_if_return_not_0(sigprocmask(SIG_SETMASK, &(sa.sa_mask), NULL), "Impossible d'installer le masque");

	int duree_attente = 500000;
	// compteur de 0 à 39
	for (int i = 0; i <= 39; i++)
	{
		usleep(duree_attente);
	}

	// Désinstallation du masque
	exit_if_return_not_0(sigdelset(&(sa.sa_mask), SIGINT), "Impossible d'enlever le signal SIGINT au masque");
	exit_if_return_not_0(sigprocmask(SIG_SETMASK, &(sa.sa_mask), NULL), "Impossible de réinstaller le masque");

	// compteur de 40 à 60
	for (int i = 40; i <= 60; i++)
	{
		usleep(duree_attente);
	}

	exit(EXIT_SUCCESS);
}