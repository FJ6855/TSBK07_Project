#include "GL_utilities.h"
#include "Renderer.h"
#include "Logic.h"
#include "UserInterface.h"
#include "stdio.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

Logic* logic;
Renderer* renderer;
UserInterface* userInterface;

void run();
void init(int argc, char* argv[]);
void displayTimer(int);
void mouseMovement(int x, int y);

int main(int argc, char* argv[])
{
    init(argc, argv);

    return 0;
}

void init(int argc, char* argv[])
{
  glutInit(&argc, argv);
  glutInitContextVersion(3, 2);
 
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  glutCreateWindow("Project");

  glutDisplayFunc(run);

 
  logic = new Logic();


  renderer = new Renderer(logic);


  logic->createWorld(renderer->getProgram());


  userInterface = new UserInterface(logic);

  initKeymapManager();
  glutTimerFunc(20, &displayTimer, 0);

  glutPassiveMotionFunc(mouseMovement);
 
  glutMainLoop();
}

void displayTimer(int value)
{
  glutPostRedisplay();
  glutTimerFunc(20, &displayTimer, value);
}

void run()
{
    //Input

    //Update
    userInterface->update();

    logic->update();
    
    //render
    renderer->render();
}

void mouseMovement(int x, int y)
{
    userInterface->moveMouse(x, y, (int)WINDOW_WIDTH, (int)WINDOW_HEIGHT);
}
