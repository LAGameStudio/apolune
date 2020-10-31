#include <Windows.h>

#include "GLWindow.h"
#include "GLWindowGLTest.h"


GLWindowGLTest::GLWindowGLTest(void)
{
 xcam=ycam=0.0; zcam = 10.0;   // initial position of camera
}


GLWindowGLTest::~GLWindowGLTest(void)
{
}

void GLWindowGLTest::Render(void) {
      // set up viewport and perspective

//   glLoadIdentity();

   glMatrixMode(GL_PROJECTION);
   gluPerspective(90.0, (float) this->w / (float) this->h, 1, 100.0);

   glMatrixMode(GL_MODELVIEW);
   glEnable(GL_DEPTH_TEST);
    // set up camera, clear screen, and render one 3D cube

   gluLookAt(xcam,ycam,zcam, 0,0,0, 0,1,0);  // eye, origin, up
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glEnable(GL_CULL_FACE);  // remove hidden faces
   glBegin(GL_QUADS);

      glColor3f(1,0,0);       // red
      glVertex3f( 3, 3,-3);   // face 1
      glVertex3f(-3, 3,-3);
      glVertex3f(-3, 3, 3);
      glVertex3f( 3, 3, 3);

      glColor3f(0,1,0);       // green
      glVertex3f( 3,-3, 3);   // face 2
      glVertex3f(-3,-3, 3);
      glVertex3f(-3,-3,-3);
      glVertex3f( 3,-3,-3);

      glColor3f(0,0,1);       // blue
      glVertex3f( 3, 3, 3);   // face 3
      glVertex3f(-3, 3, 3);
      glVertex3f(-3,-3, 3);
      glVertex3f( 3,-3, 3);

      glColor3f(1,1,0);       // red/green
      glVertex3f( 3,-3,-3);   // face 4
      glVertex3f(-3,-3,-3);
      glVertex3f(-3, 3,-3);
      glVertex3f( 3, 3,-3);

      glColor3f(0,1,1);       // green/blue
      glVertex3f(-3, 3, 3);   // face 5
      glVertex3f(-3, 3,-3);
      glVertex3f(-3,-3,-3);
      glVertex3f(-3,-3, 3);

      glColor3f(1,0,1);       // red/blue
      glVertex3f( 3, 3,-3);   // face 6
      glVertex3f( 3, 3, 3);
      glVertex3f( 3,-3, 3);
      glVertex3f( 3,-3,-3);

   glEnd();
   glDisable(GL_CULL_FACE);
   
}