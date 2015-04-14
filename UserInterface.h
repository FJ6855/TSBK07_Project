#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include "GL_utilities.h"
#include "Logic.h"

class UserInterface
{
public:
    UserInterface(Logic* logic);
    
    ~UserInterface();

    void update();

private:
    Logic* _logic;

    float _movementSpeed;
};

#endif
