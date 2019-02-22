PROG = werhauz
OBJS = main.o hash.o struct.o linked.o heap.o
SRCS = main.c hash.c struct.c linked.c heap.c
HDRS = hash.h linked.h heap.h struct.h

all: $(PROG)

hash.o:
	gcc -g3 -c hash.c

struct.o:
	gcc -g3 -c struct.c

heap.o:
	gcc -g3 -c heap.c

main.o:
	gcc -g3 -c main.c

linked.o:
	gcc -g3 -c linked.c

werhauz: main.o hash.o struct.o linked.o heap.o
	gcc -g3 $(OBJS) -o werhauz


clean:
	rm $(OBJS) $(PROG)
