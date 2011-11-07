#include <unistd.h>
#include "wm.h"

int main()
{
	init_wm();
	
	switch_window();
	usleep(500000);
	move_window_to(WM_RIGHT);
	usleep(500000);
	switch_window();
	usleep(500000);
	move_window_to(WM_DOWN);
	usleep(500000);
	switch_window();
	usleep(500000);
	move_window_to(WM_LEFT);
	usleep(500000);
	switch_window();
	usleep(500000);
	move_window_to(WM_UP);
	usleep(500000);

	return 0;
}
