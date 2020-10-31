#include "VBO_VCLines.h"


#include "PLY.h"
#include "Model.h"

void VBOStaticVCLines::fromLinesModel( ImmediateModel *m ) { EACH(m->polygons.first,mPolygon,p) Lines(p); }

void VBOStaticVCLines::toLinesVBO( ImmediateModel *m, Crayon outer, Crayon inner ) {
 Vertex *least=m->leastPoint();
 m->bary.Recycle(m->barycenter());
 m->BoxBounds();
 float spherical=(m->boxMagnitude.x>m->boxMagnitude.y && m->boxMagnitude.x>m->boxMagnitude.z ? m->boxMagnitude.x
                 :m->boxMagnitude.y>m->boxMagnitude.x && m->boxMagnitude.y>m->boxMagnitude.z ? m->boxMagnitude.y : m->boxMagnitude.z)
                 /2.0f;
 Clear();
 EACH(m->polygons.first,mPolygon,p) {
  Vertex a,b;
  EACH(p->points.first,Vertex,v) {
   if ( v==p->points.first ) {
    Vertex *w=(Vertex *)p->points.last;
    b.Set(w);
    Crayon color;
    color.ColorInterp(inner,outer,CLAMP(fdistance(m->bary->x,m->bary->y,m->bary->z,w->x,w->y,w->z)/spherical,1.0f));
    b.Color(color);
   }
   a.Set(&b);
   b.Set(v);
   Crayon color;
   color.ColorInterp(inner,outer,CLAMP(fdistance(m->bary->x,m->bary->y,m->bary->z,v->x,v->y,v->z)/spherical,1.0f));
   b.Color(color);
   PushLine(&a,&b);
  }
 }
 Create();
}