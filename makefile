SHELL = /bin/sh

.SUFFIXES:
.SUFFIXES: .o .cpp .hpp
.PHONY: all clean

CXX?=g++
CXXFLAGS?=-Wall -g

objects=primes.o semiprimes.o uhuge.o 

all: $(objects)
	$(CXX) $(ALL_CXXFLAGS) -o semiprimes $(objects)

primes.o: primes.hpp
semiprimes.o: primes.hpp uhuge.hpp
uhuge.o: uhuge.hpp

clean:
	-rm semiprimes $(objects)
