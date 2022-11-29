HEAD = $(wildcard src/*.h)
SRC = $(wildcard src/*.c)
TEMP = $(SRC:.c=.o)
OBJ = $(addprefix build/,$(notdir $(TEMP)))
$(info ${OBJ})

all: FindYourPath

FindYourPath: $(OBJ)
	gcc $^ -o $@

$(OBJ): $(SRC) $(HEAD)
	gcc -c $< -o $@

clean:
	rm -f FindYourPath build/*

run: FindYourPath
	./FindYourPath