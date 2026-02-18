#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "screen.h"
#include "terminal.h"
#include "buffer.h"

#define CI_VERSION "0.0.1"
#define	CURSOR_POS_SIZE 32

static void	screen_scroll()
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

static void	screen_draw_rows(dbuff_t *buffer, row_t *row)
{
	int	len;

	len = row->render_size - t_config.coloff;
	if (len < 0)
		len = 0;
	if (len > t_config.wincols)
		len = t_config.wincols;
	dbuff_append(buffer, &row->render[t_config.coloff], len);
}

static void	screen_draw_message(dbuff_t *buffer)
{
	char	message[MESSAGE_SIZE];
	int		len;
	int		pad;

	len = snprintf(message, sizeof(message),
			"CI Editor -- version %s", CI_VERSION);
	if (len > t_config.wincols)
		len = t_config.wincols;
	pad = (t_config.wincols - len) / 2;
	dbuff_append(buffer, "~", 1);
	while (--pad)
		dbuff_append(buffer, " ", 1);
	dbuff_append(buffer, message, len);
}

static void	screen_message_bar(dbuff_t *buffer)
{
	int	len;

	len = strlen(t_config.status_msg);
	if (len > t_config.wincols)
		len = t_config.wincols;
	if (len && time(NULL) - t_config.status_time < 5)
		dbuff_append(buffer, t_config.status_msg, len);
}

static void	screen_status_bar(dbuff_t *buffer)
{
	char	buf[MESSAGE_SIZE];
	char	*file;
	int		len;
	int		rlen;

	file = "[No Name]";
	if (t_config.filename != NULL)
		file = t_config.filename;
	len = snprintf(buf, sizeof(buf),  "%.20s - %d lines", file, t_config.nrows);
	if (len > t_config.wincols)
		len = t_config.wincols;
	dbuff_append(buffer, "\x1b[7m", 4);
	dbuff_append(buffer, buf, len);
	rlen = snprintf(buf, sizeof(buf),  "%d/%d", t_config.cursor_y + 1, t_config.nrows);
	for (; len < t_config.wincols - rlen; len++)
		dbuff_append(buffer, " ", 1);
	if (len == t_config.wincols - rlen)
		dbuff_append(buffer, buf, rlen);
	dbuff_append(buffer, "\x1b[m\r\n\x1b[K", 8);
}

static void screen_draw(dbuff_t *buffer)
{
	int y;

	for (y = 0; y < t_config.nrows && y < t_config.winrows - 2; y++)
	{
		dbuff_append(buffer, "\x1b[K", 3);
		screen_draw_rows(buffer, row_search_by_index(t_config.rows, y + t_config.rowoff));
		dbuff_append(buffer, "\r\n", 2);
	}
	for (; y < t_config.winrows - 2; y++)
	{
		dbuff_append(buffer, "\x1b[K", 3);
		if (t_config.nrows == 0 && y == t_config.winrows / 3)
			screen_draw_message(buffer);
		else
			dbuff_append(buffer, "~", 1);
		dbuff_append(buffer, "\r\n", 2);
	}
	dbuff_append(buffer, "\x1b[K", 3);
	screen_status_bar(buffer);
	screen_message_bar(buffer);
}

void	screen_refresh()
{
	dbuff_t	buffer;
	char	cursor_pos[CURSOR_POS_SIZE];
	int		len;

	screen_scroll();
	dbuff_init(&buffer);
	dbuff_append(&buffer, "\x1b[?25l\x1b[H", 9);
	screen_draw(&buffer);
	len = snprintf(cursor_pos, sizeof(cursor_pos), "\x1b[%d;%dH",
			t_config.cursor_y - t_config.rowoff + 1,
			t_config.render_x - t_config.coloff + 1);
	dbuff_append(&buffer, cursor_pos, len);
	dbuff_append(&buffer, "\x1b[?25h", 6);
	write(STDOUT_FILENO, buffer.buf, buffer.size);
	dbuff_free(&buffer);
}

void	screen_clear()
{
	write(STDOUT_FILENO, "\x1b[2J\x1b[H", 7);
}

