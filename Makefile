test1: wm.c wm.h test1.c
	gcc -o test1 -Wall -lX11 -lncurses -D_64_BIT_OS wm.o test1.c

test2: wm test2.c
	gcc -o test2 -Wall -lX11 -D_64_BIT_OS wm.o test2.c

test3: wm test3.c
	gcc -o test3 -Wall -lX11 -D_64_BIT_OS wm.o test3.c

wm: wm.c wm.h
	gcc -c -Wall -o wm.o -lX11 -D_64_BIT_OS wm.c
