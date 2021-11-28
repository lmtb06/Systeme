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
		print_flush("Recouvrant: le nombre d'arguments nécessaire n'est pas respecté.\n");
		exit(EXIT_FAILURE);
	}

	int nombre1 = atoi(argv[1]);
	int nombre2 = atoi(argv[2]);
	printf("J'ai %d %s\n", nombre1 * nombre2, argv[3]);

	exit(EXIT_SUCCESS);
}