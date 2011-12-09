#ifndef _WM_H_
#define _WM_H_

#define WM_RIGHT 0
#define WM_LEFT 1
#define WM_UP 2
#define WM_DOWN 3

int init_wm();
void list_windows();
void switch_desktop();
void switch_window();
void move_window_to(int direction);
void close_window();

#endif
