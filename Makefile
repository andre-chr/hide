OBJS = steganographylib.h steganographylib.c
CC = gcc
GFLAGS = -Wall
CFLAGS = `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs`

all : hide unhide

hide : hide.c $(OBJS)
	$(CC) -o $@ hide.c $(OBJS) hidelib.h hidelib.c $(GFLAGS) $(CFLAGS) $(LDFLAGS) -g

unhide : unhide.c $(OBJS)
	$(CC) -o $@ unhide.c $(OBJS) unhidelib.h unhidelib.c $(GFLAGS) -g

clean:
	\rm hide unhide


