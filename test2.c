#include <X11/Xlib.h>
#include <X11/keysym.h>

#include "wm.h"

int main(void)
{
	Display * dis;
	Window root;

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
					switch_window();
					break;
				case XK_Right:
					move_window_to(WM_RIGHT);
					break;
				case XK_Left:
					move_window_to(WM_LEFT);
					break;
				case XK_Down:
					move_window_to(WM_DOWN);
					break;
				case XK_Up:
					move_window_to(WM_UP);
					break;
				case XK_Escape:
					goto out;
			}
		}
	}
	out:
		XUngrabKeyboard(dis, 0);

	return 0;
}
