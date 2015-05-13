#include "Logic.h"

Logic::Logic()
{    
    _freeCam = true;

    _player = new Player();

    _cameraPos = { -10.0f, 30.0f, -20.0f };
    _cameraPos.y += 1.5;
    _cameraLookAt.z = _cameraPos.z + 1;
    _cameraLookAt.x = _cameraPos.x + 1;
    _cameraLookAt.y = _cameraPos.y - 0.5;
    _cameraUp = { 0.0f, 1.0f, 0.0f };

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
    _updateFrustum();

    _world->update();
	
    //printf("%f, %f, %f\n", _player->getPosition().x, _player->getPosition().y, _player->getPosition().z);

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


    //printf("player pos: %f, %f %f\n", _player->getPosition().x, _player->getPosition().y, _player->getPosition().z);
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

void Logic::removeBlock(vec3 pos)
{   
    vec3 position = pos;
    position.y += 1.5;
    vec3 viewVector = Normalize(VectorSub(_cameraLookAt, _cameraPos));
    
    _world->removeBlock(position, viewVector );
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
	    
	    if (index >= 0 && index <= CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_DEPTH - 1)
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
		
	    if (index >= 0 && index <= CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_DEPTH - 1)
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
		
	    if (index >= 0 && index <= CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_DEPTH - 1)
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

void Logic::_updateFrustum()
{
    // Get the direction we are looking
    vec3 viewVector = Normalize(_cameraLookAt - _cameraPos);
    
    // Get the position of the near plane
    vec3 nearPosition = _cameraPos + viewVector * near;
    
    // Get the position of the far plane
    vec3 farPosition = _cameraPos + viewVector * far;
    
    // Get the vector that is orthogonal to the view vector and the up vector
    vec3 rightVector = CrossProduct(viewVector, _cameraUp);

    // Get the positions of the far and near planes edges
    _frustum->farLeftPoint = farPosition - rightVector * (_frustum->farWidth / 2);
    _frustum->farRightPoint = farPosition + rightVector * (_frustum->farWidth / 2);

    _frustum->nearLeftPoint = nearPosition - rightVector * (_frustum->nearWidth / 2);
    _frustum->nearRightPoint = nearPosition + rightVector * (_frustum->nearWidth / 2);

    // Get the edges of the far and near planes
    _frustum->leftEdge = _frustum->farLeftPoint - _frustum->nearLeftPoint;
    _frustum->rightEdge =  _frustum->farRightPoint - _frustum->nearRightPoint;
    _frustum->topEdge = _frustum->farRightPoint - _frustum->farLeftPoint;
    _frustum->bottomEdge = _frustum->nearRightPoint - _frustum->nearLeftPoint;
    
    // Get the normals to the edges of the far and near planes
    _frustum->leftEdgeNormal = Normalize(CrossProduct(_frustum->leftEdge, vec3(0.0, 1.0f, 0.0f)));
    _frustum->rightEdgeNormal = Normalize(CrossProduct(_frustum->rightEdge, vec3(0.0, -1.0f, 0.0f)));
    _frustum->topEdgeNormal = Normalize(CrossProduct(_frustum->topEdge, vec3(0.0, 1.0f, 0.0f)));
    _frustum->bottomEdgeNormal = Normalize(CrossProduct(_frustum->bottomEdge, vec3(0.0, -1.0f, 0.0f)));
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
