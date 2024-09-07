SRCDIR=./src
INCDIR=./include
CC=gcc
INCLUDE = -IC:/msys64/mingw64/include/SDL2 -I$(INCDIR) 
CFLAGS = -g $(INCLUDE) -c
LDFLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lm #-mwindows

snake: main.o board.o
	$(CC) -o snake main.o board.o $(LDFLAGS)

main.o: $(SRCDIR)/main.c
	$(CC) $(CFLAGS) $(SRCDIR)/main.c

board.o: $(SRCDIR)/board.c $(INCDIR)/board.h
	$(CC) $(CFLAGS) $(SRCDIR)/board.c

clean:
	rm *.o
	rm *.exe