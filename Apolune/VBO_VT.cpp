#include "VBO_VT.h"

#include "PLY.h"
#include "Model.h"

void VBOStaticVT::fromModel( ImmediateModel *m ) { EACH(m->polygons.first,mPolygon,p) PushTriangle(p); }
void VBOStaticVT::fromModel( ImmediateModel *m, int group ) { EACH(m->polygons.first,mPolygon,p) if ( p->group == group ) PushTriangle(p); } 
void VBOStaticVT::fromPLY( PLY *p ) { fromModel(&p->model); Create(); }
void VBOStaticVT::fromPLY( PLY *p, bool create ) { fromModel(&p->model); if ( create ) Create(); }
