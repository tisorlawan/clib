CC=gcc
CFLAGS=-Wall -Wextra -Wno-declaration-after-statement \
	   -std=c89 -pedantic \
	   -O2 \
	   -ggdb 

iofile: iofile.c ../src/iofile.c
	$(CC) $(CFLAGS) -o iofile iofile.c ../src/iofile.c

vector: vector.c
	$(CC) $(CFLAGS) -o vector vector.c

clean:
	$(RM) iofile vector
