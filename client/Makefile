CFLAGS = -Wall -Werror -pedantic -ggdb3 -std=c++11
main: main.o client.o buffer.o
	g++ -o main main.o client.o buffer.o
client.o: client.cpp client.h
	g++ $(CFLAGS) -c client.cpp
buffer.o: buffer.cpp buffer.h
    g++ $(CFLAGS) -c buffer.cpp
main.o: main.cpp client.h buffer.h
	g++ $(CFLAGS) -c main.cpp