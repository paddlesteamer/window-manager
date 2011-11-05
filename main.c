#include <unistd.h>
#include "wm.h"

int main()
{
	init_wm();

	switch_window();
	sleep(1);
	switch_window();
	sleep(1);
	switch_window();
	sleep(1);
	switch_window();

	return 0;
}
