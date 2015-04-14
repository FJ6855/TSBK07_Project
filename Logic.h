#ifndef LOGIC_H
#define LOGIC_H

#include "MathUtils.h"

class Logic
{
public:
    Logic();

    ~Logic();

    void update();

    void moveCamera(float amount);
    void moveCameraWithStrafe(float amount);

    vec3 getCameraPos();
    vec3 getCameraLookAt();
    vec3 getCameraUp();

private:
    vec3 _cameraPos;
    vec3 _cameraLookAt;
    vec3 _cameraUp;
};

#endif
