#include "GL_utilities.h"
#include "InputHandler.h"
#include "Renderer.h"

InputHandler* inputHandler;
Renderer* renderer;

void run();
void init(int argc, char* argv[]);


int main(int argc, char* argv[])
{
    init(argc, argv);

    return 0;
}

void init(int argc, char* argv[])
{
  glutInit(&argc, argv);
  glutInitContextVersion(3, 2);
  glutCreateWindow("Project");
  glutDisplayFunc(run);
  
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glClearColor(0.2,0.2,0.5,0);
  glEnable(GL_DEPTH_TEST);
  
  inputHandler = new InputHandler();
  renderer = new Renderer();
  
  initKeymapManager();
  glutMainLoop();
}


void run()
{
    //Input

    //Update
    
    //render
}
