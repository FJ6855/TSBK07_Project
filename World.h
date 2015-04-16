#ifndef WORLD_H
#define WORLD_H

#include "Chunk.h"

class World
{
public:
    World(GLuint program);
    //World(TextureData* heightMap);

    ~World();

    std::vector<Chunk*> chunks;
    
private:
    //TextureData* _heightMap;

    void _generateWorld(GLuint program);
};

#endif
