#include "Renderer.h"
#include <stdio.h>

Renderer::Renderer(Logic* logic)
{
    _logic = logic;

    _viewMatrix = lookAtv(_logic->getCameraPos(), _logic->getCameraLookAt(), _logic->getCameraUp());

    _projectionMatrix = perspective(fovDegree, aspectRatio, near, far);

    glClearColor(1.0, 1.0, 1.0, 0);
    glEnable(GL_DEPTH_TEST);

    _shader = loadShaders("shader.vert", "shader.frag");

    _skyboxShader = loadShaders("skyboxShader.vert", "skyboxShader.frag");

    printError("load shaders");

    //skybox
    
    glUseProgram(_skyboxShader);

    _skyboxModel = LoadModelPlus("skybox.obj");

    glGenBuffers(1, &_skyboxTexture); 
    
    glActiveTexture(GL_TEXTURE0);
 
    glBindTexture(GL_TEXTURE_2D, _skyboxTexture);
    /*
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    */
    glUniform1i(glGetUniformLocation(_skyboxShader, "texUnit"), 0);

    LoadTGATextureSimple("skybox.tga", &_skyboxTexture);

    //stones
    glUseProgram(_shader);
    
    glGenBuffers(1, &stoneTexture);

    glActiveTexture(GL_TEXTURE1);

    glBindTexture(GL_TEXTURE_2D, stoneTexture);

    LoadTGATextureSimple("stone.tga", &stoneTexture);

    glUniform1i(glGetUniformLocation(_shader, "tex"), 1);
    
    printError("load model");
    
    _initSystems();
}

Renderer::~Renderer()
{

}

void Renderer::_initSystems() 
{   
    glUseProgram(_shader);
    glUniformMatrix4fv(glGetUniformLocation(_shader, "projectionMatrix"), 1, GL_TRUE, _projectionMatrix.m);

    glUseProgram(_skyboxShader);
    glUniformMatrix4fv(glGetUniformLocation(_skyboxShader, "projectionMatrix"), 1, GL_TRUE, _projectionMatrix.m);
}

void Renderer::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _viewMatrix = lookAtv(_logic->getCameraPos(), _logic->getCameraLookAt(), _logic->getCameraUp());
    
    //skybox
    glUseProgram(_skyboxShader);

    glDisable(GL_DEPTH_TEST);
 
    glDisable(GL_CULL_FACE);

    glActiveTexture(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_2D, _skyboxTexture);
  
    glUniformMatrix4fv(glGetUniformLocation(_skyboxShader, "viewMatrix"), 1, GL_TRUE, _viewMatrix.m);
    
    _modelMatrix = T(_logic->getCameraPos().x, _logic->getCameraPos().y+0.05 , _logic->getCameraPos().z);

    glUniformMatrix4fv(glGetUniformLocation(_skyboxShader, "modelMatrix"), 1, GL_TRUE, _modelMatrix.m);

    DrawModel(_skyboxModel, _skyboxShader, "in_Position", NULL, "in_Tex_Coord");

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);

    glUseProgram(_shader);

    glActiveTexture(GL_TEXTURE1);

    glBindTexture(GL_TEXTURE_2D, stoneTexture);

    glUniformMatrix4fv(glGetUniformLocation(_shader, "viewMatrix"), 1, GL_TRUE, _viewMatrix.m);
    
    /*_modelMatrix = T(_logic->getPlayer()->getPosition().x, _logic->getPlayer()->getPosition().y, _logic->getPlayer()->getPosition().z);

    glUniformMatrix4fv(glGetUniformLocation(_shader, "modelMatrix"), 1, GL_TRUE, _modelMatrix.m);

    DrawModel(_cube, _shader, "inPosition", "inNormal", NULL);*/
    
    int count = 0;
    
    //printf("render chunks\n");

    for (int z = _logic->getWorld()->loadPosition.z; z < _logic->getWorld()->loadPosition.z + LOAD_DEPTH * CHUNK_DEPTH; z += CHUNK_DEPTH)
    {
	for (int x = _logic->getWorld()->loadPosition.x; x < _logic->getWorld()->loadPosition.x + LOAD_WIDTH * CHUNK_WIDTH; x += CHUNK_WIDTH)
	{	    
	    Chunk* chunk = _logic->getWorld()->chunks->getChunk(vec3(x, 0, z));

	    if (chunk != NULL)
	    {
		_modelMatrix = T(chunk->getPos().x, chunk->getPos().y, chunk->getPos().z);
		
		glUniformMatrix4fv(glGetUniformLocation(_shader, "modelMatrix"), 1, GL_TRUE, _modelMatrix.m);
		
		glBindVertexArray(chunk->getVao());
		
		glDrawArrays(GL_TRIANGLES, 0, chunk->getNumVertices());
		
		count++;
	    }
	}
	}
    /*
    for (int z = _logic->getWorld()->minZ; z < _logic->getWorld()->maxZ + CHUNK_DEPTH; z += CHUNK_DEPTH)
    {
	for (int x = _logic->getWorld()->minX; x < _logic->getWorld()->maxX + CHUNK_WIDTH;  x += CHUNK_WIDTH)
	{	    
	    Chunk* chunk = _logic->getWorld()->chunks->getChunk(vec3(x, 0, z));
	    
	    if (chunk != NULL)
	    {
		_modelMatrix = T(chunk->getPos().x, chunk->getPos().y, chunk->getPos().z);
		
		glUniformMatrix4fv(glGetUniformLocation(_shader, "modelMatrix"), 1, GL_TRUE, _modelMatrix.m);
		
		glBindVertexArray(chunk->getVao());
		
		glDrawArrays(GL_TRIANGLES, 0, chunk->getNumVertices());
		
		count++;
	    }
	}
	}*/

    glutSwapBuffers();
}

GLuint Renderer::getProgram()
{
    return _shader;
}
