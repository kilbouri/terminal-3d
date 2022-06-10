PROG_NAME ?= terminal3d.exe
CFLAGS ?= -Wall --pedantic -lm -g

${PROG_NAME}: main.c Transform.o Utilities.o Vector.o Matrix.o Mesh.o Buffer.o Color.o Drawing.o Output.o Projection.o Debug.o TermEngine.o TerminalManip.o Quaternion.o EngineConfig.h 
	${CC} main.c Transform.o Utilities.o Vector.o Matrix.o Mesh.o Buffer.o Color.o Drawing.o Output.o Projection.o Debug.o TermEngine.o TerminalManip.o Quaternion.o ${CFLAGS} -o ${PROG_NAME}


Transform.o: ./src/TermEngine/Math/Transform.c ./src/TermEngine/Math/Transform.h
	${CC} ./src/TermEngine/Math/Transform.c -c ${CFLAGS}

Utilities.o: ./src/TermEngine/Math/Utilities.c ./src/TermEngine/Math/Utilities.h
	${CC} ./src/TermEngine/Math/Utilities.c -c ${CFLAGS}

Vector.o: ./src/TermEngine/Math/Vector.c ./src/TermEngine/Math/Vector.h
	${CC} ./src/TermEngine/Math/Vector.c -c ${CFLAGS}

Matrix.o: ./src/TermEngine/Math/Matrix.c ./src/TermEngine/Math/Matrix.h
	${CC} ./src/TermEngine/Math/Matrix.c -c ${CFLAGS}

Quaternion.o: ./src/TermEngine/Math/Quaternion.c ./src/TermEngine/Math/Quaternion.h
	${CC} ./src/TermEngine/Math/Quaternion.c -c ${CFLAGS}

Mesh.o: ./src/TermEngine/Mesh/Mesh.c ./src/TermEngine/Mesh/Mesh.h
	${CC} ./src/TermEngine/Mesh/Mesh.c -c ${CFLAGS}

Buffer.o: ./src/TermEngine/Rendering/Buffer.c ./src/TermEngine/Rendering/Buffer.h
	${CC} ./src/TermEngine/Rendering/Buffer.c -c ${CFLAGS}

Color.o: ./src/TermEngine/Rendering/Color.c ./src/TermEngine/Rendering/Color.h
	${CC} ./src/TermEngine/Rendering/Color.c -c ${CFLAGS}

Drawing.o: ./src/TermEngine/Rendering/Drawing.c ./src/TermEngine/Rendering/Drawing.h
	${CC} ./src/TermEngine/Rendering/Drawing.c -c ${CFLAGS}

Output.o: ./src/TermEngine/Rendering/Output.c ./src/TermEngine/Rendering/Output.h
	${CC} ./src/TermEngine/Rendering/Output.c -c ${CFLAGS}

Projection.o: ./src/TermEngine/Rendering/Projection.c ./src/TermEngine/Rendering/Projection.h
	${CC} ./src/TermEngine/Rendering/Projection.c -c ${CFLAGS}

Debug.o: ./src/TermEngine/Debug.c ./src/TermEngine/Debug.h
	${CC} ./src/TermEngine/Debug.c -c ${CFLAGS}

TermEngine.o: ./src/TermEngine/TermEngine.c ./src/TermEngine/TermEngine.h
	${CC} ./src/TermEngine/TermEngine.c -c ${CFLAGS}

TerminalManip.o: ./src/TerminalManip.c ./src/TerminalManip.h
	${CC} ./src/TerminalManip.c -c ${CLFAGS}

test: test.c Transform.o Utilities.o Vector.o Matrix.o Mesh.o Buffer.o Color.o Drawing.o Output.o Projection.o Debug.o TermEngine.o TerminalManip.o Quaternion.o
	${CC} test.c Transform.o Utilities.o Vector.o Matrix.o Mesh.o Buffer.o Color.o Drawing.o Output.o Projection.o Debug.o TermEngine.o TerminalManip.o Quaternion.o ${CFLAGS} -o test.exe

clean:
	rm -f Transform.o Utilities.o Vector.o Matrix.o Mesh.o Buffer.o Color.o Drawing.o Output.o Projection.o Debug.o TermEngine.o TerminalManip.o Quaternion.o ${PROG_NAME} test.exe
