HEAD = $(wildcard *.h)
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

all: game

game: $(OBJ)
	gcc $^ -o $@

%.o: %.c $(HEAD)
	gcc -c $< -o $@

clean:
	rm game *.o

run: game
	./game