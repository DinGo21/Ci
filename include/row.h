#ifndef ROW_H
# define ROW_H

# define TAB_STOP 8

typedef	struct row_s
{
	struct row_s	*next;
	char			*raw;
	char			*render;
	int				raw_size;
	int				render_size;
}	row_t;

row_t	*row_create(const char *str, const int size);
void	row_delete(row_t *row);
row_t	*row_insert_char(row_t *row, int in, int c);
void	row_list_append(row_t **list, row_t *node);
row_t	*row_search_by_index(row_t *rows, const unsigned int index);
void	row_list_clean(row_t **list);

#endif
