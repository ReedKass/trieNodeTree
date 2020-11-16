
CC = clang++
CFLAGS = -std=c++11 -Wall -Wextra -g3
DEPS = trieNodeTree.h

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

SeqMatch: main.o trieNodeTree.o
	$(CC) -o SeqMatch main.o trieNodeTree.o

UnitTest: unittests.o trieNodeTree.o
	$(CC) -o UnitTest unittests.o trieNodeTree.o

clean:
	rm -f *.o core* *~ trieNodeTree
