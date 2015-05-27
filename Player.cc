#include "Player.h"
#include <stdio.h>

Player::Player()
{
    _position = vec3(5 * 16 + 1.0f, 20.0f, 5 * 16 + 1.0f);

    _direction = vec3(0.0f, 0.0f, 0.0f);

    _yVel = 0;

    _walking = false;

    _min = vec3(_position.x - 0.4f, _position.y - 1.5f, _position.z - 0.4f);
    _max = vec3(_min.x + 0.8f, _position.y, _min.z + 0.8f);
}

vec3 Player::getPosition()
{
    return _position;
}

void Player::setPosition(vec3 position)
{
    _position = position;

    _min = vec3(_position.x - 0.4f, _position.y - 1.5f, _position.z - 0.4f);
    _max = vec3(_min.x + 0.8f, _position.y, _min.z + 0.8f);
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

vec3 Player::getMin()
{
    return _min;
}

vec3 Player::getMax()
{
    return _max;
}

