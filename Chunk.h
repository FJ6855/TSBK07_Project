#ifndef CHUNK_H
#define CHUNK_H

#include <vector>
#include "Vertex.h"
#include "MathUtils.h"
#include "GL/gl.h"

#define CHUNK_HEIGHT 16
#define CHUNK_WIDTH 16
#define CHUNK_DEPTH 16

class Chunk
{
public:
    Chunk(GLuint program);
    //Chunk(TextureData* heightMap);
    
    ~Chunk();

    GLuint getVao();
    int getNumVertices();
    void setPos(vec3 pos);
    vec3 getPos();

private:
    //TextureData* _heightMap;  
    vec3 _pos;
    
    int _numVertices;

    Vertex _cube[36];
    std::vector<bool> _activeBlocks;

    GLuint _vao;
    GLuint _vbo;

    GLuint _program;

    void _generateChunk();
};

#endif
