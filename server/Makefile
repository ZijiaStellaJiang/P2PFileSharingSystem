CFLAGS = -Wall -Werror -pedantic -ggdb3 -std=c++11 
main: main.o server.o p2pserver.o request.o 
	g++ -o main main.o server.o p2pserver.o request.o -lpthread
request.o: request.cpp request.h 
	g++ $(CFLAGS) -c request.cpp -lpthread
server.o: server.cpp server.h 
	g++ $(CFLAGS) -c server.cpp -lpthread	
p2pserver.o: p2pserver.cpp p2pserver.h server.h request.h
	g++ $(CFLAGS) -c p2pserver.cpp -lpthread
main.o: main.cpp server.h p2pserver.h request.h
	g++ $(CFLAGS) -c main.cpp -lpthread
