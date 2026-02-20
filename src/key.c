#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

#include "key.h"
#include "row.h"
#include "terminal.h"
#include "window.h"
#include "cursor.h"
#include "utils/die.h"

static keys_t	key_check_command(const char c)
{
	switch(c)
	{
		case '1':
			return HOME;
		case '3':
			return DEL;
		case '4':
			return END;
		case '5':
			return PAGE_UP;
		case '6':
			return PAGE_DOWN;
		case '7':
			return HOME;
		case '8':
			return END;
	}
	return ESCAPE;
}

static keys_t	key_check_arrow(const char c)
{
	switch (c)
	{
		case 'A':
			return UP;
		case 'B':
			return DOWN;
		case 'C':
			return RIGHT;
		case 'D':
			return LEFT;
		case 'H':
			return HOME;
		case 'F':
			return END;
	}
	return ESCAPE;
}

static keys_t	key_read_escape()
{
	char	seq[3];

	if (read(STDIN_FILENO, seq, 2) != 2)
		return ESCAPE;
	if (seq[0] == '[' && seq[1] >= '0' && seq[1] <= '9')
	{
		if (read(STDIN_FILENO, &seq[2], 1) != 1 || seq[2] != '~')
			return ESCAPE;
		return key_check_command(seq[1]);
	}
	return key_check_arrow(seq[1]);
}

keys_t	key_read(int *c)
{
	int		nb;

	while ((nb = read(STDIN_FILENO, c, 1)) != 1)
	{
		if (nb < 0 && errno != EAGAIN)
			die("read");
	}
	if (*c == '\x1b')
		return key_read_escape();
	if (*c == CTRL_KEY('l'))
		return ESCAPE;
	if (*c == CTRL_KEY('q'))
		return QUIT;
	if (*c == 127 || *c == CTRL_KEY('h'))
		return BACKSPACE;
	if (*c == '\r')
		return CARRIAGE;
	return *c;
}

void	key_process()
{
	int	c;

	switch (key_read(&c))
	{
		case ESCAPE:
		case BACKSPACE:
		case CARRIAGE:
		case DEL:
			break;
		case QUIT:
			window_clear();
			terminal_free();
			exit(EXIT_SUCCESS);
		case UP:
			return (cursor_move_up(), cursor_move_eol());
		case DOWN:
			return (cursor_move_down(), cursor_move_eol());
		case LEFT:
			return cursor_move_left();
		case RIGHT:
			return cursor_move_right();
		case PAGE_UP:
			return cursor_move_top();
		case PAGE_DOWN:
			return cursor_move_bottom();
		case HOME:
			return cursor_move_begin();
		case END:
			return cursor_move_end();
		default:
			return key_insert(c);
	}
}

void	key_insert(int c)
{
	row_t	*row;

	if (t_config.cursor_y == t_config.nrows)
	{
		if ((row = row_create("", 0)) == NULL)
			return (terminal_free(), die("row_create"));
		row_list_append(&t_config.rows, row);
		t_config.nrows++;
	}
	row_insert_char(row_search_by_index(t_config.rows, t_config.cursor_y),
			t_config.cursor_x, c);
}

