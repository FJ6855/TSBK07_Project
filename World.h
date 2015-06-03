#ifndef WORLD_H
#define WORLD_H

#include <fstream>

#include "Chunk.h"
#include "LoadTGA.h"
#include "HashTable.h"
#include "Frustum.h"

#define CHUNK_WIDTH 16
#define CHUNK_HEIGHT 16
#define CHUNK_DEPTH 16

#define VIEW_DISTANCE 5

class World
{
public:
    World(GLuint program, vec3 playerPosition);

    ~World();

    HashTable* chunks;
    HashTable* createdChunks;
   
    Chunk* getChunkAtPosition(vec3 position);
    bool isBlockActive(vec3 pos);

    void changeBlock(vec3 pos, int blockType);

    void update();

    void loadChunks(vec3 playerPosition);

    void updateRenderList(Frustum* frustum, vec3 playerPosition);

    std::vector<vec3> renderList;
    
private:
    int _nextChunkId;
    vec3 _lastPlayerPosition;

    GLuint _program;
    
    std::vector<Chunk*> _updateList;

    void _generateWorld();
    void _generateHeightmap();

    void _createChunk(int detail, int x, int z);
    void _loadChunk(int detail, int x, int z);

    std::fstream chunksFile;
};

#endif
