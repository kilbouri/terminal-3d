#! /bin/sh

# compile, clear, and run with viewport size
make clean && make && clear && ./terminal3d $(tput cols) $(tput lines)
