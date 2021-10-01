NAME?=catmesh

CFLAGS=-Wall --pedantic

${NAME}:	main.c lib/MatLib.h lib/STLMesh.h lib/TerminalGraphics.h lib/Utilities.h
	$(CC) ${CFLAGS} -o ${NAME} main.c -lm

clean:
	rm -f ${NAME}

