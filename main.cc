#include "GL_utilities.h"
#include <vector>
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

    while(true)
    {
	run();
    }
    return 0;
}

void init(int argc, char* argv[])
{
  glutInit(&argc, argv);
  glutInitContextVersion(3, 2);
 
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  glutCreateWindow("Project");
  glEnable(GL_CULL_FACE);
  glutDisplayFunc(run);

  logic = new Logic();

  renderer = new Renderer(logic);

  logic->createWorld(renderer->getProgram());

  userInterface = new UserInterface(logic);

  initKeymapManager();
  glutTimerFunc(1, &displayTimer, 0);

  glutPassiveMotionFunc(mouseMovement);
 
  glutMainLoop();
}

void displayTimer(int value)
{
  glutPostRedisplay();
  glutTimerFunc(1, &displayTimer, value);
}

int frame=0,time2,timebase=0;
float fps=0.0f;

void run()
{
    //Input

    //Update
    userInterface->update();

    logic->update();
    
    //render
    renderer->render();

    frame++;
    time2 = glutGet(GLUT_ELAPSED_TIME);

    if (time2 - timebase > 1000) 
    {
	fps = frame * 1000.0 / (time2 - timebase);
	timebase = time2;
	frame = 0;
    }

    printf("fps: %f\n", fps);
}

void mouseMovement(int x, int y)
{
    userInterface->moveMouse(x, y, (int)WINDOW_WIDTH, (int)WINDOW_HEIGHT);
}
