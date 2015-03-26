#include "GL_utilities.h"
#include "InputHandler.h"
#include "Renderer.h"

InputHandler* inputHandler;
Renderer* renderer;

void run();
void init();


int main(int argc, char* argv[])
{
    init();
    return 0;
}

void init()
{
  glutInit(&argc, argv);
  glutInitContextVersion(3, 2);
  glutCreateWindow("GL3 white triangle example");
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
