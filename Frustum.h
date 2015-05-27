#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "MathUtils.h"

#define near .1
#define far 500.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5
#define fovDegree 80.0
#define aspectRatio 16.0 / 9.0

struct Plane
{
    vec3 normal;
    vec3 corner;
};

class Frustum
{
public:
    Frustum();

    bool pointIsInsideFrustum(vec3 min, vec3 max);

    void updateFrustum(vec3 cameraPos, vec3 cameraLookAt, vec3 cameraUp);
    
    float nearHeight;
    float nearWidth;

    float farHeight;
    float farWidth;

    Plane* _planes;
};

#endif
