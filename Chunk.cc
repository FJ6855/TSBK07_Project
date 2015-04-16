#include "Chunk.h"
#include <stdio.h>

Chunk::Chunk(GLuint program)
{   
    _program = program;

    for (int y = 0; y < CHUNK_HEIGHT; y++)
    {
	for (int x = 0; x < CHUNK_WIDTH; x++)
	{
	    for (int z = 0; z < CHUNK_DEPTH; z++)
	    {
	
		_activeBlocks.push_back(true);
		 
	    }
	}
    }

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
   _cube[0] = Vertex({0.5, 0.5, 0.5}, {0, 0, 1}, {1, 1});
   _cube[1] = Vertex({-0.5, 0.5, 0.5}, {0, 0, 1}, {0, 0} );
   _cube[2] = Vertex({-0.5, -0.5, 0.5}, {0, 0, 1}, {0, 1} );
   
   _cube[3] = Vertex({0.5, 0.5, 0.5}, {0, 0, 1}, {1, 1} );
   _cube[4] = Vertex({-0.5, -0.5, 0.5}, {0, 0, 1}, {0, 1} );
   _cube[5] = Vertex({0.5, -0.5, 0.5}, {0, 0, 1}, {1, 0} );
    
//back
    _cube[6] = Vertex({-0.5, 0.5, -0.5}, {0, 0, -1}, {1, 1} );
    _cube[7] = Vertex({0.5, 0.5, -0.5,}, {0, 0, -1}, {0, 0} );
    _cube[8] = Vertex({0.5, -0.5, -0.5}, {0, 0, -1}, {0, 1} );

    _cube[9] = Vertex({-0.5, 0.5, -0.5}, {0, 0, -1}, {1, 1} );
    _cube[10] = Vertex({0.5, -0.5, -0.5}, {0, 0, -1}, {0, 1} );
    _cube[11] = Vertex({-0.5, -0.5, -0.5}, {0, 0, -1}, {1, 0} );
    
//down
    _cube[12] = Vertex({0.5, -0.5, 0.5}, {0, -1, 0}, {1, 1} );
    _cube[13] = Vertex({-0.5, -0.5, 0.5,}, {0, -1, 0}, {0, 0} );
    _cube[14] = Vertex({-0.5, -0.5, -0.5}, {0, -1, 0}, {0, 1} );

    _cube[15] = Vertex({0.5, -0.5, 0.5}, {0, -1, 0}, {1, 1} );
    _cube[16] = Vertex({-0.5, -0.5, -0.5}, {0, -1, 0}, {0, 1} );
    _cube[17] = Vertex({0.5, -0.5, -0.5}, {0, -1, 0}, {1, 0} );

//up
    _cube[18] = Vertex({0.5, 0.5, -0.5}, {0, 1, 0}, {1, 1} );
    _cube[19] = Vertex({-0.5, 0.5, -0.5,}, {0, 1, 0}, {0, 0} );
    _cube[20] = Vertex({-0.5, 0.5, 0.5}, {0, 1, 0}, {0, 1} );
    
    _cube[21] = Vertex({0.5, 0.5, -0.5}, {0, 1, 0}, {1, 1} );
    _cube[22] = Vertex({-0.5, 0.5, 0.5}, {0, 1, 0}, {0, 1} );
    _cube[23] = Vertex({0.5, 0.5, 0.5}, {0, 1, 0}, {1, 0} );

//left
    _cube[24] = Vertex({-0.5, 0.5, 0.5}, {-1, 0, 0}, {1, 1} );
    _cube[25] = Vertex({-0.5, 0.5, -0.5,}, {-1, 0, 0}, {0, 0} );
    _cube[26] = Vertex({-0.5, -0.5, -0.5}, {-1, 0, 0}, {0, 1} );
    
    _cube[27] = Vertex({-0.5, 0.5, 0.5}, {-1, 0, 0}, {1, 1} );
    _cube[28] = Vertex({-0.5, -0.5, -0.5}, {-1, 0, 0}, {0, 1} );
    _cube[29] = Vertex({-0.5, -0.5, 0.5}, {-1, 0, 0}, {1, 0} );

//right
    _cube[30] = Vertex({0.5, 0.5, -0.5}, {1, 0, 0}, {1, 1} );
    _cube[31] = Vertex({0.5, 0.5, 0.5,}, {1, 0, 0}, {0, 0} );
    _cube[32] = Vertex({0.5, -0.5, 0.5}, {1, 0, 0}, {0, 1} );
    
    _cube[33] = Vertex({0.5, 0.5, -0.5}, {1, 0, 0}, {1, 1} );
    _cube[34] = Vertex({0.5, -0.5, 0.5}, {1, 0, 0}, {0, 1} );
    _cube[35] = Vertex({0.5, -0.5, -0.5}, {1, 0, 0}, {1, 0} );

    _generateChunk();
}

/*Chunk::Chunk(TextureData* heightMap)
{

}*/

Chunk::~Chunk()
{

}

void Chunk::_generateChunk()
{
 
    Vertex _vertices[(CHUNK_HEIGHT * CHUNK_WIDTH * CHUNK_DEPTH) * 36];
   
    _numVertices = 0;
    
    glGenBuffers(1, &_vbo);
    glBindVertexArray(_vao);

    for (int y = 0; y < CHUNK_HEIGHT; y++)
    {
	for (int x = 0; x < CHUNK_WIDTH; x++)
	{
	    for (int z = 0; z < CHUNK_DEPTH; z++)
	    {
		if (_activeBlocks.at(z + x * CHUNK_DEPTH + y * CHUNK_DEPTH * CHUNK_WIDTH))
		{
		    for(int i = 0; i < 36 ; i++)
		    {
       
			_vertices[(z + x * CHUNK_DEPTH + y * CHUNK_DEPTH * CHUNK_WIDTH)*36 + i] = _cube[i];
			_vertices[(z + x * CHUNK_DEPTH + y * CHUNK_DEPTH * CHUNK_WIDTH)*36 + i].pos.x += x;
			_vertices[(z + x * CHUNK_DEPTH + y * CHUNK_DEPTH * CHUNK_WIDTH)*36 + i].pos.y += y;
			_vertices[(z + x * CHUNK_DEPTH + y * CHUNK_DEPTH * CHUNK_WIDTH)*36 + i].pos.z += z;
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
