#include "fonctions_utiles.h"

int main(int argc, char *argv[])
{
	// vérification du nombre d'argment nécessaire
	if (argc != 4)
	{
		print_flush(stdout, "Client (pid : %d): le nombre d'arguments nécessaire n'est pas respecté.\n", getpid());
		exit(EXIT_FAILURE);
	}

	// recupération de la clé
	key_t cle = get_cle_commune();
	// Récupération du numéro de la file
	int id_file = recuperer_file(cle);
	if (id_file == -1) // La récupération n'a pas marché
	{
		print_erreur_et_exit(stdout, EXIT_FAILURE, "Client (pid : %d): Impossible de récuperer l'id de la file", getpid());
	}

	message requete;
	requete.pid_demandeur = getpid();
	if (strcmp(argv[2], "+") == 0)
	{
		requete.operation = ADDITION;
	}
	else if (strcmp(argv[2], "-") == 0)
	{
		requete.operation = SOUSTRACTION;
	}
	else if (strcmp(argv[2], "*") == 0)
	{
		requete.operation = MULTIPLICATION;
	}
	else if (strcmp(argv[2], "/") == 0)
	{
		requete.operation = DIVISION;
	}
	else
	{
		requete.operation = INT_MIN;
	}
	requete.operande1 = atoi(argv[1]);
	requete.operande2 = atoi(argv[3]);
	// Envoie de message au serveur (type client)
	if (envoyer_message(id_file, &requete, requete.operation) == -1)
	{
		print_erreur_et_exit(stdout, EXIT_FAILURE, "Client (pid : %d): Erreur lors de l'envoie de la requete", getpid());
	}
	else
	{
		// reponse du serveur
		message reponse;
		if (recevoir_message(id_file, &reponse, getpid()) == -1)
		{
			print_erreur_et_exit(stdout, EXIT_FAILURE, "Client (pid : %d): Erreur lors de la réception du message", getpid());
		}
		print_flush(stdout, "Client (pid : %d): Réponse du serveur : %s %s %s = %d\n", getpid(), argv[1], argv[2], argv[3], reponse.valeur);
	}
	exit(EXIT_SUCCESS);
}