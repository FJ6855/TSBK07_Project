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

void Frustum::updateFrustum(vec3 cameraPos, vec3 cameraLookAt, vec3 cameraUp)
{
    // Get the direction we are looking
    vec3 viewVector = Normalize(cameraLookAt - cameraPos);
    
    // Get the position of the near plane
    vec3 nearPosition = cameraPos + viewVector * near;
    
    // Get the position of the far plane
    vec3 farPosition = cameraPos + viewVector * far;
    
    // Get the vector that is orthogonal to the view vector and the up vector
    vec3 rightVector = CrossProduct(viewVector, cameraUp);

    // Get the positions of the far and near planes edges
    farLeftPoint = farPosition - rightVector * (farWidth / 2);
    farRightPoint = farPosition + rightVector * (farWidth / 2);

    nearLeftPoint = nearPosition - rightVector * (nearWidth / 2);
    nearRightPoint = nearPosition + rightVector * (nearWidth / 2);

    // Get the edges of the far and near planes
    leftEdge = farLeftPoint - nearLeftPoint;
    rightEdge =  farRightPoint - nearRightPoint;
    topEdge = farRightPoint - farLeftPoint;
    bottomEdge = nearRightPoint - nearLeftPoint;
    
    // Get the normals to the edges of the far and near planes
    leftEdgeNormal = Normalize(CrossProduct(leftEdge, vec3(0.0, 1.0f, 0.0f)));
    rightEdgeNormal = Normalize(CrossProduct(rightEdge, vec3(0.0, -1.0f, 0.0f)));
    topEdgeNormal = Normalize(CrossProduct(topEdge, vec3(0.0, 1.0f, 0.0f)));
    bottomEdgeNormal = Normalize(CrossProduct(bottomEdge, vec3(0.0, -1.0f, 0.0f)));
}
