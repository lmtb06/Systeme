#include "fonctions_utiles.h"



int main(int argc, char *argv[])
{
	// vérification du nombre d'argment nécessaire
	if (argc != 2)
	{
		print_erreur_et_exit(stdout, EXIT_FAILURE, "Client (pid : %d): le nombre d'arguments nécessaire n'est pas respecté.\n", getpid());
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
	requete.valeur = atoi(argv[1]);
	// Envoie de message au serveur (type client)
	if (envoyer_message(id_file, &requete, TYPE_CLIENT) == -1)
	{
		print_erreur_et_exit(stdout, EXIT_FAILURE, "Client (pid : %d): Erreur lors de l'envoie requete de message", getpid());
	}
	else
	{
		// reponse du serveur
		message reponse;
		if (recevoir_message(id_file, &reponse, TYPE_SERVEUR) == -1)
		{
			print_erreur_et_exit(stdout, EXIT_FAILURE, "Client (pid : %d): Erreur lors de la réception du message", getpid());
		}
		print_flush(stdout, "Client (pid : %d): Réponse du serveur : %d\n", getpid(), reponse.valeur);
	}
	exit(EXIT_SUCCESS);
}