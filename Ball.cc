#include "Ball.h"
#include <stdio.h>

Ball::Ball()
{
    _position = { 0.0f, 0.0f, 0.0f };

    _direction = { 0.0f, 0.0f, 0.0f };
}
Ball::Ball(vec3 pos, vec3 lookAt)
{
    _direction = Normalize(lookAt);
    
    lookAt = Normalize(VectorSub(lookAt, pos));
    
    _position.x = pos.x + lookAt.x;
    _position.y = pos.y + 1.0f;
    _position.z = pos.z + lookAt.z;

}

vec3 Ball::getPosition()
{
    return _position;
}

void Ball::setPosition(vec3 position)
{
    _position = position;
}

vec3 Ball::getDirection()
{
    return _direction;
}

void Ball::setDirection(vec3 direction)
{
    _direction = direction;
}
