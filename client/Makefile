CFLAGS = -Wall -Werror -pedantic -ggdb3 -std=c++11
main: main.o client.o
	g++ -o main main.o client.o
client.o: client.cpp client.h
	g++ $(CFLAGS) -c client.cpp
main.o: main.cpp client.h
	g++ $(CFLAGS) -c main.cpp