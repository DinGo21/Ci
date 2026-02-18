#ifndef BUFFER_H
# define BUFFER_H

typedef struct dbuff_s
{
	char			*buf;
	unsigned int	buf_size;
	unsigned int	size;
}	dbuff_t;

void	dbuff_init(dbuff_t *dbuff);
void	dbuff_resize(dbuff_t *dbuff, const unsigned int new_size);
void	dbuff_append(dbuff_t *dbuff, const char *ptr, const unsigned int n);
void	dbuff_free(dbuff_t *dbuff);

#endif

