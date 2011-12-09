#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <stdio.h>

#include "wm.h"

int main(void)
{
	Display * dis;
	Window root;
	int shift_flag = 0;
	int wm_enabled = 0;

	dis = XOpenDisplay(NULL);

	root = DefaultRootWindow(dis);

	if (XGrabKeyboard(dis, root, True, GrabModeAsync, GrabModeAsync, 0))
	{
		return 1;
	}

	init_wm();

	while(1) {
		XEvent e;

		XNextEvent(dis, &e);

		if (e.type == KeyPress)
		{	
			switch(XLookupKeysym(&(e.xkey),0))
			{
				case XK_Tab:
					if (wm_enabled) switch_window();
					break;
				case XK_Right:
					if (wm_enabled) move_window_to(WM_RIGHT);
					break;
				case XK_Left:
					if (wm_enabled) move_window_to(WM_LEFT);
					break;
				case XK_Down:
					if (wm_enabled) move_window_to(WM_DOWN);
					break;
				case XK_Up:
					if (wm_enabled) move_window_to(WM_UP);
					break;
				case XK_Shift_L:
					shift_flag = 1;
					break;
				case XK_F9:
					if (shift_flag && !wm_enabled) wm_enabled = 1;
					else if (shift_flag && wm_enabled) wm_enabled = 0;
					break;
				case XK_F8:
					if (wm_enabled) close_window();
					break;
				case XK_Escape:
					goto out;
			}
		}
		else if (e.type == KeyRelease && XLookupKeysym(&(e.xkey),0) == XK_Shift_R) 
		{
			shift_flag = 0;
		}
	}
	out:
		XUngrabKeyboard(dis, 0);

	return 0;
}
