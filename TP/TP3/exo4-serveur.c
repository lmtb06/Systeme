#include "fonctions_utiles.h"

int id_file = -1;

void handler(int sig)
{
	if (sig == SIGINT)
	{
		// Suppression de la file lorsqu'on recoit un sigint
		print_flush(stdout, "Serveur (pid : %d): Suppression de la file\n", getpid());
		exit_si_different(msgctl(id_file, IPC_RMID, 0), 0, stdout, EXIT_FAILURE, "Serveur (pid : %d): La suppression de la file n'a pas marché", getpid());
		print_flush(stdout, "Serveur (pid : %d): Au revoir !\n", getpid());
		exit(EXIT_SUCCESS);
	}
}

int main(int argc, char *argv[])
{
	// Mise en place du handler de sigint
	struct sigaction sa;
	sa.sa_handler = handler;
	sa.sa_flags = 0;

	exit_si_different(sigemptyset(&(sa.sa_mask)), 0, stdout, EXIT_FAILURE, "Serveur (pid : %d): Impossible de vider le masque de sa", getpid());

	exit_si_different(sigaction(SIGINT, &sa, NULL), 0, stdout, EXIT_FAILURE, "Serveur (pid : %d): Impossible de mettre en place le handler de SIGINT", getpid());

	// recupération de la clé
	key_t cle = get_cle_commune();
	// Création de la file
	id_file = creer_file(cle);
	if (id_file == -1) // La création n'a pas marché
	{
		print_erreur_et_exit(stdout, EXIT_FAILURE, "Serveur (pid : %d): Impossible de créer la file de message", getpid());
	}

	message requete;
	while (1)
	{
		// Réception de la requête d'un client (type client)
		// print_flush(stdout, "Serveur (pid : %d): En attente de la requête d'un client\n", getpid());
		if (recevoir_message(id_file, &requete, -4) == -1)
		{
			print_erreur_et_exit(stdout, EXIT_FAILURE, "Serveur (pid : %d): Erreur lors de la réception du message", getpid());
		}
		else
		{
			// print_flush(stdout, "Serveur (pid : %d): Message reçu du client: %d\n", getpid(), requete.pid_demandeur);
			switch (requete.operation)
			{
			case ADDITION:
				requete.valeur = addition(requete.operande1, requete.operande2);
				break;
			case SOUSTRACTION:
				requete.valeur = soustraction(requete.operande1, requete.operande2);
				break;
			case MULTIPLICATION:
				requete.valeur = multiplication(requete.operande1, requete.operande2);
				break;
			case DIVISION:
				requete.valeur = division_entiere(requete.operande1, requete.operande2);
				break;
			default:
				//on ne sait pas quel est le type d'opération
				break;
			}
			// print_flush(stdout, "Serveur (pid : %d): Envoie de la réponse\n", getpid());
			// Envoie de la réponse au client (type pid du client demandeur)
			if (envoyer_message(id_file, &requete, requete.pid_demandeur) == -1)
			{
				print_erreur_et_exit(stdout, EXIT_FAILURE, "Serveur (pid : %d): Erreur lors de l'envoie de la reponse", getpid());
			}
		}
	}
}