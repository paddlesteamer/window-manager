#include <stdio.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

#include "wm.h"

static int init_desktop_count();
static int init_top_level_window_list();
static int get_property(Window win, const char * prop_name, unsigned char ** data);
static void send_message(const char *type, Window win, unsigned long data0);

static struct {
	Window *set;
	unsigned int length;
} top_level_window_list;

static unsigned long desktop_count = 0;

static Display *display = NULL;

int init_wm()
{
	return (init_top_level_window_list() + init_desktop_count());
}

static int init_desktop_count()
{
	Window root;
	unsigned char *number_data;

	if (display == NULL) 
	{
		if ( (display = XOpenDisplay(NULL)) == NULL ) return -1;
	}

	root = XDefaultRootWindow(display);
	

	if ( get_property(root,"_NET_NUMBER_OF_DESKTOPS", &number_data) < -1 )
	{
		return -1;
	}

	desktop_count = ((unsigned long *) number_data)[0];

	XFree(number_data);
	return 0;
}

static int init_top_level_window_list()
{
	Window root, win;
	unsigned char *list_data, *id_data;
	unsigned long list_length, list_size, *client_list;
	unsigned long i;

	if (display == NULL) 
	{
		if ( (display = XOpenDisplay(NULL)) == NULL ) return -1;
	}

	root = XDefaultRootWindow(display);

	list_length = 0;
	if ( (list_length = get_property(root, "_NET_CLIENT_LIST", &list_data)) < 0 )
	{
		return -1;
	}
	else if (list_length)
	{
		list_size = 32;

		if ( (top_level_window_list.set = (Window *) calloc(list_size, sizeof(Window))) == NULL)
		{
			return -1;
		}
	}

	client_list = (unsigned long *) list_data;

	for (i=0; i<list_length; i++) {

		win = (Window) client_list[i];

		if ( get_property(win, "_NET_WM_DESKTOP", &id_data) < 0 )
		{
			return -1;
		}

		if ( ((unsigned int *) id_data)[0] != 0xffffffff)
			top_level_window_list.set[top_level_window_list.length++] = win;
		
		XFree(id_data);
	}

	XFree(list_data);

	return 0;
}

void list_windows()
{
	XTextProperty name_info;
	int i;

	if (display == NULL) return;

	for (i=0; i<top_level_window_list.length; i++)
	{
		XGetWMName(display, top_level_window_list.set[i], &name_info);

		if (name_info.value != NULL)
			printf("  0x%.8lx %s\n", (unsigned long) top_level_window_list.set[i], (char *) name_info.value);
	}
}

void switch_desktop()
{
	Window root;
	unsigned char *desk_num;
	unsigned long new_desk_num;

	root = XDefaultRootWindow(display);

	if ( get_property(root, "_NET_CURRENT_DESKTOP", &desk_num) < 0)	
	{
		return;
	}

	new_desk_num = (*((unsigned long *) desk_num) + 1) % desktop_count;
	XFree(desk_num);

	send_message("_NET_CURRENT_DESKTOP", root, new_desk_num);
}

void switch_window()
{
	unsigned char *current_window_data;
	Window root, current_window;
	int i;

	root = XDefaultRootWindow(display);

	if ( get_property(root, "_NET_ACTIVE_WINDOW", &current_window_data) < 0)
	{
		return;
	}

	current_window = *((Window *) current_window_data);
	XFree(current_window_data);
	printf("cw: %ld ", (unsigned long) current_window);

	for (i=0; i<top_level_window_list.length 
	         && top_level_window_list.set[i] != current_window; i++);

	// will be replaced by init windowz
	if (i == top_level_window_list.length) return;
	else if (i == top_level_window_list.length-1) i=-1;

	send_message("_NET_ACTIVE_WINDOW", top_level_window_list.set[i+1], 0);
}

static int get_property(Window win, const char * prop_name, unsigned char ** data)
{
	Atom prop_type, ret_prop_type;
	int ret_format;
	unsigned long ret_length, ret_bytes_after, byte_length;
	unsigned char * ret_data;

	*data = NULL;

	if ( (prop_type = XInternAtom(display, prop_name, False)) == None)
	{
		return -1;
	}

	if ( XGetWindowProperty(display,
	                        win,
	                        prop_type,
	                        0L,
	                        ~0L,
	                        False,
	                        AnyPropertyType,
	                        &ret_prop_type,
	                        &ret_format,
	                        &ret_length,
	                        &ret_bytes_after,
	                        &ret_data) != Success )
	{
		return -1;
	}

#ifdef _64_BIT_OS
	if (ret_format == 32) ret_format = 64;
#endif
	byte_length = (ret_format/8) * ret_length;
	
	*data = (unsigned char *) malloc(byte_length);

	memcpy(*data, ret_data, byte_length);

	XFree(ret_data);

	return ret_length;
}

static void send_message(const char *type, Window win, 
                        unsigned long data0)
{
	Window root;
	XEvent event;
	long mask = SubstructureRedirectMask | SubstructureNotifyMask;
	int r;

	root = XDefaultRootWindow(display);

	event.xclient.type = ClientMessage;
	event.xclient.serial = 0;
	event.xclient.send_event = True;
	event.xclient.message_type = XInternAtom(display, type, False);
	event.xclient.window = win;
	event.xclient.format = 32;
	event.xclient.data.l[0] = data0;
	event.xclient.data.l[1] = 0;
	event.xclient.data.l[2] = 0;
	event.xclient.data.l[3] = 0;
	event.xclient.data.l[4] = 0;

	r = XSendEvent(display, root, False, mask, &event);
	printf("r: %d\n", r);

	XFlush(display);
}
