# THIS FILE IS DESIGNED FOR GNU MAKE

CFLAGS ?= -Wall --pedantic -lm -g -std=gnu11

libraryName = Logger.a
objects = Logger.o

# Creates a static library archive from each of the component object files
$(libraryName): $(objects)
	ar rcs $@ $^

# Handy cleanup target
.PHONY: clean
clean:
	rm -f $(libraryName) $(objects)
