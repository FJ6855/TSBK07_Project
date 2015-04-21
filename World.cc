#include "World.h"

World::World(GLuint program)
{
    _generateWorld(program);
}

World::~World()
{
    //delete _heightmap;
    //_heightmap = NULL;
}

void World::_generateWorld(GLuint program)
{
    TextureData _heightmap;

    LoadTGATextureData("fft-terrain2.tga", &_heightmap);

    for(int z = 0; z < WORLD_DEPTH; z++)
    {
	for(int x = 0; x < WORLD_WIDTH; x++)
	{
	    Chunk* c = new Chunk(program, &_heightmap, CHUNK_WIDTH, CHUNK_HEIGHT, CHUNK_DEPTH, x * CHUNK_WIDTH, z * CHUNK_DEPTH);

	    c->setPos(vec3(x*16, 0, z*16));

	    chunks.push_back(c);	    
	}
    }
}
