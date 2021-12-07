#!/usr/bin/bash
make
rm terminal3d.exe 

make

# rm t3dmath.o t3dgraphics.o stlmesh.o

width=$(tput cols)
height=$(tput lines)

width=`expr $width`
height=`expr $height - 1`

file=`expr $1`

./terminal3d.exe -h=$height -w=$width --path=$file
