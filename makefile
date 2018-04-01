CC    = g++

# Add/remove -g for for debug.
FLAGS = -std=c++11 -Wall -pedantic -g -I /usr/local/include -L /usr/local/lib -I /usr/local/lib/boost_1_66_0 -I src/

neuron_test.out: test/neuron_test.cpp neuron.o
	$(CC) $(FLAGS) test/neuron_test.cpp -o neuron_test.out

neuron.o: src/neuron.cpp src/neuron.hpp
	$(CC) $(FLAGS) -c src/neuron.cpp -o neuron.o

	# neuron.o: neuron.cpp
	# $(CC) $(FLAGS) test.cpp -lspatialindex -o test.out -lspatialindex

clean:
	rm -f *.o *.out