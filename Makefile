HEAD = $(wildcard src/*.h)
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

all: FindYourPath

FindYourPath: $(OBJ)
	gcc $^ -o $@

$(OBJ): $(SRC) $(HEAD)
	gcc -c $< -o $@

clean:
	rm -f FindYourPath $(OBJ)

run: FindYourPath
	./FindYourPath