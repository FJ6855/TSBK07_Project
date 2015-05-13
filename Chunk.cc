#include "Chunk.h"
#include <stdio.h>
#include <fstream>
#include <iostream>

Chunk::Chunk(int chunkId, GLuint program, TextureData* heightmap, int chunkWidth, int chunkHeight, int chunkDepth,  int heightmapX, int heightmapZ)
{   
    _chunkId = chunkId;

    _program = program;

    _chunkWidth = chunkWidth;
    _chunkHeight = chunkHeight;
    _chunkDepth = chunkDepth;

    _activeBlocks.resize(chunkWidth * chunkHeight * chunkDepth);
    
    _setHeightmap(heightmap, heightmapX, heightmapZ);
    //_setFull();
    _numVertices = 0;
    
    glGenVertexArrays(1, &_vao);
}

Chunk::Chunk(int chunkId, GLuint program, int chunkWidth, int chunkHeight, int chunkDepth)
{   
    _chunkId = chunkId;

    _program = program;

    _chunkWidth = chunkWidth;
    _chunkHeight = chunkHeight;
    _chunkDepth = chunkDepth;

    _activeBlocks.resize(chunkWidth * chunkHeight * chunkDepth);
    
    _setFull();
    
    _numVertices = 0;
    
    glGenVertexArrays(1, &_vao);
}

Chunk::~Chunk()
{
    glDeleteBuffers(1, &_vbo);
    glDeleteVertexArrays(1, &_vao);
    //printf("delete chunk: %i\n", _chunkId);
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
				
		if (y == _chunkHeight - 1 && x == 0 && z == 0)
		    _activeBlocks.at(index) = 0;

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
				
		if (y <= heightmap->imageData[(x + z * heightmap->width) * (heightmap->bpp / 8)])
		{
		  if(y < 5)
		    _activeBlocks.at(index) = 1;
		  else
		    _activeBlocks.at(index) = 2;
		}		 
		else
		{
		    _activeBlocks.at(index) = 0;
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
		    _activeBlocks.at(index) = 1;
		}		 
		else
		{
		    _activeBlocks.at(index) = 0;
		}
	    }
	}
    }
}

void Chunk::generateChunk()
{
    Vertex _vertices[(_chunkHeight * _chunkWidth * _chunkDepth) * 36];
   
    _numVertices = 0;
    
    float lightModify = 0.85;

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
			_vertices[_numVertices + i] = cube[i];
			_vertices[_numVertices + i].pos.x += x;
			_vertices[_numVertices + i].pos.y += y;
			_vertices[_numVertices + i].pos.z += z;
			
			
			_vertices[_numVertices + i].texCoord.x += (_activeBlocks.at(z + x * _chunkDepth + y * _chunkDepth * _chunkWidth) - 1) * 0.5;
			_vertices[_numVertices + i].texCoord.y -= (_activeBlocks.at(z + x * _chunkDepth + y * _chunkDepth * _chunkWidth) - 1) * 0.5;
						
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

void Chunk::saveChunk()
{
    std::ofstream file;

    file.open("chunks.txt", std::ios::app);

    if (file.is_open())
    {	
	if (_chunkId < 10)
	    file << "   ";
	else if (_chunkId < 100)
	    file << "  ";
	else if (_chunkId < 1000)
	    file << " ";

	file << _chunkId;

	file << ":";
	
	for (int i = 0; i < _activeBlocks.size(); i++)
	{
	    file << (int)_activeBlocks.at(i);
	}
	
	file.close();
    }
}

void Chunk::loadChunk()
{
    std::ifstream file;

    file.open("chunks.txt");

    if (file.is_open())
    {	
	file.seekg(_chunkId * _chunkWidth * _chunkHeight * _chunkDepth + 5 * _chunkId + 5, std::ios_base::beg);  

	char bit;
       
	for (int i = 0; i < _activeBlocks.size(); i++)
	{
	    file.get(bit);

	    if (bit == '1')
		_activeBlocks.at(i) = true;
	    else
		_activeBlocks.at(i) = false;    
	}

	file.close();
    }
}
