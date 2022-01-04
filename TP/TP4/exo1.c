#include "fonctions_utiles.h"
#include "mes_semaphores.h"

int main(int argc, char *argv[])
{
	/**
	 * 1- Création du segment de mémoire partagé de la taille d'un int par le père
	 * (utiliser IPC_PRIVATE).
	 * 2- Attachement du segment pour pouvoir l'utiliser par le père.
	 * On fait pas de exec donc lors du fork le pointeur du segment sera
	 * transmis au fils.
	 * 3- Initialisation de la valeur (*pVariablePartagée) à 0 par le père.
	 */

	int *pVariablePartagee; // Le pointeur vers le segment partagé

	// 1
	int mem_id = shmget(IPC_PRIVATE, 1 * sizeof(int), IPC_CREAT | 0660);
	if (mem_id == -1)
	{
		print_erreur_et_exit(stdout, EXIT_FAILURE, "Père (pid : %d): Impossible de créer le segment de mémoire partagé", getpid());
	}

	// 2
	pVariablePartagee = (int *)shmat(mem_id, NULL, 0);

	// 3
	(*pVariablePartagee) = 0;

	/**
	 * Création de l'ensemble de sémaphore
	 */
	int semid;
	if (sem_creation(&semid, 1) == -1)
	{
		print_erreur_et_exit(stdout, EXIT_FAILURE, "Père (pid : %d): Impossible de créer l'ensemble de sémaphore", getpid());
	}

	/**
	 * Initialisation de l'ensemble de sémaphore
	 */
	sem_initialisation(semid, 0, 1);
	int NB_INC = 100000;
	/**
	 * Création du fils
	 */
	pid_t pid_fils = creer_fils(3, 1000);

	int tmp;
	if (pid_fils == 0)
	{
		/**
	 * Incrementation par 5 par le fils 100000 fois en parallele du fils
	 * de la même manière que le père.
	 */
		for (size_t i = 0; i < NB_INC; i++)
		{
			P(semid, 0);
			tmp = (*pVariablePartagee);
			tmp = tmp + 5;
			(*pVariablePartagee) = tmp;
			V(semid, 0);
		}
	}
	else if (pid_fils > 0)
	{
		/**
	 * Incrementation par 3 par le père 100000 fois par l'algo suivant:
	 * tmp = (*pVariablePartagée);
	 * tmp = tmp + 3;
	 * (*pVariablePartagée) = tmp;
	 */
		for (size_t i = 0; i < NB_INC; i++)
		{
			P(semid, 0);
			tmp = (*pVariablePartagee);
			tmp = tmp + 3;
			(*pVariablePartagee) = tmp;
			V(semid, 0);
		}
	}
	else
	{
		print_erreur_et_exit(stdout, EXIT_FAILURE, "Père (pid : %d): Impossible de créer le fils", getpid());
	}

	/**
	 * Attente de la mort du fils puis affichage du contenu de (*pVaraiblePartagée) par le père
	 */
	if (pid_fils != 0)
	{
		wait(NULL);
		print_flush(stdout, "Père (pid : %d): Valeur dans le segment partagé : %d\n", getpid(), (*pVariablePartagee));
	}

	/**
	 * Détachement du segment de mémoire et destruction du segment par le père
	 */
	shmdt(pVariablePartagee);
	if (pid_fils == 0)
	{
		print_flush(stdout, "Fils (pid : %d): Au revoir !\n", getpid());
	}
	else
	{
		/**
		 * Destruction de l'ensemble de sémaphore
		 */
		if(sem_destruction(semid) == -1)
		{
			print_flush(stdout, "Père (pid : %d): Impossible de supprimer l'ensemble de sémaphores", getpid());
		}
		/**
		 * Suppression du segment de mémoire partagé
		 */
		shmctl(mem_id, IPC_RMID, 0);
		print_flush(stdout, "Père (pid : %d): Au revoir !\n", getpid());
	}

	exit(EXIT_SUCCESS);
}