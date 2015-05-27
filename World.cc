#include "World.h"
#include "simplexnoise.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <map>

TextureData heightmap;

World::World(GLuint program, vec3 playerPosition)
{     
    _lastPlayerPosition = playerPosition;

    chunks = new HashTable((VIEW_DISTANCE * 2 + 1) * (VIEW_DISTANCE * 2 + 1));
    createdChunks = new HashTable(2048 * 2048);

    _nextChunkId = 0;

    _program = program;

    _generateWorld();
}

World::~World()
{
    chunksFile.close();

    delete chunks;
    chunks = NULL;
}

Chunk* World::getChunkAtPosition(vec3 position)
{    
    int x = ((int)position.x - (int)position.x % CHUNK_WIDTH);
    int z = ((int)position.z - (int)position.z % CHUNK_DEPTH);

    return chunks->getChunk(vec3(x, 0, z));
}

bool World::isBlockActive(vec3 pos)
{	
    Chunk* c = getChunkAtPosition(pos);
    
    if (c != NULL)
    {
	vec3 blockPos = VectorSub(pos, c->getPos());
	
	blockPos.x = (int)blockPos.x;
	blockPos.y = (int)blockPos.y;
	blockPos.z = (int)blockPos.z;

	return c->isBlockActive(blockPos);
    }
    
    return false;
}

void World::update()
{ 
    int i = 0;

    for (i = 0; i < _updateList.size() && i < 2; i++)
    {
	Chunk* c = _updateList.at(_updateList.size() - 1 - i);

	c->generateChunk();
	
	// check if the chunk has been saved before, if so overwrite
	// if not we save at the end of the file and add it to the table of saved chunks
	if (createdChunks->get(c->getPos()) != NULL)
	{
	    // owerwrite
	    c->saveChunk(chunksFile, true);
	}
	else
	{
	    createdChunks->put(c->getPos(), NULL);

	    c->saveChunk(chunksFile, false);
	}
    }

    for (int j = 0; j < i; j++)
    {
	_updateList.pop_back();
    }
}
    
void World::loadChunks(vec3 playerPosition)
{
    int newX = ((int)playerPosition.x - (int)playerPosition.x % CHUNK_WIDTH);
    int oldX = ((int)_lastPlayerPosition.x - (int)_lastPlayerPosition.x % CHUNK_WIDTH);
    
    int newZ = ((int)playerPosition.z - (int)playerPosition.z % CHUNK_DEPTH);
    int oldZ = ((int)_lastPlayerPosition.z - (int)_lastPlayerPosition.z % CHUNK_DEPTH);
    
    if (newX != oldX)
    {
	int directionX = (newX - oldX < 0) ? -1 : 1;

	for (int i = 0; i < VIEW_DISTANCE * 2 + 1; i++)
	{
	    int x = oldX - directionX * VIEW_DISTANCE * CHUNK_WIDTH;
	    int z = oldZ - CHUNK_DEPTH * VIEW_DISTANCE + i * CHUNK_DEPTH;
	    
	    if (x >= 0 && z >= 0 && x <= heightmap.width - CHUNK_WIDTH && z <= heightmap.height - CHUNK_DEPTH)
		chunks->remove(vec3(x, 0, z));

	    x = newX + directionX * VIEW_DISTANCE * CHUNK_WIDTH;
	    z = newZ - CHUNK_DEPTH * VIEW_DISTANCE + i * CHUNK_DEPTH;

	    _createChunk(1, x, z);
	}
    }

    if (newZ != oldZ)
    {
	int directionZ = (newZ - oldZ < 0) ? -1 : 1;

	for (int i = 0; i < VIEW_DISTANCE * 2 + 1; i++)
	{
	    int x = oldX - CHUNK_WIDTH * VIEW_DISTANCE + i * CHUNK_WIDTH;
	    int z = oldZ - directionZ * VIEW_DISTANCE * CHUNK_DEPTH;
	    
	    if (x >= 0 && z >= 0 && x <= heightmap.width - CHUNK_WIDTH && z <= heightmap.height - CHUNK_DEPTH)
	    	chunks->remove(vec3(x, 0, z));

	    x = newX - CHUNK_WIDTH * VIEW_DISTANCE + i * CHUNK_WIDTH;
	    z = newZ + directionZ * VIEW_DISTANCE * CHUNK_DEPTH;

	    _createChunk(1, x, z);
	}
    }

    _lastPlayerPosition = playerPosition;
}

void World::updateRenderList(Frustum* frustum, vec3 playerPosition)
{
    renderList.clear();

    // loop through the area of loaded chunks and check if they are inside the frustum
    for (int z = playerPosition.z / CHUNK_DEPTH - VIEW_DISTANCE; z < playerPosition.z / CHUNK_DEPTH + VIEW_DISTANCE; z++)
    {
	for (int x = playerPosition.x / CHUNK_WIDTH - VIEW_DISTANCE; x < playerPosition.x / CHUNK_WIDTH + VIEW_DISTANCE; x++)
	{
	    if (x >= 0 && z >= 0 && frustum->pointIsInsideFrustum(vec3(x * CHUNK_WIDTH, 0, z * CHUNK_DEPTH), vec3(x * CHUNK_WIDTH + CHUNK_WIDTH, CHUNK_HEIGHT, z * CHUNK_DEPTH + CHUNK_DEPTH)))
	    {
		renderList.push_back(vec3(x * CHUNK_WIDTH, 0, z * CHUNK_DEPTH));
	    }
	}
    }
}

void World::changeBlock(vec3 pos, int blockType)
{
    Chunk* c = getChunkAtPosition(pos); 

    if (c != NULL)
    {
	pos.x = (int)pos.x % CHUNK_WIDTH;
	pos.y = (int)pos.y % CHUNK_HEIGHT;
	pos.z = (int)pos.z % CHUNK_DEPTH;
	
	if (c->setBlock(pos, blockType))
	{
	    _updateList.push_back(c);
	}
    }    
}

void World::_generateWorld()
{
    //std::fstream chunksFile;
    chunksFile.open("chunks.txt", std::fstream::out | std::fstream::trunc);
    chunksFile.close();
    chunksFile.open("chunks.txt", std::ios::in | std::ios::out);
    
    _generateHeightmap();
    
    for (int z = _lastPlayerPosition.z / CHUNK_DEPTH - VIEW_DISTANCE; z < _lastPlayerPosition.z / CHUNK_DEPTH + VIEW_DISTANCE; z++)
    {
	for (int x = _lastPlayerPosition.x / CHUNK_WIDTH - VIEW_DISTANCE; x < _lastPlayerPosition.x / CHUNK_WIDTH + VIEW_DISTANCE; x++)
	{
	    _createChunk(1, x * CHUNK_WIDTH, z * CHUNK_DEPTH);
	} 
    }
}

void World::_generateHeightmap()
{
    LoadTGATextureData("test2.tga", &heightmap);

    srand(time(NULL));
    
    float x1 = rand() % 5 + 1;
    float x2 = rand() % 5 + 1;
    float z1 = rand() % 4 + 1 ;

    x1 /= 10;
    x2 /= 10;
    z1 /= 100;

    for(int x = 0; x < heightmap.width; x++)
    {
	for(int z = 0; z < heightmap.height; z++)
	{
	    heightmap.imageData[(x + z * heightmap.width) * (heightmap.bpp / 8)] = scaled_octave_noise_2d(x1, x2, z1, 0.0, 15.0, x, z);
	}
    }
}

void World::_createChunk(int detail, int x, int z)
{
    if (x >= 0 && z >= 0 && x <= heightmap.width - CHUNK_WIDTH && z <= heightmap.height - CHUNK_DEPTH)
    {
	if (createdChunks->get(vec3(x, 0, z)) != NULL)
	{
	    _loadChunk(1, x, z);
	}
	else if(!chunks->chunkExistsAt(vec3(x, 0, z)))
	{
	    Chunk* c = new Chunk(_program, &heightmap, CHUNK_WIDTH, CHUNK_HEIGHT, CHUNK_DEPTH, x, z);

	    chunks->put(c->getPos(), c);
	    
	    _updateList.push_back(c);
	}
    }
}

void World::_loadChunk(int detail, int x, int z)
{
    if (x >= 0 && z >= 0 && x <= heightmap.width - CHUNK_WIDTH && z <= heightmap.height - CHUNK_DEPTH)
    {
	Chunk* c = new Chunk(_program, CHUNK_WIDTH, CHUNK_HEIGHT, CHUNK_DEPTH, x, z, heightmap.width);
	
	c->loadChunk(chunksFile);

	chunks->put(c->getPos(), c);
	
	_updateList.push_back(c);
    }
}

