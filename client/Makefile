CFLAGS = -Wall -Werror -pedantic -ggdb3 -std=c++11
SERVER_H = ../server/server.h
SERVER_C = ../server/server.cpp
PROTOCOL_CC = ../server/protocol/server_peer.pb.cc
PROFLAG = -lpqxx -lpq -lprotobuf

main: main.o client.o buffer.o peerServer.o server.o peerClient.o p2pClient.o
	g++ -o main main.o client.o buffer.o peerServer.o server.o peerClient.o p2pClient.o $(PROTOCOL_CC) -pthread $(PROFLAG)
p2pClient.o: p2pClient.cpp p2pClient.h client.h buffer.h peerClient.h peerServer.h $(SERVER_H)
	g++ $(CFLAGS) -c p2pClient.cpp $(PROTOCOL_CC) $(PROFLAG) -lpthread
buffer.o: buffer.cpp buffer.h
	g++ $(CFLAGS) -c buffer.cpp $(PROTOCOL_CC) $(PROFLAG) -lpthread
client.o: client.cpp client.h buffer.h
	g++ $(CFLAGS) -c client.cpp -pthread $(PROTOCOL_CC) $(PROFLAG) -lpthread
peerServer.o: peerServer.cpp peerServer.h $(SERVER_H)
	g++ $(CFLAGS) -c peerServer.cpp -pthread
peerClient.o: peerClient.cpp peerClient.h client.h buffer.h
	g++ $(CFLAGS) -c peerClient.cpp -pthread $(PROTOCOL_CC) $(PROFLAG) -lpthread
server.o: $(SERVER_C) $(SERVER_H)
	g++ $(CFLAGS) -c $(SERVER_C) -pthread
main.o: main.cpp client.h peerServer.h $(SERVER_H) peerClient.h buffer.h p2pClient.h
	g++ $(CFLAGS) -c main.cpp p2pClient.cpp -pthread $(PROTOCOL_CC) $(PROFLAG) -lpthread

clean:
	rm -f main *.o

	

# main: main.o client.o buffer.o peerServer.o server.o peerClient.o
# 	g++ -o main main.o client.o buffer.o peerServer.o server.o peerClient.o $(PROTOCOL_CC) -pthread $(PROFLAG)
# buffer.o: buffer.cpp buffer.h
# 	g++ $(CFLAGS) -c buffer.cpp $(PROTOCOL_CC) $(PROFLAG) -lpthread
# client.o: client.cpp client.h buffer.h
# 	g++ $(CFLAGS) -c client.cpp -pthread $(PROTOCOL_CC) $(PROFLAG) -lpthread
# peerServer.o: peerServer.cpp peerServer.h $(SERVER_H)
# 	g++ $(CFLAGS) -c peerServer.cpp -pthread
# peerClient.o: peerClient.cpp peerClient.h client.h buffer.h
# 	g++ $(CFLAGS) -c peerClient.cpp -pthread $(PROTOCOL_CC) $(PROFLAG) -lpthread
# server.o: $(SERVER_C) $(SERVER_H)
# 	g++ $(CFLAGS) -c $(SERVER_C) -pthread
# main.o: main.cpp client.h peerServer.h $(SERVER_H) peerClient.h buffer.h
# 	g++ $(CFLAGS) -c main.cpp -pthread $(PROTOCOL_CC) $(PROFLAG) -lpthread