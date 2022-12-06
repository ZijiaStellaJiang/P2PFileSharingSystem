CFLAGS = -Wall -Werror -pedantic -ggdb3 -std=c++11
SERVER_H = ../server/server.h
SERVER_C = ../server/server.cpp

main: main.o client.o peerServer.o server.o peerClient.o
	g++ -o main main.o client.o peerServer.o server.o peerClient.o -pthread
client.o: client.cpp client.h
	g++ $(CFLAGS) -c client.cpp -pthread
peerServer.o: peerServer.cpp peerServer.h $(SERVER_H)
	g++ $(CFLAGS) -c peerServer.cpp -pthread
peerClient.o: peerClient.cpp peerClient.h client.h
	g++ $(CFLAGS) -c peerClient.cpp -pthread
server.o: $(SERVER_C) $(SERVER_H)
	g++ $(CFLAGS) -c $(SERVER_C) -pthread
main.o: main.cpp client.h peerServer.h $(SERVER_H) peerClient.h
	g++ $(CFLAGS) -c main.cpp -pthread

clean:
	rm -f main *.o
	rm -rf ./clientFile/*