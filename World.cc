#include "World.h"

#include <fstream>

World::World(GLuint program)
{
    _nextChunkId = 0;

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
    if (position.x < 0 || position.z < 0 || position.y < 0 || position.x > WORLD_WIDTH * CHUNK_WIDTH || position.z > WORLD_DEPTH * CHUNK_DEPTH || position.y > WORLD_HEIGHT * CHUNK_HEIGHT)
	return NULL;

    int x = ((int)floor(position.x) - (int)floor(position.x) % CHUNK_WIDTH) / CHUNK_WIDTH;
    int z = ((int)floor(position.z) - (int)floor(position.z) % CHUNK_DEPTH) / CHUNK_DEPTH;
    //int y = ((int)floor(position.y) - (int)floor(position.y) % CHUNK_HEIGHT) / CHUNK_HEIGHT;

    int index = x + z * WORLD_DEPTH; // + y * WORLD_DEPTH * WORLD_WIDTH;

    //printf("index: %d\n", index);

    return chunks.at(index);
}

void World::_generateWorld(GLuint program)
{
    TextureData _heightmap;

    LoadTGATextureData("fft-terrain2.tga", &_heightmap);

    // clear content of chunks.txt
    std::ofstream file;
    file.open("chunks.txt", std::ofstream::out | std::ofstream::trunc);
    file.close();

    for(int z = 0; z < WORLD_DEPTH; z++)
    {
	for(int x = 0; x < WORLD_WIDTH; x++)
	{
	    Chunk* c = new Chunk(_nextChunkId, program, &_heightmap, CHUNK_WIDTH, CHUNK_HEIGHT, CHUNK_DEPTH, x * CHUNK_WIDTH, z * CHUNK_DEPTH);

	    c->setPos(vec3(x * CHUNK_WIDTH, 0, z * CHUNK_DEPTH));
	    
	    if (x == 0 && z == 0)
		chunks.push_back(c);	    

	    _nextChunkId++;

	    c->saveChunk();
	}
    }
}
