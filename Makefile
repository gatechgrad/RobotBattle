all: arena.c load_image.c splash.c load_image.c
	gcc -Wall -pedantic -ansi arena.c load_image.c object.c -o arena -lGL -lGLU `sdl-config --cflags --libs` 
	gcc -Wall -pedantic -ansi select.c load_image.c -o select -L/usr/X11R6/lib -lGL -lGLU -lXxf86vm
	gcc -Wall -pedantic -ansi splash.c load_image.c -o splash -L/usr/X11R6/lib -lGL -lGLU -lXxf86vm

arena: arena.c load_image.c 
	gcc -Wall -pedantic -ansi arena.c load_image.c -o arena -L/usr/X11R6/lib -lGL -lGLU -lXxf86vm -lm

select: load_image.c select.c
	gcc -Wall -pedantic -ansi select.c load_image.c -o select -L/usr/X11R6/lib -lGL -lGLU -lXxf86vm


splash: splash.c load_image.c
	gcc -Wall -pedantic -ansi splash.c load_image.c -o splash -L/usr/X11R6/lib -lGL -lGLU -lXxf86vm

clean:
	rm arena
	rm select
	rm splash
