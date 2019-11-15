
CC = gcc
DBGCFLAGS = -g

.PHONY: all clean debug

all:
	$(CC) -o rc_car_controller rc_car_controller.c

debug:
	$(CC) $(DBGCFLAGS) -o rc_car_controller rc_car_controller.c

clean:
	rm -f rc_car_controller