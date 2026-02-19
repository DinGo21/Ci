#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "terminal.h"
#include "utils/die.h"

#define	CURSOR_BUFFER_SIZE 32

term_config_t	t_config;

static int	terminal_get_cursor_position()
{
	char			buf[CURSOR_BUFFER_SIZE + 1];
	unsigned long	i;

	if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12)
		return -1;
	if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4)
		return -1;
	i = 0;
	while (i < sizeof(buf))
	{
		if (read(STDIN_FILENO, buf + i, 1) != 1 || buf[i] == 'R')
			break;
		i++;
	}
	if (buf[0] != '\x1b' || buf[1] != '[')
		return -1;
	if (sscanf(buf + 2, "%d;%d", &t_config.wincols, &t_config.winrows) != 2)
		return -1;
	return 0;
}

static int	terminal_get_window_size()
{
	struct winsize	ws;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) < 0 || ws.ws_col == 0)
		return terminal_get_cursor_position();
	t_config.wincols = ws.ws_col;
	t_config.winrows = ws.ws_row - 2;
	return 0;
}

void	terminal_init()
{
	if (tcgetattr(STDIN_FILENO, &t_config.termios) < 0)
		die("tcgetattr");
	if (terminal_get_window_size() < 0)
		die("terminal_get_window_size");
	t_config.status_msg[0] = '\0';
	t_config.filename = NULL;
	t_config.rows = NULL;
	t_config.status_time = 0;
	t_config.render_x = 0;
	t_config.cursor_x = 0;
	t_config.cursor_y = 0;
	t_config.nrows = 0;
	t_config.rowoff = 0;
	t_config.coloff = 0;
}

void	terminal_free()
{
	row_list_clean(&t_config.rows);
	if (t_config.filename != NULL)
		free(t_config.filename);
	t_config.filename = NULL;
	t_config.rows = NULL;
	t_config.render_x = 0;
	t_config.cursor_x = 0;
	t_config.cursor_y = 0;
	t_config.nrows = 0;
	t_config.rowoff = 0;
	t_config.coloff = 0;
}

void	terminal_set_status_msg(const char *fmt, ...)
{
	va_list	vars;

	va_start(vars, fmt);
	vsnprintf(t_config.status_msg, sizeof(t_config.status_msg), fmt, vars);
	va_end(vars);
	t_config.status_time = time(NULL);
}

void	terminal_rawmode_enable()
{
	struct termios	raw;

	raw = t_config.termios;
	raw.c_iflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP);
	raw.c_oflag &= ~(OPOST);
	raw.c_cflag |= (CS8);
	raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
	raw.c_cc[VMIN] = 1;
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) < 0)
		die("tcsetattr");
}

void	terminal_rawmode_disable()
{
	struct termios	raw;

	raw = t_config.termios;
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) < 0)
		die("tcsetattr");
}

