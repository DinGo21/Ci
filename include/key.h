#ifndef KEY_H
# define KEY_H

# define CTRL_KEY(k) (k & 0x1f)

typedef enum keys_e
{
	QUIT = 1000,
	BACKSPACE,
	CARRIAGE,
	ESCAPE,
	UP,
	DOWN,
	LEFT,
	RIGHT,
	PAGE_UP,
	PAGE_DOWN,
	HOME,
	END,
	DEL
}	keys_t;

keys_t	key_read(int *c);
void	key_process();
void	key_insert(int c);

#endif

