#pragma once

#include "Cartesian.h"
#include "Vertexd.h"
#include "Crayon.h"
#include "GLImage.h"
#include "RoundedRectangle.h"
#include "FBO.h"

// Draws graphics on a virtual screen instead of directly to the display.
// Think of this as a rudimentary canvas object, for drawing GUI, 2d sprites,
// etc.

class VirtualGraphics
{
public:
 RoundedRectangles rrects;
 Cartesiand resolution; // x and y are w/h in virtual pixels, w and h are the display's relative size of the pixel.
 int w,h; double wd,hd;
 Cartesiand pixelsPerUnit;
 Cartesiand tiny,medium,large,huge; // Font sizes relative to this resolution
 FBOColorDepth fbo1,fbo2,fbo3,fbo4;
 VirtualGraphics() {}
 void Start();
 void End();
 void LineWidth(double d);
 void Resize(int w, int h);
 void DrawLine(Crayon color, double x1, double y1, double x2, double y2);
 void DrawLine(Crayon color, int x1, int y1, int x2, int y2);
 void DrawLine2Color(Crayon color1, Crayon color2, int x1, int y1, Zint x2, Zint y2);
 void DrawLine2Color(Crayon color1, Crayon color2, Zint x1, int y1, Zint x2, int y2);
 void DrawLine2Color(Crayon color1, Crayon color2, int x1, Zint y1, int x2, Zint y2);
 void DrawLine2Color(Crayon color1, Crayon color2, Zint x1, Zint y1, Zint x2, Zint y2);
 void DrawLine2Color(Crayon color1, Crayon color2, int x1, int y1, int x2, int y2);
 void DrawLine2Color(Crayon color1, Crayon color2, Zfloat x1, Zfloat y1, Zfloat x2, Zfloat y2);
 void DrawLine2Color(Crayon color1, Crayon color2, float x1, float y1, float x2, float y2);
 void DrawLine2Color(Crayon color1, Crayon color2, Zdouble x1, Zdouble y1, Zdouble x2, Zdouble y2);
 void DrawLine2Color(Crayon color1, Crayon color2, double x1, double y1, double x2, double y2);
 void DrawLine2Color3d(Crayon color1, Crayon color2, double x1, double y1, double z1, double x2, double y2, double z2 );
 void DrawLine3d2Color(Crayon color1, Crayon color2, Vertexd *source, Vertexd *terminus );
 void Rectangle(Crayon color, Cartesian *rect);
 void Rectangle(Crayon color,int x, Zint y, Zint x2, Zint y2);
 void Rectangle(Crayon color,int x, int y, Zint x2, Zint y2);
 void Rectangle(Crayon color,int x, int y, int x2, Zint y2);
 void Rectangle(Crayon color,Zint x, Zint y, Zint x2, Zint y2);
 void Rectangle(Crayon color,int x, int y, int x2, int y2);
 void Rectangle(Crayon color,double x, Zdouble y, Zdouble x2, Zdouble y2);
 void Rectangle(Crayon color,double x, double y, Zdouble x2, Zdouble y2);
 void Rectangle(Crayon color,double x, double y, double x2, Zdouble y2);
 void Rectangle(Crayon color,Zdouble x, Zdouble y, Zdouble x2, Zdouble y2);
 void Rectangle(Crayon color,double x, double y, double x2, double y2);
 void Area(Crayon color, int x1,Zint y1,Zint x2,Zint y2);
 void Area(Crayon color, int x1,int y1,Zint x2,Zint y2);
 void Area(Crayon color, int x1,int y1,int x2,Zint y2);
 void Area(Crayon color, Zint x1,Zint y1,Zint x2,Zint y2);
 void Area(Crayon color, int x1,int y1,int x2,int y2);
 void Area(Crayon color, double x1,double y1,double x2,double y2);
 void Area(Crayon color, Cartesian *rect);
 void Area(GLImage *texture, Crayon color, Blends blend, ScreenPosition *screen);
 void BlendArea(Crayon color, Blends blend, int x1,int y1,int x2,int y2);
 void Stretchi( GLImage *source, Crayon tint, float alpha, Blends blend, int x, int y, int w, int h );
 void StretchiNoAlpha( GLImage *source, Crayon tint, Blends blend, int x, int y, int w, int h );
 void StretchdNoAlpha( GLImage *source, Crayon tint, Blends blend, double x, double y, int w, int h );
 void TexturedQuad( GLImage *source, Crayon tint, Blends blend, Cartesian *tops, Cartesian *bottoms );
 void TexturedQuad( GLImage *source, Crayon tint, Blends blend, Cartesian *tops, Cartesian *bottoms, bool xreversed );
 void FBOTexturedQuad( GLImage *source, Crayon tint, Blends blend, int x, int y, int w, int h );
 void FBOTexturedQuadUpsideDown( GLImage *source, Crayon tint, Blends blend, int x, int y, int w, int h );
 void FBOQuad( int x, int y, int w, int h );
 void FBOQuadUpsideDown( int x, int y, int w, int h );
 void FBOQuadReversed( int x, int y, int w, int h );
 void FBOQuadReversedUpsideDown( int x, int y, int w, int h );
 void Quadi( int x, int y, int w, int h );
 void Quadi( GLImage * source, Crayon tint, float alpha, Blends blend, int x, int y, int w, int h );
 void QuadiNoAlpha( GLImage * source, Crayon tint, Blends blend, Cartesian *rect );
 void SkewedQuadiNoAlpha( GLImage * source, Crayon tint, Blends blend, int x, int y, int w, int h, int xSkew );
 void SkewedQuaddNoAlpha( GLImage * source, Crayon tint, Blends blend, double x, double y, int w, int h, int xSkew );
 void SkewedQuadiNoAlpha( GLImage * source, Crayon tint, Blends blend, int x, int y, int w, int h, Cartesian *skew );
 void QuadiNoAlpha( GLImage * source, Crayon tint, Blends blend, int x, int y, int w, int h );
 void QuadiNoAlphaReversed( GLImage * source, Crayon tint, Blends blend, int x, int y, int w, int h );
 void QuaddNoAlpha( GLImage * source, Crayon tint, Blends blend, double x, double y, double w, double h );
 void XReversibleQuadiNoAlpha( GLImage * source, Crayon tint, Blends blend, int x, int y, int w, int h, bool xreversed );
 void Swabd( GLImage *source, Crayon tint, Blends blend, double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4 );
 void RotatedRecti( GLImage *source, Crayon tint, Blends blend, int x, int y, int l, int w, float a );
 void RotatedRectf( GLImage *source, Crayon tint, Blends blend, float x, float y, float h, float w, float a );
 void XInvertedPivotedRotatedRectf( GLImage *source, Crayon tint, Blends blend, float x, float y, float px, float py, float h2, float w2, float a );
 void PivotedRotatedRectf( GLImage *source, Crayon tint, Blends blend, float x, float y, float px, float py, float h2, float w2, float a );
 void FBOPivotedRotatedRectf( GLImage *source, Crayon tint, Blends blend, float x, float y, float px, float py, float h2, float w2, float a );
 void RotatedIsotrapf( GLImage *source, Crayon tint, Blends blend, float x, float y, float h, float w1, float w2, float a );
 void RotatedTrif( GLImage *source, Crayon tint, Blends blend, float x, float y, float h, float w2, float a );
 void PointSprite( GLImage *source, Crayon tint, Vertexd *coord, float size );
 void Coordinator(void);
 void DrawBar( Crayon color, float one, int x, int y, int w, int h, int divisions );
 void DrawBarHoriz( Crayon color, float one, int x, int y, int w, int h, int divisions );
 void DrawBar( Blends blendFill, Blends blendLine, Crayon color, float one, int x, int y, int w, int h, int divisions );
 void DrawBarHoriz( Blends blendFill, Blends blendLine, Crayon color, float one, int x, int y, int w, int h, int divisions );
 void DrawCircle( double cx, double cy, double R, int segments);
 void DrawNamedPoint( Crayon color, const char *name, double x, double y );
 RoundedRectangle *RoundRect( Crayon border, Crayon fill, Crayon sep, int x, int y, int width, int height, int radius, int precision, float lineWidth );
 void RoundRect9( GLImage* corner, GLImage* border, GLImage* interior, int x, int y, int w, int h, int cornerBorder );
 void Text(string t, double x, double y, double fontW, double fontH, bool centered) { Text(t.c_str(),x,y,fontW,fontH,centered); }
 void Text(const Zstring& t, double x, double y, double fontW, double fontH, bool centered ) { Text(t.value.c_str(),x,y,fontW,fontH,centered); }
 void Text(const char * t, double x, double y, double fontW, double fontH, bool centered); 
 void Text(string t, double x, double y, double fontW, double fontH, bool centered, double angle) { Text(t.c_str(),x,y,fontW,fontH,centered,angle); }
 void Text(const char * t, double x, double y, double fontW, double fontH, bool centered,double angle);
 void MultilineText(const Zstring& t, double x, double y, double fontW, double fontH, double lineSpace, bool centered) { MultilineText(t.value.c_str(),x,y,fontW,fontH,lineSpace,centered); }
 void MultilineText(const string& t, double x, double y, double fontW, double fontH, double lineSpace, bool centered) { MultilineText(t.c_str(),x,y,fontW,fontH,lineSpace,centered); }
 void MultilineText(const char *t, double x, double y, double fontW, double fontH, double lineSpace, bool centered);
};

extern VirtualGraphics vg;