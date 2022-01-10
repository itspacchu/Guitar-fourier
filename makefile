INCPATH = $(shell pkg-config --libs --cflags raylib)
main: main.cpp
	g++ -o main main.cpp -lm -lraylib