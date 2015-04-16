#ifndef LOGIC_H
#define LOGIC_H

#include "World.h"
#include "MathUtils.h"

class Logic
{
public:
    Logic();

    ~Logic();

    void update();

    void moveCamera(float amount);
    void moveCameraWithStrafe(float amount);
    void rotateCamera(float angleZ, float angleY);

    vec3 getCameraPos();
    vec3 getCameraLookAt();
    vec3 getCameraUp();

    World* getWorld();

    void createWorld(GLuint program);

private:
    vec3 _cameraPos;
    vec3 _cameraLookAt;
    vec3 _cameraUp;

    World* _world;
};

#endif
