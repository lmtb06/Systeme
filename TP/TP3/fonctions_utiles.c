#include "fonctions_utiles.h"


void print_flush(FILE* flux, char *message, ...)
{
	// on l'utilise pour stocker la liste des arguments après message
	va_list args;
	va_start(args, message);

	// on affiche puis flush dans le flux specifié ex stdout ou un fichier
	vfprintf(flux, message, args);
	fflush(flux);

	va_end(args);
}

void print_erreur_et_exit(FILE *flux, int exit_status, char *message, ...)
{
	// on l'utilise pour stocker la liste des arguments après message
	va_list args;
	va_start(args, message);

	// on affiche notre message custom suivi de l'interpretation dans
	// le format custom_message : interpretation\n puis on flush
	vfprintf(flux, message, args);
	fprintf(flux, " : %s\n", strerror(errno));
	fflush(flux);

	va_end(args);

	// on quitte notre programme avec notre code défini dans exit_status
	exit(exit_status);
}

void exit_si_different(int valeur, int valeur_attendue, FILE *flux, int exit_status, char *message, ...)
{
	if (valeur != valeur_attendue)
	{
		// on l'utilise pour stocker la liste des arguments après message
		va_list args;
		va_start(args, message);

		// ecrit le message
		print_flush(flux, message, args);

		va_end(args);

		exit(exit_status);
	}
}

pid_t creer_fils_exec(size_t max_essais, unsigned int temps_attente_entre_fork, char *chemin, char *nom, ...)
{
	pid_t pid_fils = fork();
	// réessaie de fork() si le fork initial n'a pas marché et que le processus n'est pas bloqué
	for (size_t i = 0; i < max_essais && pid_fils == -1 && errno == EAGAIN; i++)
	{
		print_flush(stdout, "Fork non réussi, nouvel essai dans %d seconde(s) \n", temps_attente_entre_fork);
		usleep(temps_attente_entre_fork);
		pid_fils = fork();
	}

	if (pid_fils == 0) // on est dans le processus fils
	{
		// on l'utilise pour stocker la liste des arguments après nom
		// ça marche parce que le programme est entiérement copié du père
		va_list args;
		va_start(args, nom);

		// recouvrement
		execl(chemin, nom, args, NULL);
		print_erreur_et_exit(stdout, EXIT_FAILURE, "Fils (pid: %d)Recouvrement de %s impossible", getpid(), nom);
	}

	return pid_fils;
}

key_t get_cle_commune()
{
	return ftok(CHEMIN_CLE, ID_CLE);
}

int creer_file(key_t cle)
{

	int id_file = msgget(cle, IPC_CREAT | 0666 | IPC_EXCL);
	if (id_file == -1 && errno == EEXIST) // La création n'a pas marché car la file existe déjà
	{
		// on supprime la file pour enlever tous les messages (//TODO on pourrait vérifier qu'on a les droits mais ça va compliquer les choses, je le ferai si j'ai le temps)
		id_file = recuperer_file(cle);
		msgctl(id_file, IPC_RMID, 0);
		id_file = msgget(cle, IPC_CREAT | 0666 );
	}
	return id_file;
}

int recuperer_file(key_t cle)
{
	int id_file = msgget(cle, 0);
	return id_file;
}

int multiplier_par_2(int entier)
{
	return entier << 1;
}

int recevoir_message(int id_file, message *message, long type)
{
	return msgrcv(id_file, message, sizeof(message), type, 0);
}

int envoyer_message(int id_file, message *message, long type)
{
	message->mtype = type;
	return msgsnd(id_file, message, sizeof(message), 0);
}