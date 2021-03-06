#include "Renderer.h"
#include <stdio.h>
#include <algorithm>

Renderer::Renderer(Logic* logic)
{
    _logic = logic;

    _viewMatrix = lookAtv(_logic->getCameraPos(), _logic->getCameraLookAt(), _logic->getCameraUp());

    _projectionMatrix = perspective(fovDegree, aspectRatio, near, far);

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glEnable(GL_DEPTH_TEST);

    _shader = loadShaders("shader.vert", "shader.frag");

    _skyboxShader = loadShaders("skyboxShader.vert", "skyboxShader.frag");

    _ballShader = loadShaders("ballShader.vert", "ballShader.frag");

    _plainShader = loadShaders("plainShader.vert", "plainShader.frag");
       
    printError("load shaders");

    //skybox
    
    glUseProgram(_skyboxShader);

    _skyboxModel = LoadModelPlus("skybox.obj");

    glGenBuffers(1, &_skyboxTexture); 
    
    glActiveTexture(GL_TEXTURE0);
 
    glBindTexture(GL_TEXTURE_2D, _skyboxTexture);

    glUniform1i(glGetUniformLocation(_skyboxShader, "texUnit"), 0); 

    LoadTGATextureSimple("skyboxFog.tga", &_skyboxTexture);

    //stones
    glUseProgram(_shader);
    
    glGenBuffers(1, &stoneTexture);

    glActiveTexture(GL_TEXTURE1);

    glBindTexture(GL_TEXTURE_2D, stoneTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    LoadTGATextureSimple("textures.tga", &stoneTexture);

    glUniform1i(glGetUniformLocation(_shader, "tex"), 1);
    
    //balls

    _ballModel = LoadModelPlus("sphere.obj");

    // cube for targeting

    _cubeModel = LoadModelPlus("cubeplus.obj");

    glUseProgram(_plainShader);

    glGenVertexArrays(1, &_cubeVao);
    glGenBuffers(1, &_cubeVbo);

    glBindVertexArray(_cubeVao);
    glBindBuffer(GL_ARRAY_BUFFER, _cubeVbo);
    glBufferData(GL_ARRAY_BUFFER, 24 * 3 * sizeof(GLfloat), cubeFrame, GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(_plainShader, "inPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
    glEnableVertexAttribArray(glGetAttribLocation(_plainShader, "inPosition"));

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

    glUseProgram(_plainShader);
    glUniformMatrix4fv(glGetUniformLocation(_plainShader, "projectionMatrix"), 1, GL_TRUE, _projectionMatrix.m);   
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

    vec3 PointLightColor = vec3(1.0, 1.0, 1.0);
    vec3 PointLightPos = _logic->getPlayer()->getPosition();

    glUniform3fv(glGetUniformLocation(_shader, "PointLightPos"), 1, &PointLightPos.x);
    glUniform3fv(glGetUniformLocation(_shader, "PointLightColor"), 1, &PointLightColor.x);

    vec3 cameraPos = _logic->getCameraPos();
    
    glUniform3fv(glGetUniformLocation(_shader, "CameraPos"), 1, &cameraPos.x);
    
    int chunkCount = 0;
    int loopCount = 0;

    for (int i = 0; i < _logic->getWorld()->renderList.size(); ++i)
    {	    
	Chunk* chunk = _logic->getWorld()->chunks->getChunk(_logic->getWorld()->renderList.at(i));
	
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

    //printf("render chunk count: %i\n", chunkCount);
    //printf("render count: %i\n", loopCount);

    //balls
    glUseProgram(_ballShader);

    glUniform3fv(glGetUniformLocation(_ballShader, "cameraPos"), 1, &cameraPos.x);
 
    glUniformMatrix4fv(glGetUniformLocation(_ballShader, "viewMatrix"), 1, GL_TRUE, _viewMatrix.m);
    
    for(int i = 0; i < 100; i++)
    {
	if(_logic->getBall(i) != NULL)
	{
	    _modelMatrix = T(_logic->getBall(i)->getPosition().x, _logic->getBall(i)->getPosition().y, _logic->getBall(i)->getPosition().z);

	    glUniformMatrix4fv(glGetUniformLocation(_ballShader, "modelMatrix"), 1, GL_TRUE, _modelMatrix.m);
    
	    DrawModel(_ballModel, _ballShader, "inPosition", "inNormal", NULL);   
	}
    }

    // cube for targeting
    glUseProgram(_plainShader);

    glLineWidth(4.0f);

    glUniformMatrix4fv(glGetUniformLocation(_plainShader, "viewMatrix"), 1, GL_TRUE, _viewMatrix.m);

    if (_logic->getRenderTarget())
    {    
	_modelMatrix = Mult(T(_logic->getTargetPosition().x,  _logic->getTargetPosition().y, _logic->getTargetPosition().z), S(1.005f, 1.001f, 1.001f));
		
	glUniformMatrix4fv(glGetUniformLocation(_plainShader, "modelMatrix"), 1, GL_TRUE, _modelMatrix.m);
	
	glBindVertexArray(_cubeVao);

	glDrawArrays(GL_LINES, 0, 24);
    }

    glutSwapBuffers();
}

GLuint Renderer::getProgram()
{
    return _shader;
}
