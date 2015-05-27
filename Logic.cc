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

    _ballIndex = 0;

    for(int i = 0; i < 100; i++)
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
    vec3 viewVector = Normalize(VectorSub(_cameraLookAt, _cameraPos));
    _cameraPos = VectorAdd(_cameraPos, viewVector * amount);
    _cameraLookAt = VectorAdd(_cameraPos, viewVector);
}

void Logic::movePlayer(float amount)
{
    vec3 viewVector = Normalize(VectorSub(_cameraLookAt, _cameraPos));
    viewVector.y = 0;
    viewVector = Normalize(viewVector);

    _player->setPosition(_collision(_player->getPosition(), VectorAdd(_player->getPosition(), viewVector * amount)));
   
    viewVector = Normalize(VectorSub(_cameraLookAt, _cameraPos));
    _cameraPos = _player->getPosition();
    _cameraPos.y += 1.5;
    _cameraLookAt = VectorAdd(_cameraPos, viewVector);   
}

void Logic::movePlayerY(float amount)
{
    vec3 viewVector = Normalize(VectorSub(_cameraLookAt, _cameraPos));

    vec3 moveVector = vec3(0.0f, amount, 0.0f);

    _player->setPosition(_collision(_player->getPosition(), VectorAdd(_player->getPosition(), moveVector)));
   
    _cameraPos = _player->getPosition();
    _cameraPos.y += 1.5;
    _cameraLookAt = VectorAdd(_cameraPos, viewVector);   
}

void Logic::strafeCamera(float amount)
{
    vec3 viewVector = Normalize(VectorSub(_cameraLookAt, _cameraPos));
    vec3 orthoViewVector;
    
    orthoViewVector.x = -viewVector.z;
    orthoViewVector.z = viewVector.x;
    orthoViewVector.y = 0;
    orthoViewVector = Normalize(orthoViewVector);

    _cameraPos = VectorAdd(_cameraPos, orthoViewVector * amount);

    _cameraLookAt = VectorAdd(viewVector, _cameraPos);
}

void Logic::strafePlayer(float amount)
{
    vec3 viewVector = Normalize(VectorSub(_cameraLookAt, _cameraPos));
    vec3 orthoViewVector;
    
    orthoViewVector.x = -viewVector.z;
    orthoViewVector.z = viewVector.x;
    orthoViewVector.y = 0;
    orthoViewVector = Normalize(orthoViewVector);

    vec3 playerPos = _player->getPosition();
    playerPos = _collision(playerPos, VectorAdd(playerPos, orthoViewVector * amount));
    
    _player->setPosition(playerPos);
    
    _cameraPos = _player->getPosition();
    _cameraPos.y += 1.5;
    _cameraLookAt = VectorAdd(_cameraPos, viewVector);
}

void Logic::rotateCamera(float angleZ, float angleY)
{   
    vec3 viewVector = Normalize(VectorSub(_cameraLookAt, _cameraPos));
    vec3 orthoViewVector;
    orthoViewVector.x = -viewVector.z;
    orthoViewVector.z = viewVector.x;

    _cameraLookAt = VectorAdd(_cameraPos, MultVec3(Ry(angleY), viewVector));
    viewVector = Normalize(VectorSub(_cameraLookAt, _cameraPos));

    _cameraLookAt = VectorAdd(_cameraPos, MultVec3(ArbRotate(orthoViewVector, angleZ), viewVector));
}

void Logic::changeBlock(int blockType)
{   
    vec3 position = _player->getPosition();
    position.y += 1.5;
    vec3 viewVector = Normalize(VectorSub(_cameraLookAt, _cameraPos));
    
    _world->changeBlock(position, viewVector, blockType);
}

vec3 Logic::_collision(vec3 oldPos, vec3 newPos)
{       
    vec3 playerDirection = VectorSub(newPos, oldPos);

    playerDirection.x = (playerDirection.x > 0.0f) ? ceil(playerDirection.x) : floor(playerDirection.x);
    playerDirection.y = (playerDirection.y > 0.0f) ? ceil(playerDirection.y) : floor(playerDirection.y);
    playerDirection.z = (playerDirection.z > 0.0f) ? ceil(playerDirection.z) : floor(playerDirection.z);

    _player->setDirection(playerDirection);

    if (floor(newPos.y) != floor(oldPos.y))
    {	
	if (_world->isBlockActive(newPos))
	{
	    newPos.y = floor(oldPos.y);

	    _player->setYVel(0.0f);
		
	    _player->setWalking(true);
	}
    }

    if (floor(newPos.x + _player->getCollisionOffset() * playerDirection.x) != floor(oldPos.x))
    {
	vec3 offsetPos = newPos;
	    
	offsetPos.x += _player->getCollisionOffset() * playerDirection.x;	    

	if (_world->isBlockActive(offsetPos))
	{
	    newPos.x = floor(oldPos.x);
	    
	    if (playerDirection.x == 1.0f)
		newPos.x += 1.0f - _player->getCollisionOffset();
	    else 
		newPos.x += _player->getCollisionOffset();
	}
    }
	
    if (floor(newPos.z + _player->getCollisionOffset() * playerDirection.z) != floor(oldPos.z))
    {	    
	vec3 offsetPos = newPos;
	    
	offsetPos.z += _player->getCollisionOffset() * playerDirection.z;	    

	if (_world->isBlockActive(offsetPos))
	{
	    newPos.z = floor(oldPos.z);
	    
	    if (playerDirection.z == 1.0f)
		newPos.z += 1.0f - _player->getCollisionOffset();
	    else 
		newPos.z += _player->getCollisionOffset();
	}
    }

    return newPos;
}


void Logic::shootBall()
{
    Ball* b = new Ball(_cameraPos, Normalize(_cameraLookAt - _cameraPos));

    if(_ballIndex > 99)
    {	
	_ballIndex = 0;
    }
    
    if(_balls[_ballIndex] != NULL)
    {
	delete _balls[_ballIndex];
	_balls[_ballIndex] = NULL;
    }
    
    _balls[_ballIndex] = b;
    
    _ballIndex++;

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
    }
}

void Logic::_ballCollision(vec3 oldPos, vec3& newPos, vec3& newDir)
{
    Chunk* c[4];
    c[0]= _world->getChunkAtPosition(newPos);
    c[1] = NULL;
    c[2] = NULL;
    c[3] = NULL;
    
    if(newPos.x >= 0 && newPos.x <= 2048 && newPos.z >= 0 && newPos.z <= 2048)
    {
	vec3 relativeBallPos =  newPos - c[0]->getPos();

	relativeBallPos.x = floor(relativeBallPos.x);
	relativeBallPos.y = floor(relativeBallPos.y);
	relativeBallPos.z = floor(relativeBallPos.z);
    
	vec3 tmp = newPos;

	if (relativeBallPos.x < 1 && newPos.x > 16.0) // if ball overlaps chunk in x
	{
	    tmp.x -= 16.0;

	    c[1] = _world->getChunkAtPosition(tmp);

	    tmp = newPos;
	}
	else if (relativeBallPos.x > 14 && newPos.x < 2032)
	{
	    tmp.x += 16.0;

	    c[1] = _world->getChunkAtPosition(tmp);

	    tmp = newPos;
	}   

	if (relativeBallPos.z < 1  && newPos.z > 16.0) // if ball overlaps chunk in z
	{
	    tmp.z -= 16.0;

	    c[2] = _world->getChunkAtPosition(tmp);

	    tmp = newPos;
	}
	else if (relativeBallPos.z > 14 && newPos.z < 2032)
	{	
	    tmp.z += 16.0;

	    c[2] = _world->getChunkAtPosition(tmp);

	    tmp = newPos;
	}

	if(c[1] != NULL && c[2] != NULL) // diagonal chunk if both z and x overlaps other chunks
	{
	    if (c[1]->getPos().x < newPos.x && c[2]->getPos().z < newPos.z)
		{
		    tmp.x -= 16;
		    tmp.z -= 16;
	 
		    c[3] = _world->getChunkAtPosition(tmp);
		}
		else if(c[1]->getPos().x > newPos.x && c[2]->getPos().z < newPos.z)
		{
		    tmp.x += 16;
		    tmp.z -= 16;
	 
		    c[3] = _world->getChunkAtPosition(tmp);
		}
		else if(c[1]->getPos().x < newPos.x && c[2]->getPos().z > newPos.z)
		{
		    tmp.x -= 16;
		    tmp.z += 16;
	 
		    c[3] = _world->getChunkAtPosition(tmp);
		}
		else
		{
		    tmp.x += 16;
		    tmp.z += 16;
	 
		    c[3] = _world->getChunkAtPosition(tmp);
		}
		}

	    for(int  i = 0; i < 4; ++i)
	    {
		if (c[i] != NULL)
		{
		    vec3 tmpNewPos = oldPos;

		    tmpNewPos.y = newPos.y;

		    if (c[i]->checkCollision(tmpNewPos, 0.2f))
		    {
			newPos.y = oldPos.y;
	    
			newDir.y = -newDir.y / 2.0f;
		    }

		    tmpNewPos = oldPos;

		    tmpNewPos.x = newPos.x;

		    if (c[i]->checkCollision(tmpNewPos, 0.2f))
		    {
			newPos.x = oldPos.x;

			newDir.x = -newDir.x;
		    }

		    tmpNewPos = oldPos;

		    tmpNewPos.z = newPos.z;

		    if (c[i]->checkCollision(tmpNewPos, 0.2f))
		    {
			newPos.z = oldPos.z;

			newDir.z = -newDir.z;
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
