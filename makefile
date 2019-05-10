SHELL = /bin/sh

.SUFFIXES:
.SUFFIXES: .o .cpp .hpp
.PHONY: all clean

CXX?=g++
CXXFLAGS?=-Wall -g

objects=semiprimes.o uhuge.o

all: $(objects)
	$(CXX) $(ALL_CXXFLAGS) -o semiprimes $(objects)

semiprimes.o: uhuge.hpp
uhuge.o: uhuge.hpp

clean:
	-rm semiprimes $(objects)
