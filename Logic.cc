#include "Logic.h"

Logic::Logic()
{    
    _cameraPos = { 0.0f, 0.0f, 0.0f };

    _cameraLookAt.z = _cameraPos.z - 1;
    _cameraLookAt.x = _cameraPos.x;
    _cameraLookAt.y = _cameraPos.y;

    _cameraUp = { 0.0f, 1.0f, 0.0f };
}

Logic::~Logic()
{

}

void Logic::update()
{

}

void Logic::moveCamera(float amount)
{
  vec3 viewVector = VectorSub(_cameraLookAt, _cameraPos);

  _cameraPos = VectorAdd(_cameraPos, ScalarMult(viewVector, amount));

  _cameraLookAt = VectorAdd(_cameraLookAt, ScalarMult(viewVector, amount));
}

void Logic::moveCameraWithStrafe(float amount)
{
  vec3 viewVector = VectorSub(_cameraLookAt, _cameraPos);

  vec3 orthoViewVector;

  orthoViewVector.x = -viewVector.z;
  orthoViewVector.z = viewVector.x;
  
  _cameraPos.x = _cameraPos.x + orthoViewVector.x * amount;
  _cameraPos.z = _cameraPos.z + orthoViewVector.z * amount;

  _cameraLookAt.x = _cameraLookAt.x + orthoViewVector.x * amount;
  _cameraLookAt.z = _cameraLookAt.z + orthoViewVector.z * amount;
}

vec3 Logic::getCameraPos()
{
    return _cameraPos;
}

vec3 Logic::getCameraLookAt()
{
    return _cameraLookAt;
}

vec3 Logic::getCameraUp()
{
    return _cameraUp;
}
