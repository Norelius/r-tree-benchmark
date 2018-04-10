CC    = g++

# Add/remove -g for for debug.
FLAGS = -std=c++11 -Wall -pedantic -g -I /usr/local/include -I /usr/local/lib/boost_1_66_0 -L /usr/local/lib

rstar.out: rstar.cpp neuron.o
	$(CC) $(FLAGS) rstar.cpp neuron.o -lspatialindex -o rstar.out

neuron.o: neuron.cpp neuron.hpp
	$(CC) $(FLAGS) -c neuron.cpp -o neuron.o

clean:
	rm -f *.o *.out