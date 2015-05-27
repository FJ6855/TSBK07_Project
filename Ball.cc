#include "Ball.h"
#include <stdio.h>

Ball::Ball()
{
    _position = vec3(0.0f, 0.0f, 0.0f);

    _direction = vec3(0.0f, 0.0f, 0.0f);
}
Ball::Ball(vec3 pos, vec3 direction)
{
    _position = pos + direction * 0.5;
    _direction = direction * 2;
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
