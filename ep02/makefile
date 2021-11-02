CFLAGS = -Wall
LDFLAGS = -lm
DEBUG_FLAGS = -g
COMPILER = -std=gnu99
MATHEVAL_FLAGS = -lmatheval

all: ep02

ep02: ep02.o utils.o dados.o
	gcc -o ep02 ep02.o utils.o dados.o $(MATHEVAL_FLAGS) $(CFLAGS) $(LDFLAGS) $(COMPILER) $(DEBUG_FLAGS)

ep02.o: ep02.c
	gcc -c ep02.c -o ep02.o $(CFLAGS)

utils.o: utils.c utils.h
	gcc -c utils.c -o utils.o $(CFLAGS)

dados.o: dados.c dados.h
	gcc -c dados.c -o dados.o $(CFLAGS)

clean:
	-rm -f *~ *.o
	
purge: clean
	-rm -f ep02
