#ifndef RENDERER_H
#define RENDERER_H

#include "GL/gl.h"
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "LoadObj.h"
#include "MathUtils.h"
#include "Logic.h"

static vec3 cubeFrame[24] =
{
    vec3(0.0f, 0.0f, 0.0f),
    vec3(0.0f, 1.0f, 0.0f),

    vec3(0.0f, 1.0f, 0.0f),
    vec3(1.0f, 1.0f, 0.0f),

    vec3(1.0f, 1.0f, 0.0f),
    vec3(1.0f, 0.0f, 0.0f),

    vec3(1.0f, 0.0f, 0.0f),
    vec3(0.0f, 0.0f, 0.0f),

    vec3(0.0f, 0.0f, 0.0f),
    vec3(0.0f, 0.0f, 1.0f),

    vec3(0.0f, 0.0f, 1.0f),
    vec3(0.0f, 1.0f, 1.0f),

    vec3(0.0f, 1.0f, 1.0f),
    vec3(0.0f, 1.0f, 0.0f),

    vec3(0.0f, 1.0f, 1.0f),
    vec3(1.0f, 1.0f, 1.0f),

    vec3(1.0f, 1.0f, 1.0f),
    vec3(1.0f, 0.0f, 1.0f),

    vec3(1.0f, 0.0f, 1.0f),
    vec3(0.0f, 0.0f, 1.0f),

    vec3(1.0f, 0.0f, 1.0f),
    vec3(1.0f, 0.0f, 0.0f),

    vec3(1.0f, 1.0f, 1.0f),
    vec3(1.0f, 1.0f, 0.0f),
};

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

    GLuint _dirtTexture;

    Model* _skyboxModel;
    
    GLuint  _skyboxShader;
    
    GLuint _skyboxTexture;
    
    Model* _ballModel;
    
    GLuint _ballShader;

    Model* _cubeModel;

    GLuint _plainShader;

    GLuint _cubeVao;
    GLuint _cubeVbo;
};

#endif
