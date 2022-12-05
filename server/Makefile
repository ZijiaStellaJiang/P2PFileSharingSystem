CFLAGS = -Wall -Werror -pedantic -ggdb3 -std=c++11 
INC_PRO = ./protocol
main: main.o server.o p2pserver.o request.o buffer.o
	g++ -o main main.o server.o p2pserver.o request.o buffer.o ./protocol/server_peer.pb.cc -lpthread -lpqxx -lpq -lprotobuf
buffer.o: buffer.cpp buffer.h
	g++ $(CFLAGS) -c buffer.cpp ./protocol/server_peer.pb.cc -lpthread -lpqxx -lpq -lprotobuf 
request.o: request.cpp request.h 
	g++ $(CFLAGS) -c request.cpp ./protocol/server_peer.pb.cc -lpthread -lpqxx -lpq -lprotobuf 
server.o: server.cpp server.h buffer.h
	g++ $(CFLAGS) -c server.cpp ./protocol/server_peer.pb.cc -lpthread -lpqxx -lpq -lprotobuf 
p2pserver.o: p2pserver.cpp p2pserver.h server.h request.h
	g++ $(CFLAGS) -c p2pserver.cpp ./protocol/server_peer.pb.cc -lpthread -lpqxx -lpq -lprotobuf 
main.o: main.cpp server.h p2pserver.h request.h buffer.h
	g++ $(CFLAGS) -c main.cpp -lpthread ./protocol/server_peer.pb.cc -lpqxx -lpq -lprotobuf 


