CC = gcc

objects = rate.o func.o

output: $(objects)
	$(CC) -o rate rate.c func.c

rate.o: func.h

func.o: func.h

.PHONY: clean

clean:
	rm $(objects) rate