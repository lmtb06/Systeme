#include "fonctions_utiles.h"

// le nombre de sigusr1 reçu du père
int nb_sigusr1 = 0;

// handler des signaux SIGUSR1 incrémente le nombre de SIGUSR1 reçus
void handler_sigusr1(int sig)
{
	if (sig == SIGUSR1)
	{
		nb_sigusr1++;
	}
}

// handler des signaux SIGTERM termine le programme et affiche le
// nombre de signaux SIGUSR1 reçus
void handler_sigterm(int sig)
{
	if (sig == SIGTERM)
	{
		print_flush(stdout, "Fils (pid : %ld): Nombre de signaux SIGUSR1 reçus : %d\n", getpid(), nb_sigusr1);
		exit(EXIT_SUCCESS);
	}
}

int main(int argc, char *argv[])
{
	// Mise en place du handler de SIGUSR1
	struct sigaction sa_usr1;
	sa_usr1.sa_handler = handler_sigusr1;
	sa_usr1.sa_flags = 0;

	exit_si_different(sigemptyset(&(sa_usr1.sa_mask)), 0, stdout, EXIT_FAILURE, "Fils (pid : %d): Impossible de vider le masque de sa_usr1", getpid());

	exit_si_different(sigaction(SIGUSR1, &sa_usr1, NULL), 0, stdout, EXIT_FAILURE, "Fils (pid : %d): Impossible de mettre en place le handler de SIGUSR1", getpid());

	// Mise en place du handler de SIGTERM
	struct sigaction sa_term;
	sa_term.sa_handler = handler_sigterm;
	sa_term.sa_flags = 0;

	exit_si_different(sigemptyset(&(sa_term.sa_mask)), 0, stdout, EXIT_FAILURE, "Fils (pid : %d): Impossible de vider le masque de sa_term", getpid());

	exit_si_different(sigaction(SIGTERM, &sa_term, NULL), 0, stdout, EXIT_FAILURE, "Fils (pid : %d): Impossible de mettre en place le handler de SIGTERM", getpid());

	// On continue d'attendre tant que le fils n'a pas reçu Un sigterm il continue la boucle
	while (1)
	{
		pause();
	}

}