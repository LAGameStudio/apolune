#include "Vertexd.h"

#include "Display.h"
#include "Vertex.h"

bool Vertexd::OnScreen() {
 return ( x > 0 && x < display.wd ) && ( y > 0 && y < display.hd );
}

bool Vertexd::OnScreen( double w, double h ) {
 return ( x > -w && (x+w) < display.wd+w ) && ( y > -h && (y+h) < display.hd+h );
}

void Vertexd::Set( Vertex *d ) {
 x=(double)d->x;
 y=(double)d->y;
 z=(double)d->z;
}

void Vertexd::Add( Vertex *v ) {
 Add( (double) v->x, (double) v->y, (double) v->z );
}

#include "Polygon.h"

void Quad::vertices( Vertices *out ) {
 Vertex *v=null;
 v=out->Add(TL.x,TL.y,TL.z); v->TexCoord(tcTL.x,tcTL.y);
 v=out->Add(TR.x,TR.y,TR.z); v->TexCoord(tcTR.x,tcTR.y);
 v=out->Add(BR.x,BR.y,BR.z); v->TexCoord(tcBR.x,tcBR.y);
 v=out->Add(BL.x,BL.y,BL.z); v->TexCoord(tcBL.x,tcBL.y);
}

void Quad::triangles( Polygons *out ) {
 Vertex *v=null;
 mPolygon *p=new mPolygon;
 v=p->points.Add(TL.x,TL.y,TL.z); v->TexCoord(tcTL.x,tcTL.y);
 v=p->points.Add(TR.x,TR.y,TR.z); v->TexCoord(tcTR.x,tcTR.y);
 v=p->points.Add(BR.x,BR.y,BR.z); v->TexCoord(tcBR.x,tcBR.y);
 out->Append(p);
 p=new mPolygon;
 v=p->points.Add(TL.x,TL.y,TL.z); v->TexCoord(tcTL.x,tcTL.y);
 v=p->points.Add(BR.x,BR.y,BR.z); v->TexCoord(tcBR.x,tcBR.y);
 v=p->points.Add(BL.x,BL.y,BL.z); v->TexCoord(tcBL.x,tcBL.y);
 out->Append(p);
}

#include "Trigger.h"

void Quad::Lerp( Quad *b, double time, Quad *out ) {
 Line line;
 line.Set(TL.x,TL.y,b->TL.x,b->TL.y); line.Point(time,&out->TL);
 line.Set(TR.x,TR.y,b->TR.x,b->TR.y); line.Point(time,&out->TR);
 line.Set(BR.x,BR.y,b->BR.x,b->BR.y); line.Point(time,&out->BR);
 line.Set(BL.x,BL.y,b->BL.x,b->BL.y); line.Point(time,&out->BL);
}

#include "GLSetup.h"

void Quad::Render() {
 glBegin(GL_QUADS);
 glTexCoord2d(tcTL.x,tcTL.y);  glVertex2d(TL.x, TL.y);
 glTexCoord2d(tcTR.x,tcTR.y);  glVertex2d(TR.x, TR.y);
 glTexCoord2d(tcBR.x,tcBR.y);  glVertex2d(BR.x, BR.y);
 glTexCoord2d(tcBL.x,tcBL.y);  glVertex2d(BL.x, BL.y);
 glEnd();
}

void Quad::RenderYInvert() {
 glBegin(GL_QUADS);
 glTexCoord2d(tcTL.x,tcBL.y);  glVertex2d(TL.x, TL.y);
 glTexCoord2d(tcTR.x,tcBR.y);  glVertex2d(TR.x, TR.y);
 glTexCoord2d(tcBR.x,tcTR.y);  glVertex2d(BR.x, BR.y);
 glTexCoord2d(tcBL.x,tcTL.y);  glVertex2d(BL.x, BL.y);
 glEnd();
}