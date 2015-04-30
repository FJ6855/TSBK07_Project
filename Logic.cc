#include "Logic.h"

Logic::Logic()
{    
    _freeCam = false;

    _player = new Player();
    _cameraLookAt.z = _cameraPos.z - 1;
    _cameraLookAt.x = _cameraPos.x;
    _cameraLookAt.y = _cameraPos.y;
    _cameraUp = { 0.0f, 1.0f, 0.0f };
    _cameraPos = _player->getPosition();
    _cameraPos.y += 1.5;
}

Logic::~Logic()
{
    delete _world;
    _world = NULL;

    delete _player;
    _player = NULL;
}

void Logic::update()
{
    _physics();
}

void Logic::createWorld(GLuint program)
{
    _world = new World(program);
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
    _cameraLookAt = VectorAdd(_cameraPos, viewVector );
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

vec3 Logic::_collision(vec3 oldPos, vec3 newPos)
{
    if (floor(newPos.y) != floor(oldPos.y))
    {
	Chunk* c = _world->getChunkAtPosition(newPos);

	if (c != NULL)
	{
	    vec3 relativePlayerPos = VectorSub(newPos, c->getPos());
		    
	    int index = floor(relativePlayerPos.z) + floor(relativePlayerPos.x) * 
		CHUNK_DEPTH + floor(relativePlayerPos.y) * CHUNK_DEPTH * CHUNK_WIDTH;
	    
	    if (index > 0 && index < CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_DEPTH - 1)
	    {
		if (c->isBlockActive(index))
		{
		    newPos.y = floor(oldPos.y);
		    _player->setYVel(0.0f);
		    _player->setWalking(true);
		}
	    }
	}
    }    
    
    vec3 playerDirection = VectorSub(newPos, oldPos);

    playerDirection.x = (playerDirection.x > 0.0f) ? ceil(playerDirection.x) : floor(playerDirection.x);
    playerDirection.z = (playerDirection.z > 0.0f) ? ceil(playerDirection.z) : floor(playerDirection.z);

    _player->setDirection(playerDirection);

    if (floor(newPos.x + .2f * playerDirection.x) != floor(oldPos.x))
    {
	vec3 tmp = newPos;
	    
	tmp.x += .2f * playerDirection.x;	    

	Chunk* c = _world->getChunkAtPosition(tmp);
	    
	if (c != NULL)
	{
	    vec3 relativePlayerPos = VectorSub(newPos, c->getPos());
		
	    int index = floor(relativePlayerPos.z) + floor(relativePlayerPos.x + .2f * playerDirection.x) *
		CHUNK_DEPTH + floor(relativePlayerPos.y) * CHUNK_DEPTH * CHUNK_WIDTH;
		
	    if (index > 0 && index < CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_DEPTH - 1)
	    {
		if (c->isBlockActive(index))
		{	 
		    newPos.x = floor(oldPos.x);

		    if (playerDirection.x == 1.0f)
			newPos.x += 0.8f;
		    else 
			newPos.x += 0.2f;
		}
	    }
	}
    }
	
    if (floor(newPos.z + .2f * playerDirection.z) != floor(oldPos.z))
    {	    
	vec3 tmp = newPos;
	    
	tmp.z += .2f * playerDirection.z;	    

	Chunk* c = _world->getChunkAtPosition(tmp);
	    
	if (c != NULL)
	{
	    vec3 relativePlayerPos = VectorSub(newPos, c->getPos());
		
	    int index = floor(relativePlayerPos.z + .2f * playerDirection.z) + floor(relativePlayerPos.x) * 
		CHUNK_DEPTH + floor(relativePlayerPos.y) * CHUNK_DEPTH * CHUNK_WIDTH;
		
	    if (index > 0 && index < CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_DEPTH - 1)
	    {
		if (c->isBlockActive(index))
		{

		    newPos.z = floor(oldPos.z);

		    if (playerDirection.z == 1.0f)
			newPos.z += 0.8f;
		    else 
			newPos.z += 0.2f;
		}
	    }
	}

    }

    return newPos;
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
