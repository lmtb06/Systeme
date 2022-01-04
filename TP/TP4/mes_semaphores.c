#include "fonctions_utiles.h"
#include "mes_semaphores.h"

int sem_creation(int *semid, int nombre_semaphores)
{
	key_t cle = get_cle_commune();
	(*semid) = semget(cle, nombre_semaphores, IPC_CREAT | 0660 | IPC_EXCL);
	int retour = -1;
	if ((*semid) != -1)
	{
		retour = 0;
	}

	return retour;
}

int sem_initialisation(int semid, int num_semaphore, int nbr_jetons)
{
	semval initsem;
	initsem.val = nbr_jetons;
	return semctl(semid, num_semaphore, SETVAL, initsem);
}

int P(int semid, int num_semaphore)
{
	struct sembuf smbf;
	smbf.sem_num = num_semaphore;
	smbf.sem_op = -1; //+1 pour V
	smbf.sem_flg = 0;
	return semop(semid, &smbf, 1);
}

int V(int semid, int num_semaphore)
{
	struct sembuf smbf;
	smbf.sem_num = num_semaphore;
	smbf.sem_op = 1; //+1 pour V
	smbf.sem_flg = 0;
	return semop(semid, &smbf, 1);
}

int sem_destruction(int semid)
{
	return semctl(semid, IPC_RMID, 0);
}

int sem_recup(int *semid, int nb_semaphores)
{
	key_t cle = get_cle_commune();
	(*semid) = semget(cle, nb_semaphores, 0);
	int retour = -1;
	if ((*semid) != -1)
	{
		retour = 0;
	}

	return retour;
}
