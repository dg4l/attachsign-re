all:
	mkdir -p ./build
	gcc -g ./src/main.c -o ./build/attachsign-re
win: 
	mkdir -p ./build
	x86_64-w64-mingw32-gcc ./src/main.c -o ./build/attachsign-re.exe
