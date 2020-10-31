#include "Art2d.h"

#include "TextureLibrary.h"
Art2d art;

void Rotated2dQuad::toTriangles( TwoDeeTriangles *out ) {
 double w2=w/2.0;
 double h2=h/2.0;
 Cartesiand A,B,C,D;
 A.Set(-w2,-h2); A.Rotate(a); A.MoveBy(x,y);
 B.Set( w2,-h2); B.Rotate(a); B.MoveBy(x,y);
 C.Set( w2, h2); C.Rotate(a); C.MoveBy(x,y);
 D.Set(-w2, h2); D.Rotate(a); D.MoveBy(x,y);
 out->Add(
  tintA,A.x,A.y,tc.x, tc.y,
  tintB,B.x,B.y,tc.x2,tc.y,
  tintC,C.x,C.y,tc.x2,tc.y2,
  tintD,D.x,D.y,tc.x, tc.y2
 );
}

void Rotated2dQuads::toTriangles( TwoDeeTriangles *out ) {
 FOREACH(Rotated2dQuad,q) {
  double w2=q->w/2.0;
  double h2=q->h/2.0;
  Cartesiand A,B,C,D;
  A.Set(-w2,-h2); A.Rotate(q->a); A.MoveBy(q->x,q->y);
  B.Set( w2,-h2); B.Rotate(q->a); B.MoveBy(q->x,q->y);
  C.Set( w2, h2); C.Rotate(q->a); C.MoveBy(q->x,q->y);
  D.Set(-w2, h2); D.Rotate(q->a); D.MoveBy(q->x,q->y);
  out->Add(
   q->tintA,A.x,A.y,q->tc.x,q->tc.y,
   q->tintB,B.x,B.y,q->tc.x2,q->tc.y,
   q->tintC,C.x,C.y,q->tc.x2,q->tc.y2,
   q->tintD,D.x,D.y,q->tc.x,q->tc.y2
  );
 }
}

#define ADD_VERT(x,y) { vertices[v+0]=(GLfloat)(x); vertices[v+1]=(GLfloat)(y); v+=stride; }
#define ADD_VERT_TINT_TC(x,y,tintr,tintg,tintb,tx,ty) { vertices[v+0]=(GLfloat)(x); vertices[v+1]=(GLfloat)(y); vertices[v+2]=(GLfloat)(tintr); vertices[v+3]=(GLfloat)(tintg); vertices[v+4]=(GLfloat)(tintb); vertices[v+5]=(GLfloat)(tx); vertices[v+6]=(GLfloat)(ty); v+=stride; }
#define ADD_ELEM(a,b,c) { elements[u+0]=(a); elements[u+1]=(b); elements[u+2]=(c); u+=3; }
#define ADD_TRI(ax,ay,bx,by,cx,cy) { GLuint v1=v/stride; ADD_VERT(ax,ay); GLuint v2=v/stride; ADD_VERT(bx,by); GLuint v3=v/stride; ADD_VERT(cx,cy); ADD_ELEM(v1,v2,v3); }
#define ADD_QUAD(ax,ay,bx,by,cx,cy,dx,dy) { ADD_TRI(ax,ay,bx,by,dx,dy); ADD_TRI(bx,by,cx,cy,dx,dy); }
#define ADD_VERT_TC(x,y,tx,ty) { vertices[v+0]=(GLfloat)(x); vertices[v+1]=(GLfloat)(y); vertices[v+5]=(GLfloat)tx; vertices[v+6]=(GLfloat)ty; v+=stride; } 
#define ADD_TRI_TC(ax,ay,txa,tya,bx,by,txb,tyb,cx,cy,txc,tyc) { GLuint v1=v/stride; ADD_VERT_TC(ax,ay,txa,tya); GLuint v2=v/stride; ADD_VERT_TC(bx,by,txb,tyb); GLuint v3=v/stride; ADD_VERT_TC(cx,cy,txc,tyc); ADD_ELEM(v1,v2,v3); }
#define ADD_QUAD_TC(ax,ay,txa,tya,bx,by,txb,tyb,cx,cy,txc,tyc,dx,dy,txd,tyd) { GLuint v1=v/stride; ADD_VERT_TC(ax,ay,txa,tya); GLuint v2=v/stride; ADD_VERT_TC(bx,by,txb,tyb); GLuint v3=v/stride; ADD_VERT_TC(cx,cy,txc,tyc); GLuint v4=v/stride; ADD_VERT_TC(dx,dy,txd,tyd); ADD_ELEM(v1,v2,v4); ADD_ELEM(v2,v3,v4); }
#define ADD_QUAD_TCv2(ax,ay,txa,tya,bx,by,txb,tyb,cx,cy,txc,tyc,dx,dy,txd,tyd) { ADD_TRI_TC(ax,ay,txa,tya,bx,by,txb,tyb,dx,dy,txd,tyd); ADD_TRI_TC(bx,by,txb,tyb,cx,cy,txc,tyc,dx,dy,txd,tyd); }


TwoDeeStrokes::~TwoDeeStrokes() {
  art.stash.Replace(stashed_id);
  Clear();
 }


void TwoDeeElementStash::Init() {
 Crayon tint(alabaster);

 rrs_rectangle         =art.stash.Get(rectangle  =GenRectangle());
 rrs_rectFlipX         =art.stash.Get(rectFlipX  =GenRectangle(crayons.Pick(alabaster),true,false));
 rrs_rectFlipY         =art.stash.Get(rectFlipY  =GenRectangle(crayons.Pick(alabaster),false,true));
 rrs_rectFlipXY        =art.stash.Get(rectFlipXY =GenRectangle(crayons.Pick(alabaster),true,true));
 
 rrs_filledRect        =art.stash.Get(filledRect        =GenQuad(0.0,0.0,1.0,1.0));
 rrs_filledRect_FlipX  =art.stash.Get(filledRect_FlipX  =GenQuad(1.0,0.0,0.0,1.0)); //GenRoundRect(0.0,tint,true);
 rrs_filledRect_FlipY  =art.stash.Get(filledRect_FlipY  =GenQuad(0.0,1.0,1.0,0.0)); //GenRoundRect(0.0,tint,false,true);
 rrs_filledRect_FlipXY =art.stash.Get(filledRect_FlipXY =GenQuad(1.0,1.0,0.0,0.0)); //GenRoundRect(0.0,tint,true,true);

 rrs_rr_Standard       =art.stash.Get(rr_Standard=count);
 rrs_rr_Button         =art.stash.Get(rr_Button   =GenRoundRect(0.1,0.3,tint,true,true));
 rrs_rr_Vertical       =art.stash.Get(rr_Vertical =GenRoundRect(0.3,0.1,tint,true,true));

 rrs_ellipsoid         =art.stash.Get(ellipsoid=GenCircle());

 rrs_ellipsoidFlipX    =art.stash.Get(ellipsoidFlipX=GenCircle(64,crayons.Pick(alabaster),true));
 rrs_linesRect         =art.stash.Get(linesRect=GenEmptyRect(0.25,crayons.Pick(alabaster),false,false,false,false,false));
 rrs_linesGrid10x10    =art.stash.Get(linesGrid10x10=GenGrid(10,10,0.0125));
 rrs_ca_BL             =art.stash.Get(ca_BL=GenCircleArc(0.25,0.5));
 rrs_ca_BR             =art.stash.Get(ca_BR=GenCircleArc(0.0,0.25));
 rrs_ca_TR             =art.stash.Get(ca_TR=GenCircleArc(0.75,1.0));
 rrs_ca_TL             =art.stash.Get(ca_TL=GenCircleArc(0.5,0.75));
 for ( int i=1; i < 20; i++ ) GenRoundRect(iratiod(i,20)/2.0);
 rrs_rr_FlipX          =art.stash.Get(rr_FlipX=count);
 for ( int i=1; i < 20; i++ ) GenRoundRect(iratiod(i,20)/2.0,tint,true);
 rrs_rr_FlipY          =art.stash.Get(rr_FlipY=count);
 for ( int i=1; i < 20; i++ ) GenRoundRect(iratiod(i,20)/2.0,tint,false,true);
 rrs_rr_FlipXY         =art.stash.Get(rr_FlipXY=count);
 for ( int i=1; i < 20; i++ ) GenRoundRect(iratiod(i,20)/2.0,tint,true,true);
 rrs_rr_Transition     =art.stash.Get(rr_Transition=count);
 for ( int i=1; i < 20; i++ ) { double xy=iratiod(i,20)/2.0; GenRoundRect(xy/10.0,(0.5-xy)/10.0,tint,true,true); }
}



int TwoDeeElementStash::GenGrid( int rows, int columns, double thickness, Crayon tint, bool texCoordsFollowLines, bool outerEdge, bool flipX, bool flipY, bool fitTo1, double texcoordsFollowLinesScale, bool roundEnds, int endPrecision ) {
 double xDelta=1.0/(double)columns;
 double yDelta=1.0/(double)rows;
 Cartesiands lines;
 for ( int i=0; i<columns; i++ ) {
  double x=xDelta*(double)i;
  lines.Append(new Cartesiand(x,0.0,0.0,1.0));
 }
 for ( int j=0; j<rows; j++ ) {
  double y=yDelta*(double)j;
  lines.Append(new Cartesiand(0.0,y,1.0,0.0));
 }
 if ( outerEdge ) {
  lines.Append(new Cartesiand(0.0,0.0,1.0,0.0));
  lines.Append(new Cartesiand(0.0,0.0,0.0,1.0));
  lines.Append(new Cartesiand(1.0,0.0,0.0,1.0));
  lines.Append(new Cartesiand(0.0,1.0,1.0,0.0));
 }
 return GenLines(&lines,tint,thickness,roundEnds,texCoordsFollowLines, flipX, flipY, fitTo1, texcoordsFollowLinesScale, endPrecision);
}

int TwoDeeElementStash::GenEmptyRect( double thickness, Crayon tint, bool texCoordsFollowLines, bool flipX, bool flipY, bool fitTo1, double texcoordsFollowLinesScale, bool roundEnds, int endPrecision ) {
 Cartesiands lines;
 lines.Append(new Cartesiand(0.0,0.0,1.0,0.0));
 lines.Append(new Cartesiand(0.0,0.0,0.0,1.0));
 lines.Append(new Cartesiand(1.0,0.0,0.0,1.0));
 lines.Append(new Cartesiand(0.0,1.0,1.0,0.0));
 return GenLines(&lines,tint,thickness,roundEnds,texCoordsFollowLines, flipX, flipY, fitTo1, texcoordsFollowLinesScale, endPrecision);
}

int TwoDeeElementStash::GenRectangle( Crayon tint, bool flipX, bool flipY ) {
 TwoDeeElement *rrs=new TwoDeeElement;
 int stride=7; // PosX,PosY,ColR,ColG,ColB,TexX,TexY
 int totalTris=2;
 int totalVerts=4;
 int totalElements=2*3;
 Indexed<GLuint> elements;  elements.Size((size_t)(totalElements));     for ( int i=0; i<(int)elements.length; i++ ) elements[i]=0;
 Indexed<GLfloat> vertices; vertices.Size((size_t)(stride*totalVerts)); for ( int i=0; i<(int)vertices.length; i++ ) vertices[i]=0.0f;
 GLuint v2=0,v3=0;
 int u=0;
 int v=0;
 ADD_VERT(0.0,0.0);
 ADD_VERT(1.0,0.0);
 ADD_VERT(1.0,1.0);
 ADD_VERT(0.0,1.0);
 ADD_ELEM(0,1,3);
 ADD_ELEM(1,2,3);
 // Set tint and tex coord
 v=0;
 for ( int i=0; i<totalVerts; i++ ) {
  vertices[v+2]=tint.rf;
  vertices[v+3]=tint.gf;
  vertices[v+4]=tint.bf;
  if ( !flipX && !flipY ) {
   vertices[v+5]=vertices[v]/1.0f;
   vertices[v+6]=vertices[v+1]/1.0f;
  } else if ( flipX && flipY ) {
   vertices[v+5]=1.0f-vertices[v]/1.0f;
   vertices[v+6]=1.0f-vertices[v+1]/1.0f;
  } else if ( flipX ) {
   vertices[v+5]=1.0f-vertices[v]/1.0f;
   vertices[v+6]=vertices[v+1]/1.0f;
  } else if ( flipY ) {
   vertices[v+5]=vertices[v]/1.0f;
   vertices[v+6]=1.0f-vertices[v+1]/1.0f;
  } 
  v+=stride;
 }
 rrs->elements=totalElements;
 OUTPUT("GenRectangle:\n");
 v=0;
 for ( int i=0; i<totalVerts; i++ ) { OUTPUT("v%d: %f %f %f %f %f %f %f\n",i,vertices[v],vertices[v+1],vertices[v+2],vertices[v+3],vertices[v+4],vertices[v+5],vertices[v+6]); v+=stride; }
 u=0;
 for ( int i=0; i<totalTris; i++ ) { OUTPUT("e%d: %d %d %d\n",i,elements[u],elements[u+1],elements[u+2]); u+=3; }
 // Upload elements and vertices
 glReportError(glGetError());
 glGenBuffers(1, &(rrs->ebo));
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rrs->ebo);
 glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(GLuint)*elements.length), elements.list, GL_STATIC_DRAW);
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
 glGenBuffers(1, &(rrs->vbo));
 glBindBuffer(GL_ARRAY_BUFFER, rrs->vbo);
 glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(GLfloat)*vertices.length), vertices.list, GL_STATIC_DRAW);
 glBindBuffer(GL_ARRAY_BUFFER,0);
 glReportError(glGetError());
 return Add(rrs);
}


int TwoDeeElementStash::GenQuad( double tcx, double tcy, double tcx2, double tcy2, Crayon tint ) {
 TwoDeeElement *rrs=new TwoDeeElement;
 int stride=7; // PosX,PosY,ColR,ColG,ColB,TexX,TexY
 int totalTris=2;
 int totalVerts=4;
 int totalElements=2*3;
 Indexed<GLuint> elements;  elements.Size((size_t)(totalElements));   for ( int i=0; i<(int)elements.length; i++ ) elements[i]=0;
 Indexed<GLfloat> vertices; vertices.Size((size_t)(stride*totalVerts)); for ( int i=0; i<(int)vertices.length; i++ ) vertices[i]=0.0f;
 int u=0;
 int v=0;
 ADD_QUAD_TC(0.0,0.0,tcx,tcy,1.0,0.0,tcx2,tcy,1.0,1.0,tcx2,tcy2,0.0,1.0,tcx,tcy2);
 // Set tint
 v=0;
 for ( int i=0; i<totalVerts; i++ ) {
  vertices[v+2]=tint.rf;
  vertices[v+3]=tint.gf;
  vertices[v+4]=tint.bf;
  v+=stride;
 }
 rrs->elements=totalElements;
 OUTPUT("GenQuad:\n");
 v=0;
 for ( int i=0; i<totalVerts; i++ ) { OUTPUT("v%d: %f %f %f %f %f %f %f\n",i,vertices[v],vertices[v+1],vertices[v+2],vertices[v+3],vertices[v+4],vertices[v+5],vertices[v+6]); v+=stride; }
 u=0;
 for ( int i=0; i<totalTris; i++ ) { OUTPUT("e%d: %d %d %d\n",i,elements[u],elements[u+1],elements[u+2]); u+=3; }
 // Upload elements and vertices
 glReportError(glGetError());
 glGenBuffers(1, &(rrs->ebo));
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rrs->ebo);
 glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(GLuint)*elements.length), elements.list, GL_STATIC_DRAW);
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
 glGenBuffers(1, &(rrs->vbo));
 glBindBuffer(GL_ARRAY_BUFFER, rrs->vbo);
 glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(GLfloat)*vertices.length), vertices.list, GL_STATIC_DRAW);
 glBindBuffer(GL_ARRAY_BUFFER,0);
 glReportError(glGetError());
 return Add(rrs);
}

int TwoDeeElementStash::GenQuad( double ax, double ay, double bx, double by, double cx, double cy, double dx, double dy, double tcx, double tcy, double tcx2, double tcy2, Crayon tint ) {
 TwoDeeElement *rrs=new TwoDeeElement;
 int stride=7; // PosX,PosY,ColR,ColG,ColB,TexX,TexY
 int totalTris=2;
 int totalVerts=4;
 int totalElements=2*3;
 Indexed<GLuint> elements;  elements.Size((size_t)(totalElements));   for ( int i=0; i<(int)elements.length; i++ ) elements[i]=0;
 Indexed<GLfloat> vertices; vertices.Size((size_t)(stride*totalVerts)); for ( int i=0; i<(int)vertices.length; i++ ) vertices[i]=0.0f;
 int u=0;
 int v=0;
 ADD_QUAD_TC(ax,ay,tcx,tcy,bx,by,tcx2,tcy,cx,cy,tcx2,tcy2,dx,dy,tcx,tcy2);
 // Set tint
 v=0;
 for ( int i=0; i<totalVerts; i++ ) {
  vertices[v+2]=tint.rf;
  vertices[v+3]=tint.gf;
  vertices[v+4]=tint.bf;
  v+=stride;
 }
 rrs->elements=totalElements;
 v=0;
 for ( int i=0; i<totalVerts; i++ ) { OUTPUT("v%d: %f %f %f %f %f %f %f\n",i,vertices[v],vertices[v+1],vertices[v+2],vertices[v+3],vertices[v+4],vertices[v+5],vertices[v+6]); v+=stride; }
 u=0;
 for ( int i=0; i<totalTris; i++ ) { OUTPUT("e%d: %d %d %d\n",i,elements[u],elements[u+1],elements[u+2]); u+=3; }
 // Upload elements and vertices
 glReportError(glGetError());
 glGenBuffers(1, &(rrs->ebo));
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rrs->ebo);
 glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(GLuint)*elements.length), elements.list, GL_STATIC_DRAW);
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
 glGenBuffers(1, &(rrs->vbo));
 glBindBuffer(GL_ARRAY_BUFFER, rrs->vbo);
 glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(GLfloat)*vertices.length), vertices.list, GL_STATIC_DRAW);
 glBindBuffer(GL_ARRAY_BUFFER,0);
 glReportError(glGetError());
 return Add(rrs);
}


int TwoDeeElementStash::GenQuadRotated( double ax, double ay, double bx, double by, double cx, double cy, double dx, double dy, double tcx, double tcy, double tcx2, double tcy2, Crayon tint ) {
 TwoDeeElement *rrs=new TwoDeeElement;
 int stride=7; // PosX,PosY,ColR,ColG,ColB,TexX,TexY
 int totalTris=2;
 int totalVerts=4;
 int totalElements=2*3;
 Indexed<GLuint> elements;  elements.Size((size_t)(totalElements));   for ( int i=0; i<(int)elements.length; i++ ) elements[i]=0;
 Indexed<GLfloat> vertices; vertices.Size((size_t)(stride*totalVerts)); for ( int i=0; i<(int)vertices.length; i++ ) vertices[i]=0.0f;
 int u=0;
 int v=0;
 ADD_QUAD_TC(dx,dy,tcx,tcy,ax,ay,tcx2,tcy,bx,by,tcx2,tcy2,cx,cy,tcx,tcy2);
 // Set tint
 v=0;
 for ( int i=0; i<totalVerts; i++ ) {
  vertices[v+2]=tint.rf;
  vertices[v+3]=tint.gf;
  vertices[v+4]=tint.bf;
  v+=stride;
 }
 rrs->elements=totalElements;
 v=0;
 for ( int i=0; i<totalVerts; i++ ) { OUTPUT("v%d: %f %f %f %f %f %f %f\n",i,vertices[v],vertices[v+1],vertices[v+2],vertices[v+3],vertices[v+4],vertices[v+5],vertices[v+6]); v+=stride; }
 u=0;
 for ( int i=0; i<totalTris; i++ ) { OUTPUT("e%d: %d %d %d\n",i,elements[u],elements[u+1],elements[u+2]); u+=3; }
 // Upload elements and vertices
 glReportError(glGetError());
 glGenBuffers(1, &(rrs->ebo));
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rrs->ebo);
 glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(GLuint)*elements.length), elements.list, GL_STATIC_DRAW);
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
 glGenBuffers(1, &(rrs->vbo));
 glBindBuffer(GL_ARRAY_BUFFER, rrs->vbo);
 glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(GLfloat)*vertices.length), vertices.list, GL_STATIC_DRAW);
 glBindBuffer(GL_ARRAY_BUFFER,0);
 glReportError(glGetError());
 return Add(rrs);
}

int TwoDeeElementStash::GenQuads( Cartesiands *quads, Cartesiands *tcs, Crayon tint ) {
 TwoDeeElement *rrs=new TwoDeeElement;
 int stride=7; // PosX,PosY,ColR,ColG,ColB,TexX,TexY
 int totalTris=2*quads->count;
 int totalVerts=4*quads->count;
 int totalElements=2*3*quads->count;
 Indexed<GLuint> elements;  elements.Size((size_t)(totalElements));   for ( int i=0; i<(int)elements.length; i++ ) elements[i]=0;
 Indexed<GLfloat> vertices; vertices.Size((size_t)(stride*totalVerts)); for ( int i=0; i<(int)vertices.length; i++ ) vertices[i]=0.0f;
 int u=0;
 int v=0;
 EACHPAIR(quads->first,Cartesiand,q,tcs->first,Cartesiand,t,{
  {
   GLuint e1=v/stride; ADD_VERT_TC(q->x, q->y, t->x, t->y);
   GLuint e2=v/stride; ADD_VERT_TC(q->x2,q->y, t->x2,t->y);
   GLuint e3=v/stride; ADD_VERT_TC(q->x2,q->y2,t->x2,t->y2);
   GLuint e4=v/stride; ADD_VERT_TC(q->x, q->y2,t->x, t->y2);
   ADD_ELEM(e1,e2,e4);
   ADD_ELEM(e2,e3,e4);
  }
 });
 // Set tint
 v=0;
 for ( int i=0; i<totalVerts; i++ ) {
  vertices[v+2]=tint.rf;
  vertices[v+3]=tint.gf;
  vertices[v+4]=tint.bf;
  v+=stride;
 }
 rrs->elements=totalElements;
 v=0;
 for ( int i=0; i<totalVerts; i++ ) { OUTPUT("v%d: %f %f %f %f %f %f %f\n",i,vertices[v],vertices[v+1],vertices[v+2],vertices[v+3],vertices[v+4],vertices[v+5],vertices[v+6]); v+=stride; }
 u=0;
 for ( int i=0; i<totalTris; i++ ) { OUTPUT("e%d: %d %d %d\n",i,elements[u],elements[u+1],elements[u+2]); u+=3; }
 // Upload elements and vertices
 glReportError(glGetError());
 glGenBuffers(1, &(rrs->ebo));
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rrs->ebo);
 glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(GLuint)*elements.length), elements.list, GL_STATIC_DRAW);
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
 glGenBuffers(1, &(rrs->vbo));
 glBindBuffer(GL_ARRAY_BUFFER, rrs->vbo);
 glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(GLfloat)*vertices.length), vertices.list, GL_STATIC_DRAW);
 glBindBuffer(GL_ARRAY_BUFFER,0);
 glReportError(glGetError());
 return Add(rrs);
}


int TwoDeeElementStash::GenQuads( Cartesiands *quads, Cartesiands *tcs, Cartesiands *colors ) {
 TwoDeeElement *rrs=new TwoDeeElement;
 int stride=7; // PosX,PosY,ColR,ColG,ColB,TexX,TexY
 int totalTris=2*quads->count;
 int totalVerts=4*quads->count;
 int totalElements=2*3*quads->count;
 Indexed<GLuint> elements;  elements.Size((size_t)(totalElements));   for ( int i=0; i<(int)elements.length; i++ ) elements[i]=0;
 Indexed<GLfloat> vertices; vertices.Size((size_t)(stride*totalVerts)); for ( int i=0; i<(int)vertices.length; i++ ) vertices[i]=0.0f;
 int u=0;
 int v=0;
 EACHPAIR(quads->first,Cartesiand,q,tcs->first,Cartesiand,t,{
  {
   GLuint e1=v/stride; ADD_VERT_TC(q->x, q->y, t->x, t->y);
   GLuint e2=v/stride; ADD_VERT_TC(q->x2,q->y, t->x2,t->y);
   GLuint e3=v/stride; ADD_VERT_TC(q->x2,q->y2,t->x2,t->y2);
   GLuint e4=v/stride; ADD_VERT_TC(q->x, q->y2,t->x, t->y2);
   ADD_ELEM(e1,e2,e4);
   ADD_ELEM(e2,e3,e4);
  }
 });
 // Set tint to default color
 v=0;
 for ( int i=0; i<totalVerts; i++ ) {
  vertices[v+2]=1.0;
  vertices[v+3]=1.0;
  vertices[v+4]=1.0;
  v+=stride;
 }
 v=0;
 EACH(colors->first,Cartesiand,clr) {
  vertices[v+2]=(float)clr->x;
  vertices[v+3]=(float)clr->y;
  vertices[v+4]=(float)clr->z;
  v+=stride;
  if ( v/stride > totalVerts ) break;
 }
 rrs->elements=totalElements;
 v=0;
 for ( int i=0; i<totalVerts; i++ ) { OUTPUT("v%d: %f %f %f %f %f %f %f\n",i,vertices[v],vertices[v+1],vertices[v+2],vertices[v+3],vertices[v+4],vertices[v+5],vertices[v+6]); v+=stride; }
 u=0;
 for ( int i=0; i<totalTris; i++ ) { OUTPUT("e%d: %d %d %d\n",i,elements[u],elements[u+1],elements[u+2]); u+=3; }
 // Upload elements and vertices
 glReportError(glGetError());
 glGenBuffers(1, &(rrs->ebo));
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rrs->ebo);
 glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(GLuint)*elements.length), elements.list, GL_STATIC_DRAW);
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
 glGenBuffers(1, &(rrs->vbo));
 glBindBuffer(GL_ARRAY_BUFFER, rrs->vbo);
 glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(GLfloat)*vertices.length), vertices.list, GL_STATIC_DRAW);
 glBindBuffer(GL_ARRAY_BUFFER,0);
 glReportError(glGetError());
 return Add(rrs);
}


int TwoDeeElementStash::GenCircleArc( double startTime, double endTime, int precision, Crayon tint, bool flipX, bool flipY, bool texcoordsCenterEdge ) {
 TwoDeeElement *rrs=new TwoDeeElement;
 int stride=7; // PosX,PosY,ColR,ColG,ColB,TexX,TexY
 int totalTris=precision;
 int totalVerts=1+precision*2;
 int totalElements=totalTris*3;
 Indexed<GLuint> elements;  elements.Size((size_t)(totalElements));     for ( int i=0; i<(int)elements.length; i++ ) elements[i]=0;
 Indexed<GLfloat> vertices; vertices.Size((size_t)(stride*totalVerts)); for ( int i=0; i<(int)vertices.length; i++ ) vertices[i]=0.0f;
 GLuint v2=0,v3=0;
 int u=0;
 int v=0;
 ADD_VERT(0.5,0.5);
 double totalDelta=endTime-startTime;
 double delta=1/(double)precision * totalDelta;
 for ( int i=0; i<precision; i++ ) {
  Circle c(0.5);
  double ang1=startTime+delta*(double)i,ang2=startTime+delta*(double)(i+1);
  Vertexd B1,B2;
  c.Point(ang1,&B1); B1.Add(0.5,0.5);
  c.Point(ang2,&B2); B2.Add(0.5,0.5);
  v2=v/stride; ADD_VERT(B2.x, B2.y);
  v3=v/stride; ADD_VERT(B1.x, B1.y);
  ADD_ELEM(0,v2,v3);
 }
 // Set tint and tex coord
 v=0;
 for ( int i=0; i<totalVerts; i++ ) {
  vertices[v+2]=tint.rf;
  vertices[v+3]=tint.gf;
  vertices[v+4]=tint.bf;
  if ( !flipX && !flipY ) {
   vertices[v+5]=vertices[v]/1.0f;
   vertices[v+6]=vertices[v+1]/1.0f;
  } else if ( flipX && flipY ) {
   vertices[v+5]=1.0f-vertices[v]/1.0f;
   vertices[v+6]=1.0f-vertices[v+1]/1.0f;
  } else if ( flipX ) {
   vertices[v+5]=1.0f-vertices[v]/1.0f;
   vertices[v+6]=vertices[v+1]/1.0f;
  } else if ( flipY ) {
   vertices[v+5]=vertices[v]/1.0f;
   vertices[v+6]=1.0f-vertices[v+1]/1.0f;
  } 
  v+=stride;
 }
 rrs->elements=totalElements;
 // Upload elements and vertices
 glReportError(glGetError());
 glGenBuffers(1, &(rrs->ebo));
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rrs->ebo);
 glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(GLuint)*elements.length), elements.list, GL_STATIC_DRAW);
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
 glGenBuffers(1, &(rrs->vbo));
 glBindBuffer(GL_ARRAY_BUFFER, rrs->vbo);
 glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(GLfloat)*vertices.length), vertices.list, GL_STATIC_DRAW);
 glBindBuffer(GL_ARRAY_BUFFER,0);
 glReportError(glGetError());
 return Add(rrs);
}

int TwoDeeElementStash::GenCircle( int precision, Crayon tint, bool flipX, bool flipY, bool texcoordsCenterEdge ) {
 TwoDeeElement *rrs=new TwoDeeElement;
 int stride=7; // PosX,PosY,ColR,ColG,ColB,TexX,TexY
 int totalTris=precision;
 int totalVerts=1+precision*2;
 int totalElements=totalTris*3;
 Indexed<GLuint> elements;  elements.Size((size_t)(totalElements));     for ( int i=0; i<(int)elements.length; i++ ) elements[i]=0;
 Indexed<GLfloat> vertices; vertices.Size((size_t)(stride*totalVerts)); for ( int i=0; i<(int)vertices.length; i++ ) vertices[i]=0.0f;
 GLuint v2=0,v3=0;
 int u=0;
 int v=0;
 ADD_VERT(0.5,0.5);
 for ( int i=0; i<precision; i++ ) {
  Circle c(0.5);
  Vertexd B1,B2;
  c.Point(iratiod(i,precision),&B1); B1.Add(0.5,0.5);
  c.Point(iratiod(i+1,precision),&B2); B2.Add(0.5,0.5);
  v2=v/stride; ADD_VERT(B2.x, B2.y);
  v3=v/stride; ADD_VERT(B1.x, B1.y);
  ADD_ELEM(0,v2,v3);
 }
 // Set tint and tex coord
 v=0;
 for ( int i=0; i<totalVerts; i++ ) {
  vertices[v+2]=tint.rf;
  vertices[v+3]=tint.gf;
  vertices[v+4]=tint.bf;
  if ( !flipX && !flipY ) {
   vertices[v+5]=vertices[v]/1.0f;
   vertices[v+6]=vertices[v+1]/1.0f;
  } else if ( flipX && flipY ) {
   vertices[v+5]=1.0f-vertices[v]/1.0f;
   vertices[v+6]=1.0f-vertices[v+1]/1.0f;
  } else if ( flipX ) {
   vertices[v+5]=1.0f-vertices[v]/1.0f;
   vertices[v+6]=vertices[v+1]/1.0f;
  } else if ( flipY ) {
   vertices[v+5]=vertices[v]/1.0f;
   vertices[v+6]=1.0f-vertices[v+1]/1.0f;
  } 
  v+=stride;
 }
 rrs->elements=totalElements;
 OUTPUT("GenCircle:\n");
 v=0;
 for ( int i=0; i<totalVerts; i++ ) { OUTPUT("v%d: %f %f %f %f %f %f %f\n",i,vertices[v],vertices[v+1],vertices[v+2],vertices[v+3],vertices[v+4],vertices[v+5],vertices[v+6]); v+=stride; }
 u=0;
 for ( int i=0; i<totalTris; i++ ) { OUTPUT("e%d: %d %d %d\n",i,elements[u],elements[u+1],elements[u+2]); u+=3; }
 // Upload elements and vertices
 glReportError(glGetError());
 glGenBuffers(1, &(rrs->ebo));
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rrs->ebo);
 glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(GLuint)*elements.length), elements.list, GL_STATIC_DRAW);
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
 glGenBuffers(1, &(rrs->vbo));
 glBindBuffer(GL_ARRAY_BUFFER, rrs->vbo);
 glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(GLfloat)*vertices.length), vertices.list, GL_STATIC_DRAW);
 glBindBuffer(GL_ARRAY_BUFFER,0);
 glReportError(glGetError());
 return Add(rrs);
}

int TwoDeeElementStash::GenThickness( Cartesiands *lines, Crayon tint, double thickness, bool roundEnds, bool texcoordsFollowLines, bool flipX, bool flipY, bool fitTo1, double texcoordsFollowLinesScale, int endPrecision ) {
 if ( lines->count < 2 ) return -1;
 Circle c(thickness/2.0);
 if ( fitTo1 ) {
  double LfitThick=0.0;
  double TfitThick=0.0;
  double BfitThick=1.0;
  double RfitThick=1.0;
  if ( roundEnds ) {
   LfitThick=thickness/2.0;
   TfitThick=thickness/2.0;
   BfitThick=1.0-thickness/2.0;
   RfitThick=1.0-thickness/2.0;
  }
  Cartesiand bounds;
  lines->Bounds2d(&bounds);
  EACH(lines->first,Cartesiand,L) {
   L->x=mapRanged(L->x,bounds.x,bounds.x2,LfitThick,RfitThick);
   L->y=mapRanged(L->y,bounds.y,bounds.y2,TfitThick,BfitThick);
  }
 }
 endPrecision=UMAX(3,endPrecision);
 double totalD=lines->Distance2d();
 TwoDeeElement *rrs=new TwoDeeElement;
 int stride=7; // PosX,PosY,ColR,ColG,ColB,TexX,TexY
 int totalTris=(lines->count-1)*2+(roundEnds?endPrecision*2:0);
 int totalVerts=totalTris*3;
 int totalElements=totalTris*3;
 Indexed<GLuint> elements;  elements.Size((size_t)(totalElements));     for ( int i=0; i<(int)elements.length; i++ ) elements[i]=0;
 int u=0;
 Indexed<GLfloat> vertices; vertices.Size((size_t)(stride*totalVerts)); for ( int i=0; i<(int)vertices.length; i++ ) vertices[i]=0.0f;
 int v=0;
 Cartesiand *p=(Cartesiand *)(lines->first);
 Cartesiand *p_next=(Cartesiand *)(p->next);
 Cartesiand *p_prev=null;
 double d=0.0;
 while ( p_next ) {
  Line seg(p->x,p->y,p_next->x,p_next->y);
  double angleA=rad2deg(seg.LineAngle());
  double angleB=angleA;
  if ( p_prev ) {
   Line seg3(p_prev->x,p_prev->y,p->x,p->y);
   angleB=rad2deg(seg3.LineAngle());
  }
  double topA=norm_deg(angleB+90.0)/360.0;
  double topB=norm_deg(angleA+90.0)/360.0;
  double bottomA=norm_deg(angleB-90.0)/360.0;  
  double bottomB=norm_deg(angleA-90.0)/360.0;  
  Vertexd A; c.Point(topA,&A);    A.Add(p->x,p->y);
  Vertexd B; c.Point(topB,&B);    B.Add(p_next->x,p_next->y);
  Vertexd C; c.Point(bottomB,&C); C.Add(p_next->x,p_next->y);
  Vertexd D; c.Point(bottomA,&D); D.Add(p->x,p->y);
  if ( texcoordsFollowLines ) {
   double d1=d/totalD;
   double d2=((d+seg.length)/totalD)*texcoordsFollowLinesScale;
   d+=seg.length;
   ADD_QUAD_TCv2(A.x,A.y,(d1),(0.0),B.x,B.y,(d2),(0.0),C.x,C.y,(d2),(1.0),D.x,D.y,(d1),(1.0));
  } else {
   ADD_QUAD(A.x,A.y,B.x,B.y,C.x,C.y,D.x,D.y);
  }
  if ( roundEnds ) {
   if ( p == lines->first ) {
    double a=topA;
    double delta=iratiod(1,endPrecision)*0.5;
    for ( int i=0; i<endPrecision; i++ ) {
     Vertexd B1; c.Point(a,&B1); B1.Add(p->x,p->y); 
     a+=delta;
     Vertexd B2; c.Point(a,&B2); B2.Add(p->x,p->y);
     if ( texcoordsFollowLines ) {
      Circle tc(0.5);
      Vertexd t1; tc.Point(a-delta,&t1); t1.Add(0.5,0.5);
      Vertexd t2; tc.Point(a,&t2); t2.Add(0.5,0.5);
      ADD_TRI_TC(B2.x,B2.y,(t2.x),(0.0),B1.x,B1.y,(t1.x),(1.0),p->x,p->y,(0.5),(0.5));
     } else {
      ADD_TRI(B2.x,B2.y,B1.x,B1.y,p->x,p->y);
     }
    }
   }
   if ( p_next == lines->last ) {
    double a=bottomB;
    double delta=iratiod(1,endPrecision)*0.5;
    for ( int i=0; i<endPrecision; i++ ) {
     Vertexd B1; c.Point(a,&B1); B1.Add(p_next->x,p_next->y);
     a+=delta;
     Vertexd B2; c.Point(a,&B2); B2.Add(p_next->x,p_next->y);
     if ( texcoordsFollowLines ) {
      Circle tc(0.5);
      Vertexd t1; tc.Point(a-delta,&t1); t1.Add(0.5,0.5);
      Vertexd t2; tc.Point(a,&t2); t2.Add(0.5,0.5);
      ADD_TRI_TC(B2.x,B2.y,(t2.x),(0.0),B1.x,B1.y,(t1.x),(1.0),p->x,p->y,(0.5),(0.5));
     } else {
      ADD_TRI(B2.x,B2.y,B1.x,B1.y,p->x,p->y);
     }
    }
   }
  }
  p_prev=p;
  p=(Cartesiand *)p->next;
  p_next=(Cartesiand *)p_next->next;
 }
 if ( !texcoordsFollowLines ) {
  v=0;
  for ( int i=0; i<totalVerts; i++ ) {
   vertices[v+2]=tint.rf;
   vertices[v+3]=tint.gf;
   vertices[v+4]=tint.bf;
   if ( !flipX && !flipY ) {
    vertices[v+5]=vertices[v]/1.0f;
    vertices[v+6]=vertices[v+1]/1.0f;
   } else if ( flipX && flipY ) {
    vertices[v+5]=1.0f-vertices[v]/1.0f;
    vertices[v+6]=1.0f-vertices[v+1]/1.0f;
   } else if ( flipX ) {
    vertices[v+5]=1.0f-vertices[v]/1.0f;
    vertices[v+6]=vertices[v+1]/1.0f;
   } else if ( flipY ) {
    vertices[v+5]=vertices[v]/1.0f;
    vertices[v+6]=1.0f-vertices[v+1]/1.0f;
   } 
   v+=stride;
  }
 } else {
  int v=0;
  for ( int i=0; i<totalVerts; i++ ) {
   vertices[v+2]=tint.rf;
   vertices[v+3]=tint.gf;
   vertices[v+4]=tint.bf;
   v+=stride;
  }
 }
 rrs->elements=totalElements;
 OUTPUT("GenQuad:\n");
 v=0;
 for ( int i=0; i<totalVerts; i++ ) { OUTPUT("v%d: %f %f %f %f %f %f %f\n",i,vertices[v],vertices[v+1],vertices[v+2],vertices[v+3],vertices[v+4],vertices[v+5],vertices[v+6]); v+=stride; }
 u=0;
 for ( int i=0; i<totalTris; i++ ) { OUTPUT("e%d: %d %d %d\n",i,elements[u],elements[u+1],elements[u+2]); u+=3; }
//v=0;
//for ( int i=0; i<totalVerts; i++ ) { OUTPUT("v%d: %f %f %f %f %f %f %f\n",i,vertices[v],vertices[v+1],vertices[v+2],vertices[v+3],vertices[v+4],vertices[v+5],vertices[v+6]); v+=stride; }
//u=0;
//for ( int i=0; i<totalTris; i++ ) { OUTPUT("e%d: %d %d %d\n",i,elements[u],elements[u+1],elements[u+2]); u+=3; }
 // Upload elements and vertices
 glReportError(glGetError());
 glGenBuffers(1, &(rrs->ebo));
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rrs->ebo);
 glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(GLuint)*elements.length), elements.list, GL_STATIC_DRAW);
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
 glGenBuffers(1, &(rrs->vbo));
 glBindBuffer(GL_ARRAY_BUFFER, rrs->vbo);
 glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(GLfloat)*vertices.length), vertices.list, GL_STATIC_DRAW);
 glBindBuffer(GL_ARRAY_BUFFER,0);
 glReportError(glGetError());
 return Add(rrs);
}



int TwoDeeElementStash::GenThickness( CartesiandsList *multiplelines, Crayon tint, double thickness, bool roundEnds, bool texcoordsFollowLines, bool flipX, bool flipY, bool fitTo1, double texcoordsFollowLinesScale, int endPrecision ) {
 Zint linesCount=multiplelines->TotalPoints();
 if ( linesCount < 2 ) return -1;
 Circle c(thickness/2.0);
 if ( fitTo1 ) {
  double LfitThick=0.0;
  double TfitThick=0.0;
  double BfitThick=1.0;
  double RfitThick=1.0;
  if ( roundEnds ) {
   LfitThick=thickness/2.0;
   TfitThick=thickness/2.0;
   BfitThick=1.0-thickness/2.0;
   RfitThick=1.0-thickness/2.0;
  }
  Cartesiand bounds;
  multiplelines->Bounds2d(&bounds);
  EACH(multiplelines->first,CartesiandsListItem,cli) {
   EACH(cli->cartesians.first,Cartesiand,L) {
    L->x=mapRanged(L->x,bounds.x,bounds.x2,LfitThick,RfitThick);
    L->y=mapRanged(L->y,bounds.y,bounds.y2,TfitThick,BfitThick);
   }
  }
 }
 endPrecision=UMAX(3,endPrecision);
 double totalD=multiplelines->Distance2d();
 TwoDeeElement *rrs=new TwoDeeElement;
 int stride=7; // PosX,PosY,ColR,ColG,ColB,TexX,TexY
 int totalTris=(linesCount)*2+(roundEnds?multiplelines->count*endPrecision*2:0);
 int totalVerts=totalTris*3;
 int totalElements=totalTris*3;
 Indexed<GLuint> elements;  elements.Size((size_t)(totalElements));     for ( int i=0; i<(int)elements.length; i++ ) elements[i]=0;
 int u=0;
 Indexed<GLfloat> vertices; vertices.Size((size_t)(stride*totalVerts)); for ( int i=0; i<(int)vertices.length; i++ ) vertices[i]=0.0f;
 int v=0;
 EACH(multiplelines->first,CartesiandsListItem,cli) {
  Cartesiands *lines=&cli->cartesians;
  Cartesiand *p=(Cartesiand *)(lines->first);
  Cartesiand *p_next=(Cartesiand *)(p->next);
  Cartesiand *p_prev=null;
  double d=0.0;
  while ( p_next ) {
   Line seg(p->x,p->y,p_next->x,p_next->y);
   double angleA=rad2deg(seg.LineAngle());
   double angleB=angleA;
   if ( p_prev ) {
    Line seg3(p_prev->x,p_prev->y,p->x,p->y);
    angleB=rad2deg(seg3.LineAngle());
   }
   double topA=norm_deg(angleB+90.0)/360.0;
   double topB=norm_deg(angleA+90.0)/360.0;
   double bottomA=norm_deg(angleB-90.0)/360.0;  
   double bottomB=norm_deg(angleA-90.0)/360.0;  
   Vertexd A; c.Point(topA,&A);    A.Add(p->x,p->y);
   Vertexd B; c.Point(topB,&B);    B.Add(p_next->x,p_next->y);
   Vertexd C; c.Point(bottomB,&C); C.Add(p_next->x,p_next->y);
   Vertexd D; c.Point(bottomA,&D); D.Add(p->x,p->y);
   if ( texcoordsFollowLines ) {
    double d1=d/totalD;
    double d2=((d+seg.length)/totalD)*texcoordsFollowLinesScale;
    d+=seg.length;
    ADD_QUAD_TCv2(A.x,A.y,(d1),(0.0),B.x,B.y,(d2),(0.0),C.x,C.y,(d2),(1.0),D.x,D.y,(d1),(1.0));
   } else {
    ADD_QUAD(A.x,A.y,B.x,B.y,C.x,C.y,D.x,D.y);
   }
   if ( roundEnds ) {
    if ( p == lines->first ) {
     double a=topA;
     double delta=iratiod(1,endPrecision)*0.5;
     for ( int i=0; i<endPrecision; i++ ) {
      Vertexd B1; c.Point(a,&B1); B1.Add(p->x,p->y); 
      a+=delta;
      Vertexd B2; c.Point(a,&B2); B2.Add(p->x,p->y);
      if ( texcoordsFollowLines ) {
       Circle tc(0.5);
       Vertexd t1; tc.Point(a-delta,&t1); t1.Add(0.5,0.5);
       Vertexd t2; tc.Point(a,&t2); t2.Add(0.5,0.5);
       ADD_TRI_TC(B2.x,B2.y,(t2.x),(0.0),B1.x,B1.y,(t1.x),(1.0),p->x,p->y,(0.5),(0.5));
      } else {
       ADD_TRI(B2.x,B2.y,B1.x,B1.y,p->x,p->y);
      }
     }
    }
    if ( p_next == lines->last ) {
     double a=bottomB;
     double delta=iratiod(1,endPrecision)*0.5;
     for ( int i=0; i<endPrecision; i++ ) {
      Vertexd B1; c.Point(a,&B1); B1.Add(p_next->x,p_next->y);
      a+=delta;
      Vertexd B2; c.Point(a,&B2); B2.Add(p_next->x,p_next->y);
      if ( texcoordsFollowLines ) {
       Circle tc(0.5);
       Vertexd t1; tc.Point(a-delta,&t1); t1.Add(0.5,0.5);
       Vertexd t2; tc.Point(a,&t2); t2.Add(0.5,0.5);
       ADD_TRI_TC(B2.x,B2.y,(t2.x),(0.0),B1.x,B1.y,(t1.x),(1.0),p_next->x,p_next->y,(0.5),(0.5));
      } else {
       ADD_TRI(B2.x,B2.y,B1.x,B1.y,p_next->x,p_next->y);
      }
     }
    }
   }
   p_prev=p;
   p=(Cartesiand *)p->next;
   p_next=(Cartesiand *)p_next->next;
  }
 }
 if ( !texcoordsFollowLines ) {
  v=0;
  for ( int i=0; i<totalVerts; i++ ) {
   vertices[v+2]=tint.rf;
   vertices[v+3]=tint.gf;
   vertices[v+4]=tint.bf;
   if ( !flipX && !flipY ) {
    vertices[v+5]=vertices[v]/1.0f;
    vertices[v+6]=vertices[v+1]/1.0f;
   } else if ( flipX && flipY ) {
    vertices[v+5]=1.0f-vertices[v]/1.0f;
    vertices[v+6]=1.0f-vertices[v+1]/1.0f;
   } else if ( flipX ) {
    vertices[v+5]=1.0f-vertices[v]/1.0f;
    vertices[v+6]=vertices[v+1]/1.0f;
   } else if ( flipY ) {
    vertices[v+5]=vertices[v]/1.0f;
    vertices[v+6]=1.0f-vertices[v+1]/1.0f;
   } 
   v+=stride;
  }
 } else {
  int v=0;
  for ( int i=0; i<totalVerts; i++ ) {
   vertices[v+2]=tint.rf;
   vertices[v+3]=tint.gf;
   vertices[v+4]=tint.bf;
   v+=stride;
  }
 }
 rrs->elements=totalElements;
//v=0;
//for ( int i=0; i<totalVerts; i++ ) { OUTPUT("v%d: %f %f %f %f %f %f %f\n",i,vertices[v],vertices[v+1],vertices[v+2],vertices[v+3],vertices[v+4],vertices[v+5],vertices[v+6]); v+=stride; }
//u=0;
//for ( int i=0; i<totalTris; i++ ) { OUTPUT("e%d: %d %d %d\n",i,elements[u],elements[u+1],elements[u+2]); u+=3; }
 // Upload elements and vertices
 glReportError(glGetError());
 glGenBuffers(1, &(rrs->ebo));
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rrs->ebo);
 glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(GLuint)*elements.length), elements.list, GL_STATIC_DRAW);
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
 glGenBuffers(1, &(rrs->vbo));
 glBindBuffer(GL_ARRAY_BUFFER, rrs->vbo);
 glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(GLfloat)*vertices.length), vertices.list, GL_STATIC_DRAW);
 glBindBuffer(GL_ARRAY_BUFFER,0);
 glReportError(glGetError());
 return Add(rrs);
}


int TwoDeeElementStash::GenLines( Cartesiands *lines, Crayon tint, double thickness, bool roundEnds, bool texcoordsFollowLines, bool flipX, bool flipY, bool fitTo1, double texcoordsFollowLinesScale, int endPrecision ) {
 if ( lines->count < 2 ) return -1;
 Circle c(thickness/2.0);
 if ( fitTo1 ) {
  double LfitThick=0.0;
  double TfitThick=0.0;
  double BfitThick=1.0;
  double RfitThick=1.0;
  Cartesiand bounds;
  lines->Bounds2dRect(&bounds);
  if ( bounds.x < 0 ) { lines->MoveBy(-bounds.x,0.0); bounds.MoveBy(-bounds.x,0.0); }
  if ( bounds.y < 0 ) { lines->MoveBy(0.0,-bounds.y); bounds.MoveBy(0.0,-bounds.y); }
  EACH(lines->first,Cartesiand,L) {
   L->x=mapRanged(L->x,bounds.x,bounds.x2,LfitThick,RfitThick);
   L->y=mapRanged(L->y,bounds.y,bounds.y2,TfitThick,BfitThick);
  }
 }
 endPrecision=UMAX(3,endPrecision);
 double totalD=lines->Distance2d();
 TwoDeeElement *rrs=new TwoDeeElement;
 int stride=7; // PosX,PosY,ColR,ColG,ColB,TexX,TexY
 int totalTris=(lines->count)*2+(roundEnds?endPrecision*2*lines->count:0);
 int totalVerts=totalTris*3;
 int totalElements=totalTris*3;
 Indexed<GLuint> elements;  elements.Size((size_t)(totalElements));     for ( int i=0; i<(int)elements.length; i++ ) elements[i]=0;
 int u=0;
 Indexed<GLfloat> vertices; vertices.Size((size_t)(stride*totalVerts)); for ( int i=0; i<(int)vertices.length; i++ ) vertices[i]=0.0f;
 int v=0;
 double d=0.0;
 EACH(lines->first,Cartesiand,p) {
  Line seg(p->x,p->y,p->x2,p->y2);
  double angleA=rad2deg(seg.LineAngle());
  double angleB=angleA;
  double topA=norm_deg(angleB+90.0)/360.0;
  double topB=norm_deg(angleA+90.0)/360.0;
  double bottomA=norm_deg(angleB-90.0)/360.0;  
  double bottomB=norm_deg(angleA-90.0)/360.0;  
  Vertexd A; c.Point(topA,&A);    A.Add(p->x,p->y);
  Vertexd B; c.Point(topB,&B);    B.Add(p->x2,p->y2);
  Vertexd C; c.Point(bottomB,&C); C.Add(p->x2,p->y2);
  Vertexd D; c.Point(bottomA,&D); D.Add(p->x,p->y);
  if ( texcoordsFollowLines ) {
   double d1=d/totalD;
   double d2=((d+seg.length)/totalD)*texcoordsFollowLinesScale;
   d+=seg.length;
   ADD_QUAD_TCv2(A.x,A.y,(d1),(0.0),B.x,B.y,(d2),(0.0),C.x,C.y,(d2),(1.0),D.x,D.y,(d1),(1.0));
  } else {
   ADD_QUAD(A.x,A.y,B.x,B.y,C.x,C.y,D.x,D.y);
  }
  if ( roundEnds ) {
   double a=topA;
   double delta=iratiod(1,endPrecision)*0.5;
   for ( int i=0; i<endPrecision; i++ ) {
    Vertexd B1; c.Point(a,&B1); B1.Add(p->x,p->y); 
    a+=delta;
    Vertexd B2; c.Point(a,&B2); B2.Add(p->x,p->y);
    if ( texcoordsFollowLines ) {
     Circle tc(0.5);
     Vertexd t1; tc.Point(a-delta,&t1); t1.Add(0.5,0.5);
     Vertexd t2; tc.Point(a,&t2); t2.Add(0.5,0.5);
     ADD_TRI_TC(B2.x,B2.y,(t2.x),(0.0),B1.x,B1.y,(t1.x),(1.0),p->x,p->y,(0.5),(0.5));
    } else {
     ADD_TRI(B2.x,B2.y,B1.x,B1.y,p->x,p->y);
    }
   }
   a=bottomB;
   delta=iratiod(1,endPrecision)*0.5;
   for ( int i=0; i<endPrecision; i++ ) {
    Vertexd B1; c.Point(a,&B1); B1.Add(p->x2,p->y2);
    a+=delta;
    Vertexd B2; c.Point(a,&B2); B2.Add(p->x2,p->y2);
    if ( texcoordsFollowLines ) {
     Circle tc(0.5);
     Vertexd t1; tc.Point(a-delta,&t1); t1.Add(0.5,0.5);
     Vertexd t2; tc.Point(a,&t2); t2.Add(0.5,0.5);
     ADD_TRI_TC(B2.x,B2.y,(t2.x),(0.0),B1.x,B1.y,(t1.x),(1.0),p->x2,p->y2,(0.5),(0.5));
    } else {
     ADD_TRI(B2.x,B2.y,B1.x,B1.y,p->x2,p->y2);
    }
   }   
  }
 }
 if ( !texcoordsFollowLines ) {
  v=0;
  for ( int i=0; i<totalVerts; i++ ) {
   vertices[v+2]=tint.rf;
   vertices[v+3]=tint.gf;
   vertices[v+4]=tint.bf;
   if ( !flipX && !flipY ) {
    vertices[v+5]=vertices[v]/1.0f;
    vertices[v+6]=vertices[v+1]/1.0f;
   } else if ( flipX && flipY ) {
    vertices[v+5]=1.0f-vertices[v]/1.0f;
    vertices[v+6]=1.0f-vertices[v+1]/1.0f;
   } else if ( flipX ) {
    vertices[v+5]=1.0f-vertices[v]/1.0f;
    vertices[v+6]=vertices[v+1]/1.0f;
   } else if ( flipY ) {
    vertices[v+5]=vertices[v]/1.0f;
    vertices[v+6]=1.0f-vertices[v+1]/1.0f;
   } 
   v+=stride;
  }
 } else {
  int v=0;
  for ( int i=0; i<totalVerts; i++ ) {
   vertices[v+2]=tint.rf;
   vertices[v+3]=tint.gf;
   vertices[v+4]=tint.bf;
   v+=stride;
  }
 }
 rrs->elements=totalElements;
v=0;
for ( int i=0; i<totalVerts; i++ ) { OUTPUT("v%d: %f %f %f %f %f %f %f\n",i,vertices[v],vertices[v+1],vertices[v+2],vertices[v+3],vertices[v+4],vertices[v+5],vertices[v+6]); v+=stride; }
u=0;
for ( int i=0; i<totalTris; i++ ) { OUTPUT("e%d: %d %d %d\n",i,elements[u],elements[u+1],elements[u+2]); u+=3; }
 // Upload elements and vertices
 glReportError(glGetError());
 glGenBuffers(1, &(rrs->ebo));
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rrs->ebo);
 glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(GLuint)*elements.length), elements.list, GL_STATIC_DRAW);
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
 glGenBuffers(1, &(rrs->vbo));
 glBindBuffer(GL_ARRAY_BUFFER, rrs->vbo);
 glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(GLfloat)*vertices.length), vertices.list, GL_STATIC_DRAW);
 glBindBuffer(GL_ARRAY_BUFFER,0);
 glReportError(glGetError());
 return Add(rrs);
}


int TwoDeeElementStash::GenLines( CartesiandsList *multiplelines, Crayon tint, double thickness, bool roundEnds, bool texcoordsFollowLines, bool flipX, bool flipY, bool fitTo1, double texcoordsFollowLinesScale, int endPrecision ) {
 Zint linesCount=multiplelines->TotalPoints();
 if ( linesCount < 2 ) return -1;
 Circle c(thickness/2.0);
 if ( fitTo1 ) {
  double LfitThick=0.0;
  double TfitThick=0.0;
  double BfitThick=1.0;
  double RfitThick=1.0;
  Cartesiand bounds;
  multiplelines->Bounds2dRect(&bounds);
  EACH(multiplelines->first,CartesiandsListItem,K) {
   EACH(K->cartesians.first,Cartesiand,L) {
    L->x=mapRanged(L->x,bounds.x,bounds.x2,LfitThick,RfitThick);
    L->y=mapRanged(L->y,bounds.y,bounds.y2,TfitThick,BfitThick);
   }
  }
 }
 endPrecision=UMAX(3,endPrecision);
 double totalD=multiplelines->Distance2d();
 TwoDeeElement *rrs=new TwoDeeElement;
 int stride=7; // PosX,PosY,ColR,ColG,ColB,TexX,TexY
 int totalTris=(linesCount)*2+(roundEnds?endPrecision*2*linesCount:0);
 int totalVerts=totalTris*3;
 int totalElements=totalTris*3;
 Indexed<GLuint> elements;  elements.Size((size_t)(totalElements));     for ( int i=0; i<(int)elements.length; i++ ) elements[i]=0;
 int u=0;
 Indexed<GLfloat> vertices; vertices.Size((size_t)(stride*totalVerts)); for ( int i=0; i<(int)vertices.length; i++ ) vertices[i]=0.0f;
 int v=0;
 double d=0.0;
 EACH(multiplelines->first,CartesiandsListItem,cli) {
  EACH(cli->cartesians.first,Cartesiand,p) {
   Line seg(p->x,p->y,p->x2,p->y2);
   double angleA=rad2deg(seg.LineAngle());
   double angleB=angleA;
   double topA=norm_deg(angleB+90.0)/360.0;
   double topB=norm_deg(angleA+90.0)/360.0;
   double bottomA=norm_deg(angleB-90.0)/360.0;  
   double bottomB=norm_deg(angleA-90.0)/360.0;  
   Vertexd A; c.Point(topA,&A);    A.Add(p->x,p->y);
   Vertexd B; c.Point(topB,&B);    B.Add(p->x2,p->y2);
   Vertexd C; c.Point(bottomB,&C); C.Add(p->x2,p->y2);
   Vertexd D; c.Point(bottomA,&D); D.Add(p->x,p->y);
   if ( texcoordsFollowLines ) {
    double d1=d/totalD;
    double d2=((d+seg.length)/totalD)*texcoordsFollowLinesScale;
    d+=seg.length;
    ADD_QUAD_TCv2(A.x,A.y,(d1),(0.0),B.x,B.y,(d2),(0.0),C.x,C.y,(d2),(1.0),D.x,D.y,(d1),(1.0));
   } else {
    ADD_QUAD(A.x,A.y,B.x,B.y,C.x,C.y,D.x,D.y);
   }
   if ( roundEnds ) {
    double a=topA;
    double delta=iratiod(1,endPrecision)*0.5;
    for ( int i=0; i<endPrecision; i++ ) {
     Vertexd B1; c.Point(a,&B1); B1.Add(p->x,p->y); 
     a+=delta;
     Vertexd B2; c.Point(a,&B2); B2.Add(p->x,p->y);
     if ( texcoordsFollowLines ) {
      Circle tc(0.5);
      Vertexd t1; tc.Point(a-delta,&t1); t1.Add(0.5,0.5);
      Vertexd t2; tc.Point(a,&t2); t2.Add(0.5,0.5);
      ADD_TRI_TC(B2.x,B2.y,(t2.x),(0.0),B1.x,B1.y,(t1.x),(1.0),p->x,p->y,(0.5),(0.5));
     } else {
      ADD_TRI(B2.x,B2.y,B1.x,B1.y,p->x,p->y);
     }
    }
    a=bottomB;
    delta=iratiod(1,endPrecision)*0.5;
    for ( int i=0; i<endPrecision; i++ ) {
     Vertexd B1; c.Point(a,&B1); B1.Add(p->x2,p->y2);
     a+=delta;
     Vertexd B2; c.Point(a,&B2); B2.Add(p->x2,p->y2);
     if ( texcoordsFollowLines ) {
      Circle tc(0.5);
      Vertexd t1; tc.Point(a-delta,&t1); t1.Add(0.5,0.5);
      Vertexd t2; tc.Point(a,&t2); t2.Add(0.5,0.5);
      ADD_TRI_TC(B2.x,B2.y,(t2.x),(0.0),B1.x,B1.y,(t1.x),(1.0),p->x2,p->y2,(0.5),(0.5));
     } else {
      ADD_TRI(B2.x,B2.y,B1.x,B1.y,p->x2,p->y2);
     }
    }   
   }
  }
 }
 if ( !texcoordsFollowLines ) {
  v=0;
  for ( int i=0; i<totalVerts; i++ ) {
   vertices[v+2]=tint.rf;
   vertices[v+3]=tint.gf;
   vertices[v+4]=tint.bf;
   if ( !flipX && !flipY ) {
    vertices[v+5]=vertices[v]/1.0f;
    vertices[v+6]=vertices[v+1]/1.0f;
   } else if ( flipX && flipY ) {
    vertices[v+5]=1.0f-vertices[v]/1.0f;
    vertices[v+6]=1.0f-vertices[v+1]/1.0f;
   } else if ( flipX ) {
    vertices[v+5]=1.0f-vertices[v]/1.0f;
    vertices[v+6]=vertices[v+1]/1.0f;
   } else if ( flipY ) {
    vertices[v+5]=vertices[v]/1.0f;
    vertices[v+6]=1.0f-vertices[v+1]/1.0f;
   } 
   v+=stride;
  }
 } else {
  int v=0;
  for ( int i=0; i<totalVerts; i++ ) {
   vertices[v+2]=tint.rf;
   vertices[v+3]=tint.gf;
   vertices[v+4]=tint.bf;
   v+=stride;
  }
 }
 rrs->elements=totalElements;
v=0;
for ( int i=0; i<totalVerts; i++ ) { OUTPUT("v%d: %f %f %f %f %f %f %f\n",i,vertices[v],vertices[v+1],vertices[v+2],vertices[v+3],vertices[v+4],vertices[v+5],vertices[v+6]); v+=stride; }
u=0;
for ( int i=0; i<totalTris; i++ ) { OUTPUT("e%d: %d %d %d\n",i,elements[u],elements[u+1],elements[u+2]); u+=3; }
 // Upload elements and vertices
 glReportError(glGetError());
 glGenBuffers(1, &(rrs->ebo));
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rrs->ebo);
 glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(GLuint)*elements.length), elements.list, GL_STATIC_DRAW);
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
 glGenBuffers(1, &(rrs->vbo));
 glBindBuffer(GL_ARRAY_BUFFER, rrs->vbo);
 glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(GLfloat)*vertices.length), vertices.list, GL_STATIC_DRAW);
 glBindBuffer(GL_ARRAY_BUFFER,0);
 glReportError(glGetError());
 return Add(rrs);
}

int TwoDeeElementStash::GenVariableThickness( Cartesiands *lines, Crayon tint, bool roundEnds, bool texcoordsFollowLines, bool teeth, bool flipX, bool flipY, bool fitTo1, double texcoordsFollowLinesScale, int endPrecision ) {
 if ( lines->count < 2 ) return -1;
 if ( fitTo1 ) {
  double LfitThick=0.0;
  double TfitThick=0.0;
  double BfitThick=1.0;
  double RfitThick=1.0;
  Cartesiand bounds;
  lines->Bounds2d(&bounds);
  EACH(lines->first,Cartesiand,L) {
   L->x=mapRanged(L->x,bounds.x,bounds.x2,LfitThick,RfitThick);
   L->y=mapRanged(L->y,bounds.y,bounds.y2,TfitThick,BfitThick);
  }
 }
 endPrecision=UMAX(3,endPrecision);
 double totalD=lines->Distance2d();
 TwoDeeElement *rrs=new TwoDeeElement;
 int stride=7; // PosX,PosY,ColR,ColG,ColB,TexX,TexY
 int totalTris=(lines->count-1)*2+(roundEnds?endPrecision*2:0);
 int totalVerts=totalTris*3;
 int totalElements=totalTris*3;
 Indexed<GLuint> elements;  elements.Size((size_t)(totalElements));     for ( int i=0; i<(int)elements.length; i++ ) elements[i]=0;
 int u=0;
 Indexed<GLfloat> vertices; vertices.Size((size_t)(stride*totalVerts)); for ( int i=0; i<(int)vertices.length; i++ ) vertices[i]=0.0f;
 int v=0;
 Cartesiand *p=(Cartesiand *)(lines->first);
 Cartesiand *p_next=(Cartesiand *)(p->next);
 Cartesiand *p_prev=null;
 double d=0.0;
 if ( !teeth ) while ( p_next ) {
  double thicknessA=p->w;
  double thicknessB=p_next->w;
  Circle c1(thicknessA);
  Circle c2(thicknessB);
  Line seg(p->x,p->y,p_next->x,p_next->y);
  double angleA=rad2deg(seg.LineAngle());
  double angleB=angleA;
  if ( p_prev ) {
   Line seg3(p_prev->x,p_prev->y,p->x,p->y);
   angleB=rad2deg(seg3.LineAngle());
  }
  double topA=norm_deg(angleB+90.0)/360.0;
  double topB=norm_deg(angleA+90.0)/360.0;
  double bottomA=norm_deg(angleB-90.0)/360.0;  
  double bottomB=norm_deg(angleA-90.0)/360.0;  
  Vertexd A; c1.Point(topA,&A);    A.Add(p->x,p->y);
  Vertexd B; c2.Point(topB,&B);    B.Add(p_next->x,p_next->y);
  Vertexd C; c2.Point(bottomB,&C); C.Add(p_next->x,p_next->y);
  Vertexd D; c1.Point(bottomA,&D); D.Add(p->x,p->y);
  if ( texcoordsFollowLines ) {
   double d1=d/totalD;
   double d2=((d+seg.length)/totalD)*texcoordsFollowLinesScale;
   d+=seg.length;
   ADD_QUAD_TCv2(A.x,A.y,(d1),(0.0),B.x,B.y,(d2),(0.0),C.x,C.y,(d2),(1.0),D.x,D.y,(d1),(1.0));
  } else {
   ADD_QUAD(A.x,A.y,B.x,B.y,C.x,C.y,D.x,D.y);
  }
  if ( roundEnds ) {
   if ( p == lines->first ) {
    double a=topA;
    double delta=iratiod(1,endPrecision)*0.5;
    for ( int i=0; i<endPrecision; i++ ) {
     Vertexd B1; c1.Point(a,&B1); B1.Add(p->x,p->y); 
     a+=delta;
     Vertexd B2; c1.Point(a,&B2); B2.Add(p->x,p->y);
     if ( texcoordsFollowLines ) {
      Circle tc(0.5);
      Vertexd t1; tc.Point(a-delta,&t1); t1.Add(0.5,0.5);
      Vertexd t2; tc.Point(a,&t2); t2.Add(0.5,0.5);
      ADD_TRI_TC(B2.x,B2.y,(t2.x),(0.0),B1.x,B1.y,(t1.x),(1.0),p->x,p->y,(0.5),(0.5));
     } else {
      ADD_TRI(B2.x,B2.y,B1.x,B1.y,p->x,p->y);
     }
    }
   }
   if ( p_next == lines->last ) {
    double a=bottomB;
    double delta=iratiod(1,endPrecision)*0.5;
    for ( int i=0; i<endPrecision; i++ ) {
     Vertexd B1; c2.Point(a,&B1); B1.Add(p_next->x,p_next->y);
     a+=delta;
     Vertexd B2; c2.Point(a,&B2); B2.Add(p_next->x,p_next->y);
     if ( texcoordsFollowLines ) {
      Circle tc(0.5);
      Vertexd t1; tc.Point(a-delta,&t1); t1.Add(0.5,0.5);
      Vertexd t2; tc.Point(a,&t2); t2.Add(0.5,0.5);
      ADD_TRI_TC(B2.x,B2.y,(t2.x),(0.0),B1.x,B1.y,(t1.x),(1.0),p->x,p->y,(0.5),(0.5));
     } else {
      ADD_TRI(B2.x,B2.y,B1.x,B1.y,p->x,p->y);
     }
    }
   }
  }
  p_prev=p;
  p=(Cartesiand *)p->next;
  p_next=(Cartesiand *)p_next->next;
 } else while ( p_next ) {
  double thickness=p->w;
  Circle c(thickness);
  Line seg(p->x,p->y,p_next->x,p_next->y);
  double angleA=rad2deg(seg.LineAngle());
  double angleB=angleA;
  if ( p_prev ) {
   Line seg3(p_prev->x,p_prev->y,p->x,p->y);
   angleB=rad2deg(seg3.LineAngle());
  }
  double topA=norm_deg(angleB+90.0)/360.0;
  double topB=norm_deg(angleA+90.0)/360.0;
  double bottomA=norm_deg(angleB-90.0)/360.0;  
  double bottomB=norm_deg(angleA-90.0)/360.0;  
  Vertexd A; c.Point(topA,&A);    A.Add(p->x,p->y);
  Vertexd B; c.Point(topB,&B);    B.Add(p_next->x,p_next->y);
  Vertexd C; c.Point(bottomB,&C); C.Add(p_next->x,p_next->y);
  Vertexd D; c.Point(bottomA,&D); D.Add(p->x,p->y);
  if ( texcoordsFollowLines ) {
   double d1=d/totalD;
   double d2=((d+seg.length)/totalD)*texcoordsFollowLinesScale;
   d+=seg.length;
   ADD_QUAD_TCv2(A.x,A.y,(d1),(0.0),B.x,B.y,(d2),(0.0),C.x,C.y,(d2),(1.0),D.x,D.y,(d1),(1.0));
  } else {
   ADD_QUAD(A.x,A.y,B.x,B.y,C.x,C.y,D.x,D.y);
  }
  if ( roundEnds ) {
   if ( p == lines->first ) {
    double a=topA;
    double delta=iratiod(1,endPrecision)*0.5;
    for ( int i=0; i<endPrecision; i++ ) {
     Vertexd B1; c.Point(a,&B1); B1.Add(p->x,p->y); 
     a+=delta;
     Vertexd B2; c.Point(a,&B2); B2.Add(p->x,p->y);
     if ( texcoordsFollowLines ) {
      Circle tc(0.5);
      Vertexd t1; tc.Point(a-delta,&t1); t1.Add(0.5,0.5);
      Vertexd t2; tc.Point(a,&t2); t2.Add(0.5,0.5);
      ADD_TRI_TC(B2.x,B2.y,(t2.x),(0.0),B1.x,B1.y,(t1.x),(1.0),p->x,p->y,(0.5),(0.5));
     } else {
      ADD_TRI(B2.x,B2.y,B1.x,B1.y,p->x,p->y);
     }
    }
   }
   if ( p_next == lines->last ) {
    double a=bottomB;
    double delta=iratiod(1,endPrecision)*0.5;
    for ( int i=0; i<endPrecision; i++ ) {
     Vertexd B1; c.Point(a,&B1); B1.Add(p_next->x,p_next->y);
     a+=delta;
     Vertexd B2; c.Point(a,&B2); B2.Add(p_next->x,p_next->y);
     if ( texcoordsFollowLines ) {
      Circle tc(0.5);
      Vertexd t1; tc.Point(a-delta,&t1); t1.Add(0.5,0.5);
      Vertexd t2; tc.Point(a,&t2); t2.Add(0.5,0.5);
      ADD_TRI_TC(B2.x,B2.y,(t2.x),(0.0),B1.x,B1.y,(t1.x),(1.0),p->x,p->y,(0.5),(0.5));
     } else {
      ADD_TRI(B2.x,B2.y,B1.x,B1.y,p->x,p->y);
     }
    }
   }
  }
  p_prev=p;
  p=(Cartesiand *)p->next;
  p_next=(Cartesiand *)p_next->next;
 }
 if ( !texcoordsFollowLines ) {
  v=0;
  for ( int i=0; i<totalVerts; i++ ) {
   vertices[v+2]=tint.rf;
   vertices[v+3]=tint.gf;
   vertices[v+4]=tint.bf;
   if ( !flipX && !flipY ) {
    vertices[v+5]=vertices[v]/1.0f;
    vertices[v+6]=vertices[v+1]/1.0f;
   } else if ( flipX && flipY ) {
    vertices[v+5]=1.0f-vertices[v]/1.0f;
    vertices[v+6]=1.0f-vertices[v+1]/1.0f;
   } else if ( flipX ) {
    vertices[v+5]=1.0f-vertices[v]/1.0f;
    vertices[v+6]=vertices[v+1]/1.0f;
   } else if ( flipY ) {
    vertices[v+5]=vertices[v]/1.0f;
    vertices[v+6]=1.0f-vertices[v+1]/1.0f;
   } 
   v+=stride;
  }
 } else {
  int v=0;
  for ( int i=0; i<totalVerts; i++ ) {
   vertices[v+2]=tint.rf;
   vertices[v+3]=tint.gf;
   vertices[v+4]=tint.bf;
   v+=stride;
  }
 }
 rrs->elements=totalElements;
//v=0;
//for ( int i=0; i<totalVerts; i++ ) { OUTPUT("v%d: %f %f %f %f %f %f %f\n",i,vertices[v],vertices[v+1],vertices[v+2],vertices[v+3],vertices[v+4],vertices[v+5],vertices[v+6]); v+=stride; }
//u=0;
//for ( int i=0; i<totalTris; i++ ) { OUTPUT("e%d: %d %d %d\n",i,elements[u],elements[u+1],elements[u+2]); u+=3; }
 // Upload elements and vertices
 glReportError(glGetError());
 glGenBuffers(1, &(rrs->ebo));
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rrs->ebo);
 glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(GLuint)*elements.length), elements.list, GL_STATIC_DRAW);
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
 glGenBuffers(1, &(rrs->vbo));
 glBindBuffer(GL_ARRAY_BUFFER, rrs->vbo);
 glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(GLfloat)*vertices.length), vertices.list, GL_STATIC_DRAW);
 glBindBuffer(GL_ARRAY_BUFFER,0);
 glReportError(glGetError());
 return Add(rrs);
}


/*
 *      . t      . s            0,0   __t_________s__   1,0
 *     .          .                  |__|_b_______|__|r                   . t
 *  m . + A     B + . r             m|a |A       B|c |            ...N .
 *  n . + D     C + . q              |  |    f    |  |            2 .
 *     .          .                 n|__|D_______C|__|          1 .
 *      .        .                   |__|_e_______|__|q          . m       . A
 *       o        p             0,1     o         p     1,1
 */

int TwoDeeElementStash::GenRoundRect( double cornerRadius, Crayon tint, bool flipX, bool flipY, int precision ) {
 TwoDeeElement *rrs=new TwoDeeElement;
 int stride=7; // PosX,PosY,ColR,ColG,ColB,TexX,TexY
 int totalTris=precision*4+10;
 int totalVerts=12+precision*4*3;
 int totalElements=totalTris*3;
 Indexed<GLuint> elements;  elements.Size((size_t)(totalElements));     for ( int i=0; i<(int)elements.length; i++ ) elements[i]=0;
 Indexed<GLfloat> vertices; vertices.Size((size_t)(stride*totalVerts)); for ( int i=0; i<(int)vertices.length; i++ ) vertices[i]=0.0f;
 Cartesiand A,B,C,D,m,n,o,p,q,r,s,t;
 A.Set(cornerRadius,cornerRadius);
 B.Set(1.0-cornerRadius,cornerRadius);
 C.Set(1.0-cornerRadius,1.0-cornerRadius);
 D.Set(cornerRadius,1.0-cornerRadius);
 m.Set(0.0,cornerRadius);
 n.Set(0.0,1.0-cornerRadius);
 o.Set(cornerRadius,1.0);
 p.Set(1.0-cornerRadius,1.0);
 q.Set(1.0,1.0-cornerRadius);
 r.Set(1.0,cornerRadius);
 s.Set(1.0-cornerRadius,0.0);
 t.Set(cornerRadius,0.0);
 int v=0;
 ADD_VERT(A.x,A.y); // A 0
 ADD_VERT(B.x,B.y); // B 1
 ADD_VERT(C.x,C.y); // C 2
 ADD_VERT(D.x,D.y); // D 3
 ADD_VERT(m.x,m.y); // m 4
 ADD_VERT(n.x,n.y); // n 5
 ADD_VERT(o.x,o.y); // o 6
 ADD_VERT(p.x,p.y); // p 7
 ADD_VERT(q.x,q.y); // q 8
 ADD_VERT(r.x,r.y); // r 9
 ADD_VERT(s.x,s.y); // s 10
 ADD_VERT(t.x,t.y); // t 11
 int u=0;
 ADD_ELEM(0, 1, 3); // f1 ABD
 ADD_ELEM(3, 1, 2); // f2 DBC
 ADD_ELEM(4, 0, 5); // a1 mAn
 ADD_ELEM(3, 0, 5); // a2 DAn
 ADD_ELEM(1, 11,0); // b1 tBA
 ADD_ELEM(11,10,1); // b2 tsB
 ADD_ELEM(1, 9, 2); // c1 BrC
 ADD_ELEM(9, 8, 2); // c2 rqC
 ADD_ELEM(3, 2, 6); // d1 DCo
 ADD_ELEM(6, 2, 7); // d2 oCp
 GLuint v1=0,v2=0,v3=0;
 for ( int i=0; i<precision; i++ ) {
  Circle c(cornerRadius);
  double ang1=iratiod(i,precision)*0.25,ang2=iratiod(i+1,precision)*0.25;
  Vertexd B1; c.Point(0.75+ang1,&B1); B1.Add(B.x,B.y);
  Vertexd B2; c.Point(0.75+ang2,&B2); B2.Add(B.x,B.y);
  v1=v/stride; ADD_VERT(B.x,  B.y );
  v2=v/stride; ADD_VERT(B2.x, B2.y);
  v3=v/stride; ADD_VERT(B1.x, B1.y);
  ADD_ELEM(v1,v2,v3);
  Vertexd A1; c.Point(0.5+ang1,&A1); A1.Add(A.x,A.y);
  Vertexd A2; c.Point(0.5+ang2,&A2); A2.Add(A.x,A.y);
  v1=v/stride; ADD_VERT(A.x,  A.y );
  v2=v/stride; ADD_VERT(A2.x, A2.y);
  v3=v/stride; ADD_VERT(A1.x, A1.y);
  ADD_ELEM(v1,v2,v3);
  Vertexd D1; c.Point(0.25+ang1,&D1); D1.Add(D.x,D.y);
  Vertexd D2; c.Point(0.25+ang2,&D2); D2.Add(D.x,D.y);
  v1=v/stride; ADD_VERT(D.x,  D.y );
  v2=v/stride; ADD_VERT(D2.x, D2.y);
  v3=v/stride; ADD_VERT(D1.x, D1.y);
  ADD_ELEM(v1,v2,v3);
  Vertexd C1; c.Point(ang1,&C1); C1.Add(C.x,C.y);
  Vertexd C2; c.Point(ang2,&C2); C2.Add(C.x,C.y);
  v1=v/stride; ADD_VERT(C.x, C.y );
  v2=v/stride; ADD_VERT(C2.x,C2.y);
  v3=v/stride; ADD_VERT(C1.x,C1.y);
  ADD_ELEM(v1,v2,v3);
 }
 // Set tint and tex coord
 v=0;
 for ( int i=0; i<totalVerts; i++ ) {
  vertices[v+2]=tint.rf;
  vertices[v+3]=tint.gf;
  vertices[v+4]=tint.bf;
  if ( !flipX && !flipY ) {
   vertices[v+5]=vertices[v]/1.0f;
   vertices[v+6]=vertices[v+1]/1.0f;
  } else if ( flipX && flipY ) {
   vertices[v+5]=1.0f-vertices[v]/1.0f;
   vertices[v+6]=1.0f-vertices[v+1]/1.0f;
  } else if ( flipX ) {
   vertices[v+5]=1.0f-vertices[v]/1.0f;
   vertices[v+6]=vertices[v+1]/1.0f;
  } else if ( flipY ) {
   vertices[v+5]=vertices[v]/1.0f;
   vertices[v+6]=1.0f-vertices[v+1]/1.0f;
  } 
  v+=stride;
 }
//v=0;
//for ( int i=0; i<totalVerts; i++ ) { OUTPUT("v%d: %f %f %f %f %f %f %f\n",i,vertices[v],vertices[v+1],vertices[v+2],vertices[v+3],vertices[v+4],vertices[v+5],vertices[v+6]); v+=stride; }
//u=0;
//for ( int i=0; i<totalTris; i++ ) { OUTPUT("e%d: %d %d %d\n",i,elements[u],elements[u+1],elements[u+2]); u+=3; }
 rrs->elements=totalElements;
 // Upload elements and vertices
 glReportError(glGetError());
 glGenBuffers(1, &(rrs->ebo));
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rrs->ebo);
 glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(GLuint)*elements.length), elements.list, GL_STATIC_DRAW);
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
 glGenBuffers(1, &(rrs->vbo));
 glBindBuffer(GL_ARRAY_BUFFER, rrs->vbo);
 glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(GLfloat)*vertices.length), vertices.list, GL_STATIC_DRAW);
 glBindBuffer(GL_ARRAY_BUFFER,0);
 glReportError(glGetError());
 return Add(rrs);
}

int TwoDeeElementStash::GenRoundRect( double xcornerRadius, double ycornerRadius, Crayon tint, bool flipX, bool flipY, int precision ) {
  TwoDeeElement *rrs=new TwoDeeElement;
  int stride=7; // PosX,PosY,ColR,ColG,ColB,TexX,TexY
  int totalTris=precision*4+10;
  int totalVerts=12+precision*4*3;
  int totalElements=totalTris*3;
  Indexed<GLuint> elements;  elements.Size((size_t)(totalElements));     for ( int i=0; i<(int)elements.length; i++ ) elements[i]=0;
  Indexed<GLfloat> vertices; vertices.Size((size_t)(stride*totalVerts)); for ( int i=0; i<(int)vertices.length; i++ ) vertices[i]=0.0f;
  Cartesiand A,B,C,D,m,n,o,p,q,r,s,t;
  A.Set(xcornerRadius,ycornerRadius);
  B.Set(1.0-xcornerRadius,ycornerRadius);
  C.Set(1.0-xcornerRadius,1.0-ycornerRadius);
  D.Set(xcornerRadius,1.0-ycornerRadius);
  m.Set(0.0,ycornerRadius);
  n.Set(0.0,1.0-ycornerRadius);
  o.Set(xcornerRadius,1.0);
  p.Set(1.0-xcornerRadius,1.0);
  q.Set(1.0,1.0-ycornerRadius);
  r.Set(1.0,ycornerRadius);
  s.Set(1.0-xcornerRadius,0.0);
  t.Set(xcornerRadius,0.0);
  int v=0;
  ADD_VERT(A.x,A.y); // A 0
  ADD_VERT(B.x,B.y); // B 1
  ADD_VERT(C.x,C.y); // C 2
  ADD_VERT(D.x,D.y); // D 3
  ADD_VERT(m.x,m.y); // m 4
  ADD_VERT(n.x,n.y); // n 5
  ADD_VERT(o.x,o.y); // o 6
  ADD_VERT(p.x,p.y); // p 7
  ADD_VERT(q.x,q.y); // q 8
  ADD_VERT(r.x,r.y); // r 9
  ADD_VERT(s.x,s.y); // s 10
  ADD_VERT(t.x,t.y); // t 11
  int u=0;
  ADD_ELEM(0, 1, 3); // f1 ABD
  ADD_ELEM(3, 1, 2); // f2 DBC
  ADD_ELEM(4, 0, 5); // a1 mAn
  ADD_ELEM(3, 0, 5); // a2 DAn
  ADD_ELEM(1, 11,0); // b1 tBA
  ADD_ELEM(11,10,1); // b2 tsB
  ADD_ELEM(1, 9, 2); // c1 BrC
  ADD_ELEM(9, 8, 2); // c2 rqC
  ADD_ELEM(3, 2, 6); // d1 DCo
  ADD_ELEM(6, 2, 7); // d2 oCp
  GLuint v1=0,v2=0,v3=0;
  for ( int i=0; i<precision; i++ ) {
   double ang1=iratiod(i,precision)*0.25,ang2=iratiod(i+1,precision)*0.25;
   Vertexd B1( B.x+cos((0.75+ang1)*TWO_PI)*xcornerRadius, B.y+sin((0.75+ang1)*TWO_PI)*ycornerRadius, 0.0 );
   Vertexd B2( B.x+cos((0.75+ang2)*TWO_PI)*xcornerRadius, B.y+sin((0.75+ang2)*TWO_PI)*ycornerRadius, 0.0 );
   v1=v/stride; ADD_VERT(B.x,  B.y );
   v2=v/stride; ADD_VERT(B2.x, B2.y);
   v3=v/stride; ADD_VERT(B1.x, B1.y);
   ADD_ELEM(v1,v2,v3);
   Vertexd A1( A.x+cos((0.5+ang1)*TWO_PI)*xcornerRadius, A.y+sin((0.5+ang1)*TWO_PI)*ycornerRadius, 0.0 );
   Vertexd A2( A.x+cos((0.5+ang2)*TWO_PI)*xcornerRadius, A.y+sin((0.5+ang2)*TWO_PI)*ycornerRadius, 0.0 );
   v1=v/stride; ADD_VERT(A.x,  A.y );
   v2=v/stride; ADD_VERT(A2.x, A2.y);
   v3=v/stride; ADD_VERT(A1.x, A1.y);
   ADD_ELEM(v1,v2,v3);
   Vertexd D1( D.x+cos((0.25+ang1)*TWO_PI)*xcornerRadius, D.y+sin((0.25+ang1)*TWO_PI)*ycornerRadius, 0.0 );
   Vertexd D2( D.x+cos((0.25+ang2)*TWO_PI)*xcornerRadius, D.y+sin((0.25+ang2)*TWO_PI)*ycornerRadius, 0.0 );
   v1=v/stride; ADD_VERT(D.x,  D.y );
   v2=v/stride; ADD_VERT(D2.x, D2.y);
   v3=v/stride; ADD_VERT(D1.x, D1.y);
   ADD_ELEM(v1,v2,v3);
   Vertexd C1( C.x+cos(ang1*TWO_PI)*xcornerRadius, C.y+sin(ang1*TWO_PI)*ycornerRadius, 0.0 );
   Vertexd C2( C.x+cos(ang2*TWO_PI)*xcornerRadius, C.y+sin(ang2*TWO_PI)*ycornerRadius, 0.0 );
   v1=v/stride; ADD_VERT(C.x, C.y );
   v2=v/stride; ADD_VERT(C2.x,C2.y);
   v3=v/stride; ADD_VERT(C1.x,C1.y);
   ADD_ELEM(v1,v2,v3);
  }
  // Set tint and tex coord
  v=0;
  for ( int i=0; i<totalVerts; i++ ) {
   vertices[v+2]=tint.rf;
   vertices[v+3]=tint.gf;
   vertices[v+4]=tint.bf;
   if ( !flipX && !flipY ) {
    vertices[v+5]=vertices[v]/1.0f;
    vertices[v+6]=vertices[v+1]/1.0f;
   } else if ( flipX && flipY ) {
    vertices[v+5]=1.0f-vertices[v]/1.0f;
    vertices[v+6]=1.0f-vertices[v+1]/1.0f;
   } else if ( flipX ) {
    vertices[v+5]=1.0f-vertices[v]/1.0f;
    vertices[v+6]=vertices[v+1]/1.0f;
   } else if ( flipY ) {
    vertices[v+5]=vertices[v]/1.0f;
    vertices[v+6]=1.0f-vertices[v+1]/1.0f;
   } 
   v+=stride;
  }
  //v=0;
  //for ( int i=0; i<totalVerts; i++ ) { OUTPUT("v%d: %f %f %f %f %f %f %f\n",i,vertices[v],vertices[v+1],vertices[v+2],vertices[v+3],vertices[v+4],vertices[v+5],vertices[v+6]); v+=stride; }
  //u=0;
  //for ( int i=0; i<totalTris; i++ ) { OUTPUT("e%d: %d %d %d\n",i,elements[u],elements[u+1],elements[u+2]); u+=3; }
  rrs->elements=totalElements;
  // Upload elements and vertices
  glReportError(glGetError());
  glGenBuffers(1, &(rrs->ebo));
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rrs->ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(GLuint)*elements.length), elements.list, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
  glGenBuffers(1, &(rrs->vbo));
  glBindBuffer(GL_ARRAY_BUFFER, rrs->vbo);
  glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(GLfloat)*vertices.length), vertices.list, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER,0);
  glReportError(glGetError());
  return Add(rrs);
 }
 
int TwoDeeElementStash::GenFacets( STLFacets *in, Crayon tint, bool fitTo1, bool flipX, bool flipY ) {
 ImmediateModel m; in->toModel(&m,fitTo1);
 TwoDeeElement *rrs=new TwoDeeElement;
 int stride=7; // PosX,PosY,ColR,ColG,ColB,TexX,TexY
 int totalTris=in->count;
 int totalVerts=in->count*3;
 int totalElements=in->count;
 Indexed<GLuint> elements;  elements.Size((size_t)(totalElements*3));   for ( int i=0; i<(int)elements.length; i++ ) elements[i]=0;
 Indexed<GLfloat> vertices; vertices.Size((size_t)(stride*totalVerts)); for ( int i=0; i<(int)vertices.length; i++ ) vertices[i]=0.0f;
 GLuint v1=0,v2=0,v3=0;
 int u=0;
 int v=0;
 EACH(m.polygons.first,mPolygon,p) {
  Vertex *a=(Vertex *)p->points.Element(0);
  Vertex *b=(Vertex *)p->points.Element(1);
  Vertex *c=(Vertex *)p->points.Element(2);
  if ( !a || !b || !c ) { delete rrs; return -1; }
  v1=v; ADD_VERT(a->x,a->y);
  v2=v; ADD_VERT(b->x,b->y);
  v3=v; ADD_VERT(c->x,c->y);
  ADD_ELEM(v1,v2,v3);
 }
 // Set tint and tex coord
 v=0;
 for ( int i=0; i<totalVerts; i++ ) {
  vertices[v+2]=tint.rf;
  vertices[v+3]=tint.gf;
  vertices[v+4]=tint.bf;
  if ( !flipX && !flipY ) {
   vertices[v+5]=vertices[v]/1.0f;
   vertices[v+6]=vertices[v+1]/1.0f;
  } else if ( flipX && flipY ) {
   vertices[v+5]=1.0f-vertices[v]/1.0f;
   vertices[v+6]=1.0f-vertices[v+1]/1.0f;
  } else if ( flipX ) {
   vertices[v+5]=1.0f-vertices[v]/1.0f;
   vertices[v+6]=vertices[v+1]/1.0f;
  } else if ( flipY ) {
   vertices[v+5]=vertices[v]/1.0f;
   vertices[v+6]=1.0f-vertices[v+1]/1.0f;
  } 
  v+=stride;
 }
 OUTPUT("GenFacets:\n");
 v=0;
 for ( int i=0; i<totalVerts; i++ ) { OUTPUT("v%d: %f %f %f %f %f %f %f\n",i,vertices[v],vertices[v+1],vertices[v+2],vertices[v+3],vertices[v+4],vertices[v+5],vertices[v+6]); v+=stride; }
 u=0;
 for ( int i=0; i<totalTris; i++ ) { OUTPUT("e%d: %d %d %d\n",i,elements[u],elements[u+1],elements[u+2]); u+=3; }
 rrs->elements=totalElements;
 // Upload elements and vertices
 glReportError(glGetError());
 glGenBuffers(1, &(rrs->ebo));
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rrs->ebo);
 glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(GLuint)*elements.length), elements.list, GL_STATIC_DRAW);
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
 glGenBuffers(1, &(rrs->vbo));
 glBindBuffer(GL_ARRAY_BUFFER, rrs->vbo);
 glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(GLfloat)*vertices.length), vertices.list, GL_STATIC_DRAW);
 glBindBuffer(GL_ARRAY_BUFFER,0);
 glReportError(glGetError());
 return Add(rrs);
}

int TwoDeeElementStash::GenTriangles( TwoDeeTriangles *in ) {
 int stride=7; // PosX,PosY,ColR,ColG,ColB,TexX,TexY
 int totalTris=in->count;
 int totalVerts=in->count*3;
 int totalElements=in->count*3;
 Indexed<GLuint> elements;  elements.Size((size_t)(totalElements));   for ( int i=0; i<(int)elements.length; i++ ) elements[i]=0;
 Indexed<GLfloat> vertices; vertices.Size((size_t)(stride*totalVerts)); for ( int i=0; i<(int)vertices.length; i++ ) vertices[i]=0.0f;
 TwoDeeElement *rrs=new TwoDeeElement;
 int u=0;
 int v=0;
 EACH(in->first,TwoDeeTriangle,tdt) {
  GLuint v1=v/stride; ADD_VERT_TINT_TC(tdt->A.x, tdt->A.y, tdt->tintA.rd, tdt->tintA.gd, tdt->tintA.bd, tdt->A.x2, tdt->A.y2 );
  GLuint v2=v/stride; ADD_VERT_TINT_TC(tdt->B.x, tdt->B.y, tdt->tintB.rd, tdt->tintB.gd, tdt->tintB.bd, tdt->B.x2, tdt->B.y2 );
  GLuint v3=v/stride; ADD_VERT_TINT_TC(tdt->C.x, tdt->C.y, tdt->tintC.rd, tdt->tintC.gd, tdt->tintC.bd, tdt->C.x2, tdt->C.y2 );
  ADD_ELEM(v1,v2,v3);
 }
 rrs->elements=totalElements;
 ///OUTPUT("GenTriangles:\n");
 ///v=0;
 ///for ( int i=0; i<totalVerts; i++ ) { OUTPUT("v%d: %f %f %f %f %f %f %f\n",i,vertices[v],vertices[v+1],vertices[v+2],vertices[v+3],vertices[v+4],vertices[v+5],vertices[v+6]); v+=stride; }
 ///u=0;
 ///for ( int i=0; i<totalTris; i++ ) { OUTPUT("e%d: %d %d %d\n",i,elements[u],elements[u+1],elements[u+2]); u+=3; }
 // Upload elements and vertices
 glReportError(glGetError());
 glGenBuffers(1, &(rrs->ebo));
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rrs->ebo);
 glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(GLuint)*elements.length), elements.list, GL_STATIC_DRAW);
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
 glGenBuffers(1, &(rrs->vbo));
 glBindBuffer(GL_ARRAY_BUFFER, rrs->vbo);
 glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(GLfloat)*vertices.length), vertices.list, GL_STATIC_DRAW);
 glBindBuffer(GL_ARRAY_BUFFER,0);
 glReportError(glGetError());
 return Add(rrs);
}

void Art2d::InitSimpleSprites() {
 OUTPUT("InitSimpleSprites:\n");
 // Black/white checkerboard
 float t1[] = {
     0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,
     1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f
 };
 checkers=library.Load(t1,2,2,"checkers",tllto_Tiling);
 // l<r ramp 
 float t2[] = {
     0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,
     0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f
 };
 rampR=library.Load(t2,2,2,"L<R",tllto_Tiling);
 // l>r ramp 
 float t3[] = {
      1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 0.0f,  
      1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 0.0f
 };
 rampL=library.Load(t3,2,2,"L>R",tllto_Tiling);
 // black  
 float t4[] = {
      0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,  
      0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f
 };
 solidblack=library.Load(t4,2,2,"black",tllto_Tiling);
 // white  
 float t5[] = {
      1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f,  
      1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f
 };
 solidwhite=library.Load(t5,2,2,"white",tllto_Tiling);
 glReportError(glGetError());
}

void Art2d::CreatePrimitives() { 
 // Create an element array
 GLuint elements[] = {     0, 1, 2,     2, 3, 0     };
 glGenBuffers(1, &ebo);
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
 glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*6, elements, GL_STATIC_DRAW);
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
 {
  // Create a Vertex Buffer Object and copy the vertex data to it
  glGenBuffers(1, &vTopLeft);
  GLfloat vertices[] = {
  //  Position       Color              Texcoords
       0.0f,  0.0f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f, // Top-left
       1.0f,  0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f, // Top-right
       1.0f,  1.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f, // Bottom-right
       0.0f,  1.0f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f  // Bottom-left
  }; 
  glBindBuffer(GL_ARRAY_BUFFER, vTopLeft);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*(7*4), vertices, GL_STATIC_DRAW); 
 }
 {
  // Create a Vertex Buffer Object and copy the vertex data to it
  glGenBuffers(1, &vTopLeftYFlipped);
  GLfloat vertices[] = {
  //  Position       Color              Texcoords
       0.0f, 1.0f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f, // Top-left
       1.0f, 1.0f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f, // Top-right
       1.0f, 0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f, // Bottom-right
       0.0f, 0.0f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f  // Bottom-left
  }; 
  glBindBuffer(GL_ARRAY_BUFFER, vTopLeftYFlipped);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*(7*4), vertices, GL_STATIC_DRAW); 
 }
 {
  // Create a Vertex Buffer Object and copy the vertex data to it
  glGenBuffers(1, &vTopLeftXFlipped);
  GLfloat vertices[] = {
  //  Position       Color              Texcoords
       0.0f, 1.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f, // Top-left
       1.0f, 1.0f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f, // Top-right
       1.0f, 0.0f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f, // Bottom-right
       0.0f, 0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f  // Bottom-left
  }; 
  glBindBuffer(GL_ARRAY_BUFFER, vTopLeftXFlipped);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*(7*4), vertices, GL_STATIC_DRAW); 
 }
 {
  // Create a Vertex Buffer Object and copy the vertex data to it
  glGenBuffers(1, &vTopLeftXYFlipped);
  GLfloat vertices[] = {
  //  Position       Color              Texcoords
       0.0f, 1.0f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f, // Top-left
       1.0f, 1.0f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f, // Top-right
       1.0f, 0.0f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f, // Bottom-right
       0.0f, 0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f  // Bottom-left
  }; 
  glBindBuffer(GL_ARRAY_BUFFER, vTopLeftXYFlipped);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*(7*4), vertices, GL_STATIC_DRAW); 
 }
 

 {
  // Create a Vertex Buffer Object and copy the vertex data to it
  glGenBuffers(1, &vbo_qn11n11);
  GLfloat vertices[] = {
  //  Position       Color              Texcoords
      -1.0f,  1.0f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f, // Top-left
       1.0f,  1.0f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f, // Top-right
       1.0f, -1.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f, // Bottom-right
      -1.0f, -1.0f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f  // Bottom-left
  }; 
  glBindBuffer(GL_ARRAY_BUFFER, vbo_qn11n11);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*(7*4), vertices, GL_STATIC_DRAW); 
 }
 {
  // Create a Vertex Buffer Object and copy the vertex data to it
  glGenBuffers(1, &vbo_qn11n11YFlipped);
  GLfloat vertices[] = {
  //  Position       Color              Texcoords
      -1.0f,  1.0f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f, // Top-left
       1.0f,  1.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f, // Top-right
       1.0f, -1.0f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f, // Bottom-right
      -1.0f, -1.0f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f  // Bottom-left
  }; 
  glBindBuffer(GL_ARRAY_BUFFER, vbo_qn11n11YFlipped);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*(7*4), vertices, GL_STATIC_DRAW); 
 }
 {
  // Create a Vertex Buffer Object and copy the vertex data to it
  glGenBuffers(1, &vbo_qn11n11XFlipped);
  GLfloat vertices[] = {
  //  Position       Color              Texcoords
      -1.0f,  1.0f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f, // Top-left
       1.0f,  1.0f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f, // Top-right
       1.0f, -1.0f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f, // Bottom-right
      -1.0f, -1.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f  // Bottom-left
  }; 
  glBindBuffer(GL_ARRAY_BUFFER, vbo_qn11n11XFlipped);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*(7*4), vertices, GL_STATIC_DRAW); 
 }
 {
  // Create a Vertex Buffer Object and copy the vertex data to it
  glGenBuffers(1, &vbo_qn11n11XYFlipped);
  GLfloat vertices[] = {
  //  Position       Color              Texcoords
      -1.0,  1.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f, // Top-left
       1.0,  1.0f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f, // Top-right
       1.0, -1.0f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f, // Bottom-right
      -1.0, -1.0f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f  // Bottom-left
  }; 
  glBindBuffer(GL_ARRAY_BUFFER, vbo_qn11n11XYFlipped);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*(7*4), vertices, GL_STATIC_DRAW); 
 }


 {
  // Create a Vertex Buffer Object and copy the vertex data to it
  glGenBuffers(1, &vCentered);
  GLfloat vertices[] = {
  //  Position       Color              Texcoords
      -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f, // Top-left
       0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f, // Top-right
       0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f, // Bottom-right
      -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f  // Bottom-left
  }; 
  glBindBuffer(GL_ARRAY_BUFFER, vCentered);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*(7*4), vertices, GL_STATIC_DRAW); 
 }
 {
  // Create a Vertex Buffer Object and copy the vertex data to it
  glGenBuffers(1, &vCenteredYFlipped);
  GLfloat vertices[] = {
  //  Position       Color              Texcoords
      -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f, // Top-left
       0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f, // Top-right
       0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f, // Bottom-right
      -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f  // Bottom-left
  }; 
  glBindBuffer(GL_ARRAY_BUFFER, vCenteredYFlipped);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*(7*4), vertices, GL_STATIC_DRAW); 
 }
 {
  // Create a Vertex Buffer Object and copy the vertex data to it
  glGenBuffers(1, &vCenteredXFlipped);
  GLfloat vertices[] = {
  //  Position       Color              Texcoords
      -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f, // Top-left
       0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f, // Top-right
       0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f, // Bottom-right
      -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f  // Bottom-left
  }; 
  glBindBuffer(GL_ARRAY_BUFFER, vCenteredXFlipped);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*(7*4), vertices, GL_STATIC_DRAW); 
 }
 {
  // Create a Vertex Buffer Object and copy the vertex data to it
  glGenBuffers(1, &vCenteredXYFlipped);
  GLfloat vertices[] = {
  //  Position       Color              Texcoords
      -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f, // Top-left
       0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f, // Top-right
       0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f, // Bottom-right
      -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f  // Bottom-left
  }; 
  glBindBuffer(GL_ARRAY_BUFFER, vCenteredXYFlipped);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*(7*4), vertices, GL_STATIC_DRAW); 
 }
 glBindBuffer(GL_ARRAY_BUFFER, 0);
}

#include "Curve2d.h"

void Art2dTest::OnLoad() {
 Fullscreen();
 background.Pick(darkTan);
 sprite1=library.Load("data/images/splash.png");
 fboScreen.background.Pick(indigo2);
 Curve2d curve;
 curve.SetLOD(32);
 Vertices *curved=curve.Bezier(0.0,0.0,1.0,1.0,1.0,1.0,1.0,0.0);
 Cartesiands lines;
 lines.fromVertices(curved);
 delete curved;
 stashedCurve=art.stash.GenThickness(&lines);
 EACH(lines.first,Cartesiand,l) l->w=0.1+uniform()*0.2;
 curveTest2=art.stash.GenVariableThickness(&lines,crayons.Pick(red255),true,true);
 curveTest3=art.stash.GenThickness(&lines,crayons.Pick(red255),0.1,false,true);
}

void Art2dTest::Render() {
 if ( input->KeyUp(DX_LEFT) ) stashNumber=UMAX(stashNumber-1,0);
 if ( input->KeyUp(DX_RIGHT) ) stashNumber=UMIN(art.stash.count-1,stashNumber+1);
 if ( input->KeyDown(DX_F) ) fboScreen.Bind();
 if ( input->KeyDown(DX_P) ) {
  art.Pivot(sprite1,crayons.Pick(macNcheese),none,
   1920.0/2.0,1080.0/2.0,0.0,0.0,
   1080.0/2.0,1920.0/2.0,input->mx,
   input->left?true:false,input->right?true:false
  );
  art.Pivot(sprite1,crayons.Pick(macNcheese),none,
   1920.0/2.0,1080.0/2.0,-1920.0/2.0/2.0,0.0,
   1080.0/2.0,1920.0/2.0,input->mx,
   input->left?true:false,input->right?true:false
  );
  art.Pivot(sprite1,crayons.Pick(macNcheese),none,
   0.0,0.0,0.0,-1080.0/2.0/2.0,
   1080.0/2.0,1920.0/2.0,input->mx,
   input->left?true:false,input->right?true:false
  );
  art.Pivot(sprite1,crayons.Pick(macNcheese),none,
   1920.0,1080.0,-1920.0/2.0/2.0,0.0,
   1080.0/2.0,1920.0/2.0,input->mx,
   input->left?true:false,input->right?true:false
  );
  art.Pivot(sprite1,crayons.Pick(macNcheese),none,
   512.0,512.0,0.0,0.0,
   108.0,192.0,input->mx,
   input->left?true:false,input->right?true:false
  );
  art.Pivot(sprite1,crayons.Pick(macNcheese),additive,
   1000.0,1000.0,10.0,10.0,
   10.0,10.0,input->mx,
   input->left?true:false,input->right?true:false
  );
  art.Stretch(sprite1,crayons.Pick(red255),additive,0.0,0.0,input->mx,input->my,
   input->left?true:false,input->right?true:false
  );
 }
 art.Element(sprite1,crayons.Pick(alabaster),none,stashNumber,input->mx-10.0,input->my-10.0,input->mx*2,input->my*2);
 art.Element(crayons.Pick(blue255),none,stashNumber,50.0,50.0,100.0,200.0);
 art.ElementLines(crayons.Pick(green255),none,stashNumber,50.0,50.0,100.0,200.0);
 for ( int i=0; i<8; i++ ) {
  Circle c(20.0);
  double ang1=iratiod(i,8)*0.25,ang2=iratiod(i+1,8)*0.25;
  Vertexd B1; c.Point(0.75+ang1,&B1);
  art.Element(crayons.Pick(gold),none,stashNumber,(double)input->mx+B1.x*10.0,(double)input->my+B1.y*10.0,20.0,20.0);
  Vertexd B2; c.Point(0.75+ang2,&B2); 
  art.Element(crayons.Pick(gold),none,stashNumber,(double)input->mx+B2.x*10.0,(double)input->my+B2.y*10.0,20.0,20.0);
  Vertexd A1; c.Point(0.5+ang1,&A1);
  art.Element(crayons.Pick(blue255),none,stashNumber,(double)input->mx+A1.x*10.0,(double)input->my+A1.y*10.0,20.0,20.0);
  Vertexd A2; c.Point(0.5+ang2,&A2); 
  art.Element(crayons.Pick(blue255),none,stashNumber,(double)input->mx+A2.x*10.0,(double)input->my+A2.y*10.0,20.0,20.0);
  Vertexd D1; c.Point(ang1,&D1);
  art.Element(crayons.Pick(red255),none,stashNumber,(double)input->mx+D1.x*10.0,(double)input->my+D1.y*10.0,20.0,20.0);
  Vertexd D2; c.Point(ang2,&D2);
  art.Element(crayons.Pick(red255),none,stashNumber,(double)input->mx+D2.x*10.0,(double)input->my+D2.y*10.0,20.0,20.0);
  Vertexd C1; c.Point(0.25+ang1,&C1);
  art.Element(crayons.Pick(orange),none,stashNumber,(double)input->mx+C1.x*10.0,(double)input->my+C1.y*10.0,20.0,20.0);
  Vertexd C2; c.Point(0.25+ang2,&C2);
  art.Element(crayons.Pick(orange),none,stashNumber,(double)input->mx+C2.x*10.0,(double)input->my+C2.y*10.0,30.0,30.0);
  art.ElementLines(crayons.Pick(orange),none,stashNumber,(double)input->mx+C2.x*10.0,(double)input->my+C2.y*10.0,30.0,30.0);
//  art.RoundRect(crayons.Pick(green255),none,stashNumber,(double)input->mx+C2.x*10.0,(double)input->my+C2.y*10.0,30.0,30.0,true);
 }
 if ( input->KeyDown(DX_F) ) { fboScreen.Unbind(); fboScreen.RenderUpsideDown(0,0,display->w,display->h); }
 art.Element(sprite1,crayons.Pick(hotPink),none,stashedCurve,0.0,0.0,512.0,512.0);
 art.Element(sprite1,crayons.Pick(alabaster),none,curveTest2,512.0,0.0,512.0,512.0);
 art.ElementLines(crayons.Pick(alabaster),none,curveTest2,512.0,0.0,512.0,512.0);
 art.Element(sprite1,crayons.Pick(gold),none,curveTest3,512.0,512.0,512.0,512.0);
 art.Element(sprite1,crayons.Pick(yellow),none,art.stash.ellipsoid,0.0,512.0,256.0,256.0);
 art.Element(sprite1,crayons.Pick(olivineGreen),none,art.stash.ellipsoidCenterEdge,0.0,768.0,256.0,256.0);
 art.Element(sprite1,crayons.Pick(macNcheese),none,art.stash.ellipsoidFlipX,256.0,512.0,256.0,256.0);
 art.Element(sprite1,crayons.Pick(indigo),none,art.stash.ellipsoidCenterEdge,256.0,768.0,256.0,256.0);
 art.ElementLines(crayons.Pick(yellow),none,art.stash.ellipsoid,0.0,512.0,256.0,256.0);
 art.ElementLines(crayons.Pick(olivineGreen),none,art.stash.ellipsoidCenterEdge,0.0,768.0,256.0,256.0);
 art.ElementLines(crayons.Pick(macNcheese),none,art.stash.ellipsoid,256.0,512.0,256.0,256.0);
 art.ElementLines(crayons.Pick(indigo),none,art.stash.ellipsoidCenterEdge,256.0,768.0,256.0,256.0);

 art.Element(crayons.Pick(purple),additive,art.stash.rectangle,64.0,64.0,128.0,128);
 art.Element(crayons.Pick(magenta),additive,art.stash.linesGrid10x10,64.0,64.0,256.0,256.0);
 art.Element(crayons.Pick(oliveDrab),additive,art.stash.linesRect,512.0,128.0,256.0,256.0);
 art.ElementLines(crayons.Pick(indigo),none,art.stash.linesRect,512.0,128.0,256.0,256.0);

 art.Element(crayons.Pick(hotPink),additive,art.stash.ca_TL,512.0,512.0,64.0,64.0);
 art.Element(crayons.Pick(hotPink),additive,art.stash.ca_TR,512.0,512.0,64.0,64.0);
 art.Element(crayons.Pick(hotPink),additive,art.stash.ca_BR,512.0,512.0,64.0,64.0);
 art.Element(crayons.Pick(hotPink),additive,art.stash.ca_BL,512.0,512.0,64.0,64.0);

 art.BevelRect2(crayons.Pick(orange),crayons.Pick(red255),none,10.0,3.0,128.0,800.0,64.0,128.0);
 art.BevelRect2(crayons.Pick(orange),crayons.Pick(red255),none,20.0,8.0,300.0,800.0,256.0,128.0);

 crayons.Pick(alabaster).gl();
 Blending(none);
 Text(I2S(stashNumber),1700.0,900.0,14.0,16.0,false);
}

#undef ADD_VERT
#undef ADD_ELEM
#undef ADD_TRI
#undef ADD_QUAD
#undef ADD_VERT_TC
#undef ADD_TRI_TC
#undef ADD_QUAD_TC
#undef ADD_QUAD_TCv2

// Used by Interface.cpp
int standardCursorAlpha = 0;
void Art2d::CreateCursor() {/*
 Cartesiands lines;
 Cartesiand *one = new Cartesiand;
 one->SetRect(0.0,0.0,1.0,0.0);
 Cartesiand *two = new Cartesiand;
 two->SetRect(0.0,0.0,0.0,1.0);
 lines.Append(one);
 lines.Append(two);*/
 this->standardMouseCursorPt1 = stash.GenCircle(16,crayons.Pick(alabaster));
 this->standardMouseCursorPt2 = stash.GenRectangle(crayons.Pick(alabaster));
}
#include "Interface.h"
#include "Display.h"
Zint oldCursorAlpha;
void Art2d::DrawStandardCursor() {
 Blending(none);
 oldCursorAlpha += 5; oldCursorAlpha = oldCursorAlpha % (255 - 64); oldCursorAlpha += 64;
 // Draw mouse cursor
 if (input.KeyDown(DX_LCTRL)) input.color.Pick(skyBlue); else input.color.Greenf((float)(oldCursorAlpha+64) / 255.0f);
 input.color.Alpha((double)(oldCursorAlpha+64)/255.0);
 double mcw=20.0;
 double mch=20.0;
 this->Element(input.color,transparency,standardMouseCursorPt1,(double)input.mx, (double)input.my, mcw,mch);
 this->Element(input.color,transparency,standardMouseCursorPt2,(double)input.mx, (double)input.my, mcw/2.0,mch/2.0);
// this->Stretch(standardMouseCursorte, input.color, input.mx, input.my, display.wd / 1920.0 * 10.0, display.hd / 1080.0 * 10.0);
}