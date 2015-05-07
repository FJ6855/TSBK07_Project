#include "World.h"
#include "simplexnoise.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

World::World(GLuint program)
{
    _generateWorld(program);
}

World::~World()
{
    for (int i = 0; i < chunks.size(); i++)
    {
	Chunk* c = chunks.at(i);

	delete c;
	c = NULL;
    }
}

Chunk* World::getChunkAtPosition(vec3 position)
{    
    if (position.x < 0 || position.z < 0 || position.y < 0 || 
	position.x > WORLD_WIDTH * CHUNK_WIDTH || position.z > WORLD_DEPTH * CHUNK_DEPTH || 
	position.y > WORLD_HEIGHT * CHUNK_HEIGHT)
	return NULL;

    int x = ((int)floor(position.x) - (int)floor(position.x) % CHUNK_WIDTH) / CHUNK_WIDTH;
    int z = ((int)floor(position.z) - (int)floor(position.z) % CHUNK_DEPTH) / CHUNK_DEPTH;

    int index = x + z * WORLD_DEPTH;

    return chunks.at(index);
}

void World::_generateWorld(GLuint program)
{
    
    TextureData _heightmap;

    srand(time(NULL));

    LoadTGATextureData("test.tga", &_heightmap);
    float x1 = rand() % 5 + 1;
    float x2 = rand() % 5 + 1;
    float z1 = rand() % 4 + 1 ;

    x1 /= 10;
    x2 /= 10;
    z1 /= 100;

    for(int x = 0; x <_heightmap.width; x++)
      { 		
	for(int z = 0; z < _heightmap.height; z++)
	  {    
	    _heightmap.imageData[(x + z * _heightmap.width) * (_heightmap.bpp / 8)] = scaled_octave_noise_2d(x1, x2, z1, 0.0, 15.0, x, z);
	  }
      }
    
    for(int z = 0; z < WORLD_DEPTH; z++)
    {
	for(int x = 0; x < WORLD_WIDTH; x++)
	{
	    Chunk* c = new Chunk(program, &_heightmap, CHUNK_WIDTH, CHUNK_HEIGHT, CHUNK_DEPTH, x * CHUNK_WIDTH, z * CHUNK_DEPTH);

	    c->setPos(vec3(x * CHUNK_WIDTH, 0, z * CHUNK_DEPTH));

	    chunks.push_back(c);
	    _updateList.push_back(c);
	}
    }
}

void World::update()
{  
    int i = 0;
    for(i = 0; i < _updateList.size() & i < 2; i++)
    {
	_updateList.at(_updateList.size() - 1 - i)->generateChunk();
    }

    for(int j = 0; j < i; j++)
	_updateList.pop_back();

}

void World::removeBlock(int x, int y, int z)
{


}
