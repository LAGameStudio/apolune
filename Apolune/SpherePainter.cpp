#include "SpherePainter.h"

SearchableSphereTextureDefinitionsCollection sphereTextureLibrary;

SearchableSphereTextureDefinitionsCollection solsystemSphereTextureLibrary;

void SpherePlacement::Render() {
 Blending(stroke->blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, stroke->image->texture);

 //printf( "%f,%f,%f,%f\n", t[0], t[1], t[2], t[3] );
 glColor4fv(stroke->tint.floats); 
 glBegin(GL_QUADS);

 glTexCoord2d(0.0f, 0.0f);  glVertex2i(this->xywh.x,  this->xywh.y);
 glTexCoord2d(1.0f, 0.0f);  glVertex2i(this->xywh.x2, this->xywh.y);
 glTexCoord2d(1.0f, 1.0f);  glVertex2i(this->xywh.x2, this->xywh.y2);
 glTexCoord2d(0.0f, 1.0f);  glVertex2i(this->xywh.x,  this->xywh.y2);
 
 glEnd();
 glDisable(GL_TEXTURE_2D);
 glDisable(GL_BLEND);
}