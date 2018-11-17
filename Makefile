OBJ =  ./out/main.o
TEST_OBJ = 
CC = gcc
ARGS = -Wall -Werror -std=c99
LD_ARGS = -lpng -lz -O3
project : $(OBJ)
	$(CC) -o png2ansi $(OBJ) $(LD_ARGS)

./out/main.o: main.c out
	$(CC) $(ARGS) -c main.c -o ./out/main.o

out:
	mkdir -p out
.PHONY : clean
clean :
	rm png2ansi ./out/*.o
