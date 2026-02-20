#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "row.h"
#include "terminal.h"
#include "utils/die.h"

void	file_open(const char *file)
{
	FILE	*fd;
	char	*line;
	row_t	*row;
	size_t	size;
	int		nb;

	fd = fopen(file, "r");
	if (fd == NULL)
		die("fopen");
	t_config.filename = strdup(file);
	if (t_config.filename == NULL)
		return (fclose(fd), die("strdup"));
	line = NULL;
	size = 0;
	while ((nb = getline(&line, &size, fd)) != 0)
	{
		if (nb < 0)
			break;
		while (line[nb - 1] == '\r' || line[nb - 1] == '\n')
			nb--;
		if ((row = row_create(line, nb)) == NULL)
			return (terminal_free(), die("row_create"));
		row_list_append(&t_config.rows, row);
		t_config.nrows++;
	}
	return (fclose(fd), free(line));
}

