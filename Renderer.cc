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
    glUseProgram(_shader);
    printError("load shaders");

    //_cube = LoadModelPlus("cubeplus.obj");
    
    printError("load model");
    
    _initSystems();
}

Renderer::~Renderer()
{

}

void Renderer::_initSystems() 
{      
    glUniformMatrix4fv(glGetUniformLocation(_shader, "projectionMatrix"), 1, GL_TRUE, _projectionMatrix.m);
}

void Renderer::render()
{ 
    _viewMatrix = lookAtv(_logic->getCameraPos(), _logic->getCameraLookAt(), _logic->getCameraUp());

    glUniformMatrix4fv(glGetUniformLocation(_shader, "viewMatrix"), 1, GL_TRUE, _viewMatrix.m);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(_shader);

    //DrawModel(_cube, _shader, "inPosition", "inNormal", NULL);

    for (int i = 0; i < _logic->getWorld()->chunks.size(); i++)
    {
	Chunk* chunk = _logic->getWorld()->chunks.at(i);

	_modelMatrix = T(chunk->getPos().x, chunk->getPos().y, chunk->getPos().z);
	
	glUniformMatrix4fv(glGetUniformLocation(_shader, "modelMatrix"), 1, GL_TRUE, _modelMatrix.m);

	glBindVertexArray(chunk->getVao());
	
	glDrawArrays(GL_TRIANGLES, 0, chunk->getNumVertices());
    }

    glutSwapBuffers();
}

GLuint Renderer::getProgram()
{
    return _shader;
}
