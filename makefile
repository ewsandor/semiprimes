CC=g++
CFLAGS=
DEPS=uhuge.hpp
OBJ=uhuge.o semiprimes.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

semiprimes: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean: $(OBJ)
	rm $^
	rm semiprimes
