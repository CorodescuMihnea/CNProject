CC = /usr/bin/gcc
CFLAGS = -I/usr/include/libxml2/ -Wall

tsserver: executor.o serv.o
	$(CC) -o execs/tsserver serv.o executor.o command.o

tscli: cli.o
	$(CC) -o execs/tscli cli.o