#include <stdio.h>
#include <unistd.h>

#include "window.h"
#include "buffer.h"
#include "terminal.h"
#include "render.h"

#define	CURSOR_POS_SIZE 32

static void	window_update_scroll()
{
	row_t	*row;

	row = row_search_by_index(t_config.rows, t_config.cursor_y);
	if (row == NULL)
		return;
	t_config.render_x = 0;
	for (int i = 0; i < t_config.cursor_x; i++)
	{
		if (row->raw[i] == '\t')
			t_config.render_x += (TAB_STOP - 1) - (t_config.render_x % TAB_STOP);
		t_config.render_x++;
	}
	if (t_config.cursor_y <= t_config.rowoff)
		t_config.rowoff = t_config.cursor_y;
	if (t_config.cursor_y >= t_config.rowoff + t_config.winrows)
		t_config.rowoff = t_config.cursor_y - t_config.winrows + 1;
	if (t_config.render_x <= t_config.coloff)
		t_config.coloff = t_config.render_x;
	if (t_config.render_x >= t_config.coloff + t_config.wincols)
		t_config.coloff = t_config.render_x - t_config.wincols + 1;
}

void	window_refresh()
{
	dbuff_t	buffer;

	window_update_scroll();
	dbuff_init(&buffer);
	dbuff_append(&buffer, "\x1b[?25l\x1b[H", 9);
	render_file(&buffer);
	render_status_bar(&buffer);
	window_set_cursor_pos(&buffer, t_config.winrows + 2, 1);
	render_msg_bar(&buffer);
	window_set_cursor_pos(&buffer, t_config.cursor_y - t_config.rowoff, 
		t_config.render_x - t_config.coloff);
	dbuff_append(&buffer, "\x1b[?25h", 6);
	write(STDOUT_FILENO, buffer.buf, buffer.size);
	dbuff_free(&buffer);
}

void	window_clear()
{
	write(STDOUT_FILENO, "\x1b[2J\x1b[H", 7);
}

void	window_set_cursor_pos(dbuff_t *buffer, const int y, const int x)
{
	char	buf[MESSAGE_SIZE];
	int		len;

	len = snprintf(buf, sizeof(buf), "\x1b[%d;%dH", y + 1, x + 1);
	dbuff_append(buffer, buf, len);
}


