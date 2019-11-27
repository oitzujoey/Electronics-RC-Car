
CC = gcc
DBGCFLAGS = -g

.PHONY: all clean debug

all:
	$(CC) -o controller controller.c

debug:
	$(CC) $(DBGCFLAGS) -o controller controller.c

clean:
	rm -f controller
