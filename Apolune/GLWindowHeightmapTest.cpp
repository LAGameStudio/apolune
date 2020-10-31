
#include <Windows.h>
#include "macros.h"
#include "ListItem.h"
#include "LinkedList.h"
#include "GLWindow.h"
#include "GLImage.h"
#include "Crayon.h"
#include "HeightMap.h"
#include "Polygon.h"
#include "Model.h"

#include "GLWindowHeightmapTest.h"

GLWindowHeightmapTest::GLWindowHeightmapTest(void)
{
 background.Int(5,5,5,127);
}


GLWindowHeightmapTest::~GLWindowHeightmapTest(void)
{
}

void GLWindowHeightmapTest::Render(void) {
      // set up viewport and perspective

//   glLoadIdentity();
 glPushMatrix();
 glPushAttrib(GL_ALL_ATTRIB_BITS);
   

    // set up camera, clear screen, and render one 3D cube
   
    // Draw the spherized heightmap

 glPopAttrib();
  glPopMatrix();

}