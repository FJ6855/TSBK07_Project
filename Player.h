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

    vec3 getNextPosition();
    void setNextPosition(vec3 position);

    vec3 getDirection();
    void setDirection(vec3 direction);

    bool getMoving();
    void setMoving(bool moving);

    void jump();
    
private:
    vec3 _position;
    vec3 _nextPosition;
    vec3 _direction;

    bool _moving;
};

#endif
