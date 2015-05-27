#include "UserInterface.h"
#include "MicroGlut.h"

UserInterface::UserInterface(Logic* logic)
{
    _logic = logic;

    _movementSpeed = 0.1f;

    _activeBlockType = 1;

    _mouseDown = false;
    _keyDown = false;
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
    else if (keyIsDown('o') && _movementSpeed > 0.0f)
    {
	_movementSpeed -= 0.01f;
    }	

    if (keyIsDown('w'))
    {
	_logic->move(_movementSpeed);
    }
    else if (keyIsDown('s'))
    {
	_logic->move(-_movementSpeed);
    }

    if (keyIsDown('a'))
    {
	_logic->strafe(-_movementSpeed);
    }
    else if (keyIsDown('d'))
    {
	_logic->strafe(_movementSpeed);
    }

    if (keyIsDown(' ') && !_keyDown)
    {
	_keyDown = true;

	if(_logic->getPlayer()->isWalking())
	    _logic->getPlayer()->jump();
    }

    _keyDown = keyIsDown(' ');

    if (keyIsDown('f'))
    {
	_logic->setFreeCam(true);
    }
    else if (keyIsDown('g'))
    {
	_logic->setFreeCam(false);
    }

    if (keyIsDown('1'))
    {
	_activeBlockType = 1;
    }
    else if (keyIsDown('2'))
    {	
	_activeBlockType = 2;
    }

    if (glutMouseIsDown(3) && !_mouseDown)
    {
    	_logic->changeBlock(0);
    } 
    else if (glutMouseIsDown(1) && !_mouseDown)
    {
    	_logic->changeBlock(_activeBlockType);
    }

    _mouseDown = (glutMouseIsDown(1) || glutMouseIsDown(3));

}

void UserInterface::moveMouse(int mouseX, int mouseY, int windowWidth, int windowHeight)
{
  int midX = windowWidth / 2;
  int midY = windowHeight / 2;
  
  if (mouseX == midX && mouseY == midY) return;

  glutWarpPointer(midX, midY);

  float angleY = (float)(midX - mouseX) / 2000;
  float angleZ = (float)(midY - mouseY) / 2000;
 
  if (mouseY / windowHeight > 0.2) angleZ = 0.0f;
  
  _logic->rotateCamera(angleZ, -angleY);
}
