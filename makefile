CCC = g++ -std=gnu++98 -lGL -lm -DGL_GLEXT_PROTOTYPES
CCC2 = gcc -std=c99 -lGL -lm -DGL_GLEXT_PROTOTYPES

game : main.o InputHandler.o Renderer.o GL_utilities.o MicroGlut.o
	$(CCC) -o game main.o InputHandler.o Renderer.o GL_utilities.o MicroGlut.o

main.o : main.cc
	$(CCC) -c main.cc

InputHandler.o : InputHandler.cc InputHandler.h
	$(CCC) -c InputHandler.cc

Renderer.o : Renderer.cc Renderer.h
	$(CCC) -c Renderer.cc

GL_utilites.o : GL_utilites.c GL_utilites.h
	$(CCC2) -c GL_utilities.c

Microglut.o : Microglut.c Microglut.h
	$(CCC2) -c Microglut.c

clean :
	rm main.o InputHandler.o Renderer.o GL_utilities.o MicroGlut.o
