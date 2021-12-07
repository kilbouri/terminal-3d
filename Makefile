terminal3d.exe: t3dmath.o t3dgraphics.o stlmesh.o
	gcc -Wall main.c t3dmath.o t3dgraphics.o stlmesh.o -o terminal3d.exe -lm -O3
# post compilation cleanup:
	rm *.o

t3dmath.o:
	gcc -c t3dmath.c -O3

t3dgraphics.o:
	gcc -c t3dgraphics.c -O3

stlmesh.o:
	gcc -c stlmesh.c -O3
