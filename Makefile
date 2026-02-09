CC      = gcc
CFLAGS  = -Wall -Wextra -Wpedantic -std=c11

OBJS    = auth.o

all: main auth_test

auth.o: auth.c auth.h
	$(CC) $(CFLAGS) -c auth.c

main.o: main.c auth.h
	$(CC) $(CFLAGS) -c main.c

auth_test.o: auth_test.c auth.h
	$(CC) $(CFLAGS) -c auth_test.c

main: main.o $(OBJS)
	$(CC) $(CFLAGS) -o main main.o $(OBJS)

auth_test: auth_test.o $(OBJS)
	$(CC) $(CFLAGS) -o auth_test auth_test.o $(OBJS)

.PHONY: clean run test

run: main
	./main

test: auth_test
	./auth_test

clean:
	del /Q *.o 2>nul || rm -f *.o
	del /Q main.exe auth_test.exe 2>nul || rm -f main auth_test

