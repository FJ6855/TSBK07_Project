#ifndef LOGIC_H
#define LOGIC_H

#include "MathUtils.h"
#include "World.h"
#include "Player.h"
#include "Frustum.h"
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

    vec3 getTargetPosition();
    bool getRenderTarget();

    World* getWorld();

    Player* getPlayer();

    void createWorld(GLuint program);

    void setFreeCam(bool value);

    void changeBlock(int blockType);
  
private:
    bool _freeCam;
   
    vec3 _cameraPos;
    vec3 _cameraLookAt;
    vec3 _cameraUp;

    vec3 _targetPosition;
    bool _renderTarget;

    vec3 _forward;

    World* _world;

    Player* _player;

    Frustum* _frustum;

    void _updateFrustum();

    vec3 _collision(vec3 oldPos, vec3 newPos);

    void _physics();

    void _updateTargetPosition();
};

#endif
