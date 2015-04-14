#include "UserInterface.h"

UserInterface::UserInterface(Logic* logic)
{
    _logic = logic;

    _movementSpeed = 2.0f;
}

UserInterface::~UserInterface()
{

}

void UserInterface::update()
{
    if (keyIsDown('p'))
    {
	_movementSpeed += 0.1f;
    }

    if (keyIsDown('o') && _movementSpeed > 0.0f)
    {
	_movementSpeed -= 0.1f;
    }	

    if (keyIsDown('w'))
    {
	_logic->moveCamera(_movementSpeed);
    }

    if (keyIsDown('s'))
    {
	_logic->moveCamera(-_movementSpeed);
    }

    if (keyIsDown('a'))
    {
	_logic->moveCameraWithStrafe(-_movementSpeed);
    }
    
    if (keyIsDown('d'))
    {
	_logic->moveCameraWithStrafe(_movementSpeed);
    }
}
