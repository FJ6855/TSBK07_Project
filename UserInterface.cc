#include "UserInterface.h"

UserInterface::UserInterface(Logic* logic)
{
    _logic = logic;

    _movementSpeed = 0.1f;
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

void UserInterface::moveMouse(int mouseX, int mouseY, int windowWidth, int windowHeight)
{
  int midX = windowWidth / 2;
  int midY = windowHeight / 2;
  
  if (mouseX == midX && mouseY == midY) return;

  glutWarpPointer(midX, midY);

  float angleY = (float)(midX - mouseX) / 5000;
  float angleZ = (float)(midY - mouseY) / 5000;
  
  _logic->rotateCamera(angleZ, -angleY);
}