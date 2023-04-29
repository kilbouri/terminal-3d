# THIS FILE IS DESIGNED FOR GNU MAKE

PROG_NAME ?= terminal3d.exe
CFLAGS ?= -Wall --pedantic -lm -std=gnu11

objects = main.o src/TerminalManip.o

all: $(PROG_NAME)

$(PROG_NAME): $(objects) lib/Logger/Logger.a lib/Engine/Engine.a
	$(CC) $^ -o $(PROG_NAME) $(CFLAGS) 

lib/Engine/Engine.a: FORCE
	make --no-print-directory -C lib/Engine

lib/Logger/Logger.a: FORCE
	make --no-print-directory -C lib/Logger

.PHONY: clean
clean:
	make --no-print-directory -C lib/Engine clean
	make --no-print-directory -C lib/Logger clean
	rm -f $(PROG_NAME) $(objects)

FORCE: 
