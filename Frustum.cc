#include "Frustum.h"

Frustum::Frustum()
{
    nearHeight = 0.0f;
    nearWidth = 0.0f;
	
    farHeight = 0.0f;
    farWidth = 0.0f;

    _planes = new Plane[6];
}

bool Frustum::pointIsInsideFrustum(vec3 min, vec3 max)
{
    for (int i = 0; i < 6; ++i)
    {
	// figure out the positive vector
	vec3 pv = min;
	
	if (_planes[i].normal.x >= 0) pv.x = max.x;
	if (_planes[i].normal.y >= 0) pv.y = max.y;
	if (_planes[i].normal.z >= 0) pv.z = max.z;

	float d = DotProduct(ScalarMult(_planes[i].normal, -1), _planes[i].corner);

	if (DotProduct(_planes[i].normal, pv) + d <= 0)
	    return false;
    }

    return true;
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
    vec3 rightVector = Normalize(CrossProduct(viewVector, cameraUp));

    // adjust the camera up direction
    cameraUp = Normalize(CrossProduct(rightVector, viewVector));

    // Get the positions of the far and near corners
    vec3 farTopLeftPoint = farPosition - rightVector * (farWidth / 2) + cameraUp * (farHeight / 2);
    vec3 farBottomLeftPoint = farPosition - rightVector * (farWidth / 2) - cameraUp * (farHeight / 2);
    
    vec3 farTopRightPoint = farPosition + rightVector * (farWidth / 2) + cameraUp * (farHeight / 2);
    vec3 farBottomRightPoint = farPosition + rightVector * (farWidth / 2) - cameraUp * (farHeight / 2);
    
    vec3 nearTopLeftPoint = nearPosition - rightVector * (nearWidth / 2) + cameraUp * (nearHeight / 2);
    vec3 nearBottomLeftPoint = nearPosition - rightVector * (nearWidth / 2) - cameraUp * (nearHeight / 2);

    vec3 nearTopRightPoint = nearPosition + rightVector * (nearWidth / 2) + cameraUp * (nearHeight / 2);
    vec3 nearBottomRightPoint = nearPosition + rightVector * (nearWidth / 2) - cameraUp * (nearHeight / 2);
    
    // Get the normals to the planes of the frustu,
    vec3 leftPlaneNormal = Normalize(CrossProduct(farBottomLeftPoint - nearBottomLeftPoint, nearTopLeftPoint - nearBottomLeftPoint));
    vec3 rightPlaneNormal = Normalize(CrossProduct(farTopRightPoint - nearTopRightPoint, nearBottomRightPoint - nearTopRightPoint));

    vec3 farPlaneNormal = Normalize(CrossProduct(farTopLeftPoint - farTopRightPoint, farBottomRightPoint - farTopRightPoint));
    vec3 nearPlaneNormal = Normalize(CrossProduct(nearTopRightPoint - nearTopLeftPoint, nearBottomLeftPoint - nearTopLeftPoint));

    vec3 topPlaneNormal = Normalize(CrossProduct(nearTopLeftPoint - nearTopRightPoint, farTopRightPoint - nearTopRightPoint));
    vec3 bottomPlaneNormal = Normalize(CrossProduct(farBottomRightPoint - nearBottomRightPoint, nearBottomLeftPoint - nearBottomRightPoint));

    //left plane
    _planes[0].normal = leftPlaneNormal;
    _planes[0].corner = farTopLeftPoint;
    //right plane
    _planes[1].normal = rightPlaneNormal;
    _planes[1].corner = nearTopRightPoint;
    //far plane
    _planes[2].normal = farPlaneNormal;
    _planes[2].corner = farTopRightPoint;
    //near plane
    _planes[3].normal = nearPlaneNormal;
    _planes[3].corner = nearTopLeftPoint;
    //top plane
    _planes[4].normal = topPlaneNormal;
    _planes[4].corner = farTopLeftPoint;
    //bottom plane
    _planes[5].normal = bottomPlaneNormal;
    _planes[5].corner = nearBottomLeftPoint;
}
