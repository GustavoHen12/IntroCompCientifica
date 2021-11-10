CFLAGS = -Wall
LDFLAGS = -lm
DEBUG_FLAGS = -g
COMPILER = -std=gnu99
MATHEVAL_FLAGS = -lmatheval

all: main

main: main.o newton.o dados.o utils.o linear.o
	gcc -o newtonSNL main.o newton.o dados.o utils.o linear.o $(MATHEVAL_FLAGS) $(CFLAGS) $(LDFLAGS) $(COMPILER) $(DEBUG_FLAGS)

main.o: main.c
	gcc -c main.c -o main.o $(CFLAGS)

newton.o: newton.c newton.h dados.h linear.h
	gcc -c newton.c $(CFLAGS)

dados.o: dados.c dados.h utils.h
	gcc -c dados.c $(CFLAGS)

utils.o: utils.c utils.h
	gcc -c utils.c -o utils.o $(CFLAGS)

linear.o: linear.c linear.h
	gcc -c linear.c -o linear.o $(CFLAGS)

clean:
	-rm -f *~ *.o
	
purge: clean
	-rm -f ep02
