#include "Player.h"
#include <stdio.h>

Player::Player()
{
    _position = vec3(15.0f, 20.0f, 15.0f);
    _direction = vec3(0.0f, 0.0f, 0.0f);
    _yVel = 0;
    _walking = false;
    _collisionOffset = 0.2f;
}

vec3 Player::getPosition()
{
    return _position;
}

void Player::setPosition(vec3 position)
{
    _position = position;
}

vec3 Player::getDirection()
{
    return _direction;
}

void Player::setDirection(vec3 direction)
{
    _direction = direction;
}

bool Player::isWalking()
{
    return _walking;
}

void Player::setWalking(bool walking)
{
    _walking = walking;
}

void Player::jump()
{
    _yVel += 0.3;
}

void Player::setYVel(float velocity)
{
    _yVel = velocity;
}

float Player::getYVel()
{
    return _yVel;
}

float Player::getCollisionOffset()
{
    return _collisionOffset;
}

