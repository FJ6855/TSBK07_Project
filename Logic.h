#ifndef LOGIC_H
#define LOGIC_H

#include "MathUtils.h"
#include "World.h"
#include "Player.h"
#include "Ball.h"

class Logic
{
public:
    Logic();

    ~Logic();

    void update();

    void move(float amount);
    void moveCamera(float amount);
    void movePlayer(float amount);
    void movePlayerY(float amount);

    void strafe(float amount);
    void strafeCamera(float amount);
    void strafePlayer(float amount);

    void rotateCamera(float angleZ, float angleY);

    vec3 getCameraPos();
    void setCameraPos(vec3 pos);

    vec3 getCameraLookAt();
    vec3 getCameraUp();

    World* getWorld();

    Player* getPlayer();

    Ball* getBall(int index);

    void createWorld(GLuint program);

    void setFreeCam(bool value);

    void removeBlock();

    void shootBall();    
  
private:

    bool _freeCam;
   
    vec3 _cameraPos;
    vec3 _cameraLookAt;
    vec3 _cameraUp;

    vec3 _forward;

    World* _world;

    Player* _player;

    Ball* _balls[15];

    int _ballIndex;

    vec3 _collision(vec3 oldPos, vec3 newPos);
    void _physics();

    void _ballMovement();
    vec3 _ballCollision(vec3 pos, vec3 amount);
};

#endif
