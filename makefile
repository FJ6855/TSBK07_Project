CCC = g++ -std=c++98 -lGL -lm -DGL_GLEXT_PROTOTYPES
CCC2 = gcc -std=c99 -lGL -lm -DGL_GLEXT_PROTOTYPES

game : main.o Logic.o World.o Chunk.o Player.o Vertex.o Renderer.o UserInterface.o GL_utilities.o MicroGlut.o LoadObj.o LoadTGA.o MathUtils.o simplexnoise.o
	$(CCC) -o game main.o Logic.o World.o Chunk.o Player.o Vertex.o Renderer.o UserInterface.o GL_utilities.o MicroGlut.o LoadObj.o LoadTGA.o MathUtils.o simplexnoise.o

main.o : main.cc
	$(CCC) -c main.cc

Logic.o : Logic.cc Logic.h
	$(CCC) -c Logic.cc

World.o : World.cc World.h
	$(CCC) -c World.cc

Chunk.o : Chunk.cc Chunk.h
	$(CCC) -c Chunk.cc

Player.o : Player.cc Player.h
	$(CCC) -c Player.cc

Vertex.o : Vertex.cc Vertex.h
	$(CCC) -c Vertex.cc

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

LoadTGA.o : LoadTGA.c LoadTGA.h
	$(CCC2) -c LoadTGA.c

MathUtils.o : MathUtils.cc MathUtils.h
	$(CCC) -c MathUtils.cc

simplexnoise.o : simplexnoise.cc simplexnoise.h
	$(CCC) -c simplexnoise.cc

clean :
	rm main.o Logic.o World.o Chunk.o Player.o Vertex.o Renderer.o UserInterface.o MicroGlut.o LoadObj.o LoadTGA.o MathUtils.o simplexnoise.o