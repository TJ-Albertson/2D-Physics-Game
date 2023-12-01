# Makefile
CC = gcc
CFLAGS = -std=c89 -Iinclude
LDFLAGS = -Llib -lglfw3 -lglfw3 -lopengl32 -lkernel32 -luser32 -lgdi32

SRC = main.c glad.c
OBJ = $(SRC:.c=.o)

.PHONY: all clean

all: physics_game_2d

physics_game_2d: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: physics_game_2d
	./physics_game_2d

clean:
	del $(OBJ) physics_game_2d.exe
