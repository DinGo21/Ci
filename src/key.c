#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

#include "key.h"
#include "terminal.h"
#include "screen.h"
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

keys_t	key_read()
{
	char	c;
	int		nb;

	while ((nb = read(STDIN_FILENO, &c, 1)) != 1)
	{
		if (nb < 0 && errno != EAGAIN)
			die("read");
	}
	if (c == '\x1b')
		return key_read_escape();
	if (c == CTRL_KEY('q'))
		return QUIT;
	return c;
}

void	key_process()
{
	switch (key_read())
	{
		case QUIT:
			screen_clear();
			terminal_free();
			exit(EXIT_SUCCESS);
		case ESCAPE:
			break;
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
		case DEL:
			break;
	}
}

