CC = gcc
SKIN = native
CFLAGS = -std=gnu99 $(shell xeno-config --skin=$(SKIN) --cflags)
LDFLAGS = $(shell xeno-config --skin=$(SKIN) --ldflags)

EXE = rt_motor
LOAD = eatmem

all: main.c gpio.c
	$(CC) $^ $(CFLAGS) $(LDFLAGS) -o $(EXE)

load:
	$(CC) eatmem.c -o $(LOAD)

clean:
	rm $(EXE) $(LOAD)
