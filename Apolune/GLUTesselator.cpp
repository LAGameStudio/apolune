#include "GLUTesselator.h"

#define GT GLUTesselator

void CALLBACK tessBeginData( GLenum t, GT* g ) { g->_BeginData(t); }
void CALLBACK tessVertexData(void* d, GT* g)   { g->_VertexData(static_cast<double*>(d));}
void CALLBACK tessCombineData( double c[3], GLdouble *d[4], GLfloat w[4],  void** o, GT* g ) {
   /* Example from http://www.flipcode.com/archives/Polygon_Tessellation_In_OpenGL.shtml
     was used partially here, but unfortunately that archive is corrupt!  Some of
     the code has been overwritten by some process which injected the word "indent"
     .. so, we only use part of it and filled in the gaps with 
     http://www.songho.ca/opengl/gl_tessellation.html 
   */
  GLdouble *p = (GLdouble *) malloc(6 * sizeof(GLdouble));
  // New vertex info
  p[0] = (GLdouble) c[0];
  p[1] = (GLdouble) c[1];
  p[2] = (GLdouble) c[2];
  p[3] = w[0] * d[0][3] + w[1] * d[1][3] + w[2] * d[2][3] + w[3] * d[3][3];
  p[4] = w[0] * d[0][4] + w[1] * d[1][4] + w[2] * d[2][4] + w[3] * d[3][4];
  p[5] = w[0] * d[0][5] + w[1] * d[1][5] + w[2] * d[2][5] + w[3] * d[3][5];
  *o = p;
  Vertex *n=g->out->Add( c[0], c[1], c[2] );
  n->data.pointer=(void *)p;
 /*
 static const double* _c = static_cast<const double*>(c);
 static const GLfloat* _w = static_cast<const GLfloat*>(w);
 static GLdouble* _vd[4];
 for ( unsigned int i=0; i<4; i++ ) {
  _vd[i] = static_cast<GLdouble*>(vd[i]);
 }
 g->_CombineData( _c, _vd, _w, o );*/
}
void CALLBACK tessEdgeFlagData( GLboolean f, GLUTesselator* g ) { g->_EdgeFlagData(f?true:false); }
void CALLBACK tessErrorData( GLenum e, GLUTesselator* g )       { g->_ErrorData(e); }
void CALLBACK tessEndData( GLUTesselator* g )                   { g->_EndData(); }
void CALLBACK tessEdgeData(void* d, GLUTesselator* g)  { g->_VertexData(static_cast<double*>(d));}

void GLUTesselator::InitGLUTesselatorFunctions() {
 TESS_BEGIN_DATA=     (GLUTesselatorFunction)tessBeginData;
 TESS_VERTEX_DATA=    (GLUTesselatorFunction)tessVertexData;
 TESS_COMBINE_DATA=   (GLUTesselatorFunction)tessCombineData;
 TESS_EDGE_FLAG_DATA= (GLUTesselatorFunction)tessEdgeFlagData;
 TESS_ERROR_DATA=     (GLUTesselatorFunction)tessErrorData;
 TESS_END_DATA=       (GLUTesselatorFunction)tessEndData;
}

#undef GT