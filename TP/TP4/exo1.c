#include "fonctions_utiles.h"

int main(int argc, char *argv[])
{
	/**
	 * 1- Création du segment de mémoire partagé par le père
	 * (utiliser IPC_PRIVATE).
	 * 2- Attachement du segment pour pouvoir l'utiliser par le père.
	 * On fait pas de exec donc lors du fork le pointeur du segment sera
	 * transmis au fils.
	 * 3- Initialisation de la valeur (*pVariablePartagée) à 0 par le père.
	 */

	float *pVariablePartagee; // Le pointeur vers le segment partagé

	/**
	 * Création du fils
	 *
	 */

	/**
	 * Incrementation par 3 par le père 100000 fois par l'algo suivant:
	 * tmp = (*pVaraiblePartagée);
	 * tmp = tmp + 3;
	 * (*pVariablePartagée) = tmp;
	 */

	/**
	 * Incrementation par 5 par le fils 100000 fois en parallele du fils
	 * de la même manière que le père.
	 */
}