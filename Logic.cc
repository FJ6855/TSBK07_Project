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

    for(int i = 0; i < 15; i++)
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
    Ball* b = new Ball{getPlayer()->getPosition(), getCameraLookAt()};

    if(_ballIndex > 14)
    {	
	_ballIndex = 0;
    }
    printf("ballIndex %i\n", _ballIndex);
    
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
    for(int i = 0; i < 15; i++)
    {
	Ball* b = _balls[i];
	if(_balls[i] != NULL)
	{
	    vec3 newPos =  _ballCollision(b->getPosition(), VectorAdd(b->getPosition(), b->getDirection()));
	    b->setPosition(newPos);

	    printf("%f. %f. %f. \n,",b->getDirection().x, b->getDirection().y, b->getDirection().z);
	}
    }
}
vec3 Logic::_ballCollision(vec3 pos, vec3 amount)
{
    //collision

    
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
