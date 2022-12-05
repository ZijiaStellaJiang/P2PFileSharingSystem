CFLAGS = -Wall -Werror -pedantic -ggdb3 -std=c++11
main: main.o client.o buffer.o
	g++ -o main main.o client.o buffer.o ../server/protocol/server_peer.pb.cc -lpqxx -lpq -lprotobuf -lpthread
buffer.o: buffer.cpp buffer.h
	g++ $(CFLAGS) -c buffer.cpp ../server/protocol/server_peer.pb.cc -lpqxx -lpq -lprotobuf -lpthread
client.o: client.cpp client.h buffer.h
	g++ $(CFLAGS) -c client.cpp ../server/protocol/server_peer.pb.cc -lpqxx -lpq -lprotobuf -lpthread
main.o: main.cpp client.h buffer.h
	g++ $(CFLAGS) -c main.cpp ../server/protocol/server_peer.pb.cc -lpqxx -lpq -lprotobuf -lpthread