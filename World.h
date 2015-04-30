#ifndef WORLD_H
#define WORLD_H

#include "Chunk.h"
#include "LoadTGA.h"

#define WORLD_WIDTH 16
#define WORLD_HEIGHT 1
#define WORLD_DEPTH 16

#define CHUNK_WIDTH 16
#define CHUNK_HEIGHT 16
#define CHUNK_DEPTH 16

class World
{
public:
    World(GLuint program);

    ~World();

    std::vector<Chunk*> chunks;
   

    Chunk* getChunkAtPosition(vec3 position);
    void removeBlock(int x, int y, int z);
    void update();
    
private:
    std::vector<Chunk*> _updateList;
    void _generateWorld(GLuint program);
};

#endif
