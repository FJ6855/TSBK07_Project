#include "World.h"

#include <fstream>
#include <algorithm>

TextureData _heightmap;

World::World(GLuint program, vec3 playerPosition)
{     
    _lastPlayerPosition = playerPosition;

    int x = ((int)floor(playerPosition.x) - (int)floor(playerPosition.x) % CHUNK_WIDTH);
    int z = ((int)floor(playerPosition.z) - (int)floor(playerPosition.z) % CHUNK_DEPTH);

    loadPosition = vec3(x - floor(LOAD_WIDTH / 2) * CHUNK_WIDTH, 0, z - floor(LOAD_DEPTH / 2) * CHUNK_DEPTH);

    chunks = new HashTable(LOAD_WIDTH * LOAD_DEPTH);

    _nextChunkId = 0;

    _program = program;
    
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

void World::updateChunks(vec3 cameraPosition, vec3 cameraLookAt, vec3 cameraUp, float near, float far, float right)
{
    vec3 viewVector = Normalize(cameraLookAt - cameraPosition);
   
    vec3 nearPosition = cameraPosition + viewVector * near;

    vec3 rightVector = CrossProduct(viewVector, cameraUp);

    vec3 farPosition = cameraPosition + viewVector * far;

    vec3 topLeftCorner = farPosition - rightVector * right / 2;
    vec3 topRightCorner = farPosition + rightVector * right / 2;
    vec3 bottomLeftCorner = nearPosition - rightVector * right / 2;
    vec3 bottomRightCorner = nearPosition + rightVector * right / 2;

    topLeftCorner.x = (int)floor(topLeftCorner.x) - (int)floor(topLeftCorner.x) % CHUNK_WIDTH;
    topRightCorner.x = (int)floor(topRightCorner.x) - (int)floor(topRightCorner.x) % CHUNK_WIDTH;
    bottomLeftCorner.x = (int)floor(bottomLeftCorner.x) - (int)floor(bottomLeftCorner.x) % CHUNK_WIDTH;
    bottomRightCorner.x = (int)floor(bottomRightCorner.x) - (int)floor(bottomRightCorner.x) % CHUNK_WIDTH;

    topLeftCorner.z = (int)floor(topLeftCorner.z) - (int)floor(topLeftCorner.z) % CHUNK_DEPTH;
    topRightCorner.z = (int)floor(topRightCorner.z) - (int)floor(topRightCorner.z) % CHUNK_DEPTH;
    bottomLeftCorner.z = (int)floor(bottomLeftCorner.z) - (int)floor(bottomLeftCorner.z) % CHUNK_DEPTH;
    bottomRightCorner.z = (int)floor(bottomRightCorner.z) - (int)floor(bottomRightCorner.z) % CHUNK_DEPTH;

    topLeftCorner.y = 0.0f;
    topRightCorner.y = 0.0f;
    bottomLeftCorner.y = 0.0f;
    bottomRightCorner.y = 0.0f;

    printf("right vector: %f, %f, %f\n", rightVector.x, rightVector.y, rightVector.z);
    printf("top left: %f, %f, %f\n", topLeftCorner.x, topLeftCorner.y, topLeftCorner.z);
    printf("top right: %f, %f, %f\n", topRightCorner.x, topRightCorner.y, topRightCorner.z);
    printf("bottom left: %f, %f, %f\n", bottomLeftCorner.x, bottomLeftCorner.y, bottomLeftCorner.z);
    printf("bottom right: %f, %f, %f\n", bottomRightCorner.x, bottomRightCorner.y, bottomRightCorner.z);

    Chunk* topLeftChunk = chunks->getChunk(topLeftCorner);
    Chunk* topRightChunk = chunks->getChunk(topRightCorner);
    Chunk* bottomLeftChunk = chunks->getChunk(bottomLeftCorner);
    Chunk* bottomRightChunk = chunks->getChunk(bottomRightCorner);
    
    int topMinX = (topLeftChunk->getPos().x < topRightChunk->getPos().x) ? topLeftChunk->getPos().x : topRightChunk->getPos().x; 
    int bottomMinX = (bottomLeftChunk->getPos().x < bottomRightChunk->getPos().x) ? bottomLeftChunk->getPos().x : bottomRightChunk->getPos().x;
    minX = (topMinX < bottomMinX) ? topMinX : bottomMinX;
    
    int topMinZ = (topLeftChunk->getPos().z < topRightChunk->getPos().z) ? topLeftChunk->getPos().z : topRightChunk->getPos().z; 
    int bottomMinZ = (bottomLeftChunk->getPos().z < bottomRightChunk->getPos().z) ? bottomLeftChunk->getPos().z : bottomRightChunk->getPos().z;
    minZ = (topMinZ < bottomMinZ) ? topMinZ : bottomMinZ;

    int topMaxX = (topLeftChunk->getPos().x < topRightChunk->getPos().x) ? topRightChunk->getPos().x : topLeftChunk->getPos().x; 
    int bottomMaxX = (bottomLeftChunk->getPos().x < bottomRightChunk->getPos().x) ? bottomRightChunk->getPos().x : bottomLeftChunk->getPos().x;
    maxX = (topMaxX < bottomMaxX) ? bottomMaxX : topMaxX;

    int topMaxZ = (topLeftChunk->getPos().z < topRightChunk->getPos().z) ? topRightChunk->getPos().z : topLeftChunk->getPos().z; 
    int bottomMaxZ = (bottomLeftChunk->getPos().z < bottomRightChunk->getPos().z) ? bottomRightChunk->getPos().z : bottomLeftChunk->getPos().z;
    maxZ = (topMaxZ < bottomMaxZ) ? bottomMaxZ : topMaxZ;
}

void World::_generateWorld(GLuint program)
{
    LoadTGATextureData("fft-terrain2.tga", &_heightmap);

    // Old idea, probably remove
    // clear content of chunks.txt
    //std::ofstream file;
    //file.open("chunks.txt", std::ofstream::out | std::ofstream::trunc);
    //file.close();

    //printf("what: %f\n", loadPosition.z);
    for(int z = loadPosition.z / CHUNK_DEPTH; z < LOAD_DEPTH + loadPosition.z / CHUNK_DEPTH; z++)
    {
	for(int x = loadPosition.x / CHUNK_WIDTH; x < LOAD_WIDTH + loadPosition.z / CHUNK_DEPTH; x++)
	{
	    //printf("%i, %i\n", x, z);
	    if (x >= 0 && z >= 0)
		_createChunk(x * CHUNK_WIDTH, z * CHUNK_DEPTH);
	}
    }
    printf("table size: %i\n", chunks->getTableSize());
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

