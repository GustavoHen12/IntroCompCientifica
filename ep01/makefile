CFLAGS = -Wall
LDFLAGS = -lm
DEBUG_FLAGS = -g
COMPILER = -std=gnu99
MATHEVAL_FLAGS = -lmatheval

all: ep01

ep01: ep01.o
	gcc -o ep01 ep01.o $(MATHEVAL_FLAGS) $(CFLAGS) $(LDFLAGS) $(COMPILER) $(DEBUG_FLAGS)

ep01.o: ep01.c
	gcc -c ep01.c -o ep01.o $(CFLAGS)

clean:
	-rm -f *~ *.o
	
purge: clean
	-rm -f ep01
