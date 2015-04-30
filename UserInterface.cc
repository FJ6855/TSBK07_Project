#include "UserInterface.h"

UserInterface::UserInterface(Logic* logic)
{
    _logic = logic;

    _movementSpeed = 0.01f;
}

UserInterface::~UserInterface()
{

}

void UserInterface::update()
{
    if (keyIsDown('p'))
    {
	_movementSpeed += 0.01f;
    }

    if (keyIsDown('o') && _movementSpeed > 0.0f)
    {
	_movementSpeed -= 0.01f;
    }	

    if (keyIsDown('w'))
    {
	_logic->move(_movementSpeed);
    }

    if (keyIsDown('s'))
    {
	_logic->move(-_movementSpeed);
    }

    if (keyIsDown('a'))
    {
	_logic->strafe(-_movementSpeed);
    }
    
    if (keyIsDown('d'))
    {
	_logic->strafe(_movementSpeed);
    }

    if (keyIsDown(' '))
    {
	if(_logic->getPlayer()->isWalking())
	    _logic->getPlayer()->jump();
    }

    if (keyIsDown('f'))
    {
	_logic->setFreeCam(true);
    }

    if (keyIsDown('g'))
    {
	_logic->setFreeCam(false);
    }
}

void UserInterface::moveMouse(int mouseX, int mouseY, int windowWidth, int windowHeight)
{
  int midX = windowWidth / 2;
  int midY = windowHeight / 2;
  
  if (mouseX == midX && mouseY == midY) return;

  glutWarpPointer(midX, midY);

  float angleY = (float)(midX - mouseX) / 2000;
  float angleZ = (float)(midY - mouseY) / 500;
  
  _logic->rotateCamera(angleZ, -angleY);
}
