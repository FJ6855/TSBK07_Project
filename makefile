CCC = g++ -std=c++98 -lGL -lm -DGL_GLEXT_PROTOTYPES
CCC2 = gcc -std=c99 -lGL -lm -DGL_GLEXT_PROTOTYPES

game : main.o Logic.o Renderer.o UserInterface.o GL_utilities.o MicroGlut.o LoadObj.o MathUtils.o
	$(CCC) -o game main.o Logic.o Renderer.o UserInterface.o GL_utilities.o MicroGlut.o LoadObj.o MathUtils.o

main.o : main.cc
	$(CCC) -c main.cc

Logic.o : Logic.cc Logic.h
	$(CCC) -c Logic.cc

Renderer.o : Renderer.cc Renderer.h
	$(CCC) -c Renderer.cc

UserInterface.o : UserInterface.cc UserInterface.h
	$(CCC) -c UserInterface.cc

GL_utilites.o : GL_utilites.c GL_utilites.h
	$(CCC2) -c GL_utilities.c

MicroGlut.o : MicroGlut.c MicroGlut.h
	$(CCC2) -c MicroGlut.c

LoadObj.o : LoadObj.c LoadObj.h
	$(CCC2) -c LoadObj.c

MathUtils.o : MathUtils.cc MathUtils.h
	$(CCC) -c MathUtils.cc

clean :
	rm main.o Renderer.o GL_utilities.o MicroGlut.o LoadObj.o MathUtils.o
