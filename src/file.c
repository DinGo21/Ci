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
	size_t	size;
	int		nb;

	fd = fopen(file, "r");
	if (fd == NULL)
		die("fopen");
	t_config.filename = strdup(file);
	if (t_config.filename == NULL)
	{
		fclose(fd);
		die("strdup");
	}
	line = NULL;
	size = 0;
	while ((nb = getline(&line, &size, fd)) != 0)
	{
		if (nb < 0)
			break;
		while (line[nb - 1] == '\r' || line[nb - 1] == '\n')
			nb--;
		row_list_append(&t_config.rows, row_create(line, nb));
		t_config.nrows++;
	}
	free(line);
	fclose(fd);
}

