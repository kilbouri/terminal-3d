# terminal-3d

A hobby project to create a fairly decent realtime 3D renderer in the terminal.
You can load custom models (as long as they follow the binary STL format) and have them rendered in your linux terminal!

## Build

Thanks to [smcameron](https://github.com/smcameron), there is now a Makefile to build the program.

Otherwise, you can compile `main.c` and all the files in the `lib` folder.

## Run

Execute your compiled program, providing a file path as the sole argument. The file must be a binary STL mesh file.

You can configure the rendering settings by editing the definitions in main.c.

Lighting and more configurations to come.
