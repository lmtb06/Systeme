#ifndef MES_SEMAPHORES
#define MES_SEMAPHORES

typedef union semun
{
	int val;							 //valeur pour SETVAL
	struct semid_ds *buf;	 // buffer pour IPC_SET et IPC_STAT
	unsigned short *array; // tableau pour GETALL et SETALL
	struct seminfo *__buf; //buffer pour IPC_INFO
} semval;

/**
 * @brief Crée un ensemble de sémaphores d'id (semid) de taille (nombre_semaphores).
 *  Renvoie 0 si la création a bien fonctionné
 *
 * @param semid le pointeur vers l'identifiant de l'ensemble de sémaphores
 * @param nombre_semaphores le nombre de sémaphores dans l'ensemble de sémaphores
 * @return int 0 si la création a bien fonctionné
 */
int
sem_creation(int *semid, int nombre_semaphores);

/**
 * @brief Initialise le sémaphore numéro (num_semaphore) de l'ensemble de sémaphore
 * d'id (semid) par la valeur (nbr_jetons).
 *
 * @param semid l'identifiant de l'ensemble de sémaphores
 * @param num_semaphore le numéro du sémaphore de l'ensemble qu'il faut initialiser
 * @param nbr_jetons le nombre de jetons avec lequel il faut initialiser le
 * sémaphore
 * @return int //TODO Je ne sais pas encore ce que retourne cette fonction
 */
int sem_initialisation(int semid, int num_semaphore, int nbr_jetons);

/**
 * @brief Appelle la primitive P du sémaphore numéro (num_semaphore) de l'ensemble
 * de sémaphores d'id (semid)
 *
 * @param semid l'identifiant de l'ensemble de sémaphores
 * @param num_semaphore le numéro du sémaphore de l'ensemble, dont il faut appeller
 * la primitive
 * @return int //TODO Je ne sais pas encore ce que retourne cette fonction
 */
int P(int semid, int num_semaphore);

/**
 * @brief Appelle la primitive V du sémaphore numéro (num_semaphore) de l'ensemble
 * de sémaphores d'id (semid)
 *
 * @param semid l'identifiant de l'ensemble de sémaphores
 * @param num_semaphore le numéro du sémaphore de l'ensemble, dont il faut appeller
 * la primitive
 * @return int //TODO Je ne sais pas encore ce que retourne cette fonction
 */
int V(int semid, int num_semaphore);

/**
 * @brief Détruit l'ensemble de sémaphores d'id (semid)
 *
 * @param semid l'identifiant de l'ensemble de sémaphores
 * @return int //TODO Je ne sais pas encore ce que retourne cette fonction
 */
int sem_destruction(int semid);

/**
 * @brief Récupère le numéro de l'ensemble de sémaphore et le met dans le pointeur
 * (semid). Fonction généralement utilisée par le fils pour récupérer l'ensemble
 * de sémaphores
 *
 * @param semid le pointeur vers l'identifiant de l'ensemble de sémaphores
 * @param nb_semaphores le nombre de sémaphores dans l'ensemble
 * @return int //TODO Je ne sais pas encore ce que retourne cette fonction
 */
int sem_recup(int *semid, int nb_semaphores);
#endif