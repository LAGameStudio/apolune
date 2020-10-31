#include "LookAtPerspective.h"

void SimpleCube::Render() {
 int i;
 glColor4f(1.0f,0.0f,1.0f,1.0f);
 for (i = 0; i < 6; i++) {
   glBegin(GL_QUADS);
   glNormal3fv(&n[i][0]);
   glVertex3fv(&v[faces[i][0]][0]);
   glVertex3fv(&v[faces[i][1]][0]);
   glVertex3fv(&v[faces[i][2]][0]);
   glVertex3fv(&v[faces[i][3]][0]);
   glEnd();
 }
}