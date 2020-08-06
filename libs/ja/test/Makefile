all: test

test: main.o string.o
	g++ -o test main.o string.o

main.o: main.cpp
	g++ -c main.cpp -o main.o

string.o: string.cpp
	g++ -c string.cpp -o string.o -g

.PHONY: clean
clean:
	rm -rf *.o test
