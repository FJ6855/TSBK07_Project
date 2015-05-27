#include "Logic.h"

Logic::Logic()
{    
    _freeCam = true;

    _player = new Player();

    _cameraPos = vec3(-10.0f, 30.0f, -20.0f);
    _cameraPos.y += 1.5;

    _cameraLookAt.z = _cameraPos.z + 1;
    _cameraLookAt.x = _cameraPos.x + 1;
    _cameraLookAt.y = _cameraPos.y - 0.5;

    _cameraUp = vec3(0.0f, 1.0f, 0.0f);

    _frustum = new Frustum();

    // Calculate the near and far planes width and height
    _frustum->nearHeight = 2 * tan((fovDegree / 2) * 3.14 / 180.0f) * near;
    _frustum->nearWidth = _frustum->nearHeight * aspectRatio;

    _frustum->farHeight = 2 * tan((fovDegree / 2) * 3.14 / 180.0f) * far;
    _frustum->farWidth = _frustum->farHeight * aspectRatio;
}

Logic::~Logic()
{
    delete _world;
    _world = NULL;

    delete _player;
    _player = NULL;

    delete _frustum;
    _frustum = NULL;
}

void Logic::update()
{
    _frustum->updateFrustum(_cameraPos, _cameraLookAt, _cameraUp);

    _world->update();
       
    if (_freeCam)
    {		
	_world->updateRenderList(_frustum, _cameraPos);
    }
    else
    {	
	_physics();
	
	_world->loadChunks(_player->getPosition());
	
	_world->updateRenderList(_frustum, _player->getPosition());
    }

    _updateTargetPosition();

    //printf("%f, %f, %f\n", _player->getPosition().x, _player->getPosition().y, _player->getPosition().z);
}

void Logic::createWorld(GLuint program)
{
    _world = new World(program, _player->getPosition());
}

void Logic::move(float amount)
{
    if(_freeCam)
    {
	moveCamera(amount);
    }
    else 
    {
	movePlayer(amount);
    }
}

void Logic::strafe(float amount)
{
    if(_freeCam)
    {
	strafeCamera(amount);
    }
    else 
    {
	strafePlayer(amount);
    }
}

void Logic::_physics()
{
    if(!_freeCam)
    {
	float yvel = _player->getYVel();

	yvel -= 0.02;
	
	if(yvel > -1)
	    _player->setYVel(yvel);
	
	_player->setWalking(false);

	movePlayerY(yvel);
    }
}

void Logic::moveCamera(float amount)
{
    vec3 viewVector = Normalize(_cameraLookAt - _cameraPos);
    
    _cameraPos = _cameraPos + viewVector * amount;
    
    _cameraLookAt = _cameraPos + viewVector;
}

void Logic::movePlayer(float amount)
{
    vec3 viewVector = Normalize(_cameraLookAt - _cameraPos);
    viewVector.y = 0;
    viewVector = Normalize(viewVector);

    _player->setPosition(_collision(_player->getPosition(), _player->getPosition() + viewVector * amount));
   
    viewVector = Normalize(_cameraLookAt - _cameraPos);
    _cameraPos = _player->getPosition();
    _cameraLookAt = _cameraPos + viewVector;   
}

void Logic::movePlayerY(float amount)
{
    vec3 viewVector = Normalize(_cameraLookAt - _cameraPos);

    vec3 moveVector = vec3(0.0f, amount, 0.0f);

    _player->setPosition(_collision(_player->getPosition(), _player->getPosition() + moveVector));
   
    _cameraPos = _player->getPosition();
    _cameraLookAt = _cameraPos + viewVector;
}

void Logic::strafeCamera(float amount)
{
    vec3 viewVector = Normalize(_cameraLookAt - _cameraPos);
    vec3 orthoViewVector;
    
    orthoViewVector.x = -viewVector.z;
    orthoViewVector.z = viewVector.x;
    orthoViewVector.y = 0;
    orthoViewVector = Normalize(orthoViewVector);

    _cameraPos = _cameraPos + orthoViewVector * amount;

    _cameraLookAt = viewVector + _cameraPos;
}

void Logic::strafePlayer(float amount)
{
    vec3 viewVector = Normalize(_cameraLookAt - _cameraPos);
    vec3 orthoViewVector;
    
    orthoViewVector.x = -viewVector.z;
    orthoViewVector.z = viewVector.x;
    orthoViewVector.y = 0;
    orthoViewVector = Normalize(orthoViewVector);

    vec3 playerPos = _player->getPosition();
    playerPos = _collision(playerPos, playerPos + orthoViewVector * amount);
    
    _player->setPosition(playerPos);
    
    _cameraPos = _player->getPosition();
    _cameraPos.y += 1.5;
    _cameraLookAt = _cameraPos + viewVector;
}

void Logic::rotateCamera(float angleZ, float angleY)
{   
    vec3 viewVector = Normalize(VectorSub(_cameraLookAt, _cameraPos));
    vec3 orthoViewVector;
    orthoViewVector.x = -viewVector.z;
    orthoViewVector.z = viewVector.x;

    _cameraLookAt = _cameraPos + MultVec3(Ry(angleY), viewVector);
    viewVector = Normalize(_cameraLookAt - _cameraPos);

    _cameraLookAt = _cameraPos + MultVec3(ArbRotate(orthoViewVector, angleZ), viewVector);
}

void Logic::changeBlock(int blockType)
{   
    _world->changeBlock(_targetPosition, blockType);
}

vec3 Logic::_collision(vec3 oldPos, vec3 newPos)
{       
    if (newPos.x < 5 * 16)
	newPos.x = 5 * 16;

    if (newPos.x > 2048 - 5 * 16)
	newPos.x = 2048 - 5 * 16;
    
    if (newPos.z < 5 * 16)
	newPos.z = 5 * 16;

    if (newPos.z > 2048 - 5 * 16)
	newPos.z = 2048 - 5 * 16;

    vec3 playerDirection = VectorSub(newPos, oldPos);

    playerDirection.x = (playerDirection.x > 0.0f) ? ceil(playerDirection.x) : floor(playerDirection.x);
    playerDirection.y = (playerDirection.y > 0.0f) ? ceil(playerDirection.y) : floor(playerDirection.y);
    playerDirection.z = (playerDirection.z > 0.0f) ? ceil(playerDirection.z) : floor(playerDirection.z);

    _player->setDirection(playerDirection);
    
    vec3 tmpNew = newPos;
    vec3 stupid = oldPos;

    stupid.y = newPos.y;
    
    _player->setPosition(stupid);
    
    // Chunk* c = _world->getChunkAtPosition(vec3(newPos.x + _player->getDirection().x * 0.4, newPos.x + _player->getDirection().y, newPos.z + _player->getDirection().z * 0.4)); 
	
    Chunk* c = _world->getChunkAtPosition(newPos);

    if (c != NULL)
    {
	if (c->checkCollision(_player->getMin(), _player->getMax()))
	{	
	    tmpNew.y = oldPos.y;
		
	    _player->setYVel(0);
		
	    _player->setWalking(true);
	}
    }

    stupid = oldPos;
	
    stupid.x = newPos.x;

    _player->setPosition(stupid);

    // c = _world->getChunkAtPosition(newPos);
	
    if (c != NULL)
    {
	if (c->checkCollision(_player->getMin(), _player->getMax()))
	{	
	    tmpNew.x = oldPos.x;
	}
    }

    stupid = oldPos;

    stupid.z = newPos.z;

    _player->setPosition(stupid);

    //  c = _world->getChunkAtPosition(newPos);
	
    if (c != NULL)
    {
	if (c->checkCollision(_player->getMin(), _player->getMax()))
	{	
	    tmpNew.z = oldPos.z;
	}
    }

    return tmpNew;
}

void Logic::_updateTargetPosition()
{    
    //vec3 blockPos = VectorAdd(_cameraPos, Normalize(VectorSub(_cameraLookAt, _cameraPos)));
 
    _targetPosition = _cameraPos + Normalize(_cameraLookAt - _cameraPos);

    _targetPosition.x = (int)_targetPosition.x;
    _targetPosition.y = (int)_targetPosition.y;
    _targetPosition.z = (int)_targetPosition.z;

    if (_world->isBlockActive(_targetPosition))	
    {
	_renderTarget = true;
    }
    else
    {
	_targetPosition = _cameraPos + Normalize(_cameraLookAt - _cameraPos) * 2.0f;
	
	_targetPosition.x = (int)_targetPosition.x;
	_targetPosition.y = (int)_targetPosition.y;
	_targetPosition.z = (int)_targetPosition.z;

	_renderTarget = _world->isBlockActive(_targetPosition);
    }
}

void Logic::setFreeCam(bool value)
{
    _freeCam = value;
}

vec3 Logic::getCameraPos()
{
    return _cameraPos;
}

void Logic::setCameraPos(vec3 pos)
{
    _cameraPos = pos;
}

vec3 Logic::getCameraLookAt()
{
    return _cameraLookAt;
}

vec3 Logic::getCameraUp()
{
    return _cameraUp;
}

vec3 Logic::getTargetPosition()
{
    return _targetPosition;
}

bool Logic::getRenderTarget()
{
    return _renderTarget;
}

World* Logic::getWorld()
{
    return _world;
}

Player* Logic::getPlayer()
{
    return _player;
}
