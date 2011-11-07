wm: wm.c wm.h main.c
	gcc -o wm -Wall -lX11 -lncurses -D_64_BIT_OS wm.c main.c
