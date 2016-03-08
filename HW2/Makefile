CFILES = $(wildcard *.cpp)
OBJS = $(CFILES:.cpp=.o)
CC ?= clang++
CFLAGS = -lc++ -Wall -DDEBUG -g3 -std=c++11 -stdlib=libc++ -v

all: foo.out
foo: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^
%.o: %.cpp 
	$(CC) $(CFLAGS) -c -o $@ $^
clean: $(OBJS) foo
	rm $^
