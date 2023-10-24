CC = g++
CFLAGS = -std=c++11 -Wall -Wextra -pedantic -g

csim: main.o Cache.o Slot.o Set.o
	$(CC) $(CFLAGS) -o csim main.o Cache.o Slot.o Set.o

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

Cache.o: Cache.cpp Cache.h
	$(CC) $(CFLAGS) -c Cache.cpp

Slot.o: Slot.cpp Slot.h
	$(CC) $(CFLAGS) -c Slot.cpp

Set.o: Set.cpp Set.h
	$(CC) $(CFLAGS) -c Set.cpp


clean:
	rm -f *.o csim depend.mak
