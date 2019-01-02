CC = /usr/bin/gcc
INCLUDES = -I/usr/include -I/usr/include/libxml2 -I.
LIBINCLUDES = -L/usr/lib
INC = $(INCLUDES) $(LIBINCLUDES) 
CFLAGS = -Wall
LIBS = -lpthread -lxml2

OBJS = command.o executor.o serv.o
TARGET = server

all : $(TARGET)
	echo "All done"

$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) $(INC) $(LIBS) -o $(TARGET) $(OBJS)

%.o : %.c
	$(CC) $(CFLAGS) $(INC) $(LIBS) -o $@ -c $<

clean:
		rm -f $(TARGET) $(OBJS)
		echo "Clean done"

