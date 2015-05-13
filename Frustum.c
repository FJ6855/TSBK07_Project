#include "Frustum.h"

bool pointIsInsideFrustum(vec3 p)
{
    return PointIsOnPositiveSide(p, leftEdgeNormal, nearLeftPoint) && PointIsOnPositiveSide(p, rightEdgeNormal, nearRightPoint) && PointIsOnPositiveSide(p, topEdgeNormal, farLeftPoint) && PointIsOnPositiveSide(p, bottomEdgeNormal, nearLeftPoint);
}
