#ifndef RENDERER_H
#define RENDERER_H

#include "GL/gl.h"
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "LoadObj.h"
#include "MathUtils.h"
#include "Logic.h"

class Renderer
{
public:
    Renderer(Logic* logic);
    ~Renderer();

    void render();

    GLuint getProgram();
    
private:
    void _initSystems();

    mat4 _modelMatrix;
    mat4 _viewMatrix;
    mat4 _projectionMatrix;
    
    GLuint _shader;

    Logic* _logic;

    GLuint stoneTexture;

    Model* _skyboxModel;
    
    GLuint  _skyboxShader;
    
    GLuint _skyboxTexture;
};

#endif
