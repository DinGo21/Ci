#include <stdlib.h>
#include <string.h>

#include "row.h"

static int	row_render(row_t *row)
{
	int	tabs;
	int	i;

	tabs = 0;
	for (i = 0; i < row->raw_size; i++)
	{
		if (row->raw[i] == '\t')
			tabs++;
	}
	row->render_size = i + tabs * (TAB_STOP - 1);
	row->render = malloc(sizeof(*row->render) * (row->render_size + 1));
	if (row->render == NULL)
		return -1;
	row->render[row->render_size] = '\0';
	i = 0;
	for (int j = 0; j < row->raw_size; j++)
	{
		while (row->raw[j] != '\t' && j < row->raw_size)
			row->render[i++] = row->raw[j++];
		if (row->raw[j] == '\t')
			row->render[i++] = ' ';
		while (row->raw[j] == '\t' && i % TAB_STOP != 0)
			row->render[i++] = ' ';
	}
	return 0;
}

row_t	*row_create(const char *str, const int size)
{
	row_t	*row;

	row = malloc(sizeof(*row));
	if (row == NULL)
		return NULL;
	row->raw = malloc(sizeof(*row->raw) * (size + 1));
	if (row->raw == NULL)
		return (free(row), NULL);
	strncpy(row->raw, str, size);
	row->raw[size] = '\0';
	row->raw_size = size;
	if (row_render(row) < 0)
		return (free(row->raw), free(row), NULL);
	row->next = NULL;
	return row;
}

void	row_delete(row_t *row)
{
	free(row->raw);
	free(row->render);
	free(row);
}

void	row_list_append(row_t **list, row_t *node)
{
	row_t	*nodes;

	if (*list == NULL)
	{
		*list = node;
		return;
	}
	nodes = *list;
	while (nodes->next != NULL)
		nodes = nodes->next;
	nodes->next = node;
}

row_t	*row_search_by_index(row_t *rows, const unsigned int index)
{
	unsigned int	i;

	for (i = 0; i < index && rows != NULL; i++)
		rows = rows->next;
	if (i == index)
		return rows;
	return NULL;
}

void	row_list_clean(row_t **list)
{
	row_t	*next;

	while (*list != NULL)
	{
		next = (*list)->next;
		row_delete(*list);
		*list = next;
	}
}

