#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include "fonctions_utiles.h"

int main(int argc, char *argv[])
{
	/* on stocke le pid du pere pour pouvoir verifier plus tard s'il est toujours vivant pour ne pas envoyer SIGUSR au nouveau processus pere si l'ancien à été tué.
	On aurait pu le mettre en argument directement mais de ce que j'ai compris dans l'enoncé il faut qu'un seul argument pour le programme
	*/
	pid_t pid_pere = getppid();
	// vérifie qu'on a le nombre d'argument nécessaire, qu'ils sont valides et quitte le programme
	// si ce n'est pas le cas
	if (argc != 2)
	{
		print_flush("Fils: le nombre d'arguments nécessaire n'est pas respecté.\n");
		exit(EXIT_FAILURE);
	}

	int arg = atoi(argv[1]);
	if (arg != 1 && arg != 2)
	{
		print_flush("Fils: les arguments ne sont pas valides.\n");
		exit(EXIT_FAILURE);
	}

	// Géneration du temps d'attente de façon aléatoire
	int max = 12000000;
	int min = 5000000;
	srand(getpid());
	int temps_attente = rand() % (max - min + 1) + min;
	//printf("Fils %s - PID : %d - Temps attente : %ds\n", argv[1], getpid(), temps_attente/1000000);
	// attente du temps nécessaire
	usleep(temps_attente);
	// choisir le type de signal suivant si on est le fils 1 ou 2
	int signal = (arg == 1) ? SIGUSR1 : SIGUSR2;
	// envoie du signal au père s'il est toujours vivant
	if (kill(pid_pere, 0) == 0)
		exit_if_return_not_0(kill(getppid(), signal), "Fils envoie du signal au père impossible");
	exit(EXIT_SUCCESS);
}