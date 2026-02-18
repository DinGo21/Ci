#include <stdio.h>
#include <stdlib.h>

#include "utils/die.h"

void	die(const char *err_message)
{
	perror(err_message);
	exit(EXIT_FAILURE);
}

