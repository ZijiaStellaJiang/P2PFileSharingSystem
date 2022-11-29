CFLAGS = -Wall -Werror -pedantic -ggdb3 -std=c++11
main: main.o server.o
	g++ -o main main.o server.o
server.o: server.cpp server.h
	g++ $(CFLAGS) -c server.cpp
main.o: main.cpp server.h
	g++ $(CFLAGS) -c main.cpp