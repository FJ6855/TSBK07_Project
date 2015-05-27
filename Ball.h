#ifndef BALL_H
#define BALL_H

#include "MathUtils.h"
#include <stdbool.h>

class Ball
{
public:
    Ball();
    Ball(vec3 pos, vec3 direction);

    vec3 getPosition();
    void setPosition(vec3 position);

    vec3 getDirection();
    void setDirection(vec3 direction);

private:
    vec3 _position;
    vec3 _direction;
    vec3 _speed;
};

#endif
