/*********************************************************************************************
 *  __    __________________   ________________________________   __________  ________       *
 * /\ \  /\  __ \  ___\__  _\ /\  __ \  ___\__  _\  == \  __ \ "-.\ \  __ \ \/\ \__  _\ (tm) *
 * \ \ \_\_\ \/\ \___  \/\ \/ \ \  __ \___  \/\ \/\  __<\ \/\ \ \-.  \  __ \ \_\ \/\ \/      *
 *  \ \_____\_____\_____\ \_\  \ \_\ \_\_____\ \_\ \_\ \_\_____\_\\"\_\_\ \_\_____\ \_\      *
 *   \/_____/_____/_____/\/_/   \/_/\/_/_____/\/_/\/_/\/_/_____/_/ \/_/_/\/_/_____/\/_/      *
 *    --------------------------------------------------------------------------------       *
 *     Lost Astronaut Game Development Framework (c) 2007-2017 H. Elwood Gilliland III       *
 *********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III *
 *********************************************************************************************/
#pragma once
#include "GLSetup.h"
#include "GLWindow.h"
#include "GLImage.h"
#include "RenderProcess.h"
#include "Vertexd.h"

inline void Blending( Blends blend );
inline void Blending( Blendings mode );  // it is unclear why this has to be here, but it has to be.

SINGLE(BlendStatement,{});
 Blends blend;
 void Apply() { Blending(blend); }
PLURAL(BlendStatement,BlendStatements,{});
ENDSET(BlendStatements);

SINGLE(BlendProgram,{});
 Zstring name;
 BlendStatements statements;
 Zp<BlendStatement> current;
 Zdouble expired,duration;
 void Apply() { if ( current ) current->Apply(); }
 void Rewind() {
  expired=0.0;
  current=(BlendStatement *) statements.first;
 }
 void Advance() {
  if ( !current ) current=(BlendStatement *) statements.first;
  else {
   current=(BlendStatement *)current->next;
  }
 }
 void Between() {
  if ( !current ) current=(BlendStatement *) statements.first;
  expired+=FRAMETIMEd;
  if ( expired >= duration ) {
   expired=0.0;
   Advance();
  }
 }
PLURAL(BlendProgram,BlendPrograms,{});
 BlendProgram *find( const char *matching ) {
  FOREACH(BlendProgram,b) if ( b->name.begins(matching) ) return b;
  return null;
 }
ENDSET(BlendPrograms);

class pBlending : public RenderProcess { public:
 Blends blend;
 void Render( GLWindow *surface ) {
  Blending(blend);
 }
};

void DrawLine(GLWindow *surface, Crayon color, int x1, int y1, int x2, int y2);
class pDrawLine : public RenderProcess { public:
 Crayon color; int x1,y1,x2,y2;
 void Render( GLWindow *surface ) { DrawLine(surface,color,x1,y1,x2,y2); }
};

void DrawLine(Crayon color, double x1, double y1, double x2, double y2);
void DrawLine(Crayon color, int x1, int y1, int x2, int y2);

void DrawCross(Crayon color, int x, int y, int cw);

void DrawLine2Color(GLWindow *surface, Crayon color1, Crayon color2, int x1, int y1, int x2, int y2);
class pDrawLine2Color : public RenderProcess { public:
 Crayon color1,color2; int x1,y1,x2,y2;
 void Render( GLWindow *surface ) { DrawLine2Color(surface,color1,color2,x1,y1,x2,y2); }
};

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
void Rectangle(Crayon color, Cartesiand *rect);
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
void Rectangle(GLWindow *surface, Crayon color, Zint x1, Zint y1, Zint x2, Zint y2 );
void Rectangle(GLWindow *surface, Crayon color, int x1,int y1,int x2,int y2);
void Rectangle(GLWindow *surface, Crayon color, Cartesian *c );
class pRectangle : public RenderProcess { public:
 Crayon color; int x1,y1,x2,y2;
 void Render( GLWindow *surface ) { Rectangle(surface,color,x1,y1,x2,y2); }
};

void Square(Crayon color, Cartesian *rect);
void Square(Crayon color, Cartesiand *rect);
void Square(Crayon color,int x, int y, int w);
void Square(Crayon color,double x, double y, double w);

void Area(GLWindow *surface, Crayon color, int x1, Zint y1, Zint x2, Zint y2 );
void Area(GLWindow *surface, Crayon color, int x1, int y1, Zint x2, Zint y2 );
void Area(GLWindow *surface, Crayon color, int x1, int y1, int x2, Zint y2 );
void Area(GLWindow *surface, Crayon color, int x1, Zint y1, Zint x2, Zint y2 );
void Area(GLWindow *surface, Crayon color, Zint x1, Zint y1, Zint x2, Zint y2 );
void Area(GLWindow *surface, Crayon color, int x1,int y1,int x2,int y2);
void Area(Crayon color, int x1,Zint y1,Zint x2,Zint y2);
void Area(Crayon color, int x1,int y1,Zint x2,Zint y2);
void Area(Crayon color, int x1,int y1,int x2,Zint y2);
void Area(Crayon color, Zint x1,Zint y1,Zint x2,Zint y2);
void Area(Crayon color, int x1,int y1,int x2,int y2);
void Area(Crayon color, double x1,double y1,double x2,double y2);
void Area(Crayon color, Cartesiand *rect);
void Area(Crayon color, Cartesian *rect);
void Area(GLImage *texture, Crayon color, Blends blend, ScreenPosition *screen);
class pArea : public RenderProcess { public:
 Crayon color; int x1, y1, x2, y2;
 void Render( GLWindow *surface ) { Area(surface,color,x1,y1,x2,y2); }
};

void BlendArea(Crayon color, Blends blend, int x1,int y1,int x2,int y2);
void BlendArea(GLWindow *surface, Crayon color, Blends blend, int x1,int y1,int x2,int y2);
class pBlendArea : public RenderProcess { public:
 Crayon color; Blends blend; int x1, y1, x2, y2;
 void Render( GLWindow *surface ) { BlendArea(surface,color,blend,x1,y1,x2,y2); }
};

//void CenterRotationScale( GLWindow *surface, GLImage *b, int cx, int cy, Vertex *rotation, Vertex *scale );
//void Pivot2d( GLWindow *surface, GLImage *b, Vertex *pivot, Vertex *axis, Vertex *rotation );

void Stretch( GLWindow * surface, GLImage * b, int dx, int dy, int sx, int sy, int w, int h, int sw, int sh );
class pStretch : public RenderProcess { public:
 GLImage *b; int dx, dy, sx, sy, w, h, sw, sh;
 void Render( GLWindow *surface ) { Stretch(surface,b,dx,dy,sx,sy,w,h,sw,sh); }
};

void BestFit( GLImage *tex, Crayon tint, Blends t, double x, double y, double w, double h, double angle=0.0, double pivotOfsX=0.0, double pivotOfsY=0.0, bool clamp1=false, bool hflip=false, bool vflip=false );
void BestFit( GLImage *tex, Crayon tint, Blends t, Cartesian *extents, double angle=0.0, double pivotOfsX=0.0, double pivotOfsY=0.0, bool clamp1=false, bool hflip=false, bool vflip=false );
void BestFit( GLImage *tex, Crayon tint, Blends t, Cartesiand *extents, double angle=0.0, double pivotOfsX=0.0, double pivotOfsY=0.0, bool clamp1=false, bool hflip=false, bool vflip=false );
void BestFit( GLImage *tex, Crayon tint, Blends t, ScreenPosition *extents, double angle=0.0, double pivotOfsX=0.0, double pivotOfsY=0.0, bool clamp1=false, bool hflip=false, bool vflip=false );
void BestFit( GLImage *tex, Crayon tint, Blends t, double x, double y, int w, int h, double angle=0.0, double pivotOfsX=0.0, double pivotOfsY=0.0, bool clamp1=false, bool hflip=false, bool vflip=false );

void Stretchi( GLWindow *dest, GLImage *source, Crayon tint, float alpha, Blends blend, int x, int y, int w, int h );
void Stretchi( GLImage *source, Crayon tint, float alpha, Blends blend, int x, int y, int w, int h );
class pStretchi : public RenderProcess { public:
 GLImage *source; Crayon tint; float alpha; Blends blend; int x, y, w, h;
 void Render( GLWindow *surface ) { Stretchi(surface,source,tint,alpha,blend,x,y,w,h); }
};

void Stretch( GLImage *source, Crayon tint, Blends blend, int x, int y, int w, int h );
void StretchiNoAlpha( GLWindow *dest, GLImage *source, Crayon tint, Blends blend, int x, int y, int w, int h );
void StretchiNoAlpha( GLImage *source, Crayon tint, Blends blend, int x, int y, int w, int h );
void StretchiNoAlphaXReversed( GLImage *source, Crayon tint, Blends blend, int x, int y, int w, int h );
class pStretchiNoAlpha : public RenderProcess { public:
 GLImage *source; Crayon tint; Blends blend; int x, y, w, h;
 void Render( GLWindow *surface ) { StretchiNoAlpha(surface,source,tint,blend,x,y,w,h); }
};


void StretchiNoAlphaXPartial( GLImage *source, Crayon tint, Blends blend, int x, int y, int w, int h, double startTC, double endTC );
void StretchiNoAlphaYPartial( GLImage *source, Crayon tint, Blends blend, int x, int y, int w, int h, double startTC, double endTC );
void StretchiNoAlphaXYPartial( GLImage *source, Crayon tint, Blends blend, int x, int y, int w, int h, double startTCx, double endTCx, double startTCy, double endTCy );

void StretchXtc( GLImage *source,  Crayon tint, Blends blend, int x, int y, int w, int h, double startTC=0.0, double endTC=1.0 );
void StretchYtc( GLImage *source,  Crayon tint, Blends blend, int x, int y, int w, int h, double startTC=0.0, double endTC=1.0 );
void StretchXYtc( GLImage *source, Crayon tint, Blends blend, int x, int y, int w, int h, double startTCx=0.0, double endTCx=1.0, double startTCy=0.0, double endTCy=1.0 );
void StretchXtc( GLImage *source,  Crayon tint, Blends blend, double x, double y, double w, double h, double startTC=0.0, double endTC=1.0 );
void StretchYtc( GLImage *source,  Crayon tint, Blends blend, double x, double y, double w, double h, double startTC=0.0, double endTC=1.0 );
void StretchXYtc( GLImage *source, Crayon tint, Blends blend, double x, double y, double w, double h, double startTCx=0.0, double endTCx=1.0, double startTCy=0.0, double endTCy=1.0 );

void Stretchd( GLImage * source, Crayon tint, float alpha, Blends blend, Cartesiand *rect );
void StretchdNoAlpha( GLWindow * dest, GLImage * source, Crayon tint, Blends blend, double x, double y, int w, int h );
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
class pQuadiNoAlpha : public RenderProcess { public:
 GLImage *source; Crayon tint; Blends blend; int x, y, w, h;
 void Render( GLWindow *surface ) { QuadiNoAlpha(source,tint,blend,x,y,w,h); }
};

void QuaddNoAlpha( GLImage * source, Crayon tint, Blends blend, double x, double y, double w, double h );

void XReversibleStretchiNoAlpha( GLWindow *dest, GLImage *source, Crayon tint, Blends blend, int x, int y, int w, int h, bool xreversed );
void XReversibleQuadiNoAlpha( GLImage * source, Crayon tint, Blends blend, int x, int y, int w, int h, bool xreversed );

void StretchiNoAlpha2Sided( GLWindow *dest, GLImage *source, Crayon tint, Blends blend, int x, int y, int w, int h );
class pStretchiNoAlpha2Sided : public RenderProcess { public:
 GLImage *source; Crayon tint; Blends blend; int x, y, w, h;
 void Render( GLWindow *surface ) { StretchiNoAlpha2Sided(surface,source,tint,blend,x,y,w,h); }
};

void Additivei( GLWindow * dest, GLImage * source, Crayon tint, int x, int y, int w, int h );
class pAdditivei : public RenderProcess { public:
 GLImage *source; Crayon tint; int x,y,w,h;
 void Render( GLWindow *surface ) { Additivei(surface,source,tint,x,y,w,h); }
};

void Stretchf( GLWindow *dest, GLImage *source, Crayon tint, float alpha, float x, float y, float w, float h );
class pStretchf : public RenderProcess { public:
 GLImage *source; Crayon tint; float alpha, x,y,w,h;
 void Render( GLWindow *surface ) { Stretchf(surface,source,tint,alpha,x,y,w,h); }
};

void Swabi( GLWindow *dest, GLImage *source, Crayon tint, float alpha, Blends blend, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4 );
class pSwabi : public RenderProcess { public:
 GLImage *source; Crayon tint; float alpha; Blends blend; int x1,y1,x2,y2,x3,y3,x4,y4;
 void Render( GLWindow *surface ) { Swabi(surface,source,tint,alpha,blend,x1,y1,x2,y2,x3,y3,x4,y4); }
};

void Swabf( GLWindow *dest, GLImage *source, Crayon tint, float alpha, Blends blend, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4 );
class pSwabf : public RenderProcess { public:
 GLImage *source; Crayon tint; Blends blend; float alpha, x1,y1,x2,y2,x3,y3,x4,y4;
 void Render( GLWindow *surface ) { Swabf(surface,source,tint,alpha,blend,x1,y1,x2,y2,x3,y3,x4,y4); }
};

void ImageColorBlend( GLImage *i, Crayon t, Blends b );
void TexColorBlend( GLuint *i, Crayon t, Blends b );

void Swabd( GLWindow *dest, GLImage *source, Crayon tint, Blends blend, double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4 );
void Swabd( GLImage *source, Crayon tint, Blends blend, double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4 );
void SwabdYInvert( GLImage *source, Crayon tint, Blends blend, double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4 );

void RotatedRecti( GLWindow *dest, GLImage *source, Crayon tint, Blends blend, int x, int y, int l, int w, float a );
void RotatedRecti( GLImage *source, Crayon tint, Blends blend, int x, int y, int l, int w, float a );
class pRotatedRecti : public RenderProcess { public:
 GLImage *source; Crayon tint; Blends blend; int x, y, w, l; float a;
 void Render( GLWindow *surface ) { RotatedRecti(surface,source,tint,blend,x,y,l,w,a); }
};
void RotatedRecti( GLImage *source, Crayon tint, Blends blend, int x, int y, int l, int w, float a );
void RotatedRectf( GLWindow *dest, GLImage *source, Crayon tint, Blends blend, float x, float y, float h, float w, float a );
void RotatedRectf( GLImage *source, Crayon tint, Blends blend, float x, float y, float h, float w, float a );
void RotatedRectd( GLImage *source, Crayon tint, Blends blend, double x, double y, double h, double w2, double a );
class pRotatedRectf : public RenderProcess { public:
 GLImage *source; Crayon tint; Blends blend; float x, y, w2, h, a;
 void Render( GLWindow *surface ) { RotatedRectf(surface,source,tint,blend,x,y,h,w2,a); }
};


void FBORotatedRectf( GLImage *source, Crayon tint, Blends blend, float x, float y, float h2, float w2, float a );
void XInvertedPivotedRotatedRectf( GLImage *source, Crayon tint, Blends blend, float x, float y, float px, float py, float h2, float w2, float a );
void PivotedRotatedRectf( GLWindow *dest, GLImage *source, Crayon tint, Blends blend, float x, float y, float px, float py, float h2, float w2, float a );
void PivotedRotatedRectf( GLImage *source, Crayon tint, Blends blend, float x, float y, float px, float py, float h2, float w2, float a );
void PivotedRotatedRectd( GLImage *source, Crayon tint, Blends blend, double x, double y, double px, double py, double h2, double w2, double a );
void PivotedRotatedRectd( GLImage *source, Crayon tint, Blends blend, double x, double y, double px, double py, double h2, double w2, double a, having leaveTextureMode );
void PivotedRotatedFlippableRectd( GLImage *source, Crayon tint, Blends blend, double x, double y, double px, double py, double h2, double w2, double a, bool flipX, bool flipY );
void FBOPivotedRotatedRectf( GLImage *source, Crayon tint, Blends blend, float x, float y, float px, float py, float h2, float w2, float a );
void FBOPivotedRotatedRectd( GLImage *source, Crayon tint, Blends blend, double x, double y, double px, double py, double h2, double w2, double a );

class pPivotedRotatedRectf : public RenderProcess { public:
 GLImage *source; Crayon tint; Blends blend; float x, y, px, py, h2, w2, a;
 void Render( GLWindow *surface ) { PivotedRotatedRectf(surface,source,tint,blend,x,y,px,py,h2,w2,a); }
};

void TextureLine( GLWindow *dest, GLImage *source, Crayon tint, Blends blend, Cartesian *c, int w2 );
void TextureLine( GLImage *source, Crayon tint, Blends blend, Cartesian *c, int w2 );
void TextureLine( GLImage *source, Crayon tint, Blends blend, Cartesiand *c, double w2 );

void RotatedIsotrapf( GLWindow *dest, GLImage *source, Crayon tint, Blends blend, float x, float y, float h, float w1, float w2, float a );
void RotatedIsotrapf( GLImage *source, Crayon tint, Blends blend, float x, float y, float h, float w1, float w2, float a );
void RotatedIsotrapd( GLImage *source, Crayon tint, Blends blend, double x, double y, double h, double w1, double w2, double a );

class pRotatedIsotrapf : public RenderProcess { public:
 GLImage *source; Crayon tint; Blends blend; float x, y, w1, w2, h, a;
 void Render( GLWindow *surface ) { RotatedIsotrapf(surface,source,tint,blend,x,y,h,w1,w2,a); }
};

void RotatedTrif( GLWindow *dest, GLImage *source, Crayon tint, Blends blend, float x, float y, float h, float w2, float a );
void RotatedTrif( GLImage *source, Crayon tint, Blends blend, float x, float y, float h, float w2, float a );
void LineRectf( GLWindow *dest, GLImage *source, Crayon tint, Blends blend, float x1, float y1, float x2, float y2, float w2 );

void DrawPixelf( GLWindow *dest, Crayon tint, float x, float y );
class pDrawPixelf : public RenderProcess { public:
 Crayon tint; float x,y;
 void Render( GLWindow *surface ) { DrawPixelf(surface,tint,x,y); }
};

void BlendTint( GLWindow *surface, GLImage *b, Crayon tint, Blends blend, int x, int y, int w, int h );
class pBlendTint : public RenderProcess { public:
 GLImage *b; Crayon tint; Blends blend; int x,y,w,h;
 void Render( GLWindow *surface ) { BlendTint(surface,b,tint,blend,x,y,w,h); }
};

void Blend( GLWindow *surface, GLImage *b, Blends blend, int dx, int dy, int sx, int sy, int w, int h, int sw, int sh );
class pBlend : public RenderProcess { public:
 GLImage *b; Blends blend; int dx,dy,sx,sy,w,h,sw,sh; 
 void Render( GLWindow *surface ) { Blend( surface, b, blend, dx, dy, sx, sy, w, h, sw, sh ); }
};

void Blendf( GLWindow *surface, GLImage *b, Blends blend, float x, float y, float w, float h);
class pBlendf : public RenderProcess { public:
 GLImage *b; Blends blend; float x,y,w,h;
 void Render( GLWindow *surface ) { Blendf(surface,b,blend,x,y,w,h); }
};

void Additive( GLWindow *surface, GLImage *b, int dx, int dy, int sx, int sy, int w, int h, int sw, int sh);
class pAdditive : public RenderProcess { public:
 GLImage *b; int dx,dy,sx,sy,w,h,sw,sh;
 void Render( GLWindow *surface ) { Additive(surface,b,dx,dy,sx,sy,w,h,sw,sh); }
};

void Multiply( GLWindow *surface, GLImage *b, int dx, int dy, int sx, int sy, int w, int h, int sw, int sh);
class pMultiply : public RenderProcess { public:
 GLImage *b; int dx,dy,sx,sy,w,h,sw,sh;
 void Render( GLWindow *surface ) { Multiply(surface,b,dx,dy,sx,sy,w,h,sw,sh); }
};

void Mask( GLWindow *surface, GLImage *b, int dx, int dy, int sx, int sy, int w, int h, int sw, int sh);
class pMask : public RenderProcess { public:
 GLImage *b; int dx,dy,sx,sy,w,h,sw,sh;
 void Render( GLWindow *surface ) { Mask(surface,b,dx,dy,sx,sy,w,h,sw,sh); }
};

void Solarize( GLWindow *surface, GLImage *b, int dx, int dy, int sx, int sy, int w, int h, int sw, int sh);
class pSolarize : public RenderProcess { public:
 GLImage *b; int dx,dy,sx,sy,w,h,sw,sh;
 void Render( GLWindow *surface ) { Solarize(surface,b,dx,dy,sx,sy,w,h,sw,sh); }
};

void AdditiveTransformf (GLWindow *surface, GLImage *b, float x1, float x2, float x3, float x4, float y1, float y2, float y3, float y4);
class pAdditiveTransformf : public RenderProcess { public:
 GLImage *b; float x1,x2,x3,x4,y1,y2,y3,y4;
 void Render( GLWindow *surface ) { AdditiveTransformf(surface,b,x1,x2,x3,x4,y1,y2,y3,y4); }
};

void MaskTransformf( GLWindow *surface, GLImage *b, float x1, float x2, float x3, float x4, float y1, float y2, float y3, float y4);
class pMaskTransformf : public RenderProcess { public:
 GLImage *b; float x1,x2,x3,x4,y1,y2,y3,y4;
 void Render( GLWindow *surface ) { MaskTransformf(surface,b,x1,x2,x3,x4,y1,y2,y3,y4); }
};

void SolarizeTransformf( GLWindow *surface, GLImage *b, float x1, float x2, float x3, float x4, float y1, float y2, float y3, float y4);
class pSolarizeTransformf : public RenderProcess { public:
 GLImage *b; float x1,x2,x3,x4,y1,y2,y3,y4;
 void Render( GLWindow *surface ) { SolarizeTransformf(surface,b,x1,x2,x3,x4,y1,y2,y3,y4); }
};

void DrawRampGL( GLWindow *surface, int x1, int y1, int w, int h, float *rgba, ColorChannel channel );
class pDrawRampGL : public RenderProcess { public:
 int x1,y1,w,h; float *rgba; ColorChannel channel;
 void Render( GLWindow *surface ) { DrawRampGL(surface,x1,y1,w,h,rgba,channel); }
};

void DrawGrid( GLWindow *surface, Cartesian *extents, Crayon gridColor, int row, int col );
void DrawGrid( Cartesian *extents, Crayon gridColor, int row, int col );
void DrawGrid( Cartesiand *extents, Crayon gridColor, int row, int col );
void DrawGrid( int x, int y, int w, int h, Crayon gridColor, int row, int col );
void DrawGrid( double x, double y, double w, double h, Crayon gridColor, int row, int col );

void PointSprite( GLImage *source, Crayon tint, Vertexd *coord, float size );

void Coordinator(void);

void DrawBar( Crayon color, float one, int x, int y, int w, int h, int divisions );
void DrawBarHoriz( Crayon color, float one, int x, int y, int w, int h, int divisions );
void DrawBar( Blends blendFill, Blends blendLine, Crayon color, float one, int x, int y, int w, int h, int divisions );
void DrawBarHoriz( Blends blendFill, Blends blendLine, Crayon color, float one, int x, int y, int w, int h, int divisions );

void DrawCircle( double cx, double cy, double R, int segments);

void DrawNamedPoint( Crayon color, const char *name, double x, double y );

class RoundedRectangle;
RoundedRectangle *RoundRect( Crayon color, Crayon fill, Crayon outer, int x, int y, int width, int height, int radius=16, int precision=8, float lineWidth=1.0f );
void RoundRect9( GLImage* corner, GLImage* border, GLImage* interior, int x, int y, int w, int h, int cornerBorder );

void DrawAngle( Crayon tint, int x, int y, double R, double lowDeg, double highDeg, double Precision=8.0 );