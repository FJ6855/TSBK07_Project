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

    Ball* getBall(int index);

    void createWorld(GLuint program);

    void setFreeCam(bool value);

    void changeBlock(int blockType);

    void shootBall();
  
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

    Ball* _balls[100];

    int _ballIndex;

    void _updateFrustum();

    void _collision(vec3 oldPos, vec3& newPos);

    void _physics();

    void _updateTargetPosition();

    void _ballMovement();
    void _ballCollision(vec3 oldPos, vec3& newPos, vec3& newDir);
    bool _ballToBallCollision(vec3 pos1, vec3 pos2, vec3& dir1, vec3& dir2);
    
    void _getChunksAroundChunk(vec3 pos, vec3 direciton, Chunk* chunks[4]);
};

#endif
