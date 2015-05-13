#include "World.h"
#include "simplexnoise.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

TextureData _heightmap;

World::World(GLuint program, vec3 playerPosition)
{     
    _lastPlayerPosition = playerPosition;

    int x = ((int)floor(playerPosition.x) - (int)floor(playerPosition.x) % CHUNK_WIDTH);
    int z = ((int)floor(playerPosition.z) - (int)floor(playerPosition.z) % CHUNK_DEPTH);

    loadPosition = vec3(x - floor(LOAD_WIDTH / 2) * CHUNK_WIDTH, 0, z - floor(LOAD_DEPTH / 2) * CHUNK_DEPTH);
    printf("load pos %f\n", loadPosition.z);
    chunks = new HashTable(LOAD_WIDTH * LOAD_DEPTH);

    _nextChunkId = 0;

    _program = program;

    srand(time(NULL));

    _generateWorld(program);
}

World::~World()
{
    /*for (int i = 0; i < chunks.size(); i++)
    {
	Chunk* c = chunks.at(i);

	delete c;
	c = NULL;
	}*/
}

Chunk* World::getChunkAtPosition(vec3 position)
{    
    int x = ((int)floor(position.x) - (int)floor(position.x) % CHUNK_WIDTH);
    int z = ((int)floor(position.z) - (int)floor(position.z) % CHUNK_DEPTH);

    return chunks->getChunk(vec3(x, 0, z));
}

void World::update()
{ 
    //printf("update world\n");
  
    int i = 0;

    for (i = 0; i < _updateList.size() & i < 2; i++)
    {
	_updateList.at(_updateList.size() - 1 - i)->generateChunk();
    }

    for (int j = 0; j < i; j++)
    {
	_updateList.pop_back();
    }

    // printf("update world done!\n");
}

void World::removeBlock(int x, int y, int z)
{

}

void World::loadChunks(vec3 playerPosition)
{
    //printf("load chunks\n");

    int newX = ((int)floor(playerPosition.x) - (int)floor(playerPosition.x) % CHUNK_WIDTH);
    int oldX = ((int)floor(_lastPlayerPosition.x) - (int)floor(_lastPlayerPosition.x) % CHUNK_WIDTH);
    
    int newZ = ((int)floor(playerPosition.z) - (int)floor(playerPosition.z) % CHUNK_DEPTH);
    int oldZ = ((int)floor(_lastPlayerPosition.z) - (int)floor(_lastPlayerPosition.z) % CHUNK_DEPTH);
    
    if (newX != oldX)
    {	
	loadPosition.x = newX - floor(LOAD_WIDTH / 2) * CHUNK_WIDTH;

        //printf("changed in x\n");

	int directionX = (newX - oldX < 0) ? -1 : 1;

	for (int i = 0; i < LOAD_DEPTH; i++)
	{
	    int x = oldX - directionX * floor(LOAD_WIDTH / 2) * CHUNK_WIDTH;
	    int z = oldZ - CHUNK_DEPTH * floor(LOAD_DEPTH / 2) + i * CHUNK_DEPTH;
	    
	    //printf("remove: %i, %i\n", x, z);
	    
	    if (x >= 0 && z >= 0)
		chunks->remove(vec3(x, 0, z));

	    x = newX + directionX * floor(LOAD_WIDTH / 2) * CHUNK_WIDTH;
	    z = newZ - CHUNK_DEPTH * floor(LOAD_DEPTH / 2) + i * CHUNK_DEPTH;

	    //printf("add: %i, %i\n", x, z);
	    
	    if (x >= 0 && z >= 0)
		_createChunk(x, z);
	}
    }
    
    if (newZ != oldZ)
    {	
	loadPosition.z = newZ - floor(LOAD_DEPTH / 2) * CHUNK_DEPTH;

	//printf("changed in z\n");

	int directionZ = (newZ - oldZ < 0) ? -1 : 1;

	for (int i = 0; i < LOAD_WIDTH; i++)
	{
	    int x = oldX - CHUNK_WIDTH * floor(LOAD_WIDTH / 2) + i * CHUNK_WIDTH;
	    int z = oldZ - directionZ * floor(LOAD_DEPTH / 2) * CHUNK_DEPTH;
	    
	    //printf("remove: %i, %i\n", x, z);
	    
	    if (x >= 0 && z >= 0)
		chunks->remove(vec3(x, 0, z));

	    x = newX - CHUNK_WIDTH * floor(LOAD_WIDTH / 2) + i * CHUNK_WIDTH;
	    z = newZ + directionZ * floor(LOAD_DEPTH / 2) * CHUNK_DEPTH;

	    //printf("add: %i, %i\n", x, z);

	    if (x >= 0 && z >= 0)
		_createChunk(x, z);
	}
    }
    
    //chunks->printTable();

    printf("Table size: %i\n", chunks->getTableSize());

    _lastPlayerPosition = playerPosition;
}

void World::updateChunks(vec3 cameraPosition, vec3 cameraLookAt, vec3 cameraUp, float near, float far, float right, float fov, float aspectRatio)
{
    // Get the direction we are looking
    vec3 viewVector = Normalize(cameraLookAt - cameraPosition);
    
    // Get the position of the near plane
    vec3 nearPosition = cameraPosition + viewVector * near;
    
    // Get the position of the far plane
    vec3 farPosition = cameraPosition + viewVector * far;
    
    // Get the vector that is orthogonal to the view vector and the up vector
    vec3 rightVector = CrossProduct(viewVector, cameraUp);

    // Calculate the near and far planes width and height
    float nearHeight = 2 * tan(fov / 2) * near;
    float nearWidth = nearHeight * aspectRatio;

    float farHeight = 2 * tan(fov / 2) * far;
    float farWidth = farHeight * aspectRatio;

    // Get the positions of the far and near planes edges
    vec3 farLeftEdge = farPosition - rightVector * (farWidth / 2);
    vec3 farRightEdge = farPosition + rightVector * (farWidth / 2);

    vec3 nearLeftEdge = nearPosition - rightVector * (nearWidth / 2);
    vec3 nearRightEdge = nearPosition + rightVector * (nearWidth / 2);

    // Round of the positions into chunk coordinates 
    farLeftEdge.x = (int)floor(farLeftEdge.x) - (int)floor(farLeftEdge.x) % CHUNK_WIDTH;
    farRightEdge.x = (int)floor(farRightEdge.x) - (int)floor(farRightEdge.x) % CHUNK_WIDTH;
   
    farLeftEdge.z = (int)floor(farLeftEdge.z) - (int)floor(farLeftEdge.z) % CHUNK_DEPTH;
    farRightEdge.z = (int)floor(farRightEdge.z) - (int)floor(farRightEdge.z) % CHUNK_DEPTH;

    nearLeftEdge.x = (int)floor(nearLeftEdge.x) - (int)floor(nearLeftEdge.x) % CHUNK_WIDTH;
    nearRightEdge.x = (int)floor(nearRightEdge.x) - (int)floor(nearRightEdge.x) % CHUNK_WIDTH;

    nearLeftEdge.z = (int)floor(nearLeftEdge.z) - (int)floor(nearLeftEdge.z) % CHUNK_DEPTH;
    nearRightEdge.z = (int)floor(nearRightEdge.z) - (int)floor(nearRightEdge.z) % CHUNK_DEPTH;

    // Calculate which are the min and max in x and z of the positions
    // the result will be the area we loop through and render the chunks falling inside these coordinates
    int topMinX = (farLeftEdge.x < farRightEdge.x) ? farLeftEdge.x : farRightEdge.x; 
    int bottomMinX = (nearLeftEdge.x < nearRightEdge.x) ? nearLeftEdge.x : nearRightEdge.x;
    minX = (topMinX < bottomMinX) ? topMinX : bottomMinX;
    
    int topMinZ = (farLeftEdge.z < farRightEdge.z) ? farLeftEdge.z : farRightEdge.z; 
    int bottomMinZ = (nearLeftEdge.z < nearRightEdge.z) ? nearLeftEdge.z : nearRightEdge.z;
    minZ = (topMinZ < bottomMinZ) ? topMinZ : bottomMinZ;

    int topMaxX = (farLeftEdge.x < farRightEdge.x) ? farRightEdge.x : farLeftEdge.x; 
    int bottomMaxX = (nearLeftEdge.x < nearRightEdge.x) ? nearRightEdge.x : nearLeftEdge.x;
    maxX = (topMaxX < bottomMaxX) ? bottomMaxX : topMaxX;

    int topMaxZ = (farLeftEdge.z < farRightEdge.z) ? farRightEdge.z : farLeftEdge.z; 
    int bottomMaxZ = (nearLeftEdge.z < nearRightEdge.z) ? nearRightEdge.z : nearLeftEdge.z;
    maxZ = (topMaxZ < bottomMaxZ) ? bottomMaxZ : topMaxZ;
}

void World::_generateWorld(GLuint program)
{
    LoadTGATextureData("test2.tga", &_heightmap);
    
    float x1 = rand() % 5 + 1;
    float x2 = rand() % 5 + 1;
    float z1 = rand() % 4 + 1 ;

    x1 /= 10;
    x2 /= 10;
    z1 = 0.05;

    for(int x = 0; x <_heightmap.width; x++)
    {
	for(int z = 0; z < _heightmap.height; z++)
	{
	    _heightmap.imageData[(x + z * _heightmap.width) * (_heightmap.bpp / 8)] = scaled_octave_noise_2d(x1, x2, z1, 0.0, 15.0, x, z);
	}
    }

    for(int z = loadPosition.z / CHUNK_DEPTH; z < LOAD_DEPTH + loadPosition.z / CHUNK_DEPTH; z++)
    {
	for(int x = loadPosition.x / CHUNK_WIDTH; x < LOAD_WIDTH + loadPosition.z / CHUNK_DEPTH; x++)
	{
	    if (x >= 0 && z >= 0)
		_createChunk(x * CHUNK_WIDTH, z * CHUNK_DEPTH);
	}
    }    
}

int World::_getChunkIndexAtPosition(vec3 position)
{
    int x = ((int)floor(position.x) - (int)floor(position.x) % CHUNK_WIDTH) / CHUNK_WIDTH;
    int z = ((int)floor(position.z) - (int)floor(position.z) % CHUNK_DEPTH) / CHUNK_DEPTH;

    int index = x + z * WORLD_DEPTH;
    
    printf("%i\n", index);

    return index;
}

void World::_createChunk(int x, int z)
{
    Chunk* c = new Chunk(_nextChunkId, _program, &_heightmap, CHUNK_WIDTH, CHUNK_HEIGHT, CHUNK_DEPTH, x, z);

    c->setPos(vec3(x, 0, z));

    chunks->put(c->getPos(), c);
		
    _updateList.push_back(c);

    _nextChunkId++;
}

