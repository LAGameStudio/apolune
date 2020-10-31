#include "Graphics.h"
#include "Display.h"
#include "GLSetup.h"
#include "RoundedRectangle.h"

VirtualGraphics vg;


void VirtualGraphics::Resize( int w, int h ) {
 // Called once display is running
 resolution.SetRect( 0.0,0.0,(double)w,(double)h );
 pixelsPerUnit.Set(
  (double)display.w/(double)w,
  (double)display.h/(double)h   // How many display pixels per virtual ones?
 );
 this->w=w;
 this->h=h;
 this->wd=(double)wd;
 this->hd=(double)hd;
 if ( fbo1.created ) fbo1.Dispose(); fbo1.Create(w,h);
 if ( fbo2.created ) fbo2.Dispose(); fbo2.Create(w,h);
 if ( fbo3.created ) fbo3.Dispose(); fbo3.Create(w,h);
 if ( fbo4.created ) fbo4.Dispose(); fbo4.Create(w,h);
}

void VirtualGraphics::Start() {
 glPushMatrix();
 glScaled(pixelsPerUnit.x,pixelsPerUnit.y,1.0);
}


void VirtualGraphics::End() {
 glPopMatrix();
}

void VirtualGraphics::LineWidth( double d ) {
 if ( d == 1.0 ) glLineWidth(1.0);
 else {
  double setting=d*pixelsPerUnit.x;
  if ( d < 1.0 ) d=1.0;
  glLineWidth((GLfloat)d);
 }
}

void VirtualGraphics::DrawLine(Crayon color, double x1, double y1, double x2, double y2)
{
 glColor4fv(color.floats);
 glBegin(GL_LINES);
 glVertex2d(x1, y1);
 glVertex2d(x2, y2);
 glEnd();
}

void VirtualGraphics::DrawLine(Crayon color, int x1, int y1, int x2, int y2)
{
 glColor4fv(color.floats);
 glBegin(GL_LINES);
 glVertex2i(x1, y1);
 glVertex2i(x2, y2);
 glEnd();
}

void VirtualGraphics::DrawLine2Color(Crayon color1, Crayon color2, int x1, int y1, Zint x2, Zint y2) { DrawLine2Color(color1,color2,x1,y1,(int)x2,(int)y2); }
void VirtualGraphics::DrawLine2Color(Crayon color1, Crayon color2, int x1, Zint y1, int x2, Zint y2) { DrawLine2Color(color1,color2,x1,(int)y1,x2,(int)y2); }
void VirtualGraphics::DrawLine2Color(Crayon color1, Crayon color2, Zint x1, int y1, Zint x2, int y2) { DrawLine2Color(color1,color2,(int)x1,y1,(int)x2,y2); }
void VirtualGraphics::DrawLine2Color(Crayon color1, Crayon color2, Zint x1, Zint y1, Zint x2, Zint y2) { DrawLine2Color(color1,color2,(int)x1,(int)y1,(int)x2,(int)y2); }
void VirtualGraphics::DrawLine2Color(Crayon color1, Crayon color2, int x1, int y1, int x2, int y2)
{
 glBegin(GL_LINES);
 glColor4fv(color1.floats);
 glVertex2i(x1, y1);
 glColor4fv(color2.floats);
 glVertex2i(x2, y2);
 glEnd();
}

void VirtualGraphics::DrawLine2Color(Crayon color1, Crayon color2, Zfloat x1, Zfloat y1, Zfloat x2, Zfloat y2) { DrawLine2Color(color1,color2,(float)x1,(float)y1,(float)x2,(float)y2); }
void VirtualGraphics::DrawLine2Color(Crayon color1, Crayon color2, float x1, float y1, float x2, float y2)
{
 glBegin(GL_LINES);
 glColor4fv(color1.floats);
 glVertex2f(x1 + 0.5f, y1 + 0.5f);
 glColor4fv(color2.floats);
 glVertex2f(x2 + 0.5f, y2 + 0.5f);
 glEnd();
}

void VirtualGraphics::DrawLine2Color(Crayon color1, Crayon color2, Zdouble x1, Zdouble y1, Zdouble x2, Zdouble y2) { DrawLine2Color(color1,color2,(double)x1,(double)y1,(double)x2,(double)y2); }
void VirtualGraphics::DrawLine2Color(Crayon color1, Crayon color2, double x1, double y1, double x2, double y2)
{
 glBegin(GL_LINES);
 glColor4fv(color1.floats);
 glVertex2d(x1 + 0.5, y1 + 0.5);  // why add 0.5? ...
 glColor4fv(color2.floats);
 glVertex2d(x2 + 0.5, y2 + 0.5);
 glEnd();
}

void VirtualGraphics::DrawLine2Color3d(Crayon color1, Crayon color2, double x1, double y1, double z1, double x2, double y2, double z2 ) {
 glBegin(GL_LINES);
 glColor4fv(color1.floats);
 glVertex3d(x1,y1,z1);
 glColor4fv(color2.floats);
 glVertex3d(x2,y2,z2);
 glEnd();
}

void VirtualGraphics::DrawLine3d2Color(Crayon color1, Crayon color2, Vertexd *source, Vertexd *terminus )
{
 glBegin(GL_LINES);
 glColor4fv(color1.floats);
 glVertex3d(source->x,source->y,source->z);
 glColor4fv(color2.floats);
 glVertex3d(terminus->x,terminus->y,terminus->z);
 glEnd();
}

void VirtualGraphics::Rectangle(Crayon color, int x, Zint y, Zint x2, Zint y2 ) { Rectangle(color,x,(int)y,(int)x2,(int)y2); }
void VirtualGraphics::Rectangle(Crayon color, int x, int y, Zint x2, Zint y2 ) { Rectangle(color,x,y,(int)x2,(int)y2); }
void VirtualGraphics::Rectangle(Crayon color, int x, int y, int x2, Zint y2 ) { Rectangle(color,x,y,x2,(int)y2); }
void VirtualGraphics::Rectangle(Crayon color, Zint x, Zint y, Zint x2, Zint y2 ) { Rectangle(color,(int)x,(int)y,(int)x2,(int)y2); }
void VirtualGraphics::Rectangle(Crayon color,int x, int y, int x2, int y2) {
 glColor4fv(color.floats);
 glBegin(GL_LINE_LOOP);
 glVertex2i(x  /* + 0.5f*/, y /* + 0.5f*/);
 glVertex2i(x  /* + 0.5f*/, y2/* + 0.5f*/);
 glVertex2i(x2 /* + 0.5f*/, y2/* + 0.5f*/);
 glVertex2i(x2 /* + 0.5f*/, y /* + 0.5f*/);
 glEnd();
}

void VirtualGraphics::Rectangle(Crayon color,double x, Zdouble y, Zdouble x2, Zdouble y2 ) { Rectangle(color,x,(double)y,(double)x2,(double)y2); }
void VirtualGraphics::Rectangle(Crayon color,double x, double y, Zdouble x2, Zdouble y2 ) { Rectangle(color,x,y,(double)x2,(double)y2); }
void VirtualGraphics::Rectangle(Crayon color,double x, double y, double x2, Zdouble y2 ) { Rectangle(color,x,y,x2,(double)y2); }
void VirtualGraphics::Rectangle(Crayon color,Zdouble x, Zdouble y, Zdouble x2, Zdouble y2 ) { Rectangle(color,(double)x,(double)y,(double)x2,(double)y2); }
void VirtualGraphics::Rectangle(Crayon color,double x, double y, double x2, double y2) {
 glColor4fv(color.floats);
 glBegin(GL_LINE_LOOP);
 glVertex2d(x  /* + 0.5f*/, y /* + 0.5f*/);
 glVertex2d(x  /* + 0.5f*/, y2/* + 0.5f*/);
 glVertex2d(x2 /* + 0.5f*/, y2/* + 0.5f*/);
 glVertex2d(x2 /* + 0.5f*/, y /* + 0.5f*/);
 glEnd();
}

void VirtualGraphics::Rectangle(Crayon color, Cartesian *rect) {
 glColor4fv(color.floats);
 glBegin(GL_LINE_LOOP);
 glVertex2i(rect->x /* + 0.5f*/, rect->y /* + 0.5f*/);
 glVertex2i(rect->x /* + 0.5f*/, rect->y2/* + 0.5f*/);
 glVertex2i(rect->x2/* + 0.5f*/, rect->y2/* + 0.5f*/);
 glVertex2i(rect->x2/* + 0.5f*/, rect->y /* + 0.5f*/);
 glEnd();
}

void VirtualGraphics::Area(Crayon color, int x1, Zint y1, Zint x2, Zint y2 ) { Area(color,x1,(int)y1,(int)x2,(int)y2); }
void VirtualGraphics::Area(Crayon color, int x1, int y1, Zint x2, Zint y2 ) { Area(color,x1,y1,(int)x2,(int)y2); }
void VirtualGraphics::Area(Crayon color, int x1, int y1, int x2, Zint y2 ) { Area(color,x1,y1,x2,(int)y2); }
void VirtualGraphics::Area(Crayon color, Zint x1, Zint y1, Zint x2, Zint y2 ) { Area(color,(int)x1,(int)y1,(int)x2,(int)y2); }
void VirtualGraphics::Area(Crayon color, int x1,int y1,int x2,int y2)
{
 glColor4fv((color.floats));
 glRecti(x1,y1,x2/*+1*/,y2/*+1*/);
 /* glRectf(x1+GLWindow *.offset.x, y1+GLWindow *.offset.y, x2+GLWindow *.offset.x + 1, y2+GLWindow *.offset.y + 1); */
}
void VirtualGraphics::Area(Crayon color, double x1,double y1,double x2,double y2)
{
 glColor4fv((color.floats));
 glRectd(x1,y1,x2/*+1*/,y2/*+1*/);
 /* glRectf(x1+GLWindow *.offset.x, y1+GLWindow *.offset.y, x2+GLWindow *.offset.x + 1, y2+GLWindow *.offset.y + 1); */
}

void VirtualGraphics::Area(Crayon color, Cartesian *rect ) {
 glColor4fv((color.floats));
 glRecti(rect->x,rect->y,rect->x2,rect->y2);
}

void VirtualGraphics::Area(GLImage *texture, Crayon color, Blends blend, ScreenPosition *screen) {
 Stretchi(texture,color,1.0f,blend, screen->x,screen->y,screen->w,screen->h);
}

void VirtualGraphics::BlendArea(Crayon color, Blends blend, int x1,int y1,int x2,int y2)
{
 Blending(blend);
 glColor4fv((color.floats));
 glRecti(x1,y1,x2,y2);
}

void VirtualGraphics::Stretchi( GLImage * source, Crayon tint, float alpha, Blends blend, int x, int y, int w, int h ) {

#if defined(GL_DEBUG)
 GL_Assert( "[entering Stretchi()]" );
 glReportError( glGetError() );
 GL_Assert( "Stretchi()" );
#endif

 glPushAttrib(GL_ALL_ATTRIB_BITS);
 glDisable(GL_DEPTH_TEST);
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);

 //printf( "%f,%f,%f,%f\n", t[0], t[1], t[2], t[3] );
 tint.ScaleByAlphaf(alpha);
 glColor4fv(tint.floats); 
 glBegin(GL_QUADS);
 if(h < 0)
 {
  glTexCoord2f(0.0f, 1.0f);  glVertex2i(x,   y);
  glTexCoord2f(1.0f, 1.0f);  glVertex2i(x+w, y);
  glTexCoord2f(1.0f, 0.0f);  glVertex2i(x+w, y-h);
  glTexCoord2f(0.0f, 0.0f);  glVertex2i(x,   y-h);
 }
 else
 {
  glTexCoord2f(0.0f, 0.0f);  glVertex2i(x,   y);
  glTexCoord2f(1.0f, 0.0f);  glVertex2i(x+w, y);
  glTexCoord2f(1.0f, 1.0f);  glVertex2i(x+w, y+h);
  glTexCoord2f(0.0f, 1.0f);  glVertex2i(x,   y+h);
 }
 glEnd();
 glDisable(GL_TEXTURE_2D);
 glDisable(GL_BLEND);
 glPopAttrib();

 #if defined(GL_DEBUG)
 GL_Assert( "[exiting Stretchi()]" );
 glReportError( glGetError() );
#endif

}
void VirtualGraphics::StretchiNoAlpha( GLImage * source, Crayon tint, Blends blend, int x, int y, int w, int h ) {

#if defined(GL_DEBUG)
 GL_Assert( "[entering StretchiNoAlpha()]" );
 glReportError( glGetError() );
 GL_Assert( "StretchiNoAlpha(): " );
#endif

 glPushAttrib(GL_ALL_ATTRIB_BITS);
 glDisable(GL_DEPTH_TEST);
 glDisable(GL_CULL_FACE);
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);

 //printf( "%f,%f,%f,%f\n", t[0], t[1], t[2], t[3] );
 glColor4fv(tint.floats); 
 glBegin(GL_QUADS);

 glTexCoord2f(0.0f, 0.0f);  glVertex2i(x,   y);
 glTexCoord2f(1.0f, 0.0f);  glVertex2i(x+w, y);
 glTexCoord2f(1.0f, 1.0f);  glVertex2i(x+w, y+h);
 glTexCoord2f(0.0f, 1.0f);  glVertex2i(x,   y+h);
 
 glEnd();
 glDisable(GL_TEXTURE_2D);
 glDisable(GL_BLEND);
 glPopAttrib();

 #if defined(GL_DEBUG)
 GL_Assert( "[exiting StretchiNoAlpha()]" );
 glReportError( glGetError() );
#endif

}


void VirtualGraphics::StretchdNoAlpha( GLImage * source, Crayon tint, Blends blend, double x, double y, int w, int h ) {
 double W=(double)w;
 double H=(double)h;
#if defined(GL_DEBUG)
 GL_Assert( "[entering StretchdNoAlpha()]" );
 glReportError( glGetError() );
 GL_Assert( "StretchdNoAlpha(): " );
#endif

 glPushAttrib(GL_ALL_ATTRIB_BITS);
 glDisable(GL_DEPTH_TEST);
 glDisable(GL_CULL_FACE);
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);

 //printf( "%f,%f,%f,%f\n", t[0], t[1], t[2], t[3] );
 glColor4fv(tint.floats); 
 glBegin(GL_QUADS);

 glTexCoord2f(0.0f, 0.0f);  glVertex2d(x,   y);
 glTexCoord2f(1.0f, 0.0f);  glVertex2d(x+W, y);
 glTexCoord2f(1.0f, 1.0f);  glVertex2d(x+W, y+H);
 glTexCoord2f(0.0f, 1.0f);  glVertex2d(x,   y+H);
 
 glEnd();
 glDisable(GL_TEXTURE_2D);
 glDisable(GL_BLEND);
 glPopAttrib();

 #if defined(GL_DEBUG)
 GL_Assert( "[exiting StretchdNoAlpha()]" );
 glReportError( glGetError() );
#endif

}

// Tops: left to right top pixels (x,y to x2,y2)                      * ---> *
// Bottoms: left to right bottom pixels (when wound: x2,y2 to x,y)    * <--- *
void VirtualGraphics::TexturedQuad( GLImage *source, Crayon tint, Blends blend, Cartesian *tops, Cartesian *bottoms ) {
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);

 //printf( "%f,%f,%f,%f\n", t[0], t[1], t[2], t[3] );
 glColor4fv(tint.floats); 
 glBegin(GL_QUADS);

 glTexCoord2f(0.0f, 0.0f);  glVertex2i(tops->x,   tops->y);
 glTexCoord2f(1.0f, 0.0f);  glVertex2i(tops->x2, tops->y2);
 glTexCoord2f(1.0f, 1.0f);  glVertex2i(bottoms->x2, bottoms->y2);
 glTexCoord2f(0.0f, 1.0f);  glVertex2i(bottoms->x,bottoms->y);
 
 glEnd();
 glDisable(GL_TEXTURE_2D);
 glDisable(GL_BLEND);
}

// Tops: left to right top pixels (x,y to x2,y2)                      * ---> *
// Bottoms: left to right bottom pixels (when wound: x2,y2 to x,y)    * <--- *
void VirtualGraphics::TexturedQuad( GLImage *source, Crayon tint, Blends blend, Cartesian *tops, Cartesian *bottoms, bool xreversed ) {
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);

 //printf( "%f,%f,%f,%f\n", t[0], t[1], t[2], t[3] );
 glColor4fv(tint.floats); 
 glBegin(GL_QUADS);

 if ( !xreversed ) {
  glTexCoord2f(0.0f, 0.0f);  glVertex2i(tops->x,   tops->y);
  glTexCoord2f(1.0f, 0.0f);  glVertex2i(tops->x2, tops->y2);
  glTexCoord2f(1.0f, 1.0f);  glVertex2i(bottoms->x2, bottoms->y2);
  glTexCoord2f(0.0f, 1.0f);  glVertex2i(bottoms->x,bottoms->y);
 } else {
  glTexCoord2f(1.0f, 0.0f);  glVertex2i(tops->x,   tops->y);
  glTexCoord2f(0.0f, 0.0f);  glVertex2i(tops->x2, tops->y2);
  glTexCoord2f(0.0f, 1.0f);  glVertex2i(bottoms->x2, bottoms->y2);
  glTexCoord2f(1.0f, 1.0f);  glVertex2i(bottoms->x,bottoms->y);
 }
 
 glEnd();
 glDisable(GL_TEXTURE_2D);
 glDisable(GL_BLEND);
}

void VirtualGraphics::FBOTexturedQuad( GLImage *source, Crayon tint, Blends blend, int x, int y, int w, int h ) {
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);

 //printf( "%f,%f,%f,%f\n", t[0], t[1], t[2], t[3] );
 glColor4fv(tint.floats); 
 glBegin(GL_QUADS);

 glTexCoord2f(0.0f, 0.0f);  glVertex2i(x,   y);
 glTexCoord2f(1.0f, 0.0f);  glVertex2i(x+w, y);
 glTexCoord2f(1.0f, 1.0f);  glVertex2i(x+w, y+h);
 glTexCoord2f(0.0f, 1.0f);  glVertex2i(x,   y+h);
 
 glEnd();
 glDisable(GL_TEXTURE_2D);
 glDisable(GL_BLEND);
}

void VirtualGraphics::FBOQuad( int x, int y, int w, int h ) {
 glBegin(GL_QUADS);
 glTexCoord2f(0.0f, 0.0f);  glVertex2i(x,   y);
 glTexCoord2f(1.0f, 0.0f);  glVertex2i(x+w, y);
 glTexCoord2f(1.0f, 1.0f);  glVertex2i(x+w, y+h);
 glTexCoord2f(0.0f, 1.0f);  glVertex2i(x,   y+h);
 glEnd();
}

void VirtualGraphics::FBOQuadUpsideDown( int x, int y, int w, int h ) {
 glBegin(GL_QUADS);
 glTexCoord2f(0.0f, 1.0f);  glVertex2i(x,   y);
 glTexCoord2f(1.0f, 1.0f);  glVertex2i(x+w, y);
 glTexCoord2f(1.0f, 0.0f);  glVertex2i(x+w, y+h);
 glTexCoord2f(0.0f, 0.0f);  glVertex2i(x,   y+h);
 glEnd();
}

void VirtualGraphics::FBOQuadReversed( int x, int y, int w, int h ) {
 glBegin(GL_QUADS);
 glTexCoord2f(1.0f, 0.0f);  glVertex2i(x,   y);
 glTexCoord2f(0.0f, 0.0f);  glVertex2i(x+w, y);
 glTexCoord2f(0.0f, 1.0f);  glVertex2i(x+w, y+h);
 glTexCoord2f(1.0f, 1.0f);  glVertex2i(x,   y+h);
 glEnd();
}

void VirtualGraphics::FBOQuadReversedUpsideDown( int x, int y, int w, int h ) {
 glBegin(GL_QUADS);
 glTexCoord2f(1.0f, 1.0f);  glVertex2i(x,   y);
 glTexCoord2f(0.0f, 1.0f);  glVertex2i(x+w, y);
 glTexCoord2f(0.0f, 0.0f);  glVertex2i(x+w, y+h);
 glTexCoord2f(1.0f, 0.0f);  glVertex2i(x,   y+h);
 glEnd();
}

/*
void VirtualGraphics::TexturedQuad3d( GLImage *source, Crayon tint, Blends blend, Vertexd *a, Vertexd *b, Vertexd *c, Vertexd *d ) {
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);

 //printf( "%f,%f,%f,%f\n", t[0], t[1], t[2], t[3] );
 glColor4fv(tint.floats); 
 glBegin(GL_QUADS);

 glTexCoord2f(0.0f, 0.0f);  glVertex3d(a->x, a->y, a->z);
 glTexCoord2f(1.0f, 0.0f);  glVertex3d(b->x, a->y, a->z);
 glTexCoord2f(1.0f, 1.0f);  glVertex3d(x+w, y+h);
 glTexCoord2f(0.0f, 1.0f);  glVertex3d(x,   y+h);
 
 glEnd();
 glDisable(GL_TEXTURE_2D);
 glDisable(GL_BLEND);
}
*/

void VirtualGraphics::FBOTexturedQuadUpsideDown( GLImage *source, Crayon tint, Blends blend, int x, int y, int w, int h ) {
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);

 //printf( "%f,%f,%f,%f\n", t[0], t[1], t[2], t[3] );
 glColor4fv(tint.floats); 
 glBegin(GL_QUADS);

 glTexCoord2f(0.0f, 1.0f);  glVertex2i(x,   y);
 glTexCoord2f(1.0f, 1.0f);  glVertex2i(x+w, y);
 glTexCoord2f(1.0f, 0.0f);  glVertex2i(x+w, y+h);
 glTexCoord2f(0.0f, 0.0f);  glVertex2i(x,   y+h);
 
 glEnd();
 glDisable(GL_TEXTURE_2D);
 glDisable(GL_BLEND);
}

void VirtualGraphics::Quadi( int x, int y, int w, int h ) {

#if defined(GL_DEBUG)
 GL_Assert( "[entering Quadi()]" );
 glReportError( glGetError() );
 GL_Assert( "Quadi(): " );
#endif

 glPushAttrib(GL_ALL_ATTRIB_BITS);
 glDisable(GL_DEPTH_TEST);
 glDisable(GL_CULL_FACE);
 glBegin(GL_QUADS);

 glTexCoord2f(0.0f, 0.0f);  glVertex2i(x,   y);
 glTexCoord2f(1.0f, 0.0f);  glVertex2i(x+w, y);
 glTexCoord2f(1.0f, 1.0f);  glVertex2i(x+w, y+h);
 glTexCoord2f(0.0f, 1.0f);  glVertex2i(x,   y+h);
 
 glEnd();
 glDisable(GL_TEXTURE_2D);
 glDisable(GL_BLEND);
 glPopAttrib();

 #if defined(GL_DEBUG)
 GL_Assert( "[exiting Quadi()]" );
 glReportError( glGetError() );
#endif
}


void VirtualGraphics::Quadi( GLImage * source, Crayon tint, float alpha, Blends blend, int x, int y, int w, int h ) {

#if defined(GL_DEBUG)
 GL_Assert( "[entering Quadi()]" );
 glReportError( glGetError() );
 GL_Assert( "Quadi()" );
#endif

 glPushAttrib(GL_ALL_ATTRIB_BITS);
 glDisable(GL_DEPTH_TEST);
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);

 //printf( "%f,%f,%f,%f\n", t[0], t[1], t[2], t[3] );
 tint.ScaleByAlphaf(alpha);
 glColor4fv(tint.floats); 
 glBegin(GL_QUADS);
 if(h < 0)
 {
  glTexCoord2f(0.0f, 1.0f);  glVertex2i(x,   y);
  glTexCoord2f(1.0f, 1.0f);  glVertex2i(x+w, y);
  glTexCoord2f(1.0f, 0.0f);  glVertex2i(x+w, y-h);
  glTexCoord2f(0.0f, 0.0f);  glVertex2i(x,   y-h);
 }
 else
 {
  glTexCoord2f(0.0f, 0.0f);  glVertex2i(x,   y);
  glTexCoord2f(1.0f, 0.0f);  glVertex2i(x+w, y);
  glTexCoord2f(1.0f, 1.0f);  glVertex2i(x+w, y+h);
  glTexCoord2f(0.0f, 1.0f);  glVertex2i(x,   y+h);
 }
 glEnd();
 glDisable(GL_TEXTURE_2D);
 glDisable(GL_BLEND);
 glPopAttrib();

 #if defined(GL_DEBUG)
 GL_Assert( "[exiting Quadi()]" );
 glReportError( glGetError() );
#endif

}


void VirtualGraphics::QuadiNoAlpha( GLImage * source, Crayon tint, Blends blend, Cartesian *rect ) {
#if defined(GL_DEBUG)
 GL_Assert( "[entering QuadiNoAlpha()]" );
 glReportError( glGetError() );
 GL_Assert( "QuadiNoAlpha(): " );
#endif

 glPushAttrib(GL_ALL_ATTRIB_BITS);
 glDisable(GL_DEPTH_TEST);
 glDisable(GL_CULL_FACE);
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);

 //printf( "%f,%f,%f,%f\n", t[0], t[1], t[2], t[3] );
 glColor4fv(tint.floats); 
 glBegin(GL_QUADS);

 glTexCoord2f(0.0f, 0.0f);  glVertex2i(rect->x,  rect->y);
 glTexCoord2f(1.0f, 0.0f);  glVertex2i(rect->x2, rect->y);
 glTexCoord2f(1.0f, 1.0f);  glVertex2i(rect->x2, rect->y2);
 glTexCoord2f(0.0f, 1.0f);  glVertex2i(rect->x,  rect->y2);
 
 glEnd();
 glDisable(GL_TEXTURE_2D);
 glDisable(GL_BLEND);
 glPopAttrib();

 #if defined(GL_DEBUG)
 GL_Assert( "[exiting QuadiNoAlpha()]" );
 glReportError( glGetError() );
#endif
}

void VirtualGraphics::QuadiNoAlpha( GLImage * source, Crayon tint, Blends blend, int x, int y, int w, int h ) {

#if defined(GL_DEBUG)
 GL_Assert( "[entering QuadiNoAlpha()]" );
 glReportError( glGetError() );
 GL_Assert( "QuadiNoAlpha(): " );
#endif

 glPushAttrib(GL_ALL_ATTRIB_BITS);
 glDisable(GL_DEPTH_TEST);
 glDisable(GL_CULL_FACE);
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);

 //printf( "%f,%f,%f,%f\n", t[0], t[1], t[2], t[3] );
 glColor4fv(tint.floats); 
 glBegin(GL_QUADS);

 glTexCoord2f(0.0f, 0.0f);  glVertex2i(x,   y);
 glTexCoord2f(1.0f, 0.0f);  glVertex2i(x+w, y);
 glTexCoord2f(1.0f, 1.0f);  glVertex2i(x+w, y+h);
 glTexCoord2f(0.0f, 1.0f);  glVertex2i(x,   y+h);
 
 glEnd();
 glDisable(GL_TEXTURE_2D);
 glDisable(GL_BLEND);
 glPopAttrib();

 #if defined(GL_DEBUG)
 GL_Assert( "[exiting QuadiNoAlpha()]" );
 glReportError( glGetError() );
#endif
}


void VirtualGraphics::SkewedQuadiNoAlpha(
 GLImage * source, Crayon tint, Blends blend,
 int x, int y, int w, int h, Cartesian *skew ) {

#if defined(GL_DEBUG)
 GL_Assert( "[entering QuadiNoAlpha()]" );
 glReportError( glGetError() );
 GL_Assert( "QuadiNoAlpha(): " );
#endif

 glPushAttrib(GL_ALL_ATTRIB_BITS);
 glDisable(GL_DEPTH_TEST);
 glDisable(GL_CULL_FACE);
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);

 //printf( "%f,%f,%f,%f\n", t[0], t[1], t[2], t[3] );
 glColor4fv(tint.floats); 
 glBegin(GL_QUADS);

 glTexCoord2f(0.0f, 0.0f);  glVertex2i(x+skew->x2,   y+skew->y2);
 glTexCoord2f(1.0f, 0.0f);  glVertex2i(x+w+skew->x, y+skew->y2);
 glTexCoord2f(1.0f, 1.0f);  glVertex2i(x+w+skew->x2, y+h+skew->y);
 glTexCoord2f(0.0f, 1.0f);  glVertex2i(x-skew->x,   y+h+skew->y);
 
 glEnd();
 glDisable(GL_TEXTURE_2D);
 glDisable(GL_BLEND);
 glPopAttrib();

 #if defined(GL_DEBUG)
 GL_Assert( "[exiting QuadiNoAlpha()]" );
 glReportError( glGetError() );
#endif
}


void VirtualGraphics::SkewedQuadiNoAlpha(
 GLImage * source, Crayon tint, Blends blend,
 int x, int y, int w, int h, int xSkew ) {

#if defined(GL_DEBUG)
 GL_Assert( "[entering QuadiNoAlpha()]" );
 glReportError( glGetError() );
 GL_Assert( "QuadiNoAlpha(): " );
#endif

 glPushAttrib(GL_ALL_ATTRIB_BITS);
 glDisable(GL_DEPTH_TEST);
 glDisable(GL_CULL_FACE);
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);

 //printf( "%f,%f,%f,%f\n", t[0], t[1], t[2], t[3] );
 glColor4fv(tint.floats); 
 glBegin(GL_QUADS);

 glTexCoord2f(0.0f, 0.0f);  glVertex2i(x,   y);
 glTexCoord2f(1.0f, 0.0f);  glVertex2i(x+w+xSkew, y);
 glTexCoord2f(1.0f, 1.0f);  glVertex2i(x+w, y+h);
 glTexCoord2f(0.0f, 1.0f);  glVertex2i(x-xSkew, y+h);
 
 glEnd();
 glDisable(GL_TEXTURE_2D);
 glDisable(GL_BLEND);
 glPopAttrib();

 #if defined(GL_DEBUG)
 GL_Assert( "[exiting QuadiNoAlpha()]" );
 glReportError( glGetError() );
#endif
}


void VirtualGraphics::SkewedQuaddNoAlpha(
 GLImage * source, Crayon tint, Blends blend,
 double x, double y, int w, int h, int xSkew ) {
 double XSKEW=(double)xSkew;
 double W=(double)w;
 double H=(double)h;
#if defined(GL_DEBUG)
 GL_Assert( "[entering QuadiNoAlpha()]" );
 glReportError( glGetError() );
 GL_Assert( "QuadiNoAlpha(): " );
#endif

 glPushAttrib(GL_ALL_ATTRIB_BITS);
 glDisable(GL_DEPTH_TEST);
 glDisable(GL_CULL_FACE);
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);

 //printf( "%f,%f,%f,%f\n", t[0], t[1], t[2], t[3] );
 glColor4fv(tint.floats); 
 glBegin(GL_QUADS);

 glTexCoord2f(0.0f, 0.0f);  glVertex2d(x,   y);
 glTexCoord2f(1.0f, 0.0f);  glVertex2d(x+W+XSKEW, y);
 glTexCoord2f(1.0f, 1.0f);  glVertex2d(x+W, y+H);
 glTexCoord2f(0.0f, 1.0f);  glVertex2d(x-XSKEW, y+H);
 
 glEnd();
 glDisable(GL_TEXTURE_2D);
 glDisable(GL_BLEND);
 glPopAttrib();

 #if defined(GL_DEBUG)
 GL_Assert( "[exiting QuadiNoAlpha()]" );
 glReportError( glGetError() );
#endif
}
void VirtualGraphics::QuadiNoAlphaReversed( GLImage * source, Crayon tint, Blends blend, int x, int y, int w, int h ) {

#if defined(GL_DEBUG)
 GL_Assert( "[entering QuadiNoAlpha()]" );
 glReportError( glGetError() );
 GL_Assert( "QuadiNoAlpha(): " );
#endif

 glPushAttrib(GL_ALL_ATTRIB_BITS);
 glDisable(GL_DEPTH_TEST);
 glDisable(GL_CULL_FACE);
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);

 //printf( "%f,%f,%f,%f\n", t[0], t[1], t[2], t[3] );
 glColor4fv(tint.floats); 
 glBegin(GL_QUADS);

 glTexCoord2f(1.0f, 0.0f);  glVertex2i(x,   y);
 glTexCoord2f(0.0f, 0.0f);  glVertex2i(x+w, y);
 glTexCoord2f(0.0f, 1.0f);  glVertex2i(x+w, y+h);
 glTexCoord2f(1.0f, 1.0f);  glVertex2i(x,   y+h);
 
 glEnd();
 glDisable(GL_TEXTURE_2D);
 glDisable(GL_BLEND);
 glPopAttrib();

 #if defined(GL_DEBUG)
 GL_Assert( "[exiting QuadiNoAlpha()]" );
 glReportError( glGetError() );
#endif
}
void VirtualGraphics::QuaddNoAlpha( GLImage * source, Crayon tint, Blends blend, double x, double y, double w, double h ) {

#if defined(GL_DEBUG)
 GL_Assert( "[entering QuaddNoAlpha()]" );
 glReportError( glGetError() );
 GL_Assert( "QuaddNoAlpha(): " );
#endif

 glPushAttrib(GL_ALL_ATTRIB_BITS);
 glDisable(GL_DEPTH_TEST);
 glDisable(GL_CULL_FACE);
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);

 //printf( "%f,%f,%f,%f\n", t[0], t[1], t[2], t[3] );
 glColor4fv(tint.floats); 
 glBegin(GL_QUADS);

 glTexCoord2f(0.0f, 0.0f);  glVertex2d(x,   y);
 glTexCoord2f(1.0f, 0.0f);  glVertex2d(x+w, y);
 glTexCoord2f(1.0f, 1.0f);  glVertex2d(x+w, y+h);
 glTexCoord2f(0.0f, 1.0f);  glVertex2d(x,   y+h);
 
 glEnd();
 glDisable(GL_TEXTURE_2D);
 glDisable(GL_BLEND);
 glPopAttrib();

 #if defined(GL_DEBUG)
 GL_Assert( "[exiting QuaddNoAlpha()]" );
 glReportError( glGetError() );
#endif

}

void VirtualGraphics::XReversibleQuadiNoAlpha( GLImage * source, Crayon tint, Blends blend, int x, int y, int w, int h, bool xreversed ) {

#if defined(GL_DEBUG)
 GL_Assert( "[entering XReversibleQuadiNoAlpha()]" );
 glReportError( glGetError() );
 GL_Assert( "XReversibleQuadiNoAlpha(): " );
#endif

 glPushAttrib(GL_ALL_ATTRIB_BITS);
 glDisable(GL_DEPTH_TEST);
 glDisable(GL_CULL_FACE);
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);

 //printf( "%f,%f,%f,%f\n", t[0], t[1], t[2], t[3] );
 glColor4fv(tint.floats); 
 glBegin(GL_QUADS);
 if ( !xreversed ) {
  glTexCoord2f(0.0f, 0.0f);  glVertex2i(x,   y);
  glTexCoord2f(1.0f, 0.0f);  glVertex2i(x+w, y);
  glTexCoord2f(1.0f, 1.0f);  glVertex2i(x+w, y+h);
  glTexCoord2f(0.0f, 1.0f);  glVertex2i(x,   y+h);
 } else {
  glTexCoord2f(1.0f, 0.0f);  glVertex2i(x,   y);
  glTexCoord2f(0.0f, 0.0f);  glVertex2i(x+w, y);
  glTexCoord2f(0.0f, 1.0f);  glVertex2i(x+w, y+h);
  glTexCoord2f(1.0f, 1.0f);  glVertex2i(x,   y+h);
 }
 glEnd();
 glDisable(GL_TEXTURE_2D);
 glDisable(GL_BLEND);
 glPopAttrib();

 #if defined(GL_DEBUG)
 GL_Assert( "[exiting XReversibleStretchiNoAlpha()]" );
 glReportError( glGetError() );
#endif

}


// Used for isometric tile effects
void VirtualGraphics::Swabd( GLImage * source, Crayon tint, Blends blend, double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4 ) {
 glReportError( glGetError() );
 glPushAttrib(GL_COLOR_BUFFER_BIT);
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);
 glColor4fv(tint.floats); 
 glBegin(GL_QUADS);
 glTexCoord2i(0, 1);  glVertex2d(x1, y1);
 glTexCoord2i(1, 1);  glVertex2d(x2, y2);
 glTexCoord2i(1, 0);  glVertex2d(x3, y3);
 glTexCoord2i(0, 0);  glVertex2d(x4, y4);
 glEnd();
 glDisable(GL_TEXTURE_2D);
 glPopAttrib();
}

void VirtualGraphics::RotatedRecti( GLImage *source, Crayon tint, Blends blend, int x, int y, int l, int w, float a ) {
 float x1,y1;
 float w2=(float) w/2.0f;
 float h=(float) l;

 x1=(float) (x-w2);
 y1=(float) y;

 glReportError( glGetError() );
 glPushAttrib(GL_COLOR_BUFFER_BIT);
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);
 glPushMatrix();
 glTranslatef((float) x,y1,0.0f);
 glRotatef(a,0.0f,0.0f,1.0f);

 glColor4fv(tint.floats); 
 glBegin(GL_QUADS);
 glTexCoord2f(0.0f, 1.0f);  glVertex2f(w2, 0.0f); //glVertex2f((GLfloat) x1, (GLfloat) y);
 glTexCoord2f(1.0f, 1.0f);  glVertex2f(-w2, 0.0f);  //glVertex2f((GLfloat) x2, (GLfloat) y);
 glTexCoord2f(1.0f, 0.0f);  glVertex2f(-w2, h);     //glVertex2f((GLfloat) x2, (GLfloat) y2);
 glTexCoord2f(0.0f, 0.0f);  glVertex2f(w2, h);    //glVertex2f((GLfloat) x1, (GLfloat) y2);
 glEnd();
 glDisable(GL_TEXTURE_2D);

 glPopMatrix();
 glPopAttrib();
}

// Rotates at angle a, blended textured rectangle around x,y that is h long and w2*2 wide
void VirtualGraphics::RotatedRectf( GLImage *source, Crayon tint, Blends blend, float x, float y, float h, float w2, float a ) {
 glReportError( glGetError() );
 glPushAttrib(GL_COLOR_BUFFER_BIT);
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);
 glPushMatrix();
 glTranslatef(x,y,0.0f);
 glRotatef(a,0.0f,0.0f,1.0f);

 glColor4fv(tint.floats); 
 glBegin(GL_QUADS);
 glTexCoord2f(0.0f, 1.0f);  glVertex2f(w2, 0.0f); 
 glTexCoord2f(1.0f, 1.0f);  glVertex2f(-w2, 0.0f);
 glTexCoord2f(1.0f, 0.0f);  glVertex2f(-w2, h);   
 glTexCoord2f(0.0f, 0.0f);  glVertex2f(w2, h);    
 glEnd();
 glDisable(GL_TEXTURE_2D);

 glPopMatrix();
 glPopAttrib();
}

// Pivots a rectangle around axis of rotation px,py at position x,y 
// rotated by angle a, h2*2 by w2*2 are rectangular dimensions
void VirtualGraphics::PivotedRotatedRectf( GLImage *source, Crayon tint, Blends blend, 
                          float x, float y, float px, float py, float h2, float w2, float a ) {
 glReportError( glGetError() );
 glPushAttrib(GL_COLOR_BUFFER_BIT);
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);
 glPushMatrix();
 glTranslatef(x,y,0.0f);
 glRotatef(a,0.0f,0.0f,1.0f);
 glColor4fv(tint.floats); 
 glBegin(GL_QUADS);
 glTexCoord2f(0.0f, 1.0f);  glVertex2f( w2+px, -h2+py); 
 glTexCoord2f(1.0f, 1.0f);  glVertex2f(-w2+px, -h2+py);
 glTexCoord2f(1.0f, 0.0f);  glVertex2f(-w2+px,  h2+py);   
 glTexCoord2f(0.0f, 0.0f);  glVertex2f( w2+px,  h2+py);    
 glEnd();
 glDisable(GL_TEXTURE_2D);
 glPopMatrix();
 glPopAttrib();
}

// Pivots a rectangle around axis of rotation px,py at position x,y 
// rotated by angle a, h2*2 by w2*2 are rectangular dimensions
void VirtualGraphics::XInvertedPivotedRotatedRectf( GLImage *source, Crayon tint, Blends blend, 
                          float x, float y, float px, float py, float h2, float w2, float a ) {
 glReportError( glGetError() );
 glPushAttrib(GL_COLOR_BUFFER_BIT);
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);
 glPushMatrix();
 glTranslatef(x,y,0.0f);
 glRotatef(a,0.0f,0.0f,1.0f);
 glColor4fv(tint.floats); 
 glBegin(GL_QUADS);
 glTexCoord2f(1.0f, 1.0f);  glVertex2f( w2+px, -h2+py); 
 glTexCoord2f(0.0f, 1.0f);  glVertex2f(-w2+px, -h2+py);
 glTexCoord2f(0.0f, 0.0f);  glVertex2f(-w2+px,  h2+py);   
 glTexCoord2f(1.0f, 0.0f);  glVertex2f( w2+px,  h2+py);    
 glEnd();
 glDisable(GL_TEXTURE_2D);
 glPopMatrix();
 glPopAttrib();
}


// Pivots a rectangle around axis of rotation px,py at position x,y 
// rotated by angle a, h2*2 by w2*2 are rectangular dimensions
void VirtualGraphics::FBOPivotedRotatedRectf( GLImage *source, Crayon tint, Blends blend, 
                          float x, float y, float px, float py, float h2, float w2, float a ) {
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);
 glPushMatrix();
 glTranslatef(x,y,0.0f);
 glRotatef(a,0.0f,0.0f,1.0f);
 glColor4fv(tint.floats); 
 glBegin(GL_QUADS);
 glTexCoord2f(0.0f, 1.0f);  glVertex2f( w2+px, -h2+py); 
 glTexCoord2f(1.0f, 1.0f);  glVertex2f(-w2+px, -h2+py);
 glTexCoord2f(1.0f, 0.0f);  glVertex2f(-w2+px,  h2+py);   
 glTexCoord2f(0.0f, 0.0f);  glVertex2f( w2+px,  h2+py);    
 glEnd();
 glPopMatrix();
}

// Can't use quad due to perspective correction, so we use a series of 8 interlocking triangles of points,
//     D ____h___ C
//      \        /
//      f+  e.  +g
//        \____/
//        A  i  B
//glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);? no
//This is the best I can do, it looks good if you use the swab.png and wanted to do a lamp/lightsource.
void VirtualGraphics::RotatedIsotrapf( GLImage *source, Crayon tint, Blends blend, float x, float y, float h, float w1, float w2, float a ) {
 glReportError( glGetError() );
 glPushAttrib(GL_COLOR_BUFFER_BIT);
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);
 glPushMatrix();
 glTranslatef(x,y,0.0f);
 glRotatef(a,0.0f,0.0f,1.0f);
 glColor4fv(tint.floats); 

 // Interlocking triangle method
 float 
  Ax=-w1,              Ay=0.0f,
  Bx=w1,               By=0.0f,
  Cx=w2,               Cy=h,
  Dx=-w2,              Dy=h,
  ex=0.0f,             ey=h/2.0f,
  gx(w1+(w2-w1)/2),    gy=ey,
  fx=-gx,              fy=ey,
  hx=0.0f,             hy=h,
  ix=0.0f,             iy=0.0f;

 // combined 1+2
 glBegin(GL_TRIANGLES);
 glTexCoord2f(1.0f,1.0f); glVertex2f(Ax,Ay);
 glTexCoord2f(0.5f,0.5f); glVertex2f(ex,ey);
 glTexCoord2f(0.0f,1.0f); glVertex2f(Bx,By);
 glEnd();

 //3
 glBegin(GL_TRIANGLES);
 glTexCoord2f(0.0f,1.0f); glVertex2f(Bx,By);
 glTexCoord2f(0.0f,0.5f); glVertex2f(gx,gy);
 glTexCoord2f(0.5f,0.5f); glVertex2f(ex,ey);
 glEnd();

 //4
 glBegin(GL_TRIANGLES);
 glTexCoord2f(0.0f,0.5f); glVertex2f(gx,gy);
 glTexCoord2f(0.0f,0.0f); glVertex2f(Cx,Cy);
 glTexCoord2f(0.5f,0.5f); glVertex2f(ex,ey);
 glEnd();

 //5
 glBegin(GL_TRIANGLES);
 glTexCoord2f(0.0f,0.0f); glVertex2f(Cx,Cy);
 glTexCoord2f(0.5f,0.0f); glVertex2f(hx,hy);
 glTexCoord2f(0.5f,0.5f); glVertex2f(ex,ey);
 glEnd();

 //6
 glBegin(GL_TRIANGLES);
 glTexCoord2f(0.5f,0.0f); glVertex2f(hx,hy);
 glTexCoord2f(1.0f,0.0f); glVertex2f(Dx,Dy);
 glTexCoord2f(0.5f,0.5f); glVertex2f(ex,ey);
 glEnd();

 //7
 glBegin(GL_TRIANGLES);
 glTexCoord2f(1.0f,0.5f); glVertex2f(fx,fy);
 glTexCoord2f(1.0f,1.0f); glVertex2f(Ax,Ay);
 glTexCoord2f(0.5f,0.5f); glVertex2f(ex,ey);
 glEnd();

 //8
 glBegin(GL_TRIANGLES);
 glTexCoord2f(0.5f,0.5f); glVertex2f(ex,ey);
 glTexCoord2f(1.0f,0.5f); glVertex2f(fx,fy);
 glTexCoord2f(1.0f,0.0f); glVertex2f(Dx,Dy);
 glEnd();
 
 glDisable(GL_TEXTURE_2D);
 glPopMatrix();
 glPopAttrib();
}

// Can't use quad due to perspective correction, so we use a series of 8 interlocking triangles of points,
//     D ____h___ C
//      \        /
//      f+  e.  +g
//        \____/
//        A  i  B
//glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);? no
//This is the best I can do, it looks good if you use the swab.png and wanted to do a lamp/lightsource.
void VirtualGraphics::RotatedTrif( GLImage *source, Crayon tint, Blends blend, float x, float y, float h, float w2, float a ) {
 glReportError( glGetError() );
 glPushAttrib(GL_COLOR_BUFFER_BIT);
 Blending(blend);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture);
 glPushMatrix();
 glTranslatef(x,y,0.0f);
 glRotatef(a,0.0f,0.0f,1.0f);
 glColor4fv(tint.floats); 

 glBegin(GL_TRIANGLES);
 glTexCoord2f(1.0f,1.0f); glVertex2f(-w2,h);
 glTexCoord2f(0.0f,0.0f); glVertex2f(0,0);
 glTexCoord2f(0.0f,1.0f); glVertex2f(w2,h);
 glEnd();

 glDisable(GL_TEXTURE_2D);
 glPopMatrix();
 glPopAttrib();
}


void VirtualGraphics::PointSprite( GLImage *source, Crayon tint, Vertexd *coord, float size ) {
 float renderSize=mapRangef(size,0.0f,1.0f,(float)gl.aliasedPointLow, (float)gl.aliasedPointHigh);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, source->texture );
 glEnable(GL_POINT_SPRITE);
 glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
 glPointSize(renderSize);
 glBegin(GL_POINTS);
 glVertex3d( coord->x,coord->y,coord->z );
 glEnd();
}

void VirtualGraphics::Coordinator() { 
 glDisable(GL_TEXTURE_2D);
 glBegin(GL_LINES);
 glColor3d(1.0,0.0,0.0); glVertex3d(0.0,0.0,0.0); glVertex3d(1.0,0.0,0.0);
 glColor3d(0.0,1.0,0.0); glVertex3d(0.0,0.0,0.0); glVertex3d(0.0,1.0,0.0);
 glColor3d(0.0,0.0,1.0); glVertex3d(0.0,0.0,0.0); glVertex3d(0.0,0.0,1.0);
 glEnd(); 
}

void VirtualGraphics::DrawBar( Crayon color, float one, int x, int y, int w, int h, int divisions ) {
 int x2=x+w;
 int y2=y+h;
 Blending(multiply);
 int filledy=(int)((float)h*(1.0f-one));
 Area(color,x,y+filledy,x2,y2);
 float dy=iratiof(h,divisions);
 int dyi=(int) dy;
 Cartesian rect;
 Crayon drawing;
 for ( int i=0; i<divisions; i++ ) {
  int deltay=(int)(dy*(float)i);
  Blending(additive);
  if ( deltay >= filledy )  drawing.fromCrayon(color);
  else drawing.Pick(gray);
  rect.SetRect(x,y+deltay,w,dyi);
  Rectangle(drawing,&rect);
 }
 Blending(additive);
}

void VirtualGraphics::DrawBar( Blends blendFill, Blends blendLine, Crayon color, float one, int x, int y, int w, int h, int divisions ) {
 int x2=x+w;
 int y2=y+h;
 Blending(blendFill);
 int filledy=(int)((float)h*(1.0f-one));
 Area(color,x,y+filledy,x2,y2);
 float dy=iratiof(h,divisions);
 int dyi=(int) dy;
 Cartesian rect;
 Crayon drawing;
 for ( int i=0; i<divisions; i++ ) {
  int deltay=(int)(dy*(float)i);
  Blending(blendLine);
  if ( deltay >= filledy )  drawing.fromCrayon(color);
  else drawing.Pick(gray);
  rect.SetRect(x,y+deltay,w,dyi);
  Rectangle(drawing,&rect);
 }
 Blending(additive);
}

void VirtualGraphics::DrawBarHoriz( Crayon color, float one, int x, int y, int w, int h, int divisions ) {
 int x2=x+w;
 int y2=y+h;
 Blending(multiply);
 int filledx=(int)((float)w*(1.0f-one));
 Area(color,x,y,x+filledx,y2);
 float dx=iratiof(w,divisions);
 int dxi=(int) dx;
 Cartesian rect;
 Crayon drawing;
 for ( int i=0; i<divisions; i++ ) {
  int deltax=(int)(dx*(float)i);
  Blending(additive);
  if ( deltax >= filledx )  drawing.fromCrayon(color);
  else drawing.Pick(gray);
  rect.SetRect(x+deltax,y,dxi,h);
  Rectangle(drawing,&rect);
 }
 Blending(additive);
}

void VirtualGraphics::DrawBarHoriz( Blends blendFill, Blends blendLine, Crayon color, float one, int x, int y, int w, int h, int divisions ) {
 int x2=x+w;
 int y2=y+h;
 Blending(blendFill);
 int filledx=(int)((float)w*(1.0f-one));
 Area(color,x,y,x+filledx,y2);
 float dx=iratiof(w,divisions);
 int dxi=(int) dx;
 Cartesian rect;
 Crayon drawing;
 for ( int i=0; i<divisions; i++ ) {
  int deltax=(int)(dx*(float)i);
  Blending(blendLine);
  if ( deltax >= filledx )  drawing.fromCrayon(color);
  else drawing.Pick(gray);
  rect.SetRect(x+deltax,y,dxi,h);
  Rectangle(drawing,&rect);
 }
 Blending(additive);
}

void VirtualGraphics::DrawCircle( double cx, double cy, double R, int segments ) {
 double delta=TWO_PI/(double) segments;
 glBegin(GL_LINE_LOOP);
 for ( double a=-PI; a<PI; a+=delta ) glVertex2d(cx+R*cos(a), cy+R*sin(a));
 glVertex2d(cx+R*cos(-PI), cy+R*sin(-PI));
 glEnd();
}

void VirtualGraphics::DrawNamedPoint( Crayon color, const char *name, double x, double y ) {
 color.gl();
 Text(
  FORMAT(buf,512,"(%d,%d) %s",(int)x,(int)y,name),
  x+3,y-3,6,8,false
 );
 Area(color,(int)x-1,(int)y-1,(int)x+1,(int)y+1);
}

RoundedRectangle *VirtualGraphics::RoundRect( Crayon border, Crayon fill, Crayon sep, int x, int y, int width, int height, int radius, int precision, float lineWidth ) {
 return rrects.Render(border,fill,sep,x,y,width,height,radius,precision,lineWidth);
 ///if ( roundrects.count > rounderects.limit ) roundrects.Clear(); // Controversial
}

void VirtualGraphics::RoundRect9( GLImage* corner, GLImage* border, GLImage* interior, int x, int y, int w, int h, int cornerBorder ) {
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, corner->texture);
 glPushMatrix();
  glBegin(GL_QUADS);
 //Top Left Corner
 glTexCoord2f(0.0f, 1.0f);  glVertex2f((GLfloat)(x+cornerBorder),(GLfloat)y); 
 glTexCoord2f(1.0f, 1.0f);  glVertex2f((GLfloat)(x+cornerBorder),(GLfloat)(y+cornerBorder));
 glTexCoord2f(1.0f, 0.0f);  glVertex2f((GLfloat)x,(GLfloat)(y+cornerBorder));   
 glTexCoord2f(0.0f, 0.0f);  glVertex2f((GLfloat)x,(GLfloat)y);    
 //Top Right Corner
 glTexCoord2f(0.0f, 0.0f);  glVertex2f((GLfloat)(x+w),(GLfloat)y); 
 glTexCoord2f(1.0f, 0.0f);  glVertex2f((GLfloat)(x+w),(GLfloat)(y+cornerBorder));
 glTexCoord2f(1.0f, 1.0f);  glVertex2f((GLfloat)(x+w-cornerBorder),(GLfloat)(y+cornerBorder));   
 glTexCoord2f(0.0f, 1.0f);  glVertex2f((GLfloat)(x+w-cornerBorder),(GLfloat)(y));
 //Bottom Left Corner
 glTexCoord2f(1.0f, 1.0f);  glVertex2f((GLfloat)(x+cornerBorder),(GLfloat)(y+h-cornerBorder)); 
 glTexCoord2f(0.0f, 1.0f);  glVertex2f((GLfloat)(x+cornerBorder),(GLfloat)(y+h));
 glTexCoord2f(0.0f, 0.0f);  glVertex2f((GLfloat)x,(GLfloat)(y+h));   
 glTexCoord2f(1.0f, 0.0f);  glVertex2f((GLfloat)x,(GLfloat)(y+h-cornerBorder));

  //Bottom Right Corner
 glTexCoord2f(1.0f, 0.0f);  glVertex2f((GLfloat)(x+w),(GLfloat)(y+h-cornerBorder));
 glTexCoord2f(0.0f, 0.0f);  glVertex2f((GLfloat)(x+w),(GLfloat)(y+h));
 glTexCoord2f(0.0f, 1.0f);  glVertex2f((GLfloat)(x+w-cornerBorder),(GLfloat)(y+h));
 glTexCoord2f(1.0f, 1.0f);  glVertex2f((GLfloat)(x+w-cornerBorder),(GLfloat)(y+h-cornerBorder)); 
 glEnd();
 glBindTexture(GL_TEXTURE_2D, border->texture);
 glBegin(GL_QUADS);

 //Left Side
 glTexCoord2f(1.0f, 0.0f);  glVertex2f((GLfloat)(x+cornerBorder),(GLfloat)(y+cornerBorder));
 glTexCoord2f(1.0f, 1.0f);  glVertex2f((GLfloat)(x+cornerBorder),(GLfloat)(y+h-cornerBorder));
 glTexCoord2f(0.0f, 1.0f);  glVertex2f((GLfloat)x,(GLfloat)(y+h-cornerBorder));
 glTexCoord2f(0.0f, 0.0f);  glVertex2f((GLfloat)x,(GLfloat)(y+cornerBorder)); 
 //Top Side
 glTexCoord2f(0.0f, 1.0f);  glVertex2f((GLfloat)(x+w-cornerBorder),(GLfloat)y); 
 glTexCoord2f(1.0f, 1.0f);  glVertex2f((GLfloat)(x+w-cornerBorder),(GLfloat)(y+cornerBorder));
 glTexCoord2f(1.0f, 0.0f);  glVertex2f((GLfloat)(x+cornerBorder),(GLfloat)(y+cornerBorder));   
 glTexCoord2f(0.0f, 0.0f);  glVertex2f((GLfloat)(x+cornerBorder),(GLfloat)y);    
 //Right Side
 glTexCoord2f(0.0f, 0.0f);  glVertex2f((GLfloat)(x+w),(GLfloat)(y+cornerBorder));
 glTexCoord2f(0.0f, 1.0f);  glVertex2f((GLfloat)(x+w),(GLfloat)(y+h-cornerBorder));
 glTexCoord2f(1.0f, 1.0f);  glVertex2f((GLfloat)(x+w-cornerBorder),(GLfloat)(y+h-cornerBorder));
 glTexCoord2f(1.0f, 0.0f);  glVertex2f((GLfloat)(x+w-cornerBorder),(GLfloat)(y+cornerBorder)); 
 //Bottom Side
 glTexCoord2f(1.0f, 1.0f);  glVertex2f((GLfloat)(x+w-cornerBorder),(GLfloat)(y+h-cornerBorder));
 glTexCoord2f(0.0f, 0.0f);  glVertex2f((GLfloat)(x+w-cornerBorder),(GLfloat)(y+h));
 glTexCoord2f(0.0f, 1.0f);  glVertex2f((GLfloat)(x+cornerBorder),(GLfloat)(y+h));
 glTexCoord2f(1.0f, 0.0f);  glVertex2f((GLfloat)(x+cornerBorder),(GLfloat)(y+h-cornerBorder)); 
 glEnd();
 glBindTexture(GL_TEXTURE_2D, interior->texture);
 glBegin(GL_QUADS);
 glTexCoord2f(1.0f, 0.0f);  glVertex2f((GLfloat)(x+w-cornerBorder),(GLfloat)(y+cornerBorder)); 
 glTexCoord2f(1.0f, 1.0f);  glVertex2f((GLfloat)(x+w-cornerBorder),(GLfloat)(y+h-cornerBorder));
 glTexCoord2f(0.0f, 1.0f);  glVertex2f((GLfloat)(x+cornerBorder),(GLfloat)(y+h-cornerBorder));   
 glTexCoord2f(0.0f, 0.0f);  glVertex2f((GLfloat)(x+cornerBorder),(GLfloat)(y+cornerBorder));    
 glEnd();
 glPopMatrix();

}

#include "UglyFont.h"
//void YsDrawUglyFontDL(const char str[],bool centering);

void VirtualGraphics::Text(const char * t, double x, double y, double fontW, double fontH, bool centered) {
 if ( strlen(t) == 0 ) {
  return;
 }
 UglyText(t,crayons.Pick(alabaster),transparency,x,y,fontW,fontH,2.0,1.0,centered);
}

void VirtualGraphics::Text(const char * t, double x, double y, double fontW, double fontH, bool centered,double angle) {
 if ( strlen(t) == 0 ) {
  return;
 }
 UglyText(t,crayons.Pick(alabaster),transparency,x,y,fontW,fontH,2.0,1.0,centered);
}

void VirtualGraphics::MultilineText(const char *t, double x, double y, double fontW, double fontH, double lineSpace, bool centered) {
 if ( strlen(t) == 0 ) {
  return;
 }
 UglyText(t,crayons.Pick(alabaster),transparency,x,y,fontW,fontH,2.0,lineSpace,centered);
}