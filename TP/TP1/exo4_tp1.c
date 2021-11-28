#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include "fonctions_utiles.h"

time_t date_arret = __LONG_MAX__;
time_t date_fils_1 = __LONG_MAX__;
time_t date_fils_2 = __LONG_MAX__;
unsigned short nb_sigaux_recu = 0;

/* prend en parametre la date d'arrêt de chaque fils ainsi que la date d'arrêt du timer et retourne:
 - 0 s'ils sont ex aequo (le cas ou l'alarme à sonné avant qu'un des deux envoie le signal)
 - 1 si le fils 1 est gagnant
 - 2 si le fils 2 est gagnant
 */
int vainqueur(time_t date_art, time_t date1, time_t date2)
{
	int return_value = 0;
	if (date_art >= date1 || date_art >= date2)
	{
		if (date1 < date2)
		{
			return_value = 1;
		}
		else if (date2 < date1)
		{
			return_value = 2;
		}
	}
	return return_value;
}

/*
	affiche le vainqueur sur le flux désigné et flush après le code vainqueur est le code retourné par la fonction vainqueur(...)
*/
void print_vainqueur(FILE *flux, int code_vainqueur)
{
	if (code_vainqueur == 0)
	{
		fprintf(flux, "Les fils sont ex aequo\n");
		fflush(flux);
	}
	else
	{
		fprintf(flux, "Le gagnant est le fils n°%d\n", code_vainqueur);
		fflush(flux);
	}
}

// handler fils 1
void handler_fils_1(int sig)
{
	if (sig == SIGUSR1)
	{
		// stocke la date de réception du signal
		// on actualise que si on a pas reçu le signal avant
		if (date_fils_1 == __LONG_MAX__)
		{
			date_fils_1 = time(NULL);
		}
		//print_flush("Reçu fils 1\n");
	}
}

// handler fils 2
void handler_fils_2(int sig)
{
	if (sig == SIGUSR2)
	{
		// stocke le date de réception du signal
		// on actualise que si on a pas reçu le signal avant
		if (date_fils_2 == __LONG_MAX__)
		{
			date_fils_2 = time(NULL);
		}
		//print_flush("Reçu fils 2\n");
	}
}

// handler de l'alarm
void handler_signal_alarm(int sig)
{
	if (sig == SIGALRM)
	{
		// stocke la date de réception du signal
		date_arret = time(NULL);
		//print_flush("Reçu alarme\n");
		// affiche le vainqueur sur la sortie standart
		print_vainqueur(stdout, vainqueur(date_arret, date_fils_1, date_fils_2));
	}
}

/*
	Creer un fils du timer le fils aura l'argument arg
	max_essais le nombre maximum d'essai de fork en cas de probléme de fork
	temps_attente_entre_fork le temps d'attente entre chaque fork non réussi
*/
void creer_fils(char* arg, size_t max_essais, unsigned int temps_attente_entre_fork)
{
	pid_t pid_fils = fork();
	// réessaie de fork() (maximum d'essais défini par max_essais) si le fork initial n'a pas marché et que le processus n'est pas bloqué avec un délai de temps_attente_entre_fork seconde entre chaque essai
	for (size_t i = 0; i < max_essais && pid_fils == -1 && errno == EAGAIN; i++)
	{
		printf("Pas de place dans la table interne du noyau, nouvel essai dans %d seconde(s) \n", temps_attente_entre_fork);
		fflush(stdout);
		sleep(temps_attente_entre_fork);
		pid_fils = fork();
	}
	if (pid_fils == -1) // le fork n'a pas marché
	{
		print_error_and_exit("fork () impossible", EXIT_FAILURE);
	}

	if (pid_fils == 0) // on est dans le processus fils
	{
		// recouvrement
		execl("./fils_timer", "fils_timer", arg, NULL);
		print_error_and_exit("Recouvrement impossible", EXIT_FAILURE);
	}
}

int main(int argc, char *argv[])
{
	// mise en place du handler de l'alarme
	struct sigaction sa_alarm;
	sa_alarm.sa_handler = handler_signal_alarm;
	sa_alarm.sa_flags = 0;
	exit_if_return_not_0(sigemptyset(&(sa_alarm.sa_mask)), "Impossible de vider le masque");
	exit_if_return_not_0(sigaction(SIGALRM, &sa_alarm, NULL), "Impossible d'ajouter le handler de l'alarme");
	// Installation du masque de l'alarme
	exit_if_return_not_0(sigaddset(&(sa_alarm.sa_mask), SIGUSR1), "Impossible d'ajouter le signal au masque de l'alarme");
	exit_if_return_not_0(sigaddset(&(sa_alarm.sa_mask), SIGUSR2), "Impossible d'ajouter le signal au masque de l'alarme");

	// Mise en place du handler de SIGUSR1
	struct sigaction sa_usr1;
	sa_usr1.sa_handler = handler_fils_1;
	sa_usr1.sa_flags = 0;
	exit_if_return_not_0(sigemptyset(&(sa_usr1.sa_mask)), "Impossible de vider le masque");
	exit_if_return_not_0(sigaction(SIGUSR1, &sa_usr1, NULL), "Impossible d'ajouter le handler du signal");
	// Installation du masque du handler du fils 1
	exit_if_return_not_0(sigaddset(&(sa_usr1.sa_mask), SIGALRM), "Impossible d'ajouter le signal au masque du fils 1");
	exit_if_return_not_0(sigaddset(&(sa_usr1.sa_mask), SIGUSR2), "Impossible d'ajouter le signal au masque du fils 1");

	// Mise en place du handler de SIGUSR2
	struct sigaction sa_usr2;
	sa_usr2.sa_handler = handler_fils_2;
	sa_usr2.sa_flags = 0;
	exit_if_return_not_0(sigemptyset(&(sa_usr2.sa_mask)), "Impossible de vider le masque");
	exit_if_return_not_0(sigaction(SIGUSR2, &sa_usr2, NULL), "Impossible d'ajouter le handler du signal");
	// Installation du masque du handler du fils 2
	exit_if_return_not_0(sigaddset(&(sa_usr2.sa_mask), SIGUSR1), "Impossible d'ajouter le signal au masque du fils 2");
	exit_if_return_not_0(sigaddset(&(sa_usr2.sa_mask), SIGALRM), "Impossible d'ajouter le signal au masque du fils 2");

	// création des fils
	// fils 1
	creer_fils("1", 5, 1);
	// fils 2
	creer_fils("2", 5, 1);

	// mise en place de l'alarme
	int n = 10;
	alarm(n);

	// attente d'un signal on continue le programme que lorsque l'alarme a été déclenchée
	while (date_arret == __LONG_MAX__)
	{
		pause();
	}

	exit(EXIT_SUCCESS);
}