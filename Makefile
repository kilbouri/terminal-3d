PROG_NAME ?= terminal3d.exe
CFLAGS ?= -Wall --pedantic -lm -g

${PROG_NAME}: main.c TerminalManip.o TermEngine.o STLMesh.o TermRendering.o TermDrawing.o TermMath.o TermDebug.o EngineConfig.h 
	${CC} main.c TerminalManip.o TermEngine.o STLMesh.o TermRendering.o TermDrawing.o TermMath.o TermDebug.o ${CFLAGS} -o ${PROG_NAME}

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

TermDebug.o: src/TermEngine/TermDebug.c src/TermEngine/TermDebug.h
	${CC} -c src/TermEngine/TermDebug.c ${CFLAGS}
	
clean:
	rm -f TerminalManip.o TermEngine.o STLMesh.o TermRendering.o TermDrawing.o TermMath.o TermDebug.o ${PROG_NAME}
