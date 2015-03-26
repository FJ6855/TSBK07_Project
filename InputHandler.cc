#include "InputHandler.h"

InputHandler::InputHandler()
{
	for (int i = 0; i < NUM_KEYS; ++i)
	{
		_keysDown[i] = false;
		_keysDownOld[i] = false;
		_keysPressed[i] = false;
	}
}

InputHandler::~InputHandler()
{

}

void InputHandler::handleInput(SDL_Event* e)
{
	if (e->type == SDL_KEYDOWN)
	{
		_keysDown[e->key.keysym.scancode] = true;
		if (_keysDownOld[e->key.keysym.scancode] == false)
		{
			_keysPressed[e->key.keysym.scancode] = true;
		}
	}
	else if (e->type == SDL_KEYUP)
	{
		_keysDown[e->key.keysym.scancode] = false;
	}
	else if (e->type == SDL_MOUSEMOTION)
	{
		_mouseRelX = e->motion.xrel;
		_mouseRelY = e->motion.yrel;
		_mouseX = e->motion.x;
		_mouseY = e->motion.y;
	}

	for (int i = 0; i < NUM_KEYS; ++i)
		_keysDownOld[i] = _keysDown[i];
}

void InputHandler::reset()
{
	_mouseRelX = 0;
	_mouseRelY = 0;
	for (int i = 0; i < NUM_KEYS; ++i)
		_keysPressed[i] = false;
}

bool InputHandler::getKeyDown(int key)
{
	return _keysDown[key];
}

bool InputHandler::getKeyPressed(int key)
{
	return _keysPressed[key];
}

float InputHandler::getMouseRelX()
{
	return _mouseRelX;
}

float InputHandler::getMouseRelY()
{
	return _mouseRelY;
}

float InputHandler::getMouseX()
{
	return _mouseX;
}

float InputHandler::getMouseY()
{
	return _mouseY;
}
