#include "fonctions_utiles.h"

// le nombre de sigusr1 reçu du père
int nb_sigusr1 = 0;

// - handle des signaux SIGTERM termine le programme et affiche le
// nombre de signaux SIGUSR1 reçus
// - handle des signaux SIGUSR1 incrémente le nombre de SIGUSR1 reçus
void handler_signaux(int sig)
{
	if (sig == SIGTERM)
	{
		print_flush(stdout, "Fils (pid : %d): Nombre de signaux SIGUSR1 reçus : %d, au revoir !\n", getpid(), nb_sigusr1);
		exit(EXIT_SUCCESS);
	}
	if (sig == SIGUSR1)
	{
		nb_sigusr1++;
	}

}

int main(int argc, char *argv[])
{
	/* On garde le pid du pere createur pour ne pas pas que envoyer
	*	le signal à un nouveau père si celui ci est mort même si ça ne
	*	devrait pas arriver vu qu'il va se mettre en zombie
	*/
	pid_t pid_pere = getppid();

	// Mise en place du handler de SIGUSR1
	struct sigaction sa;
	sa.sa_handler = handler_signaux;
	sa.sa_flags = 0;
	sigemptyset(&(sa.sa_mask));
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);

	/*
	* On utilise le même système de mise en pending que pour le père
	*/
	sigset_t masque_blocage;
	sigemptyset(&masque_blocage);
	sigaddset(&masque_blocage, SIGUSR1);
	sigprocmask(SIG_BLOCK, &masque_blocage, NULL);

	sigset_t masque_attente;
	sigemptyset(&masque_attente);


	print_flush(stdout, "Fils (pid : %d): Handlers et masques mis en place \n", getpid());

	while (1)
	{
		/*
		*	Le premier signal envoyé signale au pére qu'on a fini de mettre
		*	en place les handlers.
		*
		*	Les signaux envoyés suivants font guise de confirmation de récéption
		*	de signal.
		*/
		// Envoi la confirmation au père
		kill(pid_pere, SIGUSR2);
		// Attend le prochain signal du père
		sigsuspend(&masque_attente);
	}
}