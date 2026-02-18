#ifndef CI_H
# define CI_H

# define CTRL_KEY(k) (k & 0x1f)

typedef enum keys_e
{
	QUIT = CTRL_KEY('q')
}	keys_t;

keys_t	ci_key_read();
void	ci_key_process();
void	ci_refresh();
void	ci_draw_rows();

#endif

