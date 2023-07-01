#!/usr/bin/bash
WFLAGS="-Wall -Wextra -Wpedantic -Wno-c++17-attribute-extensions -Wno-writable-strings"
SDLFLAGS=$(pkg-config --libs --cflags sdl2)
SDLIMAGEFLAGS=$(pkg-config --libs --cflags SDL2_image)

mkdir -p ./objects

function run_cmd(){
	echo $1
	$1
}

for file in *.cpp
do
	CMD="clang++ $WFLAGS $SDLFLAGS $SDLIMAGEFLAGS -O0 -g3 $file -o objects/"$file".o -c"
	run_cmd "$CMD"
done

CMD="clang++ $SDLFLAGS $SDLIMAGEFLAGS -O0 -g3 objects/*.o -o main"
run_cmd "$CMD"
