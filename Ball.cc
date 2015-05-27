#include "Ball.h"
#include <stdio.h>

Ball::Ball()
{
    _position = { 0.0f, 0.0f, 0.0f };

    _direction = { 0.0f, 0.0f, 0.0f };
}
Ball::Ball(vec3 pos, vec3 direction, vec3 speed)
{
    _position = pos;
    _direction = direction;
    _speed = speed;
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
