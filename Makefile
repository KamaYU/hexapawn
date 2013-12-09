CC = g++
CFLAGS = -Wall
LDFLAGS = -lncurses
SOURCES = Board.cpp Pawn.cpp main.cpp
HEADERS = Board.h Pawn.h
TARGET = hexapawn

all: $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET) $(LDFLAGS)
