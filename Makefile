PROG_NAME ?= terminal3d.exe
CFLAGS ?= -Wall --pedantic -lm -g

${PROG_NAME}: main.c EngineConfig.h TerminalManip.o TermEngine.o STLMesh.o TermRendering.o TermDrawing.o TermMath.o
	${CC} main.c TermEngine.o STLMesh.o TermRendering.o TermDrawing.o TerminalManip.o TermMath.o ${CFLAGS} -o ${PROG_NAME}

TermMath.o: src/TermEngine/TermMath.c src/TermEngine/TermMath.h
	${CC} -c src/TermEngine/TermMath.c ${CFLAGS}

TerminalManip.o: src/TerminalManip.c src/TerminalManip.h
	${CC} -c src/TerminalManip.c ${CFLAGS}

TermEngine.o: src/TermEngine/TermEngine.c src/TermEngine/TermEngine.h
	${CC} -c src/TermEngine/TermEngine.c ${CFLAGS}

TermDrawing.o: src/TermEngine/TermDrawing.c src/TermEngine/TermDrawing.h
	${CC} -c src/TermEngine/TermDrawing.c ${CFLAGS}

TermRendering.o: src/TermEngine/TermRendering.c src/TermEngine/TermRendering.h
	${CC} -c src/TermEngine/TermRendering.c ${CFLAGS}

STLMesh.o: src/TermEngine/MeshLoaders/STLMesh.c src/TermEngine/MeshLoaders/STLMesh.h
	${CC} -c src/TermEngine/MeshLoaders/STLMesh.c ${CFLAGS}

clean:
	rm -f TermEngine.o TermRendering.o TermDrawing.o TerminalManip.o STLMesh.o ${PROG_NAME}
