CC = g++
ALL: client server

client: client.cc ClientInfo.h write.o read.o packet.o
	${CC} -std=c++11 -o client client.cc write.o read.o packet.o

server: server.cc server.h ClientInfo.h write.o read.o packet.o ClientAddr.o GetShadow.o Paint.o
	${CC} -std=c++11 -o server server.cc write.o read.o packet.o ClientAddr.o GetShadow.o Paint.o

write.o: write.cc write.h
	${CC} -c write.cc
read.o: read.cc read.h
	${CC} -c read.cc
packet.o: packet.cc packet.h
	${CC} -c packet.cc
ClientAddr.o: ClientAddr.cc ClientAddr.h
	${CC} -c ClientAddr.cc
GetShadow.o: GetShadow.cc GetShadow.h
	${CC} -c GetShadow.cc
Paint.o: Paint.cc Paint.h
	${CC} -c Paint.cc

.PHONY:clean

clean:
	-rm -f client server
	-rm -f *.o
