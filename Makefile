OBJ =  ./out/main.o ./out/image.o
TEST_OBJ = 
CC = gcc
ARGS = -Wall -Werror -std=c99
LD_ARGS = -lpng -lz
project : $(OBJ)
	$(CC) -o png2ansi $(OBJ) $(LD_ARGS)

./out/main.o: main.c image.h out
	$(CC) $(ARGS) -c main.c -o ./out/main.o
./out/image.o: image.c image.h out
	$(CC) $(ARGS) -c image.c -o ./out/image.o

out:
	mkdir -p out
.PHONY : clean
clean :
	rm project ./out/*.o
