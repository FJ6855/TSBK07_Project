#include "World.h"

World::World(GLuint program)
{
    _generateWorld(program);
}

/*World::World(TextureData* heightMap)
{
    _heightMap = heightMap;

    _generateWorld();
    }*/

World::~World()
{
    // delete _heightMap;
    //_heightMap = nullptr;
}

void World::_generateWorld(GLuint program)
{
    for(int z = 0; z < 4; z++)
    {
	for(int x = 0; x < 4; x++)
	{
	    Chunk* c = new Chunk(program);

	    c->setPos(vec3(x*16, 0, z*16));

	    chunks.push_back(c);	    
	}
    }
}
