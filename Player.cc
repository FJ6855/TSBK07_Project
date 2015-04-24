#include "Player.h"
#include <stdio.h>

Player::Player()
{
    _position = { 20.0f, 20.0f, 20.0f };
    _nextPosition = _position;
}

vec3 Player::getPosition()
{
    return _position;
}

void Player::setPosition(vec3 position)
{
    _position = position;
}

vec3 Player::getNextPosition()
{
    return _nextPosition;
}

void Player::setNextPosition(vec3 position)
{
    _nextPosition = position;
}

vec3 Player::getDirection()
{
    return _direction;
}

void Player::setDirection(vec3 direction)
{
    _direction = direction;
}

bool Player::getMoving()
{
    return _moving;
}

void Player::setMoving(bool moving)
{
    _moving = moving;
}

void Player::jump()
{
    _nextPosition.y += 0.8f;
}
