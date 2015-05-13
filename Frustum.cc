#include "Frustum.h"

Frustum::Frustum()
{
    nearHeight = 0.0f;
    nearWidth = 0.0f;
	
    farHeight = 0.0f;
    farWidth = 0.0f;

    farLeftPoint = vec3(0.0f, 0.0f, 0.0f);
    farRightPoint = vec3(0.0f, 0.0f, 0.0f);

    nearLeftPoint = vec3(0.0f, 0.0f, 0.0f);
    nearRightPoint = vec3(0.0f, 0.0f, 0.0f);

    leftEdge = vec3(0.0f, 0.0f, 0.0f);
    rightEdge = vec3(0.0f, 0.0f, 0.0f);
    topEdge = vec3(0.0f, 0.0f, 0.0f);
    bottomEdge = vec3(0.0f, 0.0f, 0.0f);

    leftEdgeNormal = vec3(0.0f, 0.0f, 0.0f);
    rightEdgeNormal = vec3(0.0f, 0.0f, 0.0f);
    topEdgeNormal = vec3(0.0f, 0.0f, 0.0f);
    bottomEdgeNormal = vec3(0.0f, 0.0f, 0.0f);
}

bool Frustum::pointIsInsideFrustum(vec3 p)
{
    return PointIsOnPositiveSide(p, leftEdgeNormal, nearLeftPoint) && PointIsOnPositiveSide(p, rightEdgeNormal, nearRightPoint) && PointIsOnPositiveSide(p, topEdgeNormal, farLeftPoint) && PointIsOnPositiveSide(p, bottomEdgeNormal, nearLeftPoint);
}

bool Frustum::lineIntersectsFrustum(vec3 lineDir, vec3 linePoint)
{
    return LinePlaneIntersection(lineDir, linePoint, leftEdgeNormal, nearLeftPoint) || LinePlaneIntersection(lineDir, linePoint, rightEdgeNormal, nearRightPoint) || LinePlaneIntersection(lineDir, linePoint, topEdgeNormal, farLeftPoint) || LinePlaneIntersection(lineDir, linePoint, bottomEdgeNormal, nearLeftPoint);
}

