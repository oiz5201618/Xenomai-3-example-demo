CC = gcc
SKIN = native
CFLAGS = -std=gnu99 $(shell xeno-config --skin=$(SKIN) --cflags)
LDFLAGS = $(shell xeno-config --skin=$(SKIN) --ldflags)

EXE = rt_motor
LOAD = eatmem

all: main.c gpio.c
	$(CC) $^ $(CFLAGS) $(LDFLAGS) -o $(EXE)

no_rt:
	$(CC) no_rt_motor.c -o no_rt_motor

load:
	$(CC) eatmem.c -o $(LOAD)

clean:
	rm $(EXE) $(LOAD) no_rt_motor
