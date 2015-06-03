#include "Chunk.h"
#include <ctime>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <math.h>

Chunk::Chunk(GLuint program, TextureData* heightmap, int chunkWidth, int chunkHeight, int chunkDepth,  int x, int z)
{   
    _chunkId = x / chunkWidth + z / chunkDepth * heightmap->width;
    
    _program = program;

    _chunkWidth = chunkWidth;
    _chunkHeight = chunkHeight;
    _chunkDepth = chunkDepth;

    _pos = vec3(x, 0, z);

    _activeBlocks.resize(chunkWidth * chunkHeight * chunkDepth);

    _setHeightmap(heightmap, x, z);

    _numVertices = 0;

    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);  
}

Chunk::Chunk(GLuint program, int chunkWidth, int chunkHeight, int chunkDepth, int x, int z, float heightmapWidth)
{   
    _chunkId = x / chunkWidth + z / chunkDepth * heightmapWidth;

    _program = program;

    _chunkWidth = chunkWidth;
    _chunkHeight = chunkHeight;
    _chunkDepth = chunkDepth;

    _pos = vec3(x, 0, z);

    _activeBlocks.resize(chunkWidth * chunkHeight * chunkDepth);
    
    _numVertices = 0;
    
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);  
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

bool Chunk::_blockIsSurrounded(int x, int y, int z)
{
    if(x == 0 || x == _chunkWidth-1 || y == 0 || y == _chunkHeight-1 || z == 0 || z == _chunkDepth-1)
    {
	return false;
    }

    //Over
    int index = z + x * _chunkDepth + (y + 1) * _chunkDepth * _chunkWidth;
    if(!_activeBlocks.at(index))
	return false;

    //Under
    index = z + x * _chunkDepth + (y - 1) * _chunkDepth * _chunkWidth;
    if(!_activeBlocks.at(index))
	return false;

    //Right
    index = z + (x + 1) * _chunkDepth + y * _chunkDepth * _chunkWidth;
    if(!_activeBlocks.at(index))
	return false; 
    
    //Left
    index = z + (x - 1) * _chunkDepth + y * _chunkDepth * _chunkWidth;
    if(!_activeBlocks.at(index))
	return false;
    
    //Forward
    index = (z - 1) + x * _chunkDepth + y * _chunkDepth * _chunkWidth;
    if(!_activeBlocks.at(index))
	return false;
    
    //Under
    index = (z + 1) + x * _chunkDepth + y * _chunkDepth * _chunkWidth;
    if(!_activeBlocks.at(index))
	return false;	
    
    return true;
}

void Chunk::generateChunk()
{
    Vertex* _vertices = new Vertex[(_chunkHeight * _chunkWidth * _chunkDepth) * 36];
    
    _numVertices = 0;
    
    float lightModify = 0.85;

    glBindVertexArray(_vao);

    for (int y = 0; y < _chunkHeight; y++)
    {
	for (int x = 0; x < _chunkWidth; x++)
	{
	    for (int z = 0; z < _chunkDepth; z++)
	    {		
		int index = z + x * _chunkDepth + y * _chunkDepth * _chunkWidth;
		
		if (_activeBlocks.at(index) && !_blockIsSurrounded(x, y, z))
		{
		    for(int i = 0; i < 36 ; i++)
		    {
			_vertices[_numVertices + i] = cube[i];
			_vertices[_numVertices + i].pos.x += x;
			_vertices[_numVertices + i].pos.y += y;
			_vertices[_numVertices + i].pos.z += z;
			
			_vertices[_numVertices + i].texCoord.x += (_activeBlocks.at(index) - 1) * 0.5;
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

    delete _vertices;
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

int Chunk::isBlockActive(int index)
{
    if (index >= 0 && index < _activeBlocks.size())
	return _activeBlocks.at(index);
    else
	return 0;
}

int Chunk::isBlockActive(vec3 blockPos)
{
    int index = blockPos.z + blockPos.x * _chunkDepth + blockPos.y * _chunkDepth * _chunkWidth;

    if (index >= 0 && index < _activeBlocks.size())
	return _activeBlocks.at(index);
    else
	return 0;
}

bool Chunk::setBlock(vec3 pos, int blockType)
{
    int index = pos.z + pos.x * _chunkDepth + pos.y * _chunkDepth * _chunkWidth;

    // check if index is valid, return false otherwise so we don't regenerate the chunk if we don't have to
    if (index >= 0 && index < _activeBlocks.size())
    {
	if (blockType != 0 && !_activeBlocks.at(index) && _activeBlocks.at(index) != blockType)
	{
	    _activeBlocks.at(index) = blockType;

	    return true;
	}
	else if (blockType == 0 && _activeBlocks.at(index) != blockType)
	{
	    _activeBlocks.at(index) = blockType;

	    return true;
	}
	else
	{
	    return false;
	}
    }
    else
    {
	return false;
    }
}

bool Chunk::checkCollision(vec3 min, vec3 max)
{
    vec3 relativePos = min - _pos;

    relativePos.x = floor(relativePos.x);
    relativePos.y = floor(relativePos.y);
    relativePos.z = floor(relativePos.z);
    
    if (relativePos.x < 1.0)
	relativePos.x = 1.0;
    else if (relativePos.x > 14.0)
	relativePos.x = 14.0;
    
    if (relativePos.y < 1.0)
	relativePos.y = 1.0;
    else if (relativePos.y > 13.0)
	relativePos.y = 13.0;

    if (relativePos.z < 1.0)
	relativePos.z = 1.0;
    else if (relativePos.z > 14.0)
	relativePos.z = 14.0;

    for (int y = relativePos.y - 1; y <= relativePos.y + 2; y++)
    {
	for (int x = relativePos.x - 1; x <= relativePos.x + 1; x++)
	{
	    for (int z = relativePos.z - 1; z <= relativePos.z + 1; z++)
	    {		
		if (_activeBlocks.at(z + x * _chunkDepth + y * _chunkDepth * _chunkWidth) != 0)
		{
		    vec3 bMin = vec3(x + _pos.x, y + _pos.y, z + _pos.z);
		    vec3 bMax = vec3(bMin.x + 1, bMin.y + 1, bMin.z + 1);
		    
		    if (max.x > bMin.x && min.x < bMax.x &&
			max.y > bMin.y && min.y < bMax.y &&
			max.z > bMin.z && min.z < bMax.z)
		    {
			return true;
		    }
		}
	    }
	}
    }	
}

bool Chunk::checkCollision(vec3 pos, float radius)
{
    vec3 relativeBallPos = pos - _pos;

    relativeBallPos.x = floor(relativeBallPos.x);
    relativeBallPos.y = floor(relativeBallPos.y);
    relativeBallPos.z = floor(relativeBallPos.z);
    
    if (relativeBallPos.x < 1.0)
	relativeBallPos.x = 1.0;
    else if (relativeBallPos.x > 14.0)
	relativeBallPos.x = 14.0;
    
    if (relativeBallPos.y < 1.0)
	relativeBallPos.y = 1.0;
    else if (relativeBallPos.y > 14.0)
	relativeBallPos.y = 14.0;

    if (relativeBallPos.z < 1.0)
	relativeBallPos.z = 1.0;
    else if (relativeBallPos.z > 14.0)
	relativeBallPos.z = 14.0;

    for (int y = relativeBallPos.y - 1; y <= relativeBallPos.y + 1; y++)
    {
	for (int x = relativeBallPos.x - 1 ; x <= relativeBallPos.x + 1; x++)
	{
	    for (int z = relativeBallPos.z - 1 ; z <= relativeBallPos.z + 1; z++)
	    {		
		if (_activeBlocks.at(z + x * _chunkDepth + y * _chunkDepth * _chunkWidth))
		{
		    vec3 min = vec3(x + _pos.x, y + _pos.y, z + _pos.z);

		    vec3 max = vec3(min.x + 1, min.y + 1, min.z + 1);

		    float distSquared = 0;

		    if (pos.x < min.x) 
			distSquared += pow(pos.x - min.x, 2);
		    else if (pos.x > max.x) 
			distSquared += pow(pos.x - max.x, 2);

		    if (pos.y < min.y) 
			distSquared += pow(pos.y - min.y, 2);
		    else if (pos.y > max.y) 
			distSquared += pow(pos.y - max.y, 2);
		    
		    if (pos.z < min.z) 
			distSquared += pow(pos.z - min.z, 2);
		    else if (pos.z > max.z) 
			distSquared += pow(pos.z - max.z, 2);

		    if (distSquared <= radius * radius)
			return true;
		}
	    }
	}
    }

    return false;
}

void Chunk::saveChunk(std::fstream& file, bool overwrite)
{
    //std::fstream file;

    //file.open("chunks.txt", std::ios::in | std::ios::out);

    if (file.is_open())
    {
	if (overwrite)
	{	   
	    file.seekg(0, std::ios_base::beg);
 
	    int id = -1;
	    char shit = '0';
	    
	    while (file.get(shit))
	    {    
		if (shit == ' ')
		{
		    if (file >> id)
		    {
			if (id == _chunkId)
			{
			//printf("found id!\n");
			    break;
			}
		    }
		}
		
		file.seekg(_chunkWidth * _chunkHeight * _chunkDepth + 1, std::ios_base::cur);	
	    }

	    file.seekp(file.tellg(), std::ios_base::beg);
	}
	else
	{
	    file.seekp(0, std::ios_base::end);
	    
	    if (_chunkId < 10)
		file << "      ";
	    else if (_chunkId < 100)
		file << "     ";
	    else if (_chunkId < 1000)
		file << "    ";
	    else if (_chunkId < 10000)
		file << "   ";
	    else if (_chunkId < 100000)
		file << "  ";
	    else if (_chunkId < 1000000)
		file << " ";

	    file << _chunkId;
	}

	file << ":";
	
	for (int i = 0; i < _activeBlocks.size(); i++)
	{
	    file << (int)_activeBlocks.at(i);
	}
	
//	file.close();
    }
}

void Chunk::loadChunk(std::fstream& file)
{
    //std::ifstream file;

    //file.open("chunks.txt");

    if (file.is_open())
    {	
	int id = -1;
	char shit = '0';

	file.seekg(0, std::ios_base::beg);

	while (file.get(shit))
	{      	    
	    if (shit == ' ')
	    {
		if (file >> id)
		{
		    if (id == _chunkId)
		    {
			//printf("found id!\n");
			break;
		    }
		}
	    }

	    file.seekg(_chunkWidth * _chunkHeight * _chunkDepth + 1, std::ios_base::cur);
	}

	char blockType;

	file.get(shit);
	
	for (int i = 0; i < _activeBlocks.size(); i++)
	{
	    file.get(blockType);

	    if (blockType == '0')
		_activeBlocks.at(i) = 0;
	    else if (blockType == '1')
		_activeBlocks.at(i) = 1;
	    else if (blockType == '2')
		_activeBlocks.at(i) = 2;
	}
    }
}
