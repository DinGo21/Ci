#ifndef TERMINAL_H
# define TERMINAL_H

# include <termios.h>

# include "row.h"

# define MESSAGE_SIZE 80

typedef struct term_config_s
{
	char			status_msg[MESSAGE_SIZE];
	struct termios	termios;
	row_t			*rows;
	char			*filename;
	long			status_time;
	int				nrows;
	int				rowoff;
	int				coloff;
	int				winrows;
	int				wincols;
	int				render_x;
	int				cursor_x;
	int				cursor_y;
}	term_config_t;

void	terminal_init();
void	terminal_free();
void	terminal_set_status_msg(const char *fmt, ...);
void	terminal_rawmode_enable();
void	terminal_rawmode_disable();

extern term_config_t	t_config;

#endif

