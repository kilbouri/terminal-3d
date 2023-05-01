#! /bin/sh

# compile, clear, and run with viewport size
make && clear && ./terminal3d $(tput cols) $(tput lines)
