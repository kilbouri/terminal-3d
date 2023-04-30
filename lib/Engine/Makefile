# THIS FILE IS DESIGNED FOR GNU MAKE

CFLAGS ?= -Wall --pedantic -lm -g -std=gnu11

libraryName = Engine.a
objects = \
		Mesh/Mesh.o \
		Config/EngineConfig.o \
		Math/Matrix.o Math/Quaternion.o Math/Transform.o Math/Utilities.o Math/Vector.o \
		Rendering/Buffer.o Rendering/Color.o Rendering/Drawing.o Rendering/SpaceConversion.o Rendering/Projection.o

# Creates a static library archive from each of the component object files
$(libraryName): $(objects)
	ar rcs $@ $^

# Handy cleanup target
.PHONY: clean
clean:
	rm -f $(libraryName) $(objects)
