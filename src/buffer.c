#include <stdlib.h>
#include <string.h>

#include "terminal.h"
#include "buffer.h"
#include "utils/die.h"

#define BUFFER_SIZE 80

void	dbuff_init(dbuff_t *dbuff)
{
	dbuff->buf = malloc(sizeof(*dbuff->buf) * (BUFFER_SIZE + 1));
	if (dbuff->buf == NULL)
	{
		terminal_free();
		die("malloc");
	}
	memset(dbuff->buf, 0, BUFFER_SIZE);
	dbuff->buf[BUFFER_SIZE] = '\0';
	dbuff->size = 0;
	dbuff->buf_size = BUFFER_SIZE;
}

void	dbuff_resize(dbuff_t *dbuff, const unsigned int new_size)
{
	dbuff->buf = realloc(dbuff->buf, sizeof(*dbuff->buf) * (new_size + 1));
	if (dbuff->buf == NULL)
	{
		terminal_free();
		dbuff_free(dbuff);
		die("realloc");
	}
	if (new_size > dbuff->buf_size)
		memset(dbuff->buf + dbuff->buf_size, 0, new_size - dbuff->buf_size);
	dbuff->buf[new_size] = '\0';
	if (new_size < dbuff->size)
		dbuff->size = new_size;
	dbuff->buf_size = new_size;
}

void	dbuff_append(dbuff_t *dbuff, const char *ptr, const unsigned int n)
{
	if (dbuff->size + n > dbuff->buf_size)
		dbuff_resize(dbuff, dbuff->size + n);
	memcpy(dbuff->buf + dbuff->size, ptr, n);
	dbuff->size += n;
}

void	dbuff_free(dbuff_t *dbuff)
{
	free(dbuff->buf);
	dbuff->buf = NULL;
	dbuff->size = 0;
	dbuff->buf_size = 0;
}

