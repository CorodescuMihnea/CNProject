CC = /usr/bin/gcc
INCLUDES = -I/usr/include -I.
LIBINCLUDES = -L/usr/lib
INC = $(INCLUDES) $(LIBINCLUDES) 
CFLAGS = -Wall
LIBS =

OBJS = cli.o
TARGET = client

all : $(TARGET)
	echo "All done"

$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) $(INC) $(LIBS) -o $(TARGET) $(OBJS)

%.o : %.c
	$(CC) $(CFLAGS) $(INC) $(LIBS) -o $@ -c $<

clean:
		rm -f $(TARGET) $(OBJS)
		echo "Clean done"

