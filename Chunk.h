#ifndef CHUNK_H
#define CHUNK_H

#include <vector>
#include <fstream>
#include "Vertex.h"
#include "MathUtils.h"
#include "GL/gl.h"
#include "LoadTGA.h"

static float lightValue = 0.85;

#define texX5  0.375
#define texX0  0.125
#define texY1  0.75
#define texY0  0.25

static Vertex cube[36] = {
    //front
    Vertex(vec3(1.0f, 1.0f, 1.0f), vec3(0, 0, 1), vec2(texX5, texY0), lightValue),
    Vertex(vec3(0.0f, 1.0f, 1.0f), vec3(0, 0, 1), vec2(texX0, texY0), lightValue),
    Vertex(vec3(0.0f, 0.0f, 1.0f), vec3(0, 0, 1), vec2(texX0, texY1), lightValue),
    Vertex(vec3(1.0f, 1.0f, 1.0f), vec3(0, 0, 1), vec2(texX5, texY0), lightValue),
    Vertex(vec3(0.0f, 0.0f, 1.0f), vec3(0, 0, 1), vec2(texX0, texY1), lightValue),
    Vertex(vec3(1.0f, 0.0f, 1.0f), vec3(0, 0, 1), vec2(texX5, texY1), lightValue),
    //back
    Vertex(vec3(0.0f, 1.0f, 0.0f), vec3(0, 0, -1), vec2(texX5, texY0), lightValue),
    Vertex(vec3(1.0f, 1.0f, 0.0f), vec3(0, 0, -1), vec2(texX0, texY0), lightValue),
    Vertex(vec3(1.0f, 0.0f, 0.0f), vec3(0, 0, -1), vec2(texX0, texY1), lightValue),
    Vertex(vec3(0.0f, 1.0f, 0.0f), vec3(0, 0, -1), vec2(texX5, texY0), lightValue),
    Vertex(vec3(1.0f, 0.0f, 0.0f), vec3(0, 0, -1), vec2(texX0, texY1), lightValue),
    Vertex(vec3(0.0f, 0.0f, 0.0f), vec3(0, 0, -1), vec2(texX5, texY1), lightValue),
    //down
    Vertex(vec3(1.0f, 0.0f, 1.0f), vec3(0, -1, 0), vec2(texX5, texY0), lightValue),
    Vertex(vec3(0.0f, 0.0f, 1.0f), vec3(0, -1, 0), vec2(texX0, texY0), lightValue),
    Vertex(vec3(0.0f, 0.0f, 0.0f), vec3(0, -1, 0), vec2(texX0, texY1), lightValue),
    Vertex(vec3(1.0f, 0.0f, 1.0f), vec3(0, -1, 0), vec2(texX5, texY0), lightValue),
    Vertex(vec3(0.0f, 0.0f, 0.0f), vec3(0, -1, 0), vec2(texX0, texY1), lightValue),
    Vertex(vec3(1.0f, 0.0f, 0.0f), vec3(0, -1, 0), vec2(texX5, texY1), lightValue),
    //up
    Vertex(vec3(1.0f, 1.0f, 0.0f), vec3(0, 1, 0), vec2(texX5, texY0), lightValue),
    Vertex(vec3(0.0f, 1.0f, 0.0f), vec3(0, 1, 0), vec2(texX0, texY0), lightValue),
    Vertex(vec3(0.0f, 1.0f, 1.0f), vec3(0, 1, 0), vec2(texX0, texY1), lightValue),
    Vertex(vec3(1.0f, 1.0f, 0.0f), vec3(0, 1, 0), vec2(texX5, texY0), lightValue),
    Vertex(vec3(0.0f, 1.0f, 1.0f), vec3(0, 1, 0), vec2(texX0, texY1), lightValue),
    Vertex(vec3(1.0f, 1.0f, 1.0f), vec3(0, 1, 0), vec2(texX5, texY1), lightValue),
    //left
    Vertex(vec3(0.0f, 1.0f, 1.0f), vec3(-1, 0, 0), vec2(texX5, texY0), lightValue),
    Vertex(vec3(0.0f, 1.0f, 0.0f), vec3(-1, 0, 0), vec2(texX0, texY0), lightValue),
    Vertex(vec3(0.0f, 0.0f, 0.0f), vec3(-1, 0, 0), vec2(texX0, texY1), lightValue),
    Vertex(vec3(0.0f, 1.0f, 1.0f), vec3(-1, 0, 0), vec2(texX5, texY0), lightValue),
    Vertex(vec3(0.0f, 0.0f, 0.0f), vec3(-1, 0, 0), vec2(texX0, texY1), lightValue),
    Vertex(vec3(0.0f, 0.0f, 1.0f), vec3(-1, 0, 0), vec2(texX5, texY1), lightValue),
    //right
    Vertex(vec3(1.0f, 1.0f, 0.0f), vec3(1, 0, 0), vec2(texX5, texY0), lightValue),
    Vertex(vec3(1.0f, 1.0f, 1.0f), vec3(1, 0, 0), vec2(texX0, texY0), lightValue),
    Vertex(vec3(1.0f, 0.0f, 1.0f), vec3(1, 0, 0), vec2(texX0, texY1), lightValue),
    Vertex(vec3(1.0f, 1.0f, 0.0f), vec3(1, 0, 0), vec2(texX5, texY0), lightValue),
    Vertex(vec3(1.0f, 0.0f, 1.0f), vec3(1, 0, 0), vec2(texX0, texY1), lightValue),
    Vertex(vec3(1.0f, 0.0f, 0.0f), vec3(1, 0, 0), vec2(texX5, texY1), lightValue)};

class Chunk
{
public:
    Chunk(GLuint program, TextureData* heightmap, int chunkWidth, int chunkHeight, int chunkDepth, int x, int z);

    Chunk(GLuint program, int chunkWidth, int chunkHeight, int chunkDepth, int x, int z, float heightmapWidth);
    
    ~Chunk();

    GLuint getVao();
    int getNumVertices();
    void setPos(vec3 pos);
    vec3 getPos();
    void generateChunk();

    int isBlockActive(int index);
    int isBlockActive(vec3 blockPos);

    bool setBlock(vec3 pos, int blockType);

    bool checkCollision(vec3 pos, vec3 min, vec3 max);
    bool checkCollision(vec3 pos, float radius);

    void saveChunk(std::fstream& file, bool overwrite);
    void loadChunk(std::fstream& file);

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

    std::vector<bool> _activeBlocks;

    GLuint _program;

    void _setFull();
    void _setEmpty();
    void _setHeightmap(TextureData* heightmap, int heightmapX, int heightmapZ);
    void _setTest();
    bool _blockIsSurrounded(int x, int y, int z);
};

#endif
