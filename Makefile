
CC := gcc
CFLAGS := -Wall -std=c99 -Wno-missing-braces
INC = -I include/
LIB = -L lib/ -lraylib -lGL -lm -lpthread -ldl -lrt 

out: main.o
	$(CC) main.o -o out $(CFLAGS) $(INC) $(LIB)

main.o: main.c
	$(CC) -c main.c $(CFLAGS) $(INC) $(LIB)

clean:
	rm *.o

