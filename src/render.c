#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "render.h"
#include "terminal.h"

#define CI_VERSION "0.0.1"

static void	render_rows(dbuff_t *buffer, row_t *row)
{
	int	len;

	len = row->render_size - t_config.coloff;
	if (len < 0)
		len = 0;
	if (len > t_config.wincols)
		len = t_config.wincols;
	dbuff_append(buffer, &row->render[t_config.coloff], len);
}

static void	render_welcome(dbuff_t *buffer)
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

void render_file(dbuff_t *buffer)
{
	int y;

	for (y = 0; y < t_config.nrows && y < t_config.winrows; y++)
	{
		dbuff_append(buffer, "\x1b[K", 3);
		render_rows(buffer, row_search_by_index(t_config.rows, y + t_config.rowoff));
		dbuff_append(buffer, "\r\n", 2);
	}
	for (; y < t_config.winrows; y++)
	{
		dbuff_append(buffer, "\x1b[K", 3);
		if (t_config.nrows == 0 && y == t_config.winrows / 3)
			render_welcome(buffer);
		else
			dbuff_append(buffer, "~", 1);
		dbuff_append(buffer, "\r\n", 2);
	}
	dbuff_append(buffer, "\x1b[K", 3);
}

void	render_status_bar(dbuff_t *buffer)
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
	dbuff_append(buffer, "\x1b[m", 3);
}

void	render_msg_bar(dbuff_t *buffer)
{
	int	len;

	dbuff_append(buffer, "\x1b[K", 3);
	len = strlen(t_config.status_msg);
	if (len > t_config.wincols)
		len = t_config.wincols;
	if (len && time(NULL) - t_config.status_time < 5)
		dbuff_append(buffer, t_config.status_msg, len);
}
