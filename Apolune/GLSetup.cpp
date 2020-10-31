#include "GLSetup.h"

BlendModes blends;

void BlendModes::Init() {
 BlendOp* b=modes.Increase(); // Defines the default blend.
 Define( _additive,         "Additive",            "additive",          GL_ONE_MINUS_DST_COLOR, GL_ZERO );
 Define( _solarize,         "Solarize",            "solarize",          GL_DST_COLOR,           GL_ZERO ); 
 Define( _mask,             "Mask",                "mask",              GL_ONE,                 GL_ONE );
 Define( _multiply,         "Multiply",            "multiply",          GL_SRC_ALPHA,           GL_ONE );
 Define( _none,             "Overwrite",           "none",              GL_ONE_MINUS_SRC_ALPHA, GL_ONE );
 Define( _transparency,     "Transparency",        "transparency",      GL_ZERO,                GL_SRC_COLOR );
 Define(                    "Transparency Alt",    "transparency2",     GL_ONE,                 GL_SRC_COLOR );
 Define( _overlay,          "Overlay",             "overlay",           GL_ONE,                 GL_ONE_MINUS_SRC_ALPHA );
 Define( _screened,         "Screen",              "screened",          GL_SRC_ALPHA,           GL_ONE_MINUS_SRC_ALPHA );
 Define( _saturate,         "Saturate",            "saturate",          GL_SRC_ALPHA_SATURATE,  GL_ONE );
 Define( _mask1,            "Mask",                "mask1",             GL_DST_COLOR,           GL_ONE );
 Define( _subtractive,      "Subtractive",         "subtractive",       GL_ONE,                 GL_ZERO );
 Define( _translucency,     "Translucency",        "translucency",      GL_DST_COLOR,           GL_SRC_COLOR );
 Define( _transluminant,    "Transluminant",       "transluminant",     GL_SRC_ALPHA,           GL_DST_ALPHA );
 Define( _silhouette,       "Silhouette",          "silhouette",        GL_CONSTANT_ALPHA,      GL_ONE_MINUS_SRC_ALPHA );
 Define( _subtractAlpha,    "SubtractAlpha",       "subtractAlpha",     GL_SRC_ALPHA,GL_ONE,    GL_FUNC_REVERSE_SUBTRACT );
 Define( _transparentAlpha, "TransparentAlpha",    "transparentAlpha",  GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,GL_FUNC_ADD );
}

void Blend( int id ) {
 BlendOp *b=blends.find(id);
 b->Enable();
}
void Blend( BlendOp *b ) {
 b->Enable();
}
BlendOp *Blend( string name ) {
 return blends.find(name.c_str());
}
void Blend( bool on_off ) {
 if ( on_off ) glEnable(GL_BLEND);
 else glDisable(GL_BLEND);
}

///#if defined(GL_DEBUG)
///char gl_info[1024];
///char old_gl_info[1024];
///#endif

// Legacy blending system.

bool operator== ( const Blends& a, const Blends& b ) { return ( a.blend == b.blend ); }
bool operator== ( Blendings a, const Blends& b ) { return ( a == b.blend ); }
bool operator== ( const Blends& a, Blendings b ) { return ( a.blend == b ); }

const Blends
 additive(_additive),
 solarize(_solarize),
 mask(_mask),
 multiply(_multiply),
 none(_none),
 transparency(_transparency),
 overlay(_overlay),
 screened(_screened),
 saturate(_saturate),
 mask1(_mask1),
 subtractive(_subtractive),
 translucency(_translucency),
 transluminant(_transluminant),
 silhouette(_silhouette),
 subtractAlpha(_subtractAlpha),
 transparentAlpha(_transparentAlpha);

// Pushes all states
void glPush() {
 glMatrixMode(GL_PROJECTION);
 glPushMatrix();
 glLoadIdentity();
 glMatrixMode(GL_MODELVIEW);
 glPushMatrix();
 glLoadIdentity();
 glPushAttrib(GL_ALL_ATTRIB_BITS);
}

// Pushes all states
void glPush( bool dont_load_identity ) {
 glMatrixMode(GL_PROJECTION);
 glPushMatrix();
 glMatrixMode(GL_MODELVIEW);
 glPushMatrix();
 glPushAttrib(GL_ALL_ATTRIB_BITS);
}

// Pops all states; swallows errors unless GL_DEBUG is enabled.
void glPop() {
#if defined(GL_DEBUG)
 GLenum err=glGetError();
 switch ( err ) {
 case GL_NO_ERROR: break;                       
 case GL_INVALID_ENUM:      OUTPUT("glError:glPop() [pre] GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument, offending command is ignored.\n" ); break;
 case GL_INVALID_VALUE:     OUTPUT("glError:glPop() [pre] GL_INVALID_VALUE: A numeric argument is out of range, offending command is ignored.\n" ); break;                       
 case GL_INVALID_OPERATION: OUTPUT("glError:glPop() [pre] GL_INVALID_OPERATION: The specified operation is not allowed in the current state, offending command is ignored.\n" ); break;
 case GL_STACK_OVERFLOW:    OUTPUT("glError:glPop() [pre] GL_STACK_OVERFLOW: A command would cause a stack overflow, the offending command is ignored.\n" ); break;
 case GL_STACK_UNDERFLOW:   OUTPUT("glError:glPop() [pre] GL_STACK_UNDERFLOW: A command would cause a stack underflow, the offending command is ignored.\n" ); break;
 case GL_OUT_OF_MEMORY:     OUTPUT("glError:glPop() [pre] GL_OUT_OF_MEMORY: There is not enough memory left to execute the command. The state of the GL is undefined,\n except for the state of the error flags, after this error is recorded.\n" ); break;                       
 case GL_TABLE_TOO_LARGE:   OUTPUT("glError:glPop() [pre] GL_TABLE_TOO_LARGE: The specified table exceeds the implementation's maximum supported table size. The offending command is ignored.\n" ); break;
 default: OUTPUT("glError:glPop() unknown error!\n"); break;
 }
#endif
 glMatrixMode(GL_PROJECTION);
 glPopMatrix();
 glMatrixMode(GL_MODELVIEW);
 glPopMatrix();
 glPopAttrib();
#if defined(GL_DEBUG) && defined(REPORT_ALL_ERRORS)
 err=glGetError();
 switch ( err ) {
 case GL_NO_ERROR: break;                       
 case GL_INVALID_ENUM:      OUTPUT("glError:glPop() [post]GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument, offending command is ignored.\n" ); break;
 case GL_INVALID_VALUE:     OUTPUT("glError:glPop() [post]GL_INVALID_VALUE: A numeric argument is out of range, offending command is ignored.\n" ); break;                       
 case GL_INVALID_OPERATION: OUTPUT("glError:glPop() [post]GL_INVALID_OPERATION: The specified operation is not allowed in the current state, offending command is ignored.\n" ); break;
 case GL_STACK_OVERFLOW:    OUTPUT("glError:glPop() [post]GL_STACK_OVERFLOW: A command would cause a stack overflow, the offending command is ignored.\n" ); break;
 case GL_STACK_UNDERFLOW:   OUTPUT("glError:glPop() [post]GL_STACK_UNDERFLOW: A command would cause a stack underflow, the offending command is ignored.\n" ); break;
 case GL_OUT_OF_MEMORY:     OUTPUT("glError:glPop() [post]GL_OUT_OF_MEMORY: There is not enough memory left to execute the command. The state of the GL is undefined,\n except for the state of the error flags, after this error is recorded.\n" ); break;                       
 case GL_TABLE_TOO_LARGE:   OUTPUT("glError:glPop() [post]GL_TABLE_TOO_LARGE: The specified table exceeds the implementation's maximum supported table size. The offending command is ignored.\n" ); break;
 default: OUTPUT("glError:glPop() unknown error!\n"); break;
 }
#else
 glGetError();
#endif
}

// Calculates using gluProject a point from 3d->2d screen coordinates.
void Point3dto2d( double obX, double obY, double obZ, double *tx, double *ty, double *tz ) {
 GLdouble model_view[16]; glGetDoublev(GL_MODELVIEW_MATRIX, model_view);
 GLdouble projection[16]; glGetDoublev(GL_PROJECTION_MATRIX, projection);
 GLint viewport[4]; glGetIntegerv(GL_VIEWPORT, viewport);
 if ( !gluProject(obX, obY, obZ,
	model_view, projection, viewport,
    tx,ty,tz) ) OUTPUT("Point3dto2d::gluProject returned FALSE. (Failure)\n");
}

// Calculates using gluProject a point from 3d->2d screen coordinates.
void Point3dto2d( int x, int y, int w, int h,  double obX, double obY, double obZ, double *tx, double *ty, double *tz ) {
 GLdouble model_view[16]; glGetDoublev(GL_MODELVIEW_MATRIX, model_view);
 GLdouble projection[16]; glGetDoublev(GL_PROJECTION_MATRIX, projection);
 GLint viewport[4];
 viewport[0]=x;
 viewport[1]=y;
 viewport[2]=w;
 viewport[3]=h;
 if ( !gluProject(obX, obY, obZ,	model_view, projection, viewport, tx,ty,tz) )
  OUTPUT("Point3dto2d::gluProject returned FALSE. (Failure)\n");
}

void glToScreen( double *x, double *y, double *z, double *w ) { 
 // Get the matrices and viewport
 double modelView[16];
 double projection[16];
 double viewport[4];
 double depthRange[2];
 glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
 glGetDoublev(GL_PROJECTION_MATRIX, projection);
 glGetDoublev(GL_VIEWPORT, viewport);
 glGetDoublev(GL_DEPTH_RANGE, depthRange);
 // Compose the matrices into a single row-major transformation
 double T[4][4];
 int r, c, i;
 for (r = 0; r < 4; ++r) {
     for (c = 0; c < 4; ++c) {
         T[r][c] = 0;
         for (i = 0; i < 4; ++i) {
             // OpenGL matrices are column major
             T[r][c] += projection[r + i * 4] * modelView[i + c * 4];
         }
     }
 }
 // Transform the vertex
 double result[4];
 result[0]=T[0][0]*(*x)+T[0][1]*(*y)+T[0][2]*(*z)+T[0][3]*(*w);
 result[1]=T[1][0]*(*x)+T[1][1]*(*y)+T[1][2]*(*z)+T[1][3]*(*w);
 result[2]=T[2][0]*(*x)+T[2][1]*(*y)+T[2][2]*(*z)+T[2][3]*(*w);
 result[3]=T[3][0]*(*x)+T[3][1]*(*y)+T[3][2]*(*z)+T[3][3]*(*w);
 // Homogeneous divide
 double rhw = 1 / result[3];
 *x= (1 + result[0] * rhw) * viewport[2] / 2 + viewport[0],
 *y= (1 - result[1] * rhw) * viewport[3] / 2 + viewport[1],
 *z= (result[3] * rhw) * (depthRange[1] - depthRange[0]) + depthRange[0],
 *w= rhw;
} 

void glToScreen( int vx, int vy, int vw, int vh, double *x, double *y, double *z, double *w ) { 
 // Get the matrices and viewport
 double modelView[16];
 double projection[16];
 double viewport[4];
 viewport[0]=vx;
 viewport[1]=vy;
 viewport[2]=vw;
 viewport[3]=vh;
 double depthRange[2];
 glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
 glGetDoublev(GL_PROJECTION_MATRIX, projection);
 glGetDoublev(GL_DEPTH_RANGE, depthRange);
 // Compose the matrices into a single row-major transformation
 double T[4][4];
 int r, c, i;
 for (r = 0; r < 4; ++r) {
     for (c = 0; c < 4; ++c) {
         T[r][c] = 0;
         for (i = 0; i < 4; ++i) {
             // OpenGL matrices are column major
             T[r][c] += projection[r + i * 4] * modelView[i + c * 4];
         }
     }
 }
 // Transform the vertex
 double result[4];
 result[0]=T[0][0]*(*x)+T[0][1]*(*y)+T[0][2]*(*z)+T[0][3]*(*w);
 result[1]=T[1][0]*(*x)+T[1][1]*(*y)+T[1][2]*(*z)+T[1][3]*(*w);
 result[2]=T[2][0]*(*x)+T[2][1]*(*y)+T[2][2]*(*z)+T[2][3]*(*w);
 result[3]=T[3][0]*(*x)+T[3][1]*(*y)+T[3][2]*(*z)+T[3][3]*(*w);
 // Homogeneous divide
 double rhw = 1 / result[3];
 *x= (1 + result[0] * rhw) * viewport[2] / 2 + viewport[0],
 *y= (1 - result[1] * rhw) * viewport[3] / 2 + viewport[1],
 *z= (result[3] * rhw) * (depthRange[1] - depthRange[0]) + depthRange[0],
 *w= rhw;
} 

#include "Display.h"

void FullscreenQuad() {
 glBegin(GL_QUADS);
 glTexCoord2f(0.0f, 0.0f);  glVertex2i(0,   0);
 glTexCoord2f(1.0f, 0.0f);  glVertex2i(display.w, 0);
 glTexCoord2f(1.0f, 1.0f);  glVertex2i(display.w, display.h);
 glTexCoord2f(0.0f, 1.0f);  glVertex2i(0,   display.h);
 glEnd();
}

void FullscreenQuadXInverted() {
 glBegin(GL_QUADS);
 glTexCoord2f(1.0f, 0.0f);  glVertex2i(0,   0);
 glTexCoord2f(0.0f, 0.0f);  glVertex2i(display.w, 0);
 glTexCoord2f(0.0f, 1.0f);  glVertex2i(display.w, display.h);
 glTexCoord2f(1.0f, 1.0f);  glVertex2i(0,   display.h);
 glEnd();
}

void FullscreenQuadYInverted() {
 glBegin(GL_QUADS);
 glTexCoord2f(0.0f, 1.0f);  glVertex2i(0,   0);
 glTexCoord2f(1.0f, 1.0f);  glVertex2i(display.w, 0);
 glTexCoord2f(1.0f, 0.0f);  glVertex2i(display.w, display.h);
 glTexCoord2f(0.0f, 0.0f);  glVertex2i(0,   display.h);
 glEnd();
}

void FullscreenQuadXYInverted() {
 glBegin(GL_QUADS);
 glTexCoord2f(1.0f, 1.0f);  glVertex2i(0,   0);
 glTexCoord2f(0.0f, 1.0f);  glVertex2i(display.w, 0);
 glTexCoord2f(0.0f, 0.0f);  glVertex2i(display.w, display.h);
 glTexCoord2f(1.0f, 0.0f);  glVertex2i(0,   display.h);
 glEnd();
}

#include "Crayon.h"
Crayon GLTexImage::Probe(int probeX, int probeY, having fromTopLeft) {
 Crayon result(clear);
 if (probeX < 0 || probeY < 0 || probeX >= width || probeY >= height) {}
 else {
  int r = 0;
  int g = 0;
  int b = 0;
  int a = 0;
  if (stride == 3) {
   a = 255;
   if (direction > 0) { // GL_RGB
    r = (int)pixel.list[probeX + probeY * width];
    g = (int)pixel.list[probeX + probeY * width+1];
    b = (int)pixel.list[probeX + probeY * width+2];
   } else { // GL_BGR
    b = (int)pixel.list[probeX + probeY * width];
    g = (int)pixel.list[probeX + probeY * width + 1];
    r = (int)pixel.list[probeX + probeY * width + 2];
   }
  } else if (stride == 4) {
   if (direction > 0) { // GL_RGBA
    r = (int)pixel.list[probeX + probeY * width];
    g = (int)pixel.list[probeX + probeY * width + 1];
    b = (int)pixel.list[probeX + probeY * width + 2];
    a = (int)pixel.list[probeX + probeY * width + 3];
   }
   else { // GL_BGRA
    b = (int)pixel.list[probeX + probeY * width];
    g = (int)pixel.list[probeX + probeY * width + 1];
    r = (int)pixel.list[probeX + probeY * width + 2];
    a = (int)pixel.list[probeX + probeY * width + 3];
   }
  }
  result.Int(r, g, b, a);
 }
 return result;
}

Crayon GLTexImage::Probe(int probeX, int probeY) {
 return Probe(width / 2 + probeX, height / 2 + probeY, true);
}


#include "Crayon.h"
Crayon GLTexImage::Probe(int probeX, int probeY, bool flippedX, bool flippedY) {
 if (flippedX) probeX = width - probeX;
 if (flippedY) probeY = height - probeY;
 Crayon result(clear);
 if (probeX < 0 || probeY < 0 || probeX >= width || probeY >= height) {}
 else {
  int r = 0;
  int g = 0;
  int b = 0;
  int a = 0;
  if (stride == 3) {
   a = 255;
   if (direction > 0) { // GL_RGB
    r = (int)pixel.list[probeX + probeY * width];
    g = (int)pixel.list[probeX + probeY * width + 1];
    b = (int)pixel.list[probeX + probeY * width + 2];
   }
   else { // GL_BGR
    b = (int)pixel.list[probeX + probeY * width];
    g = (int)pixel.list[probeX + probeY * width + 1];
    r = (int)pixel.list[probeX + probeY * width + 2];
   }
  }
  else if (stride == 4) {
   if (direction > 0) { // GL_RGBA
    r = (int)pixel.list[probeX + probeY * width];
    g = (int)pixel.list[probeX + probeY * width + 1];
    b = (int)pixel.list[probeX + probeY * width + 2];
    a = (int)pixel.list[probeX + probeY * width + 3];
   }
   else { // GL_BGRA
    b = (int)pixel.list[probeX + probeY * width];
    g = (int)pixel.list[probeX + probeY * width + 1];
    r = (int)pixel.list[probeX + probeY * width + 2];
    a = (int)pixel.list[probeX + probeY * width + 3];
   }
  }
  result.Int(r, g, b, a);
 }
 return result;
}

int Zeroi=0;
char gl_info[1024];
char old_gl_info[1024];

#if defined(GL_DEBUG) 
void glReportError( GLenum err ) {
 switch ( err ) {
 case GL_NO_ERROR: break;                       
 case GL_INVALID_ENUM:      OUTPUT("glError:%s GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument, offending command is ignored.\n", gl_info ); break;
 case GL_INVALID_VALUE:     OUTPUT("glError:%s GL_INVALID_VALUE: A numeric argument is out of range, offending command is ignored.\n", gl_info ); break;                       
 case GL_INVALID_OPERATION: OUTPUT("glError:%s GL_INVALID_OPERATION: The specified operation is not allowed in the current state, offending command is ignored.\n", gl_info ); break;
 case GL_STACK_OVERFLOW:    OUTPUT("glError:%s GL_STACK_OVERFLOW: A command would cause a stack overflow, the offending command is ignored.\n", gl_info ); break;
 case GL_STACK_UNDERFLOW:   OUTPUT("glError:%s GL_STACK_UNDERFLOW: A command would cause a stack underflow, the offending command is ignored.\n", gl_info ); break;
 case GL_OUT_OF_MEMORY:     OUTPUT("glError:%s GL_OUT_OF_MEMORY: There is not enough memory left to execute the command. The state of the GL is undefined,\n except for the state of the error flags, after this error is recorded.\n", gl_info ); break;                       
 case GL_TABLE_TOO_LARGE:   OUTPUT("glError:%s GL_TABLE_TOO_LARGE: The specified table exceeds the implementation's maximum supported table size. The offending command is ignored.\n", gl_info ); break;
 default: OUTPUT("glError:%s unknown error!\n", gl_info); break;
 }
}
#endif