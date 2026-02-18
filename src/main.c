#include <stdlib.h>

#include "key.h"
#include "screen.h"
#include "terminal.h"
#include "file.h"

int main(int argc, char **argv)
{
	terminal_init();
	terminal_rawmode_enable();
	if (argc > 1)
		file_open(argv[1]);
	terminal_set_status_msg("HELP: CTRL-Q = quit");
	atexit(terminal_rawmode_disable);
	while (1)
	{
		screen_refresh();
		key_process();
	}
}

