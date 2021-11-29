#include "fonctions_utiles.h"

int main(int argc, char *argv[])
{
	// recupération de la clé
	key_t cle = get_cle_commune();
	// Création de la file
	int id_file = creer_file(cle);
	if (id_file == -1) // La création n'a pas marché
	{
		print_erreur_et_exit(stdout, EXIT_FAILURE, "Serveur (pid : %d): Impossible de créer la file de message", getpid());
	}

	message requete;
	message reponse;
	while (1)
	{
		// Réception de la requête d'un client (type client)
		print_flush(stdout, "Serveur (pid : %d): En attente de la requête du client\n", getpid());
		if (recevoir_message(id_file, &requete, TYPE_CLIENT) == -1)
		{
			print_erreur_et_exit(stdout, EXIT_FAILURE, "Serveur (pid : %d): Erreur lors de la réception du message", getpid());
		}
		else
		{
			print_flush(stdout, "Serveur (pid : %d): Message reçu du client: %d\n", getpid(), requete.pid_demandeur);
			reponse.pid_demandeur = requete.pid_demandeur;
			reponse.valeur = multiplier_par_2(requete.valeur);
			print_flush(stdout, "Serveur (pid : %d): Envoie de la réponse\n", getpid());
			// Envoie de la réponse au client (type pid du client demandeur)
			if (envoyer_message(id_file, &reponse, reponse.pid_demandeur) == -1)
			{
				print_erreur_et_exit(stdout, EXIT_FAILURE, "Serveur (pid : %d): Erreur lors de l'reponse de message", getpid());
			}
		}
	}

	sleep(2);

	// Suppression de la file
	print_flush(stdout, "Serveur (pid : %d): Suppression de la file\n", getpid());
	exit_si_different(msgctl(id_file, IPC_RMID, 0), 0, stdout, EXIT_FAILURE, "Serveur (pid : %d): La suppression de la file n'a pas marché", getpid());

	exit(EXIT_SUCCESS);
}