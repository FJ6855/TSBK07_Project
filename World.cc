#include "World.h"
#include "simplexnoise.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <map>

TextureData _heightmap;

World::World(GLuint program, vec3 playerPosition)
{     
    _lastPlayerPosition = playerPosition;

    chunks = new HashTable((VIEW_DISTANCE * 2) * (VIEW_DISTANCE * 2));

    _nextChunkId = 0;

    _program = program;

    srand(time(NULL));
    _generateWorld(program);
}

World::~World()
{
    delete chunks;
    chunks = NULL;
}

Chunk* World::getChunkAtPosition(vec3 position)
{    
    int x = ((int)position.x - (int)position.x % CHUNK_WIDTH);
    int z = ((int)position.z - (int)position.z % CHUNK_DEPTH);

    return chunks->getChunk(vec3(x, 0, z));
}

void World::update()
{ 
    //printf("update world\n");
  
    int i = 0;

    for (i = 0; i < _updateList.size() & i < 2; i++)
    {
	Chunk* c = _updateList.at(_updateList.size() - 1 - i);

	c->generateChunk();
	
	//chunks->remove(c->getPos());
    }

    for (int j = 0; j < i; j++)
    {
	_updateList.pop_back();
    }

    // printf("update world done!\n");
}

void World::removeBlock(vec3 pos, vec3 direction)
{
    vec3 blockPos = VectorAdd(pos, direction);

    printf("Player pos, %f, %f\n", pos.x, pos.z);
    printf("block pos, %f, %f\n", blockPos.x, blockPos.z);

    int chunkX = blockPos.x - (int)blockPos.x % CHUNK_WIDTH;
    int chunkZ = blockPos.z - (int)blockPos.z % CHUNK_DEPTH;

    

    Chunk* c = chunks->getChunk(vec3(chunkX, 0, chunkZ));
   
    if(c != NULL)
    {
	c->setBlock(vec3((int)blockPos.x % CHUNK_WIDTH, (int)blockPos.y % CHUNK_HEIGHT, (int)blockPos.z % CHUNK_DEPTH), 0);
	//c->generateChunk();
	_updateList.push_back(c);
    }
}

void World::loadChunks(vec3 playerPosition)
{
    //printf("load chunks\n");

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
	    
	    if (x >= 0 && z >= 0)
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
	    
	    if (x >= 0 && z >= 0)
	    	chunks->remove(vec3(x, 0, z));

	    x = newX - CHUNK_WIDTH * VIEW_DISTANCE + i * CHUNK_WIDTH;
	    z = newZ + directionZ * VIEW_DISTANCE * CHUNK_DEPTH;

	    _createChunk(1, x, z);
	}
    }

    _lastPlayerPosition = playerPosition;

    //printf("table size: %i\n", chunks->getTableSize());
}

void World::updateRenderList(Frustum* frustum, vec3 playerPosition)
{
    int playerPosX = (int)playerPosition.x - (int)playerPosition.x % CHUNK_WIDTH;
    int playerPosZ = (int)playerPosition.z - (int)playerPosition.z % CHUNK_DEPTH;

    // Round of the positions into chunk coordinates 
    int farLeftChunkX = (int)frustum->farLeftPoint.x - (int)frustum->farLeftPoint.x % CHUNK_WIDTH;
    int farLeftChunkZ = (int)frustum->farLeftPoint.z - (int)frustum->farLeftPoint.z % CHUNK_DEPTH;

    int farRightChunkX = (int)frustum->farRightPoint.x - (int)frustum->farRightPoint.x % CHUNK_WIDTH;   
    int farRightChunkZ = (int)frustum->farRightPoint.z - (int)frustum->farRightPoint.z % CHUNK_DEPTH;

    int nearLeftChunkX = (int)frustum->nearLeftPoint.x - (int)frustum->nearLeftPoint.x % CHUNK_WIDTH;
    int nearLeftChunkZ = (int)frustum->nearLeftPoint.z - (int)frustum->nearLeftPoint.z % CHUNK_DEPTH;

    int nearRightChunkX = (int)frustum->nearRightPoint.x - (int)frustum->nearRightPoint.x % CHUNK_WIDTH;
    int nearRightChunkZ = (int)frustum->nearRightPoint.z - (int)frustum->nearRightPoint.z % CHUNK_DEPTH;

    // Calculate which are the min and max in x and z of the positions
    // the result will be the area we loop through and render the chunks falling inside these coordinates
    int topMinX = (farLeftChunkX < farRightChunkX) ? farLeftChunkX : farRightChunkX; 
    int bottomMinX = (nearLeftChunkX < nearRightChunkX) ? nearLeftChunkX : nearRightChunkX;
    int minX = (topMinX < bottomMinX) ? topMinX : bottomMinX;
    
    int topMinZ = (farLeftChunkZ < farRightChunkZ) ? farLeftChunkZ : farRightChunkZ; 
    int bottomMinZ = (nearLeftChunkZ < nearRightChunkZ) ? nearLeftChunkZ : nearRightChunkZ;
    int minZ = (topMinZ < bottomMinZ) ? topMinZ : bottomMinZ;

    int topMaxX = (farLeftChunkX < farRightChunkX) ? farRightChunkX : farLeftChunkX; 
    int bottomMaxX = (nearLeftChunkX < nearRightChunkX) ? nearRightChunkX : nearLeftChunkX;
    int maxX = (topMaxX < bottomMaxX) ? bottomMaxX : topMaxX;

    int topMaxZ = (farLeftChunkZ < farRightChunkZ) ? farRightChunkZ : farLeftChunkZ; 
    int bottomMaxZ = (nearLeftChunkZ < nearRightChunkZ) ? nearRightChunkZ : nearLeftChunkZ;
    int maxZ = (topMaxZ < bottomMaxZ) ? bottomMaxZ : topMaxZ;

    renderList.clear();

    for (int z = minZ; z <= maxZ; z += CHUNK_DEPTH)
    {
	for (int x = minX; x <= maxX; x += CHUNK_WIDTH)
	{
	    if (frustum->pointIsInsideFrustum(vec3(x, 0, z)))
	    {
		renderList.push_back(vec3(x, 0, z));
	    }
	    else if (frustum->pointIsInsideFrustum(vec3(x + CHUNK_WIDTH, 0, z)))
	    {
		renderList.push_back(vec3(x, 0, z));
	    }
	    else if (frustum->pointIsInsideFrustum(vec3(x, 0, z + CHUNK_DEPTH)))
	    {
		renderList.push_back(vec3(x, 0, z));
	    }
	    else if (frustum->pointIsInsideFrustum(vec3(x + CHUNK_WIDTH, 0, z + CHUNK_DEPTH)))
	    {
		renderList.push_back(vec3(x, 0, z));
	    }
	}
    }

    renderList.push_back(vec3(playerPosX, 0, playerPosZ));
    renderList.push_back(vec3(farLeftChunkX, 0, farLeftChunkZ));
    renderList.push_back(vec3(farRightChunkX, 0, farRightChunkZ));
    renderList.push_back(vec3(nearLeftChunkX, 0, nearLeftChunkZ));
    renderList.push_back(vec3(nearRightChunkX, 0, nearRightChunkZ));
}

void World::_generateWorld(GLuint program)
{
    LoadTGATextureData("test2.tga", &_heightmap);
    
    float x1 = rand() % 5 + 1;
    float x2 = rand() % 5 + 1;
    float z1 = rand() % 4 + 1 ;

    x1 /= 10;
    x2 /= 10;
    z1 = 0.01;

    for(int x = 0; x <_heightmap.width; x++)
    {
	for(int z = 0; z < _heightmap.height; z++)
	{
	    _heightmap.imageData[(x + z * _heightmap.width) * (_heightmap.bpp / 8)] = scaled_octave_noise_2d(x1, x2, z1, 0.0, 15.0, x, z);
	}
    }
    
    for (int z = _lastPlayerPosition.z / CHUNK_DEPTH - VIEW_DISTANCE; z < _lastPlayerPosition.z / CHUNK_DEPTH + VIEW_DISTANCE; z++)
    {
	for (int x = _lastPlayerPosition.x / CHUNK_WIDTH - VIEW_DISTANCE; x < _lastPlayerPosition.x / CHUNK_WIDTH + VIEW_DISTANCE; x++)
	{
	    _createChunk(1, x * CHUNK_WIDTH, z * CHUNK_DEPTH);
	} 
    } 

    //printf("table size: %i\n", chunks->getTableSize());
    //chunks->printTable();
}

void World::_createChunk(int detail, int x, int z)
{
    if (x >= 0 && z >= 0 && !chunks->chunkExistsAt(vec3(x, 0, z)))
    {
	Chunk* c = new Chunk(_nextChunkId, detail, _program, &_heightmap, CHUNK_WIDTH, CHUNK_HEIGHT, CHUNK_DEPTH, x, z);
	c->setPos(vec3(x, 0, z));
	
	chunks->put(c->getPos(), c);
	
	_updateList.push_back(c);
	
	_nextChunkId++;
    }
}

