#include "fonctions_utiles.h"

void print_flush(char *message)
{
	printf("%s", message);
	fflush(stdout);
}

void print_error_and_exit(char *message, int exit_status)
{
	perror(message);
	exit(exit_status);
}

void exit_if_return_not_0(int value, char* message)
{
	if (value != 0)
	{
		print_error_and_exit(message, EXIT_FAILURE);
	}
}