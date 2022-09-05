PROG_NAME ?= terminal3d.exe
CFLAGS ?= -Wall --pedantic -lm -g -std=gnu11

${PROG_NAME}: EngineConfig.h main.c Transform.o Utilities.o Vector.o Matrix.o Mesh.o Buffer.o Color.o Drawing.o Output.o Projection.o TermEngine.o TerminalManip.o Logger.o Quaternion.o Pipeline.o
	${CC} main.c Transform.o Utilities.o Vector.o Matrix.o Mesh.o Buffer.o Color.o Drawing.o Output.o Projection.o TermEngine.o TerminalManip.o Logger.o Quaternion.o Pipeline.o ${CFLAGS} -o ${PROG_NAME}


Transform.o: EngineConfig.h ./src/TermEngine/Math/Transform.c ./src/TermEngine/Math/Transform.h
	${CC} ./src/TermEngine/Math/Transform.c -c ${CFLAGS}

Utilities.o: EngineConfig.h ./src/TermEngine/Math/Utilities.c ./src/TermEngine/Math/Utilities.h
	${CC} ./src/TermEngine/Math/Utilities.c -c ${CFLAGS}

Vector.o: EngineConfig.h ./src/TermEngine/Math/Vector.c ./src/TermEngine/Math/Vector.h
	${CC} ./src/TermEngine/Math/Vector.c -c ${CFLAGS}

Matrix.o: EngineConfig.h ./src/TermEngine/Math/Matrix.c ./src/TermEngine/Math/Matrix.h
	${CC} ./src/TermEngine/Math/Matrix.c -c ${CFLAGS}

Quaternion.o: EngineConfig.h ./src/TermEngine/Math/Quaternion.c ./src/TermEngine/Math/Quaternion.h
	${CC} ./src/TermEngine/Math/Quaternion.c -c ${CFLAGS}

Mesh.o: EngineConfig.h ./src/TermEngine/Mesh/Mesh.c ./src/TermEngine/Mesh/Mesh.h
	${CC} ./src/TermEngine/Mesh/Mesh.c -c ${CFLAGS}

Buffer.o: EngineConfig.h ./src/TermEngine/Rendering/Buffer.c ./src/TermEngine/Rendering/Buffer.h
	${CC} ./src/TermEngine/Rendering/Buffer.c -c ${CFLAGS}

Color.o: EngineConfig.h ./src/TermEngine/Rendering/Color.c ./src/TermEngine/Rendering/Color.h
	${CC} ./src/TermEngine/Rendering/Color.c -c ${CFLAGS}

Drawing.o: EngineConfig.h ./src/TermEngine/Rendering/Drawing.c ./src/TermEngine/Rendering/Drawing.h
	${CC} ./src/TermEngine/Rendering/Drawing.c -c ${CFLAGS}

Output.o: EngineConfig.h ./src/TermEngine/Rendering/Output.c ./src/TermEngine/Rendering/Output.h
	${CC} ./src/TermEngine/Rendering/Output.c -c ${CFLAGS}

Projection.o: EngineConfig.h ./src/TermEngine/Rendering/Projection.c ./src/TermEngine/Rendering/Projection.h
	${CC} ./src/TermEngine/Rendering/Projection.c -c ${CFLAGS}

TermEngine.o: EngineConfig.h ./src/TermEngine/TermEngine.c ./src/TermEngine/TermEngine.h
	${CC} ./src/TermEngine/TermEngine.c -c ${CFLAGS}

TerminalManip.o: EngineConfig.h ./src/TerminalManip.c ./src/TerminalManip.h
	${CC} ./src/TerminalManip.c -c ${CLFAGS}

Logger.o: EngineConfig.h ./src/Logger.c ./src/Logger.h
	${CC} ./src/Logger.c -c ${CLFAGS}

Pipeline.o: EngineConfig.h ./src/TermEngine/Rendering/Pipeline.c ./src/TermEngine/Rendering/Pipeline.h
	${CC} ./src/TermEngine/Rendering/Pipeline.c -c ${CFLAGS}

clean:
	rm -f Transform.o Utilities.o Vector.o Matrix.o Mesh.o Buffer.o Color.o Drawing.o Output.o Projection.o TermEngine.o TerminalManip.o Logger.o Quaternion.o ${PROG_NAME}
