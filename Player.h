#ifndef PLAYER_H
#define PLAYER_H

#include "MathUtils.h"
#include <stdbool.h>

class Player
{
public:
    Player();

    vec3 getPosition();
    void setPosition(vec3 position);

    vec3 getDirection();
    void setDirection(vec3 direction);

    bool isWalking();
    void setWalking(bool walking);

    void setYVel(float velocity);
    float getYVel();

    vec3 getMin();
    vec3 getMax();

    void jump();

    float getCollisionOffset();
    
private:
    vec3 _position;
    vec3 _direction;

    float _yVel;

    bool _walking;

    vec3 _min;
    vec3 _max;
};

#endif
