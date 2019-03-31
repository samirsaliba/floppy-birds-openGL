# Makefile
install_audiolib:
	sudo apt-get install libsfml-dev

all: compila linka executa

compila:
	g++ tp1.cpp -c

linka:
	g++ tp1.o -o tp1 -lGL -lGLU -lglut -lSOIL -lsfml-audio

executa:
	./tp1