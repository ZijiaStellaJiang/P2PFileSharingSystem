CFLAGS = -Wall -Werror -pedantic -ggdb3 -std=c++11 
INC_PRO = ./protocol
PROTOCOL_CC = ./protocol/server_peer.pb.cc
PROFLAG = -lpqxx -lpq -lprotobuf

main: main.o server.o p2pserver.o request.o buffer.o database.o
	g++ -o main main.o server.o p2pserver.o request.o buffer.o database.o $(PROTOCOL_CC) -lpthread $(PROFLAG)
buffer.o: buffer.cpp buffer.h
	g++ $(CFLAGS) -c buffer.cpp $(PROTOCOL_CC) -lpthread $(PROFLAG)
request.o: request.cpp request.h 
	g++ $(CFLAGS) -c request.cpp $(PROTOCOL_CC) -lpthread $(PROFLAG)
database.o: database.cpp database.h
	g++ $(CFLAGS) -c database.cpp $(PROTOCOL_CC) -lpthread $(PROFLAG)
server.o: server.cpp server.h buffer.h
	g++ $(CFLAGS) -c server.cpp $(PROTOCOL_CC) -lpthread $(PROFLAG) 
p2pserver.o: p2pserver.cpp p2pserver.h server.h request.h database.h
	g++ $(CFLAGS) -c p2pserver.cpp $(PROTOCOL_CC) -lpthread $(PROFLAG)
main.o: main.cpp server.h p2pserver.h request.h buffer.h database.h
	g++ $(CFLAGS) -c main.cpp -lpthread $(PROTOCOL_CC) $(PROFLAG)

clean:
	rm -f main *.o

