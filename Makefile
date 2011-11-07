test1: wm.c wm.h test1.c
	gcc -o wm -Wall -lX11 -lncurses -D_64_BIT_OS wm.c test1.c

test2: wm.c wm.h test2.c
	gcc -o wm -Wall -lX11 -D_64_BIT_OS wm.c test2.c

