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

class Frustum
{
public:
    Frustum();

    bool pointIsInsideFrustum(vec3 p);
    bool lineIntersectsFrustum(vec3 lineDir, vec3 linePoint);

    void updateFrustum(vec3 cameraPos, vec3 cameraLookAt, vec3 cameraUp);
    
    float nearHeight;
    float nearWidth;

    float farHeight;
    float farWidth;

    vec3 farLeftPoint;
    vec3 farRightPoint;

    vec3 nearLeftPoint;
    vec3 nearRightPoint;

    vec3 leftEdge;
    vec3 rightEdge;
    vec3 topEdge;
    vec3 bottomEdge;

    vec3 leftEdgeNormal;
    vec3 rightEdgeNormal;
    vec3 topEdgeNormal;
    vec3 bottomEdgeNormal;
};

#endif
