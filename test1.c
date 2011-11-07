#include <ncurses.h>
#include "wm.h"

int main()
{
	int ch;

	init_wm();
	initscr();
	
	while((ch = getch()) != 'q')
	{
		switch(ch)
		{
			case 'd':
				move_window_to(WM_RIGHT);
				break;
			case 'a':
				move_window_to(WM_LEFT);
				break;
			case 's':
				move_window_to(WM_DOWN);
				break;
			case 'w':
				move_window_to(WM_UP);
				break;
		}
	}

	endwin();
	return 0;
}
