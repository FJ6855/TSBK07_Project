#include "Chunk.h"
#include <stdio.h>

Chunk::Chunk(GLuint program, TextureData* heightmap, int chunkWidth, int chunkHeight, int chunkDepth,  int heightmapX, int heightmapZ)
{   
    _program = program;

    _chunkWidth = chunkWidth;
    _chunkHeight = chunkHeight;
    _chunkDepth = chunkDepth;

    _activeBlocks.resize(chunkWidth * chunkHeight * chunkDepth);
    
    //_setFull();
    _setHeightmap(heightmap, heightmapX, heightmapZ);
    
   glGenVertexArrays(1, &_vao);
/*

1   -0.5 -0.5 0.5
2   -0.5 0.5 0.5
3   0.5 0.5 0.5
4   0.5 -0.5 0.5
5   -0.5 -0.5 -0.5
6   -0.5 0.5 -0.5
7   0.5 0.5 -0.5
8   0.5 -0.5 -0.5

*/
//front
   _cube[0] = Vertex({1.0, 1.0, 1.0}, {0, 0, 1}, {1, 1});
   _cube[1] = Vertex({0.0, 1.0, 1.0}, {0, 0, 1}, {0, 0} );
   _cube[2] = Vertex({0.0, 0.0, 1.0}, {0, 0, 1}, {0, 1} );
   
   _cube[3] = Vertex({1.0, 1.0, 1.0}, {0, 0, 1}, {1, 1} );
   _cube[4] = Vertex({0.0, 0.0, 1.0}, {0, 0, 1}, {0, 1} );
   _cube[5] = Vertex({1.0, 0.0, 1.0}, {0, 0, 1}, {1, 0} );
    
//back
    _cube[6] = Vertex({0.0, 1.0, 0.0}, {0, 0, -1}, {1, 1} );
    _cube[7] = Vertex({1.0, 1.0, 0.0,}, {0, 0, -1}, {0, 0} );
    _cube[8] = Vertex({1.0, 0.0, 0.0}, {0, 0, -1}, {0, 1} );

    _cube[9] = Vertex({0.0, 1.0, 0.0}, {0, 0, -1}, {1, 1} );
    _cube[10] = Vertex({1.0, 0.0, 0.0}, {0, 0, -1}, {0, 1} );
    _cube[11] = Vertex({0.0, 0.0, 0.0}, {0, 0, -1}, {1, 0} );
    
//down
    _cube[12] = Vertex({1.0, 0.0, 1.0}, {0, -1, 0}, {1, 1} );
    _cube[13] = Vertex({0.0, 0.0, 1.0,}, {0, -1, 0}, {0, 0} );
    _cube[14] = Vertex({0.0, 0.0, 0.0}, {0, -1, 0}, {0, 1} );

    _cube[15] = Vertex({1.0, 0.0, 1.0}, {0, -1, 0}, {1, 1} );
    _cube[16] = Vertex({0.0, 0.0, 0.0}, {0, -1, 0}, {0, 1} );
    _cube[17] = Vertex({1.0, 0.0, 0.0}, {0, -1, 0}, {1, 0} );

//up
    _cube[18] = Vertex({1.0, 1.0, 0.0}, {0, 1, 0}, {1, 1} );
    _cube[19] = Vertex({0.0, 1.0, 0.0,}, {0, 1, 0}, {0, 0} );
    _cube[20] = Vertex({0.0, 1.0, 1.0}, {0, 1, 0}, {0, 1} );
    
    _cube[21] = Vertex({1.0, 1.0, 0.0}, {0, 1, 0}, {1, 1} );
    _cube[22] = Vertex({0.0, 1.0, 1.0}, {0, 1, 0}, {0, 1} );
    _cube[23] = Vertex({1.0, 1.0, 1.0}, {0, 1, 0}, {1, 0} );

//left
    _cube[24] = Vertex({0.0, 1.0, 1.0}, {-1, 0, 0}, {1, 1} );
    _cube[25] = Vertex({0.0, 1.0, 0.0,}, {-1, 0, 0}, {0, 0} );
    _cube[26] = Vertex({0.0, 0.0, 0.0}, {-1, 0, 0}, {0, 1} );
    
    _cube[27] = Vertex({0.0, 1.0, 1.0}, {-1, 0, 0}, {1, 1} );
    _cube[28] = Vertex({0.0, 0.0, 0.0}, {-1, 0, 0}, {0, 1} );
    _cube[29] = Vertex({0.0, 0.0, 1.0}, {-1, 0, 0}, {1, 0} );

//right
    _cube[30] = Vertex({1.0, 1.0, 0.0}, {1, 0, 0}, {1, 1} );
    _cube[31] = Vertex({1.0, 1.0, 1.0,}, {1, 0, 0}, {0, 0} );
    _cube[32] = Vertex({1.0, 0.0, 1.0}, {1, 0, 0}, {0, 1} );
    
    _cube[33] = Vertex({1.0, 1.0, 0.0}, {1, 0, 0}, {1, 1} );
    _cube[34] = Vertex({1.0, 0.0, 1.0}, {1, 0, 0}, {0, 1} );
    _cube[35] = Vertex({1.0, 0.0, 0.0}, {1, 0, 0}, {1, 0} );

    _generateChunk();
}

Chunk::~Chunk()
{

}

void Chunk::_setFull()
{ 
    std::fill(_activeBlocks.begin(), _activeBlocks.end(), true);
    
    for (int y = 0; y < _chunkHeight; y++)
    {
	for (int x = 0; x < _chunkWidth; x++)
	{
	    for (int z = 0; z < _chunkDepth; z++)
	    {	
		int index = z + x * _chunkDepth + y * _chunkDepth * _chunkWidth;
		
		if (index == 18 || index == 19 || index == 20 || index == 21 || index == 23 || index == 24 || index == 25 || index == 26)
		{
		    _activeBlocks.at(index) = false;
		}
	    }
	}
    }
}

void Chunk::_setEmpty()
{
    std::fill(_activeBlocks.begin(), _activeBlocks.end(), false);
}

void Chunk::_setHeightmap(TextureData* heightmap, int heightmapX, int heightmapZ)
{    
    for (int y = 0; y < _chunkHeight; y++)
    {
	for (int x = heightmapX; x < _chunkWidth + heightmapX; x++)
	{
	    for (int z = heightmapZ; z < _chunkDepth + heightmapZ; z++)
	    {	
		int index = (z - heightmapZ) + (x - heightmapX) * _chunkDepth + y * _chunkDepth * _chunkWidth;
				
		if (y <= heightmap->imageData[(x + z * heightmap->width) * (heightmap->bpp / 8)] / 10.0f)
		{
		    _activeBlocks.at(index) = true;
		}		 
		else
		{
		    _activeBlocks.at(index) = false;
		}		
	    }
	}
    }
}

void Chunk::_setTest()
{
    for (int y = 0; y < _chunkHeight; y++)
    {
	for (int x = 0; x < _chunkWidth; x++)
	{
	    for (int z = 0; z < _chunkDepth; z++)
	    {	
		int index = z + x * _chunkDepth + y * _chunkDepth * _chunkWidth;

		if (z < 3 && x < 3 && y < 3)
		{
		    _activeBlocks.at(index) = true;
		}		 
		else
		{
		    _activeBlocks.at(index) = false;
		}
	    }
	}
    }
}

void Chunk::_generateChunk()
{
    Vertex _vertices[(_chunkHeight * _chunkWidth * _chunkDepth) * 36];
   
    _numVertices = 0;
    
    glGenBuffers(1, &_vbo);
    glBindVertexArray(_vao);

    for (int y = 0; y < _chunkHeight; y++)
    {
	for (int x = 0; x < _chunkWidth; x++)
	{
	    for (int z = 0; z < _chunkDepth; z++)
	    {
		if (_activeBlocks.at(z + x * _chunkDepth + y * _chunkDepth * _chunkWidth))
		{
		    for(int i = 0; i < 36 ; i++)
		    {
			/*_vertices[(z + x * _chunkDepth + y * _chunkDepth * _chunkWidth)*36 + i] = _cube[i];
			_vertices[(z + x * _chunkDepth + y * _chunkDepth * _chunkWidth)*36 + i].pos.x += x;
			_vertices[(z + x * _chunkDepth + y * _chunkDepth * _chunkWidth)*36 + i].pos.y += y;
			_vertices[(z + x * _chunkDepth + y * _chunkDepth * _chunkWidth)*36 + i].pos.z += z;
			*/
			_vertices[_numVertices + i] = _cube[i];
			_vertices[_numVertices + i].pos.x += x;
			_vertices[_numVertices + i].pos.y += y;
			_vertices[_numVertices + i].pos.z += z;
		    }
		    
		    _numVertices += 36;
		}
	    }
	}
    }

    // VBO for vertex data
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, _numVertices * 8 * sizeof(GLfloat), _vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(_program, "inPosition"), 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0); 
    glEnableVertexAttribArray(glGetAttribLocation(_program, "inPosition"));

    glVertexAttribPointer(glGetAttribLocation(_program, "inNormal"), 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (const GLfloat*)(3 * sizeof(GLfloat))); 
    glEnableVertexAttribArray(glGetAttribLocation(_program, "inNormal"));
    
    glVertexAttribPointer(glGetAttribLocation(_program, "inTexCoord"), 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),(const GLfloat*)(6 * sizeof(GLfloat))); 
    glEnableVertexAttribArray(glGetAttribLocation(_program, "inTexCoord"));
}

GLuint Chunk::getVao()
{
    return _vao;
}

int Chunk::getNumVertices()
{
    return _numVertices;
}

void Chunk::setPos(vec3 pos)
{
    _pos = pos;
}

vec3 Chunk::getPos()
{
    return _pos;
}

bool Chunk::isBlockActive(int index)
{
    return _activeBlocks.at(index);
}
