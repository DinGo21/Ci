#include "row.h"
#include "terminal.h"
#include "cursor.h"

void	cursor_move_up()
{
	if (t_config.cursor_y > 0)
		t_config.cursor_y--;
}

void	cursor_move_down()
{
	if (t_config.cursor_y < t_config.nrows)
		t_config.cursor_y++;
}

void	cursor_move_left()
{
	if (t_config.cursor_x > 0)
		t_config.cursor_x--;
}

void	cursor_move_right()
{
	row_t	*row;

	row = row_search_by_index(t_config.rows, t_config.cursor_y);
	if (row && t_config.cursor_x < row->raw_size)
		t_config.cursor_x++;
}

void	cursor_move_top()
{
	int	times;

	t_config.cursor_y = t_config.rowoff;
	times = t_config.winrows;
	while (times--)
		cursor_move_up();
}

void	cursor_move_bottom()
{
	int	times;

	t_config.cursor_y = t_config.rowoff + t_config.winrows;
	if (t_config.cursor_y > t_config.nrows)
		t_config.cursor_y = t_config.nrows;
	times = t_config.winrows;
	while (times--)
		cursor_move_down();
}

void	cursor_move_begin()
{
	t_config.cursor_x = 1;
}

void	cursor_move_end()
{
	row_t	*row;

	row = row_search_by_index(t_config.rows, t_config.cursor_y);
	if (row)
		t_config.cursor_x = row->raw_size;
}

void	cursor_move_eol()
{
	row_t	*row;

	row = row_search_by_index(t_config.rows, t_config.cursor_y);
	if (row && t_config.cursor_x > row->raw_size)
		t_config.cursor_x = row->raw_size;
}

