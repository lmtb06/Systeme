#include "fonctions_utiles.h"

int main(int argc, char *argv[])
{
	// vérification du nombre d'arguments valide
	if (argc != 2)
	{
		print_flush(stdout, "Père (pid : %ld): Le nombre d'arguments nécessaire n'est pas respecté.\n", getpid());
		exit(EXIT_FAILURE);
	}

	// récuperation du nombre signaux à envoyer
	int N = atoi(argv[1]);


	// vérifie que le nombre de signal à envoyer passé en paramètre est > 100000 et quitte le programme sinon
	if (N <= 100000)
	{
		print_flush(stdout, "Père (pid : %d): le nombre de signaux à envoyer doit être > 100 000.\n", getpid());
		exit(EXIT_FAILURE);
	}

	// Création du fils et recouvrement
	pid_t pid_fils = creer_fils_exec(5, 100000, "./exo1fils", "exo1fils");

	if (pid_fils == -1) // Le fork a échoué
	{
		print_erreur_et_exit(stdout, EXIT_FAILURE, "Père (pid : %d): Impossible de forker", getpid());
	}

	// Attente de la mise en place des handlers du fils
	sleep(1);

	for (size_t i = 1; i <= N; i++)
	{
		// envoie du signal au fils s'il est toujours vivant
		if (kill(pid_fils, 0) == 0)
			exit_si_different(kill(pid_fils, SIGUSR1), 0, stdout, EXIT_FAILURE, "Père (pid : %d): Impossible d'envoyer le signal SIGSUR1 au fils", getpid());
	}

	// envoie de SIGTERM au fils s'il est toujours vivant
	if (kill(pid_fils, 0) == 0)
		exit_si_different(kill(pid_fils, SIGTERM), 0, stdout, EXIT_FAILURE, "Père (pid : %d): Impossible d'envoyer le signal  SIGTERM au fils", getpid());

	// Attente du fils
	wait(NULL);

	exit(EXIT_SUCCESS);
}