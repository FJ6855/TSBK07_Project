#include "Renderer.h"
#include <stdio.h>
#include <algorithm>

Renderer::Renderer(Logic* logic)
{
    _logic = logic;

    _viewMatrix = lookAtv(_logic->getCameraPos(), _logic->getCameraLookAt(), _logic->getCameraUp());

    _projectionMatrix = perspective(fovDegree, aspectRatio, near, far);

    glClearColor(1.0, 1.0, 1.0, 0);
    glEnable(GL_DEPTH_TEST);

    _shader = loadShaders("shader.vert", "shader.frag");

    _skyboxShader = loadShaders("skyboxShader.vert", "skyboxShader.frag");

    _ballShader = loadShaders("ballShader.vert", "ballShader.frag");
       
    printError("load shaders");

    //skybox
    
    glUseProgram(_skyboxShader);

    _skyboxModel = LoadModelPlus("skybox.obj");

    glGenBuffers(1, &_skyboxTexture); 
    
    glActiveTexture(GL_TEXTURE0);
 
    glBindTexture(GL_TEXTURE_2D, _skyboxTexture);

    glUniform1i(glGetUniformLocation(_skyboxShader, "texUnit"), 0);

    LoadTGATextureSimple("skybox.tga", &_skyboxTexture);

    //stones
    glUseProgram(_shader);
    
    glGenBuffers(1, &stoneTexture);

    glActiveTexture(GL_TEXTURE1);

    glBindTexture(GL_TEXTURE_2D, stoneTexture);

    LoadTGATextureSimple("poop.tga", &stoneTexture);

    glUniform1i(glGetUniformLocation(_shader, "tex"), 1);
    
    //balls

    _ballModel = LoadModelPlus("sphere.obj");

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

    glUseProgram(_ballShader);
    glUniformMatrix4fv(glGetUniformLocation(_ballShader, "projectionMatrix"), 1, GL_TRUE, _projectionMatrix.m);
    
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
 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glUniformMatrix4fv(glGetUniformLocation(_skyboxShader, "viewMatrix"), 1, GL_TRUE, _viewMatrix.m);
    
    _modelMatrix = T(_logic->getCameraPos().x, _logic->getCameraPos().y + 0.05 , _logic->getCameraPos().z);

    glUniformMatrix4fv(glGetUniformLocation(_skyboxShader, "modelMatrix"), 1, GL_TRUE, _modelMatrix.m);

    DrawModel(_skyboxModel, _skyboxShader, "in_Position", NULL, "in_Tex_Coord");

    //World
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);

    glUseProgram(_shader);

    glActiveTexture(GL_TEXTURE1);

    glBindTexture(GL_TEXTURE_2D, stoneTexture);

    glUniformMatrix4fv(glGetUniformLocation(_shader, "viewMatrix"), 1, GL_TRUE, _viewMatrix.m);
    
    vec3 playerPos =_logic->getPlayer()->getPosition();
    int playerX = playerPos.x / 16;
    int playerZ = playerPos.z / 16;

    vec3 PointLightColor = vec3(1.0, 0.0, 0.0);
    vec3 PointLightPos = _logic->getPlayer()->getPosition();
    PointLightPos.y += 1.5;

    glUniform3fv(glGetUniformLocation(_shader, "PointLightPos"), 1, &PointLightPos.x);
    glUniform3fv(glGetUniformLocation(_shader, "PointLightColor"), 1, &PointLightColor.x);
    
    int chunkCount = 0;
    int loopCount = 0;

    //printf("render chunks\n");
    //printf("from z: %f to z: %f\n", _logic->getWorld()->loadPosition.z, _logic->getWorld()->loadPosition.z + LOAD_DEPTH * CHUNK_DEPTH);

    /*for (int z = _logic->getWorld()->loadPosition.z; z < _logic->getWorld()->loadPosition.z + LOAD_DEPTH * CHUNK_DEPTH; z += CHUNK_DEPTH)
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
		
		chunkCount++;
	    }

	    loopCount++;
	}
	}*/

    for (int z = _logic->getWorld()->minZ; z <= _logic->getWorld()->maxZ; z += CHUNK_DEPTH)
    {
	for (int x = _logic->getWorld()->minX; x <= _logic->getWorld()->maxX; x += CHUNK_WIDTH)
	{	    
	    Chunk* chunk = _logic->getWorld()->chunks->getChunk(vec3(x, 0, z));

	    if (chunk != NULL)
	    {
		_modelMatrix = T(chunk->getPos().x, chunk->getPos().y, chunk->getPos().z);
		
		glUniformMatrix4fv(glGetUniformLocation(_shader, "modelMatrix"), 1, GL_TRUE, _modelMatrix.m);
		
		glBindVertexArray(chunk->getVao());
		
		glDrawArrays(GL_TRIANGLES, 0, chunk->getNumVertices());
		
		chunkCount++;
	    }

	    loopCount++;
	}
    }

    printf("render chunk count: %i\n", chunkCount);
    printf("render count: %i\n", loopCount);

    //balls
    glUseProgram(_ballShader);
    
    glUniform3fv(glGetUniformLocation(_ballShader, "PointLightPos"), 1, &PointLightPos.x);
    glUniform3fv(glGetUniformLocation(_ballShader, "PointLightColor"), 1, &PointLightColor.x);
 
    glUniformMatrix4fv(glGetUniformLocation(_ballShader, "viewMatrix"), 1, GL_TRUE, _viewMatrix.m);
    for(int i = 0; i < 15; i++)
    {
	if(_logic->getBall(i) != NULL)
	{
	    _modelMatrix = T(_logic->getBall(i)->getPosition().x, _logic->getBall(i)->getPosition().y, _logic->getBall(i)->getPosition().z);
	
	    glUniformMatrix4fv(glGetUniformLocation(_ballShader, "modelMatrix"), 1, GL_TRUE, _modelMatrix.m);
    
	    DrawModel(_ballModel, _ballShader, "inPosition", "inNormal", NULL);
	}
    }
    glutSwapBuffers();
}

GLuint Renderer::getProgram()
{
    return _shader;
}
