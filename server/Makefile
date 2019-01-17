CC = /usr/bin/gcc
INCLUDES = -I/usr/include -I/usr/include/libxml2 -I. -IInclude/
LIBINCLUDES = -L/usr/lib
INC = $(INCLUDES) $(LIBINCLUDES) 
CFLAGS = -Wall -g
LIBS = -lpthread -lxml2 -pthread

OBJS = executor.o serv.o command_queue.o response_queue.o utils.o
TARGET = server

all : $(TARGET)
	@echo "All done"

$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) $(INC) $(LIBS) -o $(TARGET) $(OBJS)

%.o : %.c
	$(CC) $(CFLAGS) $(INC) $(LIBS) -o $@ -c $<

clean:
	rm -f $(TARGET) $(OBJS)
	@echo "Clean done"

