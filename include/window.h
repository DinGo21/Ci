#ifndef WINDOW_H
# define WINDOW_H

# include "buffer.h"

void	window_refresh();
void	window_clear();
void	window_set_cursor_pos(dbuff_t *buffer, const int y, const int x);

#endif

