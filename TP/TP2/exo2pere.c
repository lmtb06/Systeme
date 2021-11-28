#include "fonctions_utiles.h"


void handler_sigusr2(int sig)
{

}
// J'ai perdu tout les documents des autres exos à cause d'une erreur dans mon makefile j'avais mis OBJ := $(SRCS:%.c=%.c) et en faisant clean ça a supprimé les fichiers c
// La seule partie que j'ai pu récuperer c'est celle là car j'avais commit avant de continuer
int main(int argc, char *argv[])
{
	// vérification du nombre d'arguments valide
	if (argc != 2)
	{
		print_flush(stdout, "Père (pid : %d): Le nombre d'arguments nécessaire n'est pas respecté.\n", getpid());
		exit(EXIT_FAILURE);
	}

	// récuperation du nombre signaux à envoyer
	int N = atoi(argv[1]);

	// vérifie que le nombre de signal à envoyer passé en paramètre est > 100000 et quitte le programme sinon
	if (N <= 100000)
	{
		print_flush(stdout, "Père (pid : %d): Le nombre de signaux à envoyer doit être > 100 000.\n", getpid());
		exit(EXIT_FAILURE);
	}

	// Mise en place du handler de SIGUSR2 pour pas que le programme quitte une fois reçu le signal
	struct sigaction sa_usr2;
	sa_usr2.sa_handler = handler_sigusr2;
	sa_usr2.sa_flags = 0;
	exit_si_different(sigemptyset(&(sa_usr2.sa_mask)), 0, stdout, EXIT_FAILURE, "Père (pid : %d): Impossible de vider le masque du handler de SIGUSR2", getpid());
	exit_si_different(sigaction(SIGUSR2, &sa_usr2, NULL), 0, stdout, EXIT_FAILURE, "Père (pid : %d): Impossible de mettre en place le handler de SIGUSR2", getpid());

	/*
	* On masque le signal SIGUSR2 pour que le signal du fils soit
	*	mis en pending pour ne pas que le signal soit "raté" par le
	*	sigsuspend. Cela peut arriver entre le moment où le signal est
	*	envoyé au fils et le sigsuspend (le fils envoie son signal
	*	avant la mis en place du sigsupend).
	*
	*	On choisit aussi de mettre en place le masque avant de créer
	*	le fils vu que le fils envoie un signal dès qu'il a mis en
	*	place ses handlers et on ne voudrait pas le rater sinon les
	* deux processus restent bloqué en attente de l'autre.
	*/

	// Masque de blockage du signal SIGUSR2
	sigset_t masque_blocage;
	exit_si_different(sigemptyset(&masque_blocage), 0, stdout, EXIT_FAILURE, "Père (pid : %d): Impossible de vider le masque de bloquage", getpid());
	exit_si_different(sigaddset(&masque_blocage, SIGUSR2), 0, stdout, EXIT_FAILURE, "Père (pid : %d): Impossible d'ajouter SIGUSR2 au masque de bloquage", getpid());

	// Masquage de SIGUSR2
	exit_si_different(sigprocmask(SIG_BLOCK, &masque_blocage, NULL), 0, stdout, EXIT_FAILURE, "Père (pid : %d): Impossible de mettre en place le masque de blocage", getpid());

	// Création du fils et recouvrement
	pid_t pid_fils = creer_fils_exec(5, 100000, "./exo2fils", "exo2fils");

	if (pid_fils == -1) // Le fork a échoué
	{
		print_erreur_et_exit(stdout, EXIT_FAILURE, "Père (pid : %d): Impossible de forker", getpid());
	}

	print_flush(stdout, "Père (pid : %d): Fork réussi, pid fils : %ld\n", getpid(), pid_fils);

	/*
	* sigsuspend est temporaire juste le temps de l'attente donc
	*	l'ancien masque pour mettre le signal en pending est maintenu
	*	entre les différents appels de sigsuspend.
	*/

	// Masque du sigsuspend
	sigset_t masque_attente;
	exit_si_different(sigemptyset(&masque_attente), 0, stdout, EXIT_FAILURE, "Père (pid : %d): Impossible de vider le masque d'attente'", getpid());

	// On attend que le fils nous signal qu'il a mis en place ses handlers
	sigsuspend(&masque_attente);

	for (size_t i = 1; i <= N; i++)
	{
		// envoie du signal au fils
		kill(pid_fils, SIGUSR1);

		// attente de l'accusé de reception du fils
		sigsuspend(&masque_attente);
	}

	// envoie de sigterm au fils pour finir et afficher le nombre de signaux reçus
	// envoie du signal au fils
	kill(pid_fils, SIGTERM);
	// Attente du fils
	wait(NULL);

	print_flush(stdout, "Père (pid : %d): Au revoir !\n", getpid());

	exit(EXIT_SUCCESS);
}