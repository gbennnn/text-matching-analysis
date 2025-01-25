CC = g++
OPT = -O3
#OPT = -g
WARN = -Wall
CFLAGS = $(OPT) $(WARN) $(INC) $(LIB)

SIM_OBJ = main.o

all: main


# rule for making sim

main:
	$(CC) -o main $(CFLAGS) main.cpp -lm
	@echo "\n---------------DONE -----------------\n--------Type make run to run the program--------\n"
run:
	./main

clean:
	rm -f *.o main
	@echo "\n-----------Executable and object files deleted------------\n"