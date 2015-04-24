#ifndef LOGIC_H
#define LOGIC_H

#include "MathUtils.h"
#include "World.h"
#include "Player.h"

class Logic
{
public:
    Logic();

    ~Logic();

    void update();

    void moveCamera(float amount);
    void moveCameraWithStrafe(float amount);
    void rotateCamera(float angleZ, float angleY);

    vec3 getCameraPos();
    void setCameraPos(vec3 pos);

    vec3 getCameraLookAt();
    vec3 getCameraUp();

    World* getWorld();

    Player* getPlayer();

    void createWorld(GLuint program);

private:
    vec3 _cameraPos;
    vec3 _cameraLookAt;
    vec3 _cameraUp;

    World* _world;

    Player* _player;

    void _gravity();
    void _handleCollision();
    void _checkCollisionAgainstBlock(int index);
};

#endif
