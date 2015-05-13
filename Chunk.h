#ifndef CHUNK_H
#define CHUNK_H

#include <vector>
#include "Vertex.h"
#include "MathUtils.h"
#include "GL/gl.h"
#include "LoadTGA.h"

static float lightValue = 0.85;


static Vertex cube[36] = {
    //front
    Vertex({1.0f, 1.0f, 1.0f}, {0, 0, 1}, {1, 0}, lightValue),
    Vertex({0.0f, 1.0f, 1.0f}, {0, 0, 1}, {0, 0}, lightValue),
    Vertex({0.0f, 0.0f, 1.0f}, {0, 0, 1}, {0, 1},lightValue),
    Vertex({1.0f, 1.0f, 1.0f}, {0, 0, 1}, {1, 0},lightValue),
    Vertex({0.0f, 0.0f, 1.0f}, {0, 0, 1}, {0, 1} , lightValue),
    Vertex({1.0f, 0.0f, 1.0f}, {0, 0, 1}, {1, 1}, lightValue),
    //back
    Vertex({0.0f, 1.0f, 0.0f}, {0, 0, -1}, {1, 0}, lightValue ),
    Vertex({1.0f, 1.0f, 0.0f}, {0, 0, -1}, {0, 0}, lightValue),
    Vertex({1.0f, 0.0f, 0.0f}, {0, 0, -1}, {0, 1}, lightValue),
    Vertex({0.0f, 1.0f, 0.0f}, {0, 0, -1}, {1, 0}, lightValue),
    Vertex({1.0f, 0.0f, 0.0f}, {0, 0, -1}, {0, 1}, lightValue),
    Vertex({0.0f, 0.0f, 0.0f}, {0, 0, -1}, {1, 1}, lightValue),
    //down
    Vertex({1.0f, 0.0f, 1.0f}, {0, -1, 0}, {1, 0}, lightValue),
    Vertex({0.0f, 0.0f, 1.0f}, {0, -1, 0}, {0, 0}, lightValue),
    Vertex({0.0f, 0.0f, 0.0f}, {0, -1, 0}, {0, 1}, lightValue),
    Vertex({1.0f, 0.0f, 1.0f}, {0, -1, 0}, {1, 0}, lightValue),
    Vertex({0.0f, 0.0f, 0.0f}, {0, -1, 0}, {0, 1}, lightValue),
    Vertex({1.0f, 0.0f, 0.0f}, {0, -1, 0}, {1, 1}, lightValue),
    //up
    Vertex({1.0f, 1.0f, 0.0f}, {0, 1, 0}, {1, 0}, lightValue),
    Vertex({0.0f, 1.0f, 0.0f}, {0, 1, 0}, {0, 0}, lightValue),
    Vertex({0.0f, 1.0f, 1.0f}, {0, 1, 0}, {0, 1}, lightValue),
    Vertex({1.0f, 1.0f, 0.0f}, {0, 1, 0}, {1, 0}, lightValue),
    Vertex({0.0f, 1.0f, 1.0f}, {0, 1, 0}, {0, 1}, lightValue),
    Vertex({1.0f, 1.0f, 1.0f}, {0, 1, 0}, {1, 1}, lightValue),
    //left
    Vertex({0.0f, 1.0f, 1.0f}, {-1, 0, 0}, {1, 0}, lightValue),
    Vertex({0.0f, 1.0f, 0.0f}, {-1, 0, 0}, {0, 0}, lightValue),
    Vertex({0.0f, 0.0f, 0.0f}, {-1, 0, 0}, {0, 1}, lightValue),
    Vertex({0.0f, 1.0f, 1.0f}, {-1, 0, 0}, {1, 0}, lightValue),
    Vertex({0.0f, 0.0f, 0.0f}, {-1, 0, 0}, {0, 1}, lightValue),
    Vertex({0.0f, 0.0f, 1.0f}, {-1, 0, 0}, {1, 1}, lightValue),
    //right
    Vertex({1.0f, 1.0f, 0.0f}, {1, 0, 0}, {1, 0}, lightValue),
    Vertex({1.0f, 1.0f, 1.0f}, {1, 0, 0}, {0, 0}, lightValue),
    Vertex({1.0f, 0.0f, 1.0f}, {1, 0, 0}, {0, 1}, lightValue),
    Vertex({1.0f, 1.0f, 0.0f}, {1, 0, 0}, {1, 0}, lightValue),
    Vertex({1.0f, 0.0f, 1.0f}, {1, 0, 0}, {0, 1}, lightValue),
    Vertex({1.0f, 0.0f, 0.0f}, {1, 0, 0}, {1, 1}, lightValue)};

class Chunk
{
public:
    Chunk(int chunkId, int detail, GLuint program, TextureData* heightmap, int chunkWidth, int chunkHeight, int chunkDepth,  int heightmapX, int heightmapZ);

    Chunk(int chunkId, int detail, GLuint program, int chunkWidth, int chunkHeight, int chunkDepth);
    
    ~Chunk();

    GLuint getVao();
    int getNumVertices();
    void setPos(vec3 pos);
    vec3 getPos();
    void generateChunk();

    bool isBlockActive(int index);
    void setBlockActive(vec3 pos, bool active);

    void saveChunk();
    void loadChunk();

private: 
    int _chunkId;
    int _detail;

    GLuint _vbo;
    GLuint _vao;

    int _chunkWidth;
    int _chunkHeight;
    int _chunkDepth;
    
    int _heightmapX;
    int _heightmapZ;

    vec3 _pos;
    
    int _numVertices;

    int _activeBlocksCount;

    std::vector<bool> _activeBlocks;

    GLuint _program;

    void _setFull();
    void _setEmpty();
    void _setHeightmap(TextureData* heightmap, int heightmapX, int heightmapZ);
    void _setTest();
    bool _blockIsSurrounded(int x, int y, int z);
};

#endif
