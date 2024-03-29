# THIS FILE IS DESIGNED FOR GNU MAKE

PROG_NAME ?= terminal3d
CFLAGS ?= -Wall --pedantic -lm -std=gnu11 -g

objects = src/main.o src/TerminalManip.o
libraries = lib/Logger/Logger.a lib/Engine/Engine.a
config = src/Config.h

all: $(PROG_NAME)

$(PROG_NAME): $(config) $(objects) $(libraries)
	$(CC) $(objects) $(libraries) -o $(PROG_NAME) $(CFLAGS) 

lib/Engine/Engine.a: FORCE
	make --no-print-directory -C lib/Engine CLFAGS="${CFLAGS}"

lib/Logger/Logger.a: FORCE
	make --no-print-directory -C lib/Logger CLFAGS="${CFLAGS}"

.PHONY: clean
clean:
	make --no-print-directory -C lib/Engine clean
	make --no-print-directory -C lib/Logger clean
	rm -f $(PROG_NAME) $(objects)

FORCE: 
