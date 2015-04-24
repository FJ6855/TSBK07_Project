#include "Chunk.h"
#include <stdio.h>
#include <iostream>

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


   float lightValue = 0.85;

   //front
   _cube[0] = Vertex({1.0f, 1.0f, 1.0f}, {0, 0, 1}, {1, 0}, lightValue);
   _cube[1] = Vertex({0.0f, 1.0f, 1.0f}, {0, 0, 1}, {0, 0}, lightValue);
   _cube[2] = Vertex({0.0f, 0.0f, 1.0f}, {0, 0, 1}, {0, 1},lightValue);
   
   _cube[3] = Vertex({1.0f, 1.0f, 1.0f}, {0, 0, 1}, {1, 0},lightValue);
   _cube[4] = Vertex({0.0f, 0.0f, 1.0f}, {0, 0, 1}, {0, 1} , lightValue);
   _cube[5] = Vertex({1.0f, 0.0f, 1.0f}, {0, 0, 1}, {1, 1}, lightValue);
    
   //back
    _cube[6] = Vertex({0.0f, 1.0f, 0.0f}, {0, 0, -1}, {1, 0}, lightValue );
    _cube[7] = Vertex({1.0f, 1.0f, 0.0f,}, {0, 0, -1}, {0, 0}, lightValue);
    _cube[8] = Vertex({1.0f, 0.0f, 0.0f}, {0, 0, -1}, {0, 1}, lightValue);

    _cube[9] = Vertex({0.0f, 1.0f, 0.0f}, {0, 0, -1}, {1, 0}, lightValue);
    _cube[10] = Vertex({1.0f, 0.0f, 0.0f}, {0, 0, -1}, {0, 1}, lightValue);
    _cube[11] = Vertex({0.0f, 0.0f, 0.0f}, {0, 0, -1}, {1, 1}, lightValue);
    
    //down
    _cube[12] = Vertex({1.0f, 0.0f, 1.0f}, {0, -1, 0}, {1, 0}, lightValue);
    _cube[13] = Vertex({0.0f, 0.0f, 1.0f,}, {0, -1, 0}, {0, 0}, lightValue);
    _cube[14] = Vertex({0.0f, 0.0f, 0.0f}, {0, -1, 0}, {0, 1}, lightValue);

    _cube[15] = Vertex({1.0f, 0.0f, 1.0f}, {0, -1, 0}, {1, 0}, lightValue);
    _cube[16] = Vertex({0.0f, 0.0f, 0.0f}, {0, -1, 0}, {0, 1}, lightValue);
    _cube[17] = Vertex({1.0f, 0.0f, 0.0f}, {0, -1, 0}, {1, 1}, lightValue);

    //up
    _cube[18] = Vertex({1.0f, 1.0f, 0.0f}, {0, 1, 0}, {1, 0}, lightValue);
    _cube[19] = Vertex({0.0f, 1.0f, 0.0f,}, {0, 1, 0}, {0, 0}, lightValue);
    _cube[20] = Vertex({0.0f, 1.0f, 1.0f}, {0, 1, 0}, {0, 1}, lightValue);
    
    _cube[21] = Vertex({1.0f, 1.0f, 0.0f}, {0, 1, 0}, {1, 0}, lightValue);
    _cube[22] = Vertex({0.0f, 1.0f, 1.0f}, {0, 1, 0}, {0, 1}, lightValue);
    _cube[23] = Vertex({1.0f, 1.0f, 1.0f}, {0, 1, 0}, {1, 1}, lightValue);

    //left
    _cube[24] = Vertex({0.0f, 1.0f, 1.0f}, {-1, 0, 0}, {1, 0}, lightValue);
    _cube[25] = Vertex({0.0f, 1.0f, 0.0f,}, {-1, 0, 0}, {0, 0}, lightValue);
    _cube[26] = Vertex({0.0f, 0.0f, 0.0f}, {-1, 0, 0}, {0, 1}, lightValue);
    
    _cube[27] = Vertex({0.0f, 1.0f, 1.0f}, {-1, 0, 0}, {1, 0}, lightValue);
    _cube[28] = Vertex({0.0f, 0.0f, 0.0f}, {-1, 0, 0}, {0, 1}, lightValue);
    _cube[29] = Vertex({0.0f, 0.0f, 1.0f}, {-1, 0, 0}, {1, 1}, lightValue);

    //right
    _cube[30] = Vertex({1.0f, 1.0f, 0.0f}, {1, 0, 0}, {1, 0}, lightValue);
    _cube[31] = Vertex({1.0f, 1.0f, 1.0f,}, {1, 0, 0}, {0, 0}, lightValue);
    _cube[32] = Vertex({1.0f, 0.0f, 1.0f}, {1, 0, 0}, {0, 1}, lightValue);
    
    _cube[33] = Vertex({1.0f, 1.0f, 0.0f}, {1, 0, 0}, {1, 0}, lightValue);
    _cube[34] = Vertex({1.0f, 0.0f, 1.0f}, {1, 0, 0}, {0, 1}, lightValue);
    _cube[35] = Vertex({1.0f, 0.0f, 0.0f}, {1, 0, 0}, {1, 1}, lightValue);

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

    float lightModify = 0.85;

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
			_vertices[_numVertices + i] = _cube[i];
			_vertices[_numVertices + i].pos.x += x;
			_vertices[_numVertices + i].pos.y += y;
			_vertices[_numVertices + i].pos.z += z;
		    }

		    //Ambient occlusion vertical
		    
		    if(z != 0 && x != 0 && _activeBlocks.at(z-1 + (x-1) * _chunkDepth + y * _chunkDepth * _chunkWidth))
		    {
			_vertices[_numVertices + 25].lightValue *= lightModify;
			_vertices[_numVertices + 26].lightValue *= lightModify;
			_vertices[_numVertices + 28].lightValue *= lightModify;

			_vertices[_numVertices + 6].lightValue *= lightModify;
			_vertices[_numVertices + 9].lightValue *= lightModify;
			_vertices[_numVertices + 11].lightValue *= lightModify;
		    }

		    if(z != 0 && x < _chunkWidth-1 && _activeBlocks.at(z-1 + (x+1) * _chunkDepth + y * _chunkDepth * _chunkWidth))
		    {
			_vertices[_numVertices + 7].lightValue *= lightModify;
			_vertices[_numVertices + 8].lightValue *= lightModify;
			_vertices[_numVertices + 10].lightValue *= lightModify;

			_vertices[_numVertices + 30].lightValue *= lightModify ;
			_vertices[_numVertices + 33].lightValue *= lightModify;
			_vertices[_numVertices + 35].lightValue *= lightModify;
		    }

		    if(z < _chunkDepth - 1 && x != 0 && _activeBlocks.at(z+1 + (x-1) * _chunkDepth + y * _chunkDepth * _chunkWidth))
		    {
			_vertices[_numVertices + 1].lightValue *= lightModify;
			_vertices[_numVertices + 2].lightValue *= lightModify;
			_vertices[_numVertices + 4].lightValue *= lightModify ;

			_vertices[_numVertices + 24].lightValue *= lightModify;
			_vertices[_numVertices + 27].lightValue *= lightModify;
			_vertices[_numVertices + 29].lightValue *= lightModify;
		    }

		    if(z < _chunkDepth - 1 && x < _chunkWidth - 1 && _activeBlocks.at(z+1 + (x+1) * _chunkDepth + y * _chunkDepth * _chunkWidth))
		    {
			_vertices[_numVertices + 3].lightValue *= lightModify;
			_vertices[_numVertices + 0].lightValue *= lightModify;
			_vertices[_numVertices + 5].lightValue *= lightModify;

			_vertices[_numVertices + 31].lightValue *= lightModify ;
			_vertices[_numVertices + 32].lightValue *= lightModify;
			_vertices[_numVertices + 34].lightValue *= lightModify ;
		    }

                    //Ambient occlusion Horizontal Down
		    //Back
		    if(y != 0 && z != 0 && _activeBlocks.at(z-1 + x * _chunkDepth + (y-1) * _chunkDepth * _chunkWidth))
		    {
			_vertices[_numVertices + 8].lightValue *= lightModify;
			_vertices[_numVertices + 10].lightValue *= lightModify;
			_vertices[_numVertices + 11].lightValue *= lightModify;
		    }

		    //Front
		    if(y != 0 && z < _chunkDepth - 1 && _activeBlocks.at(z + 1 + x * _chunkDepth + (y-1) * _chunkDepth * _chunkWidth))
		    {
			_vertices[_numVertices + 2].lightValue *= lightModify;
			_vertices[_numVertices + 4].lightValue *= lightModify;
			_vertices[_numVertices + 5].lightValue *= lightModify;
		    }

		    //Left
		    if(y != 0 && x != 0 && _activeBlocks.at(z + (x-1) * _chunkDepth + (y-1) * _chunkDepth * _chunkWidth))
		    {
			_vertices[_numVertices + 26].lightValue *= lightModify;
			_vertices[_numVertices + 28].lightValue *= lightModify;
			_vertices[_numVertices + 29].lightValue *= lightModify;
		    }

                    //Right
		    if(y != 0 && x < _chunkWidth - 1 && _activeBlocks.at(z + (x+1) * _chunkDepth + (y-1) * _chunkDepth * _chunkWidth))
		    {
			_vertices[_numVertices + 32].lightValue *= lightModify;
			_vertices[_numVertices + 34].lightValue *= lightModify;
			_vertices[_numVertices + 35].lightValue *= lightModify;
		    }

                    //Ambient occlusion Horizontal UP
		    //Back
		    if(y < _chunkHeight - 1 && z != 0 && _activeBlocks.at(z-1 + x * _chunkDepth + (y+1) * _chunkDepth * _chunkWidth))
		    {
			_vertices[_numVertices + 18].lightValue *= lightModify;
			_vertices[_numVertices + 19].lightValue *= lightModify;
			_vertices[_numVertices + 21].lightValue *= lightModify;
		    }

		    //Front
		    if(y < _chunkHeight - 1 && z < _chunkDepth - 1 && _activeBlocks.at(z + 1 + x * _chunkDepth + (y+1) * _chunkDepth * _chunkWidth))
		    {
			_vertices[_numVertices + 20].lightValue *= lightModify;
			_vertices[_numVertices + 22].lightValue *= lightModify;
			_vertices[_numVertices + 23].lightValue *= lightModify;
		    }

                    
		    //Left
		    if(y < _chunkHeight - 1 && x != 0 && _activeBlocks.at(z + (x-1) * _chunkDepth + (y+1) * _chunkDepth * _chunkWidth))
		    {
			_vertices[_numVertices + 19].lightValue *= lightModify;
			_vertices[_numVertices + 20].lightValue *= lightModify;
			_vertices[_numVertices + 22].lightValue *= lightModify;
		    }

                    
		    if(y < _chunkHeight - 1 && x < _chunkWidth - 1 && _activeBlocks.at(z + (x+1) * _chunkDepth + (y+1) * _chunkDepth * _chunkWidth))
		    {
			_vertices[_numVertices + 18].lightValue *= lightModify;
			_vertices[_numVertices + 21].lightValue *= lightModify;
			_vertices[_numVertices + 23].lightValue *= lightModify;
		    }
		    

		    _numVertices += 36;

		}
	    }
	}
    }

    // VBO for vertex data
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, _numVertices * 9 * sizeof(GLfloat), _vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(_program, "inPosition"), 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), 0); 
    glEnableVertexAttribArray(glGetAttribLocation(_program, "inPosition"));

    glVertexAttribPointer(glGetAttribLocation(_program, "inNormal"), 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (const GLfloat*)(3 * sizeof(GLfloat))); 
    glEnableVertexAttribArray(glGetAttribLocation(_program, "inNormal"));
    
    glVertexAttribPointer(glGetAttribLocation(_program, "inTexCoord"), 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat),(const GLfloat*)(6 * sizeof(GLfloat))); 
    glEnableVertexAttribArray(glGetAttribLocation(_program, "inTexCoord"));
    
    glVertexAttribPointer(glGetAttribLocation(_program, "inLightValue"), 1, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat),(const GLfloat*)(8 * sizeof(GLfloat))); 
    glEnableVertexAttribArray(glGetAttribLocation(_program, "inLightValue"));

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
