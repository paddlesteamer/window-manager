#ifndef _WM_H_
#define _WM_H_

#define WM_RIGHT 0
#define WM_LEFT 1
#define WM_UP 2
#define WM_DOWN 3

/* every program should call
   this before every program */
int init_wm();

/* prints names of the windows to
   stdout */
void list_windows();

/* switches current desktop */
void switch_desktop();

/* switches current active window */
void switch_window();

/* moves current active window to
   specified direction */
void move_window_to(int direction);

/* closes current active window */
void close_window();

#endif
