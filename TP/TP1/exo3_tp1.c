#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include "fonctions_utiles.h"

int main(int argc, char *argv[])
{
	// vérifie qu'on a le nombre d'argument nécessaire et quitte le programme
	// si ce n'est pas le cas
	if (argc != 4)
	{
		print_flush("Père: le nombre d'arguments nécessaire n'est pas respecté.\n");
		exit(EXIT_FAILURE);
	}

	pid_t pid_fils = fork();
	// nombre d'essais de fork maximum
	size_t max_essais = 5;
	// temps d'attente en secondes entre chaque essai de fork
	unsigned int temps_attente_fork = 1;
	// réessaie de fork() (maximum d'essais défini par max_essais) si le fork initial n'a pas marché et que le processus n'est pas bloqué avec un délai de temps_attente_fork seconde entre chaque essai
	for (size_t i = 0; i < max_essais && pid_fils == -1 && errno == EAGAIN; i++)
	{
		printf("Pas de place dans la table interne du noyau, nouvel essai dans %d seconde(s) \n", temps_attente_fork);
		fflush(stdout);
		sleep(temps_attente_fork);
		pid_fils = fork();
	}
	if (pid_fils == -1) // le fork n'a pas marché
	{
		print_error_and_exit("fork () impossible", EXIT_FAILURE);
	}

	if (pid_fils == 0) // on est dans le processus fils
	{
		execl("./recouvrant", "recouvrant", argv[1], argv[2], argv[3], NULL);
		print_error_and_exit("Recouvrement impossible", EXIT_FAILURE);
	}
	else // on est toujours dans le processus père
	{
		if(pid_fils != wait(NULL))
			print_error_and_exit("Problème avec le fils", EXIT_FAILURE); // on attend que le proessus fils finisse avant de continuer
	}

	exit(EXIT_SUCCESS);
}