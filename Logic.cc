#include "Logic.h"

Logic::Logic()
{    
    _cameraPos = { 0.0f, 0.0f, 0.0f };

    _cameraLookAt.z = _cameraPos.z - 1;
    _cameraLookAt.x = _cameraPos.x;
    _cameraLookAt.y = _cameraPos.y;

    _cameraUp = { 0.0f, 1.0f, 0.0f };

    _player = new Player();
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
    _gravity();

    // _handleCollision();
}

void Logic::createWorld(GLuint program)
{
    _world = new World(program);
}

void Logic::moveCamera(float amount)
{
    _player->setMoving(true);

    vec3 viewVector = Normalize(VectorSub(_cameraLookAt, _cameraPos));

    vec3 playerPos = _player->getPosition();

    playerPos.x = playerPos.x + viewVector.x * amount;
    playerPos.z = playerPos.z + viewVector.z * amount;

    _player->setNextPosition(playerPos);

    vec3 playerDir = VectorSub(playerPos, _player->getPosition());

    playerDir.x = (playerDir.x > 0.0f) ? ceil(playerDir.x) : floor(playerDir.x);
    playerDir.z = (playerDir.z > 0.0f) ? ceil(playerDir.z) : floor(playerDir.z);

    _player->setDirection(playerDir);

    //printf("player dir x: %f\n", _player->getDirection().x);
    
    _cameraPos = VectorAdd(_cameraPos, ScalarMult(viewVector, amount));
    
    _cameraLookAt.x = _cameraLookAt.x + viewVector.x * amount;
    _cameraLookAt.z = _cameraLookAt.z + viewVector.z * amount;

    //_cameraLookAt = VectorAdd(_cameraLookAt, ScalarMult(viewVector, amount));
}

void Logic::moveCameraWithStrafe(float amount)
{
    _player->setMoving(true);

    vec3 viewVector = Normalize(VectorSub(_cameraLookAt, _cameraPos));

    vec3 orthoViewVector;
    
    orthoViewVector.x = -viewVector.z;
    orthoViewVector.z = viewVector.x;

    vec3 playerPos = _player->getPosition();

    playerPos.x = playerPos.x + orthoViewVector.x * amount;
    playerPos.z = playerPos.z + orthoViewVector.z * amount;

    _player->setNextPosition(playerPos);
    
    
    vec3 playerDir = VectorSub(playerPos, _player->getPosition());

    playerDir.x = (playerDir.x > 0.0f) ? ceil(playerDir.x) : floor(playerDir.x);
    playerDir.z = (playerDir.z > 0.0f) ? ceil(playerDir.z) : floor(playerDir.z);

    _player->setDirection(playerDir);
    
    //_cameraPos.x = _cameraPos.x + orthoViewVector.x * amount;
    //_cameraPos.z = _cameraPos.z + orthoViewVector.z * amount;
    
    _cameraLookAt.x = _cameraLookAt.x + orthoViewVector.x * amount;
    _cameraLookAt.z = _cameraLookAt.z + orthoViewVector.z * amount;
}

void Logic::rotateCamera(float angleZ, float angleY)
{   
  _cameraLookAt.y += angleZ * 2;

  /*if ((_cameraLookAt.y - _cameraPos.y) > 15) 
      _cameraLookAt.y = _cameraPos.y + 15;
  
  if ((_cameraLookAt.y - _cameraPos.y) < -35) 
  _cameraLookAt.y = _cameraPos.y - 35;*/
 
  vec3 viewVector = VectorSub(_cameraLookAt, _cameraPos);

  _cameraLookAt = VectorAdd(_cameraPos, MultVec3(Ry(angleY), viewVector));
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

void Logic::_gravity()
{
    vec3 playerPos = _player->getNextPosition();

    playerPos.y -= 0.3f;

    if (floor(playerPos.y) != floor(_player->getPosition().y))
    {
	Chunk* c = _world->getChunkAtPosition(playerPos);

	if (c != NULL)
	{
	    vec3 relativePlayerPos = VectorSub(playerPos, c->getPos());
		    
	    int index = floor(relativePlayerPos.z) + floor(relativePlayerPos.x) * CHUNK_DEPTH + floor(relativePlayerPos.y) * CHUNK_DEPTH * CHUNK_WIDTH;
	    
	    if (index > 0 && index < CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_DEPTH - 1)
	    {
		if (c->isBlockActive(index))
		{
		    //printf("pos y: %f\n", playerPos.y);
		    //printf("rel player y: %f\n", relativePlayerPos.y);
		    //printf("player y: %f\n", _player->getPosition().y);
		    playerPos.y = floor(_player->getPosition().y);
		}
	    }
	}
    }

    if (_player->getMoving())
    {
	if (floor(playerPos.x + .2f * _player->getDirection().x) != floor(_player->getPosition().x))
	{
	    vec3 tmp = playerPos;
	    
	    tmp.x += .2f * _player->getDirection().x;	    

	    Chunk* c = _world->getChunkAtPosition(tmp);
	    
	    if (c != NULL)
	    {
		vec3 relativePlayerPos = VectorSub(playerPos, c->getPos());
		
		int index = floor(relativePlayerPos.z) + floor(relativePlayerPos.x + .2f * _player->getDirection().x) * CHUNK_DEPTH + floor(relativePlayerPos.y) * CHUNK_DEPTH * CHUNK_WIDTH;
		
		if (index > 0 && index < CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_DEPTH - 1)
		{
		    if (c->isBlockActive(index))
		    {
			//printf("index in x: %i\n", index);
			//printf("player dir x: %f\n", _player->getDirection().x);
			//printf("player x: %f\n", _player->getPosition().x);

			playerPos.x = floor(_player->getPosition().x);

			if (_player->getDirection().x == 1.0f)
			    playerPos.x += 0.8f;
			else 
			    playerPos.x += 0.2f;
		    }
		}
	    }
	}
	
	if (floor(playerPos.z + .2f * _player->getDirection().z) != floor(_player->getPosition().z))
	{	    
	    vec3 tmp = playerPos;
	    
	    tmp.z += .2f * _player->getDirection().z;	    

	    Chunk* c = _world->getChunkAtPosition(tmp);
	    
	    if (c != NULL)
	    {
		vec3 relativePlayerPos = VectorSub(playerPos, c->getPos());
		
		int index = floor(relativePlayerPos.z + .2f * _player->getDirection().z) + floor(relativePlayerPos.x) * CHUNK_DEPTH + floor(relativePlayerPos.y) * CHUNK_DEPTH * CHUNK_WIDTH;
		
		if (index > 0 && index < CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_DEPTH - 1)
		{
		    if (c->isBlockActive(index))
		    {
			//printf("index in z: %i\n", index);
			//printf("player dir z: %f\n", _player->getDirection().z)
			//printf("player z: %f\n", _player->getPosition().z);

			playerPos.z = floor(_player->getPosition().z);

			if (_player->getDirection().z == 1.0f)
			    playerPos.z += 0.8f;
			else 
			    playerPos.z += 0.2f;
		    }
		}
	    }

	 }
    }
    
    _player->setPosition(playerPos);
    _player->setNextPosition(playerPos);

    //printf("player pos: %f, %f, %f\n", _player->getPosition().x, _player->getPosition().y, _player->getPosition().z); 

    _cameraPos = _player->getPosition();

    _cameraPos.y += 1.0f;

    //_cameraLookAt.y = _cameraPos.y;
}

void Logic::_handleCollision()
{
    Chunk* c = _world->getChunkAtPosition(_player->getPosition());

    //printf("-----------------------\n");

    //printf("player pos: %f, %f, %f\n", _player->getPosition().x, _player->getPosition().y, _player->getPosition().z);
    
    if (c != NULL)
    {  
	vec3 playerPos = VectorSub(_player->getPosition(), c->getPos());
		    
	int playerAtIndex = (floor(playerPos.z) + 1) + (floor(playerPos.x) + 1) * CHUNK_DEPTH + (floor(playerPos.y) + 1) * CHUNK_DEPTH * CHUNK_WIDTH;

	for (int y = -1; y < 2; y++)
	{
	    for (int x = -1; x < 2; x++)
	    {
		for (int z = -1; z < 2; z++)
		{		    
		    int index = (floor(playerPos.z) + z) + (floor(playerPos.x) + x) * CHUNK_DEPTH + (floor(playerPos.y) + y) * CHUNK_DEPTH * CHUNK_WIDTH;

		    if (index < 0)
			index = 0;
		    else if (index > CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_DEPTH - 1)
			index = CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_DEPTH - 1;

		    if (c->isBlockActive(index))
		    {
			

			return;
		    }
		}
	    }
	}

	
    }

    //printf("-----------------------\n");
}
