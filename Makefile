OBJ= Client.o main.o HTTPServer.o
PROGRAM= server

CC=g++
LIBS=
CFLAGS=-std=c++11

all:$(PROGRAM)

$(PROGRAM):$(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o:%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ) $(PROGRAM)
