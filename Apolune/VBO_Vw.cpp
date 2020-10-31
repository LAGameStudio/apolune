#include "VBO_Vw.h"

#include "PLY.h"
#include "Model.h"

void VBOStaticVw::fromModel( ImmediateModel *m ) { EACH(m->polygons.first,mPolygon,p) PushTriangle(p); }
void VBOStaticVw::fromModel( ImmediateModel *m, int group ) { EACH(m->polygons.first,mPolygon,p) if ( p->group == group ) PushTriangle(p); } 
void VBOStaticVw::fromPLY( PLY *p ) { fromModel(&p->model); Create(); }
void VBOStaticVw::fromPLY( PLY *p, bool create ) { fromModel(&p->model); if ( create ) Create(); }
