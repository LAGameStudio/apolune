#include "VBO_VNTC.h"

#include "PLY.h"
#include "Model.h"

void VBOStaticVNTC::fromModel( ImmediateModel *m ) { EACH(m->polygons.first,mPolygon,p) PushTriangle(p); } 
void VBOStaticVNTC::fromModel( ImmediateModel *m, int group ) { EACH(m->polygons.first,mPolygon,p) if ( p->group == group ) PushTriangle(p); } 
void VBOStaticVNTC::fromPLY( PLY *p ) { fromModel(&p->model); Create(); }
void VBOStaticVNTC::fromPLY( PLY *p, bool create ) { fromModel(&p->model); if ( create ) Create(); }
