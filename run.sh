#! /bin/sh

# compile, clear, and run with viewport size
make && clear && ./terminal3d.exe $(tput cols) $(tput lines)
