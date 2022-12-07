CFLAGS = -Wall -Werror -pedantic -ggdb3 -std=c++11 
INC_PRO = ./protocol
LIBS= -lpthread -lpqxx -lpq `pkg-config --cflags --libs protobuf`
main: main.o server.o p2pserver.o request.o database.o buffer.o
	g++ -o main main.o server.o p2pserver.o request.o database.o buffer.o ./protocol/server_peer.pb.cc $(LIBS)
buffer.o: buffer.cpp buffer.h
	g++ $(CFLAGS) -c buffer.cpp ./protocol/server_peer.pb.cc $(LIBS) 
request.o: request.cpp request.h 
	g++ $(CFLAGS) -c request.cpp ./protocol/server_peer.pb.cc $(LIBS)
server.o: server.cpp server.h buffer.h
	g++ $(CFLAGS) -c server.cpp ./protocol/server_peer.pb.cc $(LIBS)
database.o: database.cpp database.h
	g++ $(CFLAGS) -c database.cpp ./protocol/server_peer.pb.cc $(LIBS)
p2pserver.o: p2pserver.cpp p2pserver.h server.h request.h buffer.h database.h
	g++ $(CFLAGS) -c p2pserver.cpp ./protocol/server_peer.pb.cc $(LIBS)
main.o: main.cpp server.h p2pserver.h request.h buffer.h database.h
	g++ $(CFLAGS) -c main.cpp ./protocol/server_peer.pb.cc $(LIBS)

# dbtest: databasetest.o database.o
# 	g++ -o dbtest databasetest.o database.o ./protocol/server_peer.pb.cc -lpqxx -lpq -lprotobuf
# database.o: database.cpp database.h
# 	g++ $(CFLAGS) -c database.cpp ./protocol/server_peer.pb.cc -lpqxx -lpq -lprotobuf

# handlertest: testhandler.o database.o server.o p2pserver.o
# 	g++ -o handlertest testhandler.o database.o server.o p2pserver.o ./protocol/server_peer.pb.cc -lpqxx -lpq -lprotobuf
# database.o: database.cpp database.h
# 	g++ $(CFLAGS) -c database.cpp ./protocol/server_peer.pb.cc -lpqxx -lpq -lprotobuf
# server.o: server.cpp server.h 
# 	g++ $(CFLAGS) -c server.cpp -lpthread -lpqxx -lpq -lprotobuf	
# p2pserver.o: p2pserver.cpp p2pserver.h server.h 
# 	g++ $(CFLAGS) -c p2pserver.cpp -lpthread -lpqxx -lpq -lprotobuf
# testhandler.o: testhandler.cpp server.h p2pserver.h database.h
# 	g++ $(CFLAGS) -c testhandler.cpp -lpthread -lpqxx -lpq -lprotobuf

.PHONY: clean
clean:
	rm -f main *.o
	rm -f dbtest *.o
	rm -f handlertest *.o
