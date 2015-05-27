#include "Logic.h"

Logic::Logic()
{    
    _freeCam = false;

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

    _ballIndex = 0;

    for (int i = 0; i < 100; i++)
    {
	_balls[i] = NULL;
    }
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

    printf("%f, %f, %f\n", _player->getPosition().x, _player->getPosition().y, _player->getPosition().z);
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

    _ballMovement();
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

    vec3 newPos = _player->getPosition() + viewVector * amount;

    _collision(_player->getPosition(), newPos);

    _player->setPosition(newPos);
    
    viewVector = Normalize(_cameraLookAt - _cameraPos);
    _cameraPos = _player->getPosition();
    _cameraLookAt = _cameraPos + viewVector;
}

void Logic::movePlayerY(float amount)
{
    vec3 viewVector = Normalize(_cameraLookAt - _cameraPos);

    vec3 moveVector = vec3(0.0f, amount, 0.0f);

    vec3 newPos = _player->getPosition() + moveVector;

    _collision(_player->getPosition(), newPos);

    _player->setPosition(newPos);
    
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

    vec3 newPos = _player->getPosition() + orthoViewVector * amount;

    _collision(_player->getPosition(), newPos);

    _player->setPosition(newPos);
    
    _cameraPos = _player->getPosition();
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

void Logic::shootBall()
{
    Ball* b = new Ball(_cameraPos, Normalize(_cameraLookAt - _cameraPos));

    if (_ballIndex > 99)
    {	
	_ballIndex = 0;
    }
    
    if (_balls[_ballIndex] != NULL)
    {
	delete _balls[_ballIndex];

	_balls[_ballIndex] = NULL;
    }
    
    _balls[_ballIndex] = b;
    
    _ballIndex++;
}

void Logic::_collision(vec3 oldPos, vec3& newPos)
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

    Chunk* chunks[4];

    chunks[0]= _world->getChunkAtPosition(oldPos);

    if (chunks[0] != NULL)
    {
	chunks[1] = NULL;
	chunks[2] = NULL;
	chunks[3] = NULL;
	
	_getChunksAroundChunk(chunks[0]->getPos(), playerDirection, chunks);
	
	for (int i = 0; i < 4; ++i)
	{
	    if (chunks[i] != NULL)
	    {
		vec3 tmpNewPos = oldPos;
	
		tmpNewPos.x = newPos.x;

		_player->setPosition(tmpNewPos);
	
		if (chunks[i]->checkCollision(tmpNewPos, _player->getMin(), _player->getMax()))
		{	
		    newPos.x = oldPos.x;
		}	    

		tmpNewPos = oldPos;

		tmpNewPos.y = newPos.y;
    
		_player->setPosition(tmpNewPos);
    
		if (chunks[i]->checkCollision(tmpNewPos, _player->getMin(), _player->getMax()))
		{	
		    newPos.y = oldPos.y;
		
		    _player->setYVel(0);
		
		    _player->setWalking(true);
		}

		tmpNewPos = oldPos;

		tmpNewPos.z = newPos.z;

		_player->setPosition(tmpNewPos);

		if (chunks[i]->checkCollision(tmpNewPos, _player->getMin(), _player->getMax()))
		{	
		    newPos.z = oldPos.z;
		}
	    }
	}
    }
}

void Logic::_updateTargetPosition()
{ 
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

void Logic::_ballMovement()
{
    for(int i = 0; i < 100; i++)
    {
	Ball* b = _balls[i];
	
 	if(_balls[i] != NULL)
	{
	    vec3 tmpDir = b->getDirection();
	    tmpDir.y -= 0.05f;
	    tmpDir.x *= 0.99;
	    tmpDir.z *= 0.99;
	    b->setDirection(tmpDir);

	    vec3 newPos = b->getPosition() + b->getDirection() * 0.1f;

	    if (newPos.y >= 0)
	    {
		vec3 newDir = b->getDirection();

		_ballCollision(b->getPosition(), newPos, newDir);
	    
		b->setPosition(newPos);
		b->setDirection(newDir);

		for (int j = i; j < 100; j++)
		{
		    Ball* b2 = _balls[j];

		    if (b2 != NULL)
		    {
			vec3 dir1 = b->getDirection();
			vec3 dir2 = b2->getDirection();
		
			if (i != j && _ballToBallCollision(b->getPosition(), b2->getPosition(), dir1, dir2))
			{
			    b->setDirection(dir1);
			    b2->setDirection(dir2);
			}	
		    }
		}
	    }
	    else
	    {
		delete _balls[i];

		_balls[i] = NULL;
	    }
	}
    }
}

void Logic::_ballCollision(vec3 oldPos, vec3& newPos, vec3& newDir)
{
    Chunk* chunks[4];

    chunks[0]= _world->getChunkAtPosition(newPos);

    chunks[1] = NULL;
    chunks[2] = NULL;
    chunks[3] = NULL;
    
    if (newPos.x >= 0 && newPos.x <= 2048 && newPos.z >= 0 && newPos.z <= 2048)
    {
	if (chunks[0] != NULL)
	{
	    vec3 relativeBallPos = newPos - chunks[0]->getPos();

	    relativeBallPos.x = floor(relativeBallPos.x);
	    relativeBallPos.y = floor(relativeBallPos.y);
	    relativeBallPos.z = floor(relativeBallPos.z);
    	
	    _getChunksAroundChunk(chunks[0]->getPos(), newDir, chunks);

	    for (int  i = 0; i < 4; ++i)
	    {
		if (chunks[i] != NULL)
		{
		    vec3 tmpNewPos = oldPos;

		    tmpNewPos.y = newPos.y;

		    if (chunks[i]->checkCollision(tmpNewPos, 0.2f))
		    {
			newPos.y = oldPos.y;
	    
			newDir.y = -newDir.y / 2.0f;
		    }

		    tmpNewPos = oldPos;

		    tmpNewPos.x = newPos.x;

		    if (chunks[i]->checkCollision(tmpNewPos, 0.2f))
		    {
			newPos.x = oldPos.x;

			newDir.x = -newDir.x;
		    }

		    tmpNewPos = oldPos;

		    tmpNewPos.z = newPos.z;

		    if (chunks[i]->checkCollision(tmpNewPos, 0.2f))
		    {
			newPos.z = oldPos.z;

			newDir.z = -newDir.z;
		    }
		}
	    }
	}
    }
}

bool Logic::_ballToBallCollision(vec3 pos1, vec3 pos2, vec3& dir1, vec3& dir2)
{
    vec3 v = pos1 - pos2;

    float distSquared = DotProduct(v, v);

    float radiusSumSquared = pow(0.2f + 0.2f, 2);

    if (distSquared <= radiusSumSquared)
    {
	vec3 tmpDir = dir1;

	dir1 = dir2;

	dir2 = tmpDir;

	return true;
    }

    return false;
}

void Logic::_getChunksAroundChunk(vec3 pos, vec3 direction, Chunk* chunks[4])
{
    vec3 tmp = pos;

    if (direction.x < 0)
    {
	tmp.x -= 16.0;

	chunks[1] = _world->getChunkAtPosition(tmp);

	tmp = pos;
    }
    else if (direction.x > 0)
    {
	tmp.x += 16.0;

	chunks[1] = _world->getChunkAtPosition(tmp);

	tmp = pos;
    }   

    if (direction.z < 0)
    {
	tmp.z -= 16.0;

	chunks[2] = _world->getChunkAtPosition(tmp);

	tmp = pos;
    }
    else if (direction.z > 0)
    {	
	tmp.z += 16.0;

	chunks[2] = _world->getChunkAtPosition(tmp);

	tmp = pos;
    }

    if (chunks[1] != NULL && chunks[2] != NULL) // diagonal chunk if both z and x overlaps other chunks
    {
	if (direction.x < 0 && direction.z < 0)
	{
	    tmp.x -= 16;
	    tmp.z -= 16;
	 
	    chunks[3] = _world->getChunkAtPosition(tmp);
	}
	else if (direction.x > 0 && direction.z < 0)
	{
	    tmp.x += 16;
	    tmp.z -= 16;
	 
	    chunks[3] = _world->getChunkAtPosition(tmp);
	}
	else if (direction.x < 0 && direction.z > 0)
	{
	    tmp.x -= 16;
	    tmp.z += 16;
	 
	    chunks[3] = _world->getChunkAtPosition(tmp);
	}
	else if (direction.x > 0 && direction.z > 0)
	{
	    tmp.x += 16;
	    tmp.z += 16;
	 
	    chunks[3] = _world->getChunkAtPosition(tmp);
	}
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

Ball* Logic::getBall(int index)
{
    return _balls[index];
}
