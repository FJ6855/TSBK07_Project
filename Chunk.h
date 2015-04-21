#ifndef CHUNK_H
#define CHUNK_H

#include <vector>
#include "Vertex.h"
#include "MathUtils.h"
#include "GL/gl.h"
#include "LoadTGA.h"

class Chunk
{
public:
    Chunk(GLuint program, TextureData* heightmap, int chunkWidth, int chunkHeight, int chunkDepth,  int heightmapX, int heightmapZ);
    
    ~Chunk();

    GLuint getVao();
    int getNumVertices();
    void setPos(vec3 pos);
    vec3 getPos();

private: 
    int _chunkWidth;
    int _chunkHeight;
    int _chunkDepth;
    
    int _heightmapX;
    int _heightmapZ;

    vec3 _pos;
    
    int _numVertices;

    Vertex _cube[36];
    std::vector<bool> _activeBlocks;

    GLuint _vao;
    GLuint _vbo;

    GLuint _program;

    void _generateChunk();

    void _setFull();
    void _setEmpty();
    void _setHeightmap(TextureData* heightmap, int heightmapX, int heightmapZ);
};

#endif
