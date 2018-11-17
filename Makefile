OBJ =  ./out/main.o
TEST_OBJ = 
CC = gcc
ARGS = -Wall -Werror -std=c99
LD_ARGS = -lpng -lz
project : $(OBJ)
	$(CC) -o project $(OBJ) $(LD_ARGS)

tests: 
check: tests
	


./out/main.o: main.c out
	$(CC) $(ARGS) -c main.c -o ./out/main.o

out:
	mkdir -p out
.PHONY : clean
clean :
	rm project ./out/*.o
