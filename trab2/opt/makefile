DEBUG_FLAGS = -g
COMPILER = -std=gnu99
MATHEVAL_FLAGS = -lmatheval

LIKWID = /home/soft/likwid
LIKWID_FLAGS = -I$(LIKWID)/include -DLIKWID_PERFMON 
LIKWID_LIBS = -L$(LIKWID)/lib -llikwid
AVX_FLAGS = -O3 -mavx -march=native
CFLAGS = $(LIKWID_FLAGS) $(AVX_FLAGS) -Wall -Wextra

LDFLAGS = -lm $(LIKWID_LIBS)

all: main

main: main.o newton.o dados.o utils.o linear.o
	gcc -o newtonSNL main.o newton.o dados.o utils.o linear.o $(MATHEVAL_FLAGS) $(CFLAGS) $(LDFLAGS) $(COMPILER) $(DEBUG_FLAGS)

main.o: main.c
	gcc -c main.c -o main.o $(CFLAGS) $(DEBUG_FLAGS)

newton.o: newton.c newton.h dados.h linear.h
	gcc -c newton.c $(CFLAGS) $(DEBUG_FLAGS)

dados.o: dados.c dados.h utils.h
	gcc -c dados.c $(CFLAGS) $(DEBUG_FLAGS)

utils.o: utils.c utils.h
	gcc -c utils.c -o utils.o $(CFLAGS) $(DEBUG_FLAGS)

linear.o: linear.c linear.h
	gcc -c linear.c -o linear.o $(CFLAGS) $(DEBUG_FLAGS)

clean:
	-rm -f *~ *.o
	
purge: clean
	-rm -f newtonSNL
