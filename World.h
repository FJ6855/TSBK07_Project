#ifndef WORLD_H
#define WORLD_H

#include "Chunk.h"
#include "LoadTGA.h"
#include "HashTable.h"

#define WORLD_WIDTH 3
#define WORLD_HEIGHT 1
#define WORLD_DEPTH 3

#define CHUNK_WIDTH 16
#define CHUNK_HEIGHT 16
#define CHUNK_DEPTH 16

#define LOAD_WIDTH 15
#define LOAD_DEPTH 15

struct Buffer
{
    bool free;
    GLuint vbo;
    GLuint vao;
};

class World
{
public:
    World(GLuint program, vec3 playerPosition);

    ~World();

    HashTable* chunks;
   
    Chunk* getChunkAtPosition(vec3 position);
    void removeBlock(int x, int y, int z);
    void update();
    void loadChunks(vec3 playerPosition);
    void updateChunks(vec3 cameraPosition, vec3 cameraLookat, vec3 cameraUp, float near, float far, float right);
    
    vec3 loadPosition;

    int minX;
    int minZ;
    int maxX;
    int maxZ;

    std::vector<Buffer> buffers;

private:
    int _nextChunkId;
    vec3 _lastPlayerPosition;

    GLuint _program;

    std::vector<Chunk*> _updateList;

    void _generateWorld(GLuint program);
    int _getChunkIndexAtPosition(vec3 position);
    void _createChunk(int x, int z);
};

#endif
